#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
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
    bool isRealContour = false;
    bool isContour = false;
    Line(float x1, float y1, float x2, float y2, int type) : x1(x1), y1(y1), x2(x2), y2(y2), type(type), next(nullptr), prev(nullptr){};
};
class Index
{
public:
    float index;
    float low;
    float big;
    Index *next = nullptr;
    Index *prev = nullptr;
    Line *Lptr;
    Index(float index, float low, float big) : index(index), low(low), big(big), next(nullptr), prev(nullptr){};
    Index(float index, float low, float big, Index *next) : index(index), low(low), big(big), next(next), prev(nullptr){};
    Index(Index *prev, float index, float low, float big) : index(index), low(low), big(big), next(nullptr), prev(prev){};
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
void ysolve(int i, multimap<float, Line *> &x2ymptr, multimap<float, Line *> &yleft, Index *&root, int xbegin, int xsize, int ybegin, int ysize);
void xsolve(int i, multimap<float, Line *> &x2ymptr, multimap<float, Line *> &yleft, Index *&root, int xbegin, int xsize, int ybegin, int ysize);
Line *ycutfront(Line *temp, Index *a);
Line *xcutfront(Line *temp, Index *a);
multimap<float, Line *> yleft_cal(bool left, multimap<float, Line *> &x2ymptr, int xbegin, int xsize, int ybegin, int ysize);
multimap<float, Line *> xleft_cal(bool left, multimap<float, Line *> &x2ymptr, int xbegin, int xsize, int ybegin, int ysize);
void ycutback(Line *temp, Index *a);
void print(vector<Line *> &line_ptr, const int x, const int y, const int xmin, const int ymin);
void arcpts();
bool ycheck(vector<Line> &yChecked, int ysize, int yfirst);
bool ydfs(multimap<float, Line *> &y, int ysize, int yfirst, int ypos);
bool xyfs(multimap<pair<float, float>, Line *> &xy, pair<float, float> &xystart, pair<float, float> &xypos, pair<float, float> xysize);
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    //map store for x -> multiend y point
    multimap<float, Line *> x2ymptr;
    multimap<float, Line *> y2xmptr;
    multimap<float, Line *> slashx2ymptr;
    multimap<float, Line *> slashy2xmptr;

    //string use for stringstream->stof()
    vector<Line *> line_ptr;
    string input;
    string data_index;
    string _expand;
    string _NotchSize;
    float expand;
    float NotchSize;
    int xsize;
    int ysize;
    int xbegin;
    int ybegin;
    int xend;
    int yend;
    bool init = false;
    //vector for line & point storage

    vector<Arc> arc_vec;
    //vector<Point> point_vec;
    cout << "Which input to Test?" << endl
         << "1. Q1" << endl
         << "2. Q2" << endl
         << "3. Q3\n"
         << endl;

    ifstream fin;
    string path = __FILE__;
    const int choice = 3;
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

            ys = floor(ys);
            yb = ceil(yb);
            xs = floor(xs);
            xb = ceil(xb);

            if (init == false)
            {
                if (x1 < x2)
                {
                    xbegin = x1;
                    xend = x2;
                }
                else
                {
                    xbegin = x2;
                    xend = x1;
                }
                if (y1 < y2)
                {
                    ybegin = y1;
                    yend = y2;
                }
                else
                {
                    ybegin = y2;
                    yend = y1;
                }
                init = true;
            }
            if (x1 == x2) // verti
                type = 1;
            else if (y1 == y2) //horiz
                type = 0;
            else
                type = 2; // slash

