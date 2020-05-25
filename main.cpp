#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <cmath>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
using std::vector;
/*
vector struct 
[0]for type 0=line 1=Arc
[1]for direction [line]0=horiz 1=verti 2=slash
                 [Arc] 0=CW 1=CCW
[2,3]for x1,y1(small -> big)
[4,5]for x2,y2 
(if necessary)
[6,7]for c1,c2

*/
class Line
{
public:
    float x1;
    float x2;
    float y1;
    float y2;
    int type;
    Line *next;
    Line *prev;
    Line(float x1, float x2, float y1, float y2, int type) : x1(x1), x2(x2), y1(y1), y2(y2), type(type), next(nullptr), prev(nullptr){};
};
class linear
{
public:
    float x1;
    float x2;
    float y1;
    float y2;
    int type; //0 for verti , 1 for horiz ,2 for slash
    linear(float x1, float y1, float x2, float y2, int type) : x1(x1), y1(y1), x2(x2), y2(y2), type(type){};
};
class Arc
{
public:
    float x1, x2, y1, y2, cx, cy;
    bool direction; //true for CW; false for CCW
    Arc(float x1, float y1, float x2, float y2, float cx, float cy, bool direction) : x1(x1), y1(y1), x2(x2), y2(y2), cx(cx), cy(cy), direction(direction){};
};
class Point
{
public:
    float x, y;
    Point(float x, float y) : x(x), y(y){};
    Point() : x(0), y(0){};
    friend const bool operator<(const Point &a, const Point &b)
    {
        return a.x < b.x;
    }
    friend const bool operator<<(const Point &a, const Point &b)
    {
        return a.y < b.y;
    }
    friend const bool operator>(const Point &a, const Point &b)
    {
        return a.x > b.x;
    }
    friend const bool operator>>(const Point &a, const Point &b)
    {
        return a.y > b.y;
    }
    friend const bool operator==(const Point &a, const Point &b)
    {
        return (a.x < b.x) || (a.x == b.x && a.y < b.y);
    }
};