            //point vector
            //point_vec.push_back(Point(x1, y1));
            //point_vec.push_back(Point(x2, y2));
            if (type == 1) //verti x1==x2
            {
                if (y1 >= y2)
                    swap(ys, yb);
                Line *_line = new Line(xs, ys, xb, yb, type);
                x2ymptr.insert(make_pair(xs, _line));
                line_ptr.push_back(_line);
            }
            else if (type == 0) // horiz y1==y2
            {
                if (x1 >= x2)
                    swap(xs, xb);
                Line *_line = new Line(xs, ys, xb, yb, type);
                y2xmptr.insert(make_pair(ys, _line));
                line_ptr.push_back(_line);
            }
            else //slash
            {
                Line *x2y = new Line(xs, ys, xb, yb, 2);
                Line *y2x = new Line(xb, yb, xs, ys, 2);
                x2ymptr.insert(make_pair(xs, x2y));
                x2ymptr.insert(make_pair(xb, y2x));
                y2xmptr.insert(make_pair(ys, x2y));
                y2xmptr.insert(make_pair(yb, y2x));
                line_ptr.push_back(x2y);
                line_ptr.push_back(y2x);
            }
            //Map creation (deprecated)
            if (ys < ybegin)
                ybegin = ys;
            if (xs < xbegin)
                xbegin = xs;
            if (yb > yend)
                yend = yb;
            if (xb > xend)
                xend = xb;
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
            //point_vec.push_back(obj1);
            //point_vec.push_back(obj2);
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
    //vector<Point> Andrew_Chain = getChain(point_vec);
    //print input
    ysize = yend - ybegin;
    xsize = xend - xbegin;

    print(line_ptr, xsize + 1, ysize + 1, xbegin, ybegin);