int stringtoint(string str);
const bool xcomp(const Point &x, const Point &y);
const bool ycomp(const Point &x, const Point &y);
double cross(const Point &o, const Point &a, const Point &b);
bool compare(const Point &a, const Point &b);
int _equal(const linear a, const Point b);
std::vector<Point> getChain(std::vector<Point> &point_vec);
void print(vector<linear *> &line_ptr, const int x, const int y, const int xmin, const int ymin);
bool sort_verti(const vector<float> &a, const vector<float> &b);
bool sort_horiz(const vector<float> &a, const vector<float> &b);
void arcpts();
bool ycheck(vector<Line> &yChecked, int ysize, int yfirst);
bool ydfs(multimap<float, Line *> &y, int ysize, int yfirst, int ypos);
bool xyfs(multimap<pair<float,float>,Line*>&xy,pair<float,float>&xystart,pair<float,float>&xypos,pair<float,float>xysize);
int main()
{
    //map store for x -> multiend y point
    map<float, vector<linear *>> x2yptr;
    map<float, vector<linear *>> y2xptr;
    multimap<float, linear *> x2ymptr;
    multimap<float, linear *> y2xmptr;
    multimap<float, linear *> slashmptr;

    //string use for stringstream->stof()
    vector<linear *> line_ptr;
    string input;
    string data_index;
    string _expand;
    string _NotchSize;
    float expand;
    float NotchSize;

    //vector for line & point storage

    vector<Arc> arc_vec;
    vector<Point> point_vec;
    cout << "Which input to Test?" << endl
         << "1. Q1" << endl
         << "2. Q2" << endl
         << "3. Q3\n"
         << endl;

    ifstream fin;
    string path = __FILE__;
    int choice = 1;
    //cin >> choice;
    switch (choice)
    {
    case 1:
        path = path.substr(0, 1 + path.find_last_of('\\')); //removes file name
        path += "Q1.txt";                                   //adds input file to path
        fin.open(path, ios::in);
        break;
    case 2:
        path = path.substr(0, 1 + path.find_last_of('\\')); //removes file name
        path += "Q2.txt";                                   //adds input file to path
        fin.open(path, ios::in);
        break;
    case 3:
        path = path.substr(0, 1 + path.find_last_of('\\')); //removes file name
        path += "Q3.txt";                                   //adds input file to path
        fin.open(path, ios::in);
        break;
    }
    while (true) //wait for input
    {
        getline(fin, input);
        if (input.empty())
        {
            break;
        }
        input.pop_back(); //get rid of ';'
        istringstream ss(input);
        string token;
        float x1, y1, x2, y2, cx, cy;
        int type;
        bool rotation;
        getline(ss, token, ',');
        if (token == "End")
        {
            break;
        }
        if (token == "Data")
        {
            getline(ss, data_index);
        }
        else if (token == "Expand")
        {
            getline(ss, _expand, ',');
            expand = stof(_expand);
        }
        else if (token == "NotchSize")
        {
            getline(ss, _NotchSize, ',');
            NotchSize = stof(_NotchSize);
        }
        else if (token == "Line")
        {
            string temp;
            getline(ss, temp, ',');
            x1 = stof(temp);
            getline(ss, temp, ',');
            y1 = stof(temp);
            getline(ss, temp, ',');
            x2 = stof(temp);
            getline(ss, temp, ',');
            y2 = stof(temp);
            float xs = x1, xb = x2, ys = y1, yb = y2;
            if (x1 == x2) // verti
                type = 1;
            else if (y1 == y2) //horiz
                type = 0;
            else
                type = 2; // slash

            //point vector
            point_vec.push_back(Point(x1, y1));
            point_vec.push_back(Point(x2, y2));
            if (type == 1) //verti x1==x2
            {
                if (y1 >= y2)
                    swap(ys, yb);
                linear *_line = new linear(xs, ys, xb, yb, type);
                x2ymptr.insert(make_pair(xs, _line));
                line_ptr.push_back(_line);
            }
            else if (type == 0) // horiz y1==y2
            {
                if (x1 >= x2)
                    swap(ys, yb);
                linear *_line = new linear(x1, y1, x2, y2, type);
                y2xmptr.insert(make_pair(y1, _line));
                line_ptr.push_back(_line);
            }
            else //slash
            {
                linear *x2y = new linear(xs, ys, xb, yb, 2);
                linear *y2x = new linear(xb, yb, xs, ys, 2);
                x2ymptr.insert(make_pair(xs, x2y));
                x2ymptr.insert(make_pair(xb, y2x));
                y2xmptr.insert(make_pair(ys, x2y));
                y2xmptr.insert(make_pair(yb, y2x));
                line_ptr.push_back(x2y);
                line_ptr.push_back(y2x);
            }
            //Map creation (deprecated)
        }
        else if (token == "Arc")
        {
            string temp;
            getline(ss, temp, ',');
            x1 = stof(temp);
            getline(ss, temp, ',');
            y1 = stof(temp);
            getline(ss, temp, ',');
            x2 = stof(temp);
            getline(ss, temp, ',');
            y2 = stof(temp);
            getline(ss, temp, ',');
            cx = stof(temp);
            getline(ss, temp, ',');
            cy = stof(temp);
            getline(ss, temp, ',');
            if (temp == "CW")
                rotation = 1;
            else
                rotation = 0;
            float rad = pow(pow(x1 - cx, 2) + pow(y1 - cy, 2), 0.5);
            float tx1 = cx + rad;
            float tx2 = cx - rad;
            float ty1 = cy + rad;
            float ty2 = cy - rad;
            arc_vec.push_back(Arc(x1, x2, y1, y2, cx, cy, rotation));

            Point obj1(x1, y1);
            Point obj2(x2, y2);
            point_vec.push_back(obj1);
            point_vec.push_back(obj2);
            /*Point obj1(tx2,ty2);
            Point obj2(tx2,cy);
            Point obj3(tx2,ty1);
            Point obj4(cx,ty2);
            Point obj5(cx,ty1);
            Point obj6(tx1,ty2);
            Point obj7(tx1,cy);
            Point obj8(tx1,ty1);
            point_vec.push_back(obj1);
            point_vec.push_back(obj2);
            point_vec.push_back(obj3);
            point_vec.push_back(obj4);
            point_vec.push_back(obj5);
            point_vec.push_back(obj6);
            point_vec.push_back(obj7);
            point_vec.push_back(obj8);*/
        }
    }
    //call Andrew_chain
    vector<Point> Andrew_Chain = getChain(point_vec);
    //print input

    const int xsize = (--(x2ymptr.end()))->first - (x2ymptr.begin()->first);
    const int ysize = (--(y2xmptr.end()))->first - (y2xmptr.begin()->first);
    print(line_ptr, xsize + 1, ysize + 1, (x2ymptr.begin()->first), (y2xmptr.begin()->first));

    vector<Point> xSort(Andrew_Chain);
    vector<Point> ySort(Andrew_Chain);
    /*
    sort(horiz.begin(), horiz.end(), [](const std::vector<float> &a, const std::vector<float> &b) {
        //If you want to sort in ascending order, then substitute > with <
        return a[0] < b[0];
    });
    sort(verti.begin(), verti.end(), [](const std::vector<float> &a, const std::vector<float> &b) {
        //If you want to sort in ascending order, then substitute > with <
        return a[0] < b[0];
    });
    */
    //sort(line_vec.begin(), line_vec.end());
    sort(xSort.begin(), xSort.end(), xcomp);
    sort(ySort.begin(), ySort.end(), ycomp);

    //time to mark

    //get keys
    set<float> xkeys;
    //xkeys.reserve(x2ymptr.size());
    for (auto const &it : x2ymptr)
        xkeys.insert(it.first);
    //xkeys.push_back(it.first);
    set<float> ykeys;
    //ykeys.reserve(y2xmptr.size());
    for (auto const &it : y2xmptr)
        ykeys.insert(it.first);
    //ykeys.push_back(it.first);
    vector<Line *> marked;
    vector<Line *> mark_v2;
    //get horiz or verti line
  /*  for (int i = 1; i < Andrew_Chain.size(); i++)
    { //
        int type;
        if (Andrew_Chain[i].x == Andrew_Chain[i - 1].x)
        {
            type = 1;
        }
        else if (Andrew_Chain[i].y == Andrew_Chain[i - 1].y)
        {
            type = 0;
        }
        else
            type = 2;
        marked.push_back(new Line(Andrew_Chain[i - 1].x, Andrew_Chain[i - 1].y, Andrew_Chain[i].x, Andrew_Chain[i].y, type));
    }
    Line *cur = marked[0];
    for (int i = 0; i < marked.size(); i++)
    {
        Line *now = marked[i];
        if (now != cur && ((cur->x2 == now->x1) && cur->y2 == now->y1))
        {
            cur->next = now;
            now->prev = cur;
            cur = now;
        }
    }*/

    //mark x left & xright
    const float xhalf = x2ymptr.begin()->first + xsize;
    const float yhalf = y2xmptr.begin()->first + ysize;
    vector<Line *> yupper;
    vector<pair<float, float>> ypair;
    multimap<float,Line*>yleft;
    vector<float>yEndPoint;
    vector<float>yStartPoint;
    for (int i = 0; i < xsize / 2; i++)
    {
        const float xnow = i + x2ymptr.begin()->first;
        auto it = x2ymptr.equal_range(xnow);
        if(it.first==it.second){
            continue;
        }
        for (auto a = it.first; a != it.second; ++a)
        {
            auto _linear_ = a->second;
            auto ret =yleft.equal_range(_linear_->y1);
            bool skip=false;
            if(ret.first != ret.second){
                for(auto t = ret.first;t!=ret.second;++t){
                    if(_linear_->y1 == t->second->y1 && _linear_->y2 <= t->second->y2){
                        skip=true;
                        break;
                    }
                    //cut long into short
                    else if(find(yStartPoint.begin(),yStartPoint.end(),_linear_->y1) == yStartPoint.end()){
                        
                    }
                    else if(find(yEndPoint.begin(),yEndPoint.end(),_linear_->y1)!=yEndPoint.end()){
                        continue;
                    }   
                    else if(_linear_->y1 <= t->second->y2 ){
                        skip=true;
                        break;
                    }
                }
                if(skip){
                    skip=false;
                    continue;
                }
            }
            
            
            
            Line* _newLine = new Line(_linear_->x1,_linear_->x2,_linear_->y1,_linear_->y2,_linear_->type);
            yEndPoint.push_back(_newLine->y2);
            yStartPoint.push_back(_newLine->y1);
            for(int i=_newLine->y1;i<=_newLine->y2;i++){
                yleft.insert(make_pair(i,_newLine));
            }
            
        }
        ydfs(yleft,ysize,y2xmptr.begin()->first,y2xmptr.begin()->first);
    }
    return 0;
}