    //mark x left & xright
    const float xhalf = xbegin + xsize / 2;
    const float yhalf = ybegin + ysize / 2;
    vector<Line *> yleft_vec;
    vector<Line *> yright_vec;
    vector<Line *> xup_vec;
    vector<Line *> xdown_vec;
    multimap<float, Line *> yleft = yleft_cal(true, x2ymptr, xbegin, xsize, ybegin, ysize);
    for (auto &it : yleft)
    {
        yleft_vec.push_back(it.second);
    }
    print(yleft_vec, xsize + 1, ysize + 1, xbegin, ybegin);
    multimap<float, Line *> yright = yleft_cal(false, x2ymptr, xbegin, xsize, ybegin, ysize);
    for (auto &it : yright)
    {
        yright_vec.push_back(it.second);
    }
    print(yright_vec, xsize + 1, ysize + 1, xbegin, ybegin);
    multimap<float, Line *> xdown = xleft_cal(true, y2xmptr, xbegin, xsize, ybegin, ysize);
    for (auto &it : xdown)
    {
        xdown_vec.push_back(it.second);
    }
    print(xdown_vec, xsize + 1, ysize + 1, xbegin, ybegin);
    multimap<float, Line *> xup = xleft_cal(false, y2xmptr, xbegin, xsize, ybegin, ysize);
    for (auto &it : xup)
    {
        xup_vec.push_back(it.second);
    }
    print(xup_vec, xsize + 1, ysize + 1, xbegin, ybegin);
    //ydfs(yleft,ysize,y2xmptr.begin()->first,y2xmptr.begin()->first);
    vector<Line *> yvec;
    yvec.reserve(yleft_vec.size() + yright_vec.size()); // preallocate memory
    yvec.insert(yvec.end(), yleft_vec.begin(), yleft_vec.end());
    yvec.insert(yvec.end(), yright_vec.begin(), yright_vec.end());
    print(yvec, xsize + 1, ysize + 1, xbegin, ybegin);
    cout << "\nyleftcomplete" << endl;
    vector<Line *> xvec;
    xvec.reserve(xdown_vec.size() + xup_vec.size()); // preallocate memory
    xvec.insert(xvec.end(), xdown_vec.begin(), xdown_vec.end());
    xvec.insert(xvec.end(), xup_vec.begin(), xup_vec.end());
    print(xvec, xsize + 1, ysize + 1, xbegin, ybegin);
    vector<Line *> contour;
    contour.reserve(xvec.size() + yvec.size());
    contour.insert(contour.end(), xvec.begin(), xvec.end());
    contour.insert(contour.end(), yvec.begin(), yvec.end());
    print(contour, xsize + 1, ysize + 1, xbegin, ybegin);
    return 0;
}
multimap<float, Line *> yleft_cal(bool ydirect, multimap<float, Line *> &x2ymptr, int xbegin, int xsize, int ybegin, int ysize)
{
    bool complete = false;
    multimap<float, Line *> y;
    Index *root = nullptr;
    if (ydirect)
    {
        for (int i = 0; i < xsize; ++i)
        {
            ysolve(i, x2ymptr, y, root, xbegin, xsize, ybegin, ysize);
            Index *cur = root;

            if (cur == nullptr)
            {
                continue;
            }
            else if (cur->low != ybegin)
            {
                continue;
            }
            else
            {
                while (cur->next != nullptr)
                {
                    if (cur->big == cur->next->low)
                    {
                        cur = cur->next;
                    }
                    else
                        break;
                }
                if (cur->big == ybegin + ysize)
                {
                    complete = true;
                }
            }
            if (complete)
            {
                break;
            }
        }
    }
    else
    {
        for (int i = xsize; i >= 0; --i)
        {
            ysolve(i, x2ymptr, y, root, xbegin, xsize, ybegin, ysize);
            Index *cur = root;
            if (cur == nullptr)
            {
                continue;
            }
            else if (cur->low != ybegin)
            {
                continue;
            }
            else
            {
                while (cur->next != nullptr)
                {
                    if (cur->big == cur->next->low)
                    {
                        cur = cur->next;
                    }
                    else
                        break;
                }
                if (cur->big == ybegin + ysize)
                {
                    complete = true;
                }
            }
            if (complete)
            {
                break;
            }
        }
    }
    Index *cur = root;
    if (complete)
    {
        while (cur->next != nullptr)
        {
            if (cur->index != cur->next->index)
            {
                auto xlow = cur->index;
                auto xbig = cur->next->index;
                if (xlow > xbig)
                {
                    swap(xlow, xbig);
                }
                Line *sub = new Line(xlow, cur->big, xbig, cur->next->low, 0);
                cur->Lptr->next = sub;
                sub->prev = cur->Lptr;
                cur->next->Lptr->prev = sub;
                sub->next = cur->next->Lptr;
                y.insert(make_pair(cur->index, sub));
                cur = cur->next;
            }
            else{
                cur = cur->next;
            }
        }
        return y;
    }
    else
    {
        //root !=ybegin
        auto temp_begin = xbegin;
        if(!ydirect){
            xbegin = xbegin + xsize;
        }
        if(cur -> low != ybegin){
            Index *n = new Index(xbegin, ybegin, cur->low,cur);
            Line *s = new Line(xbegin, ybegin, xbegin, cur->low, 1);
            y.insert(make_pair(xbegin,s));
            n->Lptr=s;
            if(cur->index!=xbegin){
                Line *sub = new Line(xbegin, cur->low, cur->index, cur->low, 0);
                s->next = sub;
                sub -> next = cur->Lptr;
                sub->prev = s;
                cur->Lptr->prev = sub;
                y.insert(make_pair(cur->index, sub));
            }
            else{
                s->next = cur->Lptr;
                cur->Lptr->prev = s;
            }
            cur->prev = n;
            root = n;
            if(cur->next == nullptr){
                return y;
            }
        }
        while(cur->next!=nullptr){
            if(cur->big != cur->next->low){
                Line *sub = new Line(xbegin, cur->big, xbegin, cur->next->low, 1);
                y.insert(make_pair(xbegin, sub));
                if(cur->index != xbegin){
                    Line *sub1 = new Line(xbegin, cur->big, cur->index, cur->big,0);
                    y.insert(make_pair(xbegin, sub1));
                    cur->Lptr->next = sub1;
                    sub1->next = sub;
                }
                else{
                    cur->Lptr->next = sub;
                    sub->next = cur->next->Lptr;
                }
                if (cur->next->index != xbegin){
                    Line *sub1 = new Line(xbegin, cur->next->low, cur->next->index, cur->next->low, 0);
                    y.insert(make_pair(xbegin, sub1));
                }
                else{
                    ;
                }
            }
            else if (cur->index != cur->next->index)
            {
                auto xlow = cur->index;
                auto xbig = cur->next->index;
                if (xlow > xbig)
                {
                    swap(xlow, xbig);
                }
                Line *sub = new Line(xlow, cur->big, xbig, cur->next->low, 0);
                cur->Lptr->next = sub;
                sub->prev = cur->Lptr;
                cur->next->Lptr->prev = sub;
                sub->next = cur->next->Lptr;
                y.insert(make_pair(xlow, sub));
                
            }
            else{
                cur->Lptr->next = cur->next->Lptr;
            }
            cur = cur->next;
        }
        xbegin = temp_begin;
        return y;
    }
    return y;
}
multimap<float, Line *> xleft_cal(bool ydirect, multimap<float, Line *> &y2xmptr, int xbegin, int xsize, int ybegin, int ysize)
{
    bool complete = false;
    multimap<float, Line *> x;
    Index *root = nullptr;
    if (ydirect)
    {
        for (int i = 0; i < ysize; ++i)
        {
            xsolve(i, y2xmptr, x, root, xbegin, xsize, ybegin, ysize);
            Index *cur = root;

            if (cur == nullptr)
            {
                continue;
            }
            else if (cur->low != xbegin)
            {
                continue;
            }
            else
            {
                while (cur->next != nullptr)
                {
                    if (cur->big == cur->next->low)
                    {
                        cur = cur->next;
                    }
                    else
                        break;
                }
                if (cur->big == xbegin + xsize)
                {
                    complete = true;
                }
            }
            if (complete)
            {
                break;
            }
        }
    }
    else
    {
        for (int i = ysize; i >= 0; --i)
        {
            xsolve(i, y2xmptr, x, root, xbegin, xsize, ybegin, ysize);
            Index *cur = root;
            if (cur == nullptr)
            {
                continue;
            }
            else if (cur->low != xbegin)
            {
                continue;
            }
            else
            {
                while (cur->next != nullptr)
                {
                    if (cur->big == cur->next->low)
                    {
                        cur = cur->next;
                    }
                    else
                        break;
                }
                if (cur->big == xbegin + xsize)
                {
                    complete = true;
                }
            }
            if (complete)
            {
                break;
            }
        }
    }
    if (complete)
    {
        Index *cur = root;
        while (cur->next != nullptr)
        {
            if (cur->index != cur->next->index)
            {
                auto ylow = cur->index;
                auto ybig = cur->next->index;
                if (ylow > ybig)
                {
                    swap(ylow, ybig);
                }
                Line *sub = new Line(cur->big, ylow, cur->next->low, ybig,1);
                cur->Lptr->next = sub;
                sub->prev = cur->Lptr;
                cur->next->Lptr->prev = sub;
                sub->next = cur->next->Lptr;
                x.insert(make_pair(cur->index, sub));
                cur = cur->next;
            }
            else{
                cur = cur->next;
            }
        }
        return x;
    }
    //root !=ybegin
    else{
        Index *cur = root;
        auto temp_begin = ybegin;
        if(ydirect){
            ybegin = ybegin + xsize;
        }
        if(cur -> low != ybegin){
            Index *n = new Index(ybegin, xbegin, cur->low,cur);
            Line *s = new Line(xbegin, ybegin, cur->low, ybegin, 0);
            x.insert(make_pair(ybegin,s));
            n->Lptr=s;
            if(cur->index!=ybegin){
                Line *sub = new Line(cur->low, ybegin, cur->low, cur->index, 1);
                s->next = sub;
                sub -> next = cur->Lptr;
                sub->prev = s;
                cur->Lptr->prev = sub;
                x.insert(make_pair(cur->index, sub));
            }
            else{
                s->next = cur->Lptr;
                cur->Lptr->prev = s;
            }
            cur->prev = n;
            root = n;
            if(cur->next == nullptr){
                return x;
            }
        }
        while(cur->next!=nullptr){
            if(cur->big != cur->next->low){
                Line *sub = new Line(cur->big, ybegin, cur->next->low, ybegin, 0);
                x.insert(make_pair(xbegin, sub));
                if(cur->index != xbegin){
                    Line *sub1 = new Line(cur->big,ybegin ,cur->big, cur->index,1);
                    x.insert(make_pair(xbegin, sub1));
                    cur->Lptr->next = sub1;
                    sub1->next = sub;
                }
                else{
                    cur->Lptr->next = sub;
                    sub->next = cur->next->Lptr;
                }
                if (cur->next->index != ybegin){
                    Line *sub1 = new Line(cur->next->low,ybegin, cur->next->low, cur->next->index, 0);
                    x.insert(make_pair(ybegin, sub1));
                }
                else{
                    ;
                }
            }
            else if (cur->index != cur->next->index)
            {
                auto ylow = cur->index;
                auto ybig = cur->next->index;
                if (ylow > ybig)
                {
                    swap(ylow, ybig);
                }
                Line *sub = new Line(cur->big, ylow, cur->next->low, ybig, 0);
                cur->Lptr->next = sub;
                sub->prev = cur->Lptr;
                cur->next->Lptr->prev = sub;
                sub->next = cur->next->Lptr;
                x.insert(make_pair(ylow, sub));
            }
            else{
                cur->Lptr->next = cur->next->Lptr;
            }
            cur = cur->next;
        }
        ybegin = temp_begin;
        return x;
    }
    return x; 
}
void xsolve(int i, multimap<float, Line *> &y2xmptr, multimap<float, Line *> &yleft, Index *&root, int xbegin, int xsize, int ybegin, int ysize)
{
    Index *temp = nullptr;
    const float ynow = i + ybegin;
    auto it = y2xmptr.equal_range(ynow);
    if (it.first == it.second)
    {
        //continue;
        return;
    }
    for (auto a = it.first; a != it.second; ++a)
    {
        temp = root;
        auto line_t = a->second;
        auto ret = yleft.equal_range(line_t->x1);
        bool skip = false;
        if (temp == NULL)
        {
            a->second->isContour = true;
            yleft.insert(make_pair(a->first, a->second));
            Index *tmp = new Index(a->first, line_t->x1, line_t->x2);
            tmp->Lptr = a->second;
            if (root == nullptr)
            {
                root = tmp;
            }
        }
        else
        {
            /*if (skip == true)
                {
                    break;
                }*/
            while (line_t->x2 >= temp->big)
            {
                if (line_t->x1 >= temp->low && line_t->x2 <= temp->big)
                {
                    //skip = true;
                    break;
                }
                else if (temp->next != nullptr || (line_t->x1 < temp->low && line_t->x2 >= temp->big) || (line_t->x1 >= temp->low && line_t->x1 < temp->big && line_t->x2 >= temp->big))
                {
                    if (line_t->x1 < temp->low && line_t->x2 >= temp->big)
                    {
                        Index *n = new Index(a->first, line_t->x1, temp->low, temp);
                        Line *xcut = xcutfront(line_t, temp);
                        n->Lptr = xcut;
                        yleft.insert(make_pair(a->first, xcut));
                        if (root == temp)
                        {
                            temp->prev = n;
                            root = n;
                        }
                        else
                        {
                            n->prev = temp->prev;
                            temp->prev->next = n;
                            temp->prev = n;
                        }
                        if (temp->next != nullptr)
                        {
                            temp = temp->next;
                        }
                        else
                        {
                            break;
                        }
                        //temp = temp->next;
                    }
                    else if (line_t->x1 >= temp->low && line_t->x1 < temp->big && line_t->x2 >= temp->big)
                    {
                        //ycutback(&line_t,temp);
                        line_t->x1 = temp->big;
                        if (temp->next != nullptr)
                        {
                            temp = temp->next;
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        if (temp->next != nullptr)
                        {
                            temp = temp->next;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                else
                {
                    line_t->isContour = true;
                    yleft.insert(make_pair(a->first, line_t));
                    if (line_t->x1 < temp->low && line_t->x2 <= temp->low)
                    {
                        Index *n = new Index(a->first, line_t->x1, line_t->x2, temp);
                        n->Lptr = line_t;
                        if (temp->prev == nullptr)
                        {
                            root = n;
                            temp->prev = n;
                        }
                        else
                        {
                            n->prev = temp->prev;
                            temp->prev->next = n;
                            temp->prev = n;
                        }
                    }
                    else if (line_t->x1 >= temp->low && line_t->x2 > temp->big)
                    {
                        Index *n = new Index(temp, a->first, line_t->x1, line_t->x2);
                        n->Lptr = line_t;
                        if (temp->next == nullptr)
                        {
                            temp->next = n;
                        }
                        else
                        {
                            n->next = temp->next;
                            temp->next->prev = n;
                            temp->next = n;
                        }
                    }
                    temp = temp->next;
                    break;
                }
            }
            if (temp != nullptr)
            {
                if (line_t->x1 < temp->low && line_t->x2 <= temp->low)
                //if (line_t->x1 >= temp->big && line_t->x2 > temp->big)
                {
                    line_t->isContour = true;
                    yleft.insert(make_pair(a->first, line_t));
                    Index *n = new Index(a->first, line_t->x1, line_t->x2, temp);
                    n->Lptr = line_t;
                    if (temp->prev == nullptr)
                    {
                        root = n;
                        temp->prev = n;
                    }
                    else
                    {
                        n->prev = temp->prev;
                        temp->prev->next = n;
                        temp->prev = n;
                    }
                }
                else if (line_t->x1 < temp->low && line_t->x2 <= temp->big)
                {
                    Line *sub = new Line(line_t->x1, line_t->y1, temp->low, line_t->y2, line_t->type);
                    sub->isContour = true;
                    Index *n = new Index(a->first, line_t->x1, temp->low, temp);
                    n->Lptr = sub;
                    yleft.insert(make_pair(a->first, sub));
                    if (temp->prev == nullptr)
                    {
                        root = n;
                        temp->prev = n;
                    }
                    else
                    {
                        n->prev = temp->prev;
                        temp->prev->next = n;
                        temp->prev = n;
                    }
                }
                // (line_t->x1 < temp->low && line_t->x2 <= temp->low)
                else if (line_t->x1 >= temp->big && line_t->x2 > temp->big)
                {
                    line_t->isContour = true;
                    yleft.insert(make_pair(a->first, line_t));
                    Index *n = new Index(temp, a->first, line_t->x1, line_t->x2);
                    n->Lptr = line_t;
                    if (temp->next == nullptr)
                    {
                        temp->next = n;
                    }
                    else
                    {
                        n->next = temp->next;
                        temp->next->prev = n;
                        temp->next = n;
                    }
                }
            }
        }
    }
}
void ysolve(int i, multimap<float, Line *> &x2ymptr, multimap<float, Line *> &yleft, Index *&root, int xbegin, int xsize, int ybegin, int ysize)
{
    Index *temp = nullptr;
    const float xnow = i + xbegin;
    auto it = x2ymptr.equal_range(xnow);
    if (it.first == it.second)
    {
        //continue;
        return;
    }
    for (auto a = it.first; a != it.second; ++a)
    {
        temp = root;
        auto line_t = a->second;
        auto ret = yleft.equal_range(line_t->y1);
        bool skip = false;
        if (temp == NULL)
        {
            a->second->isContour = true;
            yleft.insert(make_pair(a->first, a->second));
            Index *tmp = new Index(a->first, line_t->y1, line_t->y2);
            tmp->Lptr = line_t;
            if (root == nullptr)
            {
                root = tmp;
            }
        }
        else
        {
            /*if (skip == true)
                {
                    break;
                }*/
            while (line_t->y2 >= temp->big)
            {
                if (line_t->y1 >= temp->low && line_t->y2 <= temp->big)
                {
                    //skip = true;
                    break;
                }
                else if (temp->next != nullptr || (line_t->y1 < temp->low && line_t->y2 >= temp->big) || (line_t->y1 >= temp->low && line_t->y1 < temp->big && line_t->y2 >= temp->big))
                {
                    if (line_t->y1 < temp->low && line_t->y2 >= temp->big)
                    {

                        Index *n = new Index(a->first, line_t->y1, temp->low, temp);
                        Line *ycut = ycutfront(line_t, temp);
                        n->Lptr = ycut;
                        yleft.insert(make_pair(a->first, ycut));
                        if (root == temp)
                        {
                            temp->prev = n;
                            root = n;
                        }
                        else
                        {
                            n->prev = temp->prev;
                            temp->prev->next = n;
                            temp->prev = n;
                        }
                        if (temp->next != nullptr)
                        {
                            temp = temp->next;
                        }
                        else
                        {
                            break;
                        }
                        //temp = temp->next;
                    }
                    else if (line_t->y1 >= temp->low && line_t->y1 < temp->big && line_t->y2 >= temp->big)
                    {
                        //ycutback(&line_t,temp);
                        line_t->y1 = temp->big;
                        if (temp->next != nullptr)
                        {
                            temp = temp->next;
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        if (temp->next != nullptr)
                        {
                            temp = temp->next;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                else
                {
                    line_t->isContour = true;
                    yleft.insert(make_pair(a->first, line_t));
                    if (line_t->y1 < temp->low && line_t->y2 <= temp->low)
                    {
                        Index *n = new Index(a->first, line_t->y1, line_t->y2, temp);
                        n->Lptr = line_t;
                        if (temp->prev == nullptr)
                        {
                            root = n;
                            temp->prev = n;
                        }
                        else
                        {
                            n->prev = temp->prev;
                            temp->prev->next = n;
                            temp->prev = n;
                        }
                    }
                    else if (line_t->y1 >= temp->low && line_t->y2 > temp->big)
                    {
                        Index *n = new Index(temp, a->first, line_t->y1, line_t->y2);
                        n->Lptr = line_t;
                        if (temp->next == nullptr)
                        {
                            temp->next = n;
                        }
                        else
                        {
                            n->next = temp->next;
                            temp->next->prev = n;
                            temp->next = n;
                        }
                    }
                    temp = temp->next;
                    break;
                }
            }
            if (temp != nullptr)
            {
                if (line_t->y1 < temp->low && line_t->y2 <= temp->low)
                //if (line_t->y1 >= temp->big && line_t->y2 > temp->big)
                {
                    line_t->isContour = true;
                    yleft.insert(make_pair(a->first, line_t));
                    Index *n = new Index(a->first, line_t->y1, line_t->y2, temp);
                    n->Lptr = line_t;
                    if (temp->prev == nullptr)
                    {
                        root = n;
                        temp->prev = n;
                    }
                    else
                    {
                        n->prev = temp->prev;
                        temp->prev->next = n;
                        temp->prev = n;
                    }
                }
                else if (line_t->y1 < temp->low && line_t->y2 <= temp->big)
                {
                    Line *sub = new Line(line_t->x1, line_t->y1, line_t->x2, temp->low, line_t->type);
                    sub->isContour = true;
                    Index *n = new Index(a->first, line_t->y1, temp->low, temp);
                    n->Lptr = sub;
                    yleft.insert(make_pair(a->first, sub));
                    if (temp->prev == nullptr)
                    {
                        root = n;
                        temp->prev = n;
                    }
                    else
                    {
                        n->prev = temp->prev;
                        temp->prev->next = n;
                        temp->prev = n;
                    }
                }
                // (line_t->y1 < temp->low && line_t->y2 <= temp->low)
                else if (line_t->y1 >= temp->big && line_t->y2 > temp->big)
                {
                    line_t->isContour = true;
                    yleft.insert(make_pair(a->first, line_t));
                    Index *n = new Index(temp, a->first, line_t->y1, line_t->y2);
                    n->Lptr = line_t;
                    if (temp->next == nullptr)
                    {
                        temp->next = n;
                    }
                    else
                    {
                        n->next = temp->next;
                        temp->next->prev = n;
                        temp->next = n;
                    }
                }
            }
        }
    }
}
Line *ycutfront(Line *temp, Index *a)
{
    Line *sub = new Line(temp->x1, temp->y1, temp->x2, a->low, temp->type);
    sub->isContour = true;
    (temp->y1) = (a->big);
    return sub;
}
Line *xcutfront(Line *temp, Index *a)
{
    Line *sub = new Line(temp->x1, temp->y1, a->low, temp->y2, temp->type);
    sub->isContour = true;
    (temp->x1) = (a->big);
    return sub;
}
void ycutback(Line *temp, Index *a)
{
}
void print(std::vector<Line *> &line_ptr, int x, int y, int xmin, int ymin)
{
    vector<vector<int>> print(y, vector<int>(x, 0));
    for (int i = 0; i < line_ptr.size(); i++)
    {
        auto lptr = line_ptr[i];
        /*if (print[28][56] == 1)
        {
            cout << "fuck";
        }
        */
        if (lptr->type == 1) //verti
        {
            float yl, ys;
            //lptr->y1 > lptr->y2 ? (yl = lptr->y1, ys = lptr->y2) : (yl = lptr->y2, ys = lptr->y1);
            ys = lptr->y1;
            yl = lptr->y2;
            for (int k = ys; k <= yl; k++)
            {
                print[k - ymin][lptr->x1 - xmin] = 1;
            }
        }
        else if (lptr->type == 0) //horiz
        {
            /*if (lptr->x1 <= lptr->x2)
            {
                for (int j = lptr->x1 - xmin; j <= lptr->x2 - xmin; j++)
                {
                    print[lptr->y1 - ymin][j] = 1;
                }
            }
            else if (lptr->x1 > lptr->x2)
            {
                for (int j = lptr->x2 - xmin; j <= lptr->x1 - xmin; j++)
                {
                    print[lptr->y1 - ymin][j] = 1;
                }
            }*/
            for (int j = lptr->x1; j <= lptr->x2; ++j)
            {
                print[lptr->y1 - ymin][j - xmin] = 1;
            }
        }
        /*else if (lptr->type == 2) //slash
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
        }*/
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
    cout << "\n"
         << endl;
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
bool ydfs(multimap<float, Line *> &y, int ysize, int yfirst, int ypos)
{
    //auto ynextup=y.upper_bound(ypos);
    //auto ynextlow=y.lower_bound(ypos);

    auto ynext = y.equal_range(ypos);
    //found nothing
    if (ynext.first == ynext.second)
        return false;
    for (auto ret = ynext.first; ret != ynext.second; ++ret)
    {
        //if y2 reached ymax
        if (ret->second->y2 == ysize + yfirst)
            return true;
        else //continue dfs
            ydfs(y, ysize, yfirst, ret->second->y2);
    }
}
bool xydfs(multimap<pair<float, float>, Line *> &xy, pair<float, float> xystart, pair<float, float> xypos, pair<float, float> xysize)
{
    auto xynext = xy.equal_range(xypos);
    //found nothing
    if (xynext.first == xynext.second)
        return false;
    for (auto ret = xynext.first; ret != xynext.second; ++ret)
    {
        //if y2 reached ymax
        if (ret->second->y2 == xystart.second && ret->second->x2 == xystart.first)
            return true;
        else //continue dfs
            //make_pair(ret->second->x2,ret->second->y2)
            xydfs(xy, xystart, make_pair(ret->second->x2, ret->second->y2), xysize);
    }
}