int stringtoint(string str)
{
    return str.compare("Line");
}
const bool xcomp(const Point &x, const Point &y)
{
    return x < y;
}
const bool ycomp(const Point &x, const Point &y)
{
    return x << y;
}
double cross(const Point &o, const Point &a, const Point &b)
{
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}
bool compare(const Point &a, const Point &b)
{
    return (a.x < b.x) || (a.x == b.x && a.y < b.y);
}
int _equal(linear a, Point b)
{
    if (a.x1 == b.x && a.y1 == b.y)
    {
        return 1;
    }
    else if (a.x2 == b.x && a.y2 == b.y)
    {
        return 2;
    }
    else
        return 0;
}
vector<Point> getChain(vector<Point> &point_vec)
{
    vector<Point> CH(point_vec.size() + 1, Point());

    std::sort(point_vec.begin(), point_vec.end(), compare);
    int m = 0;

    for (int i = 0; i < point_vec.size(); ++i)
    {
        while (m >= 2 && cross(CH[m - 2], CH[m - 1], point_vec[i]) <= 0)
            m--;
        CH[m++] = point_vec[i];
    }

    for (int i = point_vec.size() - 2, t = m + 1; i >= 0; --i)
    {
        while (m >= t && cross(CH[m - 2], CH[m - 1], point_vec[i]) <= 0)
            m--;
        CH[m++] = point_vec[i];
    }
    CH.erase(std::next(CH.begin(), m), CH.end());
    m--;
    return CH;
}
void print(std::vector<linear *> &line_ptr, int x, int y, int xmin, int ymin)
{
    vector<vector<int>> print(y, vector<int>(x, 0));
    for (int i = 0; i < line_ptr.size(); i++)
    {
        if (line_ptr[i]->type == 1) //verti
        {
            for (int j = 0; j < print.size(); j++)
            {
                float yl, ys;
                line_ptr[i]->y1 > line_ptr[i]->y2 ? (yl = line_ptr[i]->y1, ys = line_ptr[i]->y2) : (yl = line_ptr[i]->y2, ys = line_ptr[i]->y1);
                for (int k = ys; k <= yl; k++)
                {
                    print[k - ymin][line_ptr[i]->x1 - xmin] = 1;
                }
            }
        }
        else if (line_ptr[i]->type == 0) //horiz
        {
            if (line_ptr[i]->x1 <= line_ptr[i]->x2)
            {
                for (int j = line_ptr[i]->x1 - xmin; j <= line_ptr[i]->x2 - xmin; j++)
                {
                    print[line_ptr[i]->y1 - ymin][j] = 1;
                }
            }
            else if (line_ptr[i]->x1 > line_ptr[i]->x2)
            {
                for (int j = line_ptr[i]->x2 - xmin; j <= line_ptr[i]->x1 - xmin; j++)
                {
                    print[line_ptr[i]->y1 - ymin][j] = 1;
                }
            }
        }
        else if (line_ptr[i]->type == 2) //slash
        {
            const auto it = line_ptr[i];
            //get m
            if ((int)((it->x1 - it->x2) / (it->y1 - it->y2)) == 1)
            {
                if (it->x1 < it->x2)
                {
                    for (int k = it->x1 - xmin; k <= it->x2 - xmin; k++)
                    {
                        print[it->y1 - ymin + (k - (it->x1 - xmin))][k] = 1;
                    }
                }
                else if (it->x2 < it->x1)
                {
                    for (int k = it->x2 - xmin; k <= it->x1 - xmin; k++)
                    {
                        print[it->y2 - ymin + (k - (it->x2 - xmin))][k] = 1;
                    }
                }
            }
            else if ((int)((it->x1 - it->x2) / (it->y1 - it->y2)) == -1)
            {
                if (it->x1 < it->x2)
                {
                    for (int k = it->x1 - xmin; k <= it->x2 - xmin; k++)
                    {
                        print[it->y1 - ymin - (k - (it->x1 - xmin))][k] = 1;
                    }
                }
                else if (it->x2 < it->x1)
                {
                    for (int k = it->x2 - xmin; k <= it->x1 - xmin; k++)
                    {
                        print[it->y2 - ymin - (k - (it->x2 - xmin))][k] = 1;
                    }
                }
            }
        }
    }
    cout << endl;

    for (int i = print.size() - 1; i >= 0; --i)
    {

        cout << "row = " << i + ymin << "  \t";
        for (int j = 0; j < print[i].size(); j++)
        {
            print[i][j] ? (cout << "+") : (cout << " ");
        }
        cout << endl;
    }
    cout << "row = x"
         << "  \t";
    for (int i = xmin; i < xmin + x; i++)
    {
        cout << abs(i % 10);
    }
    cout << endl;
}
bool sort_verti(const vector<float> &a, const vector<float> &b)
{
    return a[0] < b[0];
}
bool sort_horiz(const vector<float> &a, const vector<float> &b)
{
    return a[0] < b[0];
}
bool ycheck(vector<Line *> &yChecked, int ysize, int yfirst)
{
    int ycur = yfirst;
    int ytemp = ycur;
    for (int i = 0; i < yChecked.size(); i++)
    {
        for (const auto &it : yChecked)
        {
            if (ycur == it->y1)
            {
                ytemp = it->y2;
            }
        }
        if (ytemp == ycur)
            return false;
        else
            ycur = ytemp;
    }
    if (ycur == yfirst + ysize)
    {
        return true;
    }
    else
        return false;
}
/*bool ycheck(multimap<float, Line *> &yChecked, int ysize, int yfirst, int ypos)
{
    multimap<float, Line *> altyChecked;
    if (altyChecked.find(ypos))
    {
    }
    int ycur = yfirst;
    int ytemp = ycur;
    //using dfs

    if (altyChecked.find())
        if (ycur == yfirst + ysize)
        {
            return true;
        }
        else
            return false;
}*/
bool ydfs(multimap<float, Line *> &y, int ysize, int yfirst, int ypos)
{
    //auto ynextup=y.upper_bound(ypos);
    //auto ynextlow=y.lower_bound(ypos);
    
    auto ynext = y.equal_range(ypos);
    //found nothing
    if(ynext.first == ynext.second)
        return false;
    for (auto ret = ynext.first; ret != ynext.second; ++ret)
    {
        //if y2 reached ymax
        if(ret->second->y2 == ysize + yfirst)
            return true;
        else//continue dfs
            ydfs(y,ysize,yfirst,ret->second->y2);
    }
}
bool xydfs(multimap<pair<float,float>,Line*>&xy,pair<float,float>xystart,pair<float,float>xypos,pair<float,float>xysize){
    auto xynext = xy.equal_range(xypos);
    //found nothing
    if(xynext.first == xynext.second)
        return false;
    for (auto ret = xynext.first; ret != xynext.second; ++ret)
    {
        //if y2 reached ymax
        if(ret->second->y2 == xystart.second && ret->second->x2 == xystart.first)
            return true;
        else//continue dfs
            //make_pair(ret->second->x2,ret->second->y2)
            xydfs(xy,xystart,make_pair(ret->second->x2,ret->second->y2),xysize);
    }
}
