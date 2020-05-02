#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
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
double cross(const Point &o, const Point &a,const Point &b);
bool compare(const Point &a,const Point &b);
int _equal(const linear a,const Point b);
vector<Point> getChain(const vector<Point> &point_vec);
void print(const vector<linear> &line_vec,const int x,const int y,const int xmin,const int ymin);
bool sort_verti(const vector<float> &a,const vector<float> &b);
bool sort_horiz(const vector<float> &a,const vector<float> &b);

int main()
{
    //map store for x -> multiend y point
    map<float, set<float>> x2yMap;
    map<float, set<float>> y2xMap;
    map<float, set<float>> slash;

    //string use for stringstream->stof()

    string input;
    string data_index;
    string _expand;
    string _NotchSize;
    float expand;
    float NotchSize;

    //vector for line & point storage

    vector<vector<float>> line;
    vector<linear> line_vec;
    vector<Arc> arc_vec;
    vector<Point> point_vec;
    vector<float> xy;
    vector<vector<float>> verti;
    vector<vector<float>> horiz;
    vector<vector<float>> slashv;

    int type;
    while (true) //wait for input
    {
        cout << "Which input to Test?" << endl
             << "1. Q1" << endl
             << "2. Q2" << endl
             << "3. Q3\n"
             << endl;

        ifstream fin;
        int choice;
        cin >> choice;
        switch (choice)
        {
        case 1:
            fin.open("Q1.txt", ios::in);
            break;
        case 2:
            fin.open("Q2.txt", ios::in);
            break;
        case 3:
            fin.open("Q3.txt", ios::in);
            break;
        }
        getline(fin, input);
        if (input.empty())
        {
            break;
        }
        input.pop_back(); //get rid of ';'
        istringstream ss(input);
        string token;
        float x1, y1, x2, y2, cx, cy;
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
            if (x1 == x2) // verti
                type = 1;
            else if (y1 == y2) //horiz
                type = 0;
            else
                type = 2; // slash
            //point_vec.push_back(Point(x1, y1));
            //point_vec.push_back(Point(x2, y2));
            if (type == 1)
            {
                switch (y2 >= y1) //verti x1==x2
                {
                case true:
                    line.push_back(vector<float>{0, 1, x1, y1, x2, y2});
                    verti.push_back(vector<float>{x1, y1, y2});
                    break;
                case false:
                    line.push_back(vector<float>{0, 1, x1, y2, x2, y1});
                    verti.push_back(vector<float>{x1, y2, y1});
                    break;
                default:
                    line.push_back(vector<float>{0, 1, x1, y1, x2, y2});
                    verti.push_back(vector<float>{x1, y1, y2});
                    break;
                }
            }
            else if (type == 0) // horiz y1==y2
            {
                switch (x2 >= x1)
                {
                case true:
                    line.push_back(vector<float>{0, 0, x1, y1, x2, y2});
                    horiz.push_back(vector<float>{y1, x1, x2});
                    break;
                case false:
                    line.push_back(vector<float>{0, 0, x2, y1, x1, y2});
                    horiz.push_back(vector<float>{y1, x2, x1});
                    break;
                default:
                    line.push_back(vector<float>{0, 0, x1, y1, x2, y2});
                    horiz.push_back(vector<float>{y1, x1, x2});
                    break;
                }
            }

            else if (type == 2)
            { //slash
                line.push_back(vector<float>{0, 2, x1, y1, x2, y2});
                slashv.push_back(vector<float>{x1, y1, x2, y2});
            }
            line_vec.push_back(linear(x1, y1, x2, y2, type));

            //Map creation
            if (type == 1)
            {
                if (x2yMap.count(x1) == 0)
                {
                    x2yMap.insert(pair<float, set<float>>(x1, set<float>{y1}));
                }
                else
                {
                    x2yMap.find(x1)->second.insert(y1);
                }
                if (x2yMap.count(x2) == 0)
                {
                    x2yMap.insert(pair<float, set<float>>(x2, set<float>{y2}));
                }
                else
                {
                    x2yMap.find(x2)->second.insert(y2);
                }
            }
            if (type == 0)
            {
                if (y2xMap.count(y1) == 0)
                {
                    y2xMap.insert(pair<float, set<float>>(y1, set<float>{x1}));
                }
                else
                {
                    y2xMap.find(y1)->second.insert(x1);
                }
                if (y2xMap.count(y2) == 0)
                {
                    y2xMap.insert(pair<float, set<float>>(y2, set<float>{x2}));
                }
                else
                {
                    y2xMap.find(y2)->second.insert(x2);
                }
            }
            else if (type == 2)
            {
                if (y2xMap.count(y1) == 0)
                {
                    y2xMap.insert(pair<float, set<float>>(y1, set<float>{x1}));
                }
                else
                {
                    y2xMap.find(y1)->second.insert(x1);
                }
                if (y2xMap.count(y2) == 0)
                {
                    y2xMap.insert(pair<float, set<float>>(y2, set<float>{x2}));
                }
                else
                {
                    y2xMap.find(y2)->second.insert(x2);
                }
            }
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
            arc_vec.push_back(Arc(x1, x2, y1, y2, cx, cy, rotation));
            Point obj1(x1, y1);
            Point obj2(x2, y2);
            point_vec.push_back(obj1);
            point_vec.push_back(obj2);
        }
    }
    //call Andrew_chain
    vector<Point> Andrew_Chain = getChain(point_vec);

    /*
    bool inspect = false;
    for (int i = 0; i < 100; i++)
    {
        if (_equal(line_vec[i], CH[i]) == 1)
        {
            for (int j = 0; j < 100; j++)
            {
                if (_equal(line_vec[i], CH[j]) == 2)
                {
                    inspect = true;
                }
            }
        }
        if (!inspect)
        {
        }
    }
*/
    //print input

    int x = (--(x2yMap.end()))->first - (x2yMap.begin()->first);
    x++;
    int y = (--(y2xMap.end()))->first - (y2xMap.begin()->first);
    y++;
    int xmin = (x2yMap.begin()->first);
    int ymin = (y2xMap.begin()->first);
    print(line_vec, x, y, xmin, ymin);

    vector<Point> xSort(point_vec);
    vector<Point> ySort(point_vec);
    sort(horiz.begin(), horiz.end(), sort_horiz);
    sort(verti.begin(), verti.end(), sort_verti);

    //sort(line_vec.begin(), line_vec.end());
    //sort(xSort.begin(), xSort.end(), xcomp);
    //sort(ySort.begin(), ySort.end(), ycomp);
    //vector<linear> marked;

    //start from xmin for verti
    //assume map's key is already sorted
    for (auto &it : x2yMap)
    {
        for (int i = 0; i < line_vec.size(); i++)
        {
            if (line_vec[i].type == 1 && line_vec[i].x1 == it.first)
            {
                //deter y1 < y2?
                auto a = line_vec[i];
                //locate ysmall and ylarge
                float yl, ys;
                a.y1 > a.y2 ? (yl = a.y1, ys = a.y2) : (yl = a.y2, ys = a.y1);
                //delete element between ys and yl
                if (it.second.find(ys) == it.second.end() || it.second.find(yl) == it.second.end())
                {
                    continue;
                }
                else
                    it.second.erase(++(it.second.find(ys)), it.second.find(yl));
            }
        }
    }
    //start again from ymin
    for (auto &it : y2xMap)
    {
        for (int i = 0; i < line_vec.size(); i++)
        {
            if (line_vec[i].type == 0 && line_vec[i].y1 == it.first)
            {
                //deter x1 < x2?
                auto a = line_vec[i];
                //locate xsmall and xlarge
                float xl, xs;
                a.x1 > a.x2 ? (xl = a.x1, xs = a.x2) : (xl = a.x2, xs = a.x1);
                //delete element between xs and xl
                if (it.second.find(xs) == it.second.end() || it.second.find(xl) == it.second.end())
                {
                    continue;
                }
                else
                    it.second.erase(++(it.second.find(xs)), it.second.find(xl));
            }
        }
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
double cross(const Point &o, const Point &a,const Point &b)
{
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}
bool compare(const Point &a,const Point &b)
{
    return (a.x < b.x) || (a.x == b.x && a.y < b.y);
}
/*void Andrew_monotone_chain()
{
    sort(P, P + 100, compare);
    int m = 0;

    for (int i = 0; i < 100; ++i)
    {
        while (m >= 2 && cross(CH[m - 2], CH[m - 1], P[i]) <= 0)
            m--;
        CH[m++] = P[i];
    }

    for (int i = 98, t = m + 1; i >= 0; --i)
    {
        while (m >= t && cross(CH[m - 2], CH[m - 1], P[i]) <= 0)
            m--;
        CH[m++] = P[i];
    }

    m--;
}*/
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
    m--;
    return CH;
}
void print(vector<linear> &line_vec, int x, int y, int xmin, int ymin)
{
    vector<vector<int>> print(y, vector<int>(x, 0));
    for (int i = 0; i < line_vec.size(); i++)
    {
        if (line_vec[i].type == 1)
        {
            for (int j = 0; j < print.size(); j++)
            {
                float yl, ys;
                line_vec[i].y1 > line_vec[i].y2 ? (yl = line_vec[i].y1, ys = line_vec[i].y2) : (yl = line_vec[i].y2, ys = line_vec[i].y1);
                for (int k = ys; k <= yl; k++)
                {
                    print[k - ymin][line_vec[i].x1 - xmin] = 1;
                }
            }
        }
        if (line_vec[i].type == 0)
        {
            if (line_vec[i].x1 < line_vec[i].x2)
            {
                for (int j = line_vec[i].x1 - xmin; j < line_vec[i].x2 - xmin; j++)
                {
                    print[line_vec[i].y1 - ymin][j] = 1;
                }
            }
        }
    }
    for (int i = 0; i < print.size(); i++)
    {
        for (int j = 0; j < print[i].size(); j++)
        {
            print[i][j] ? (cout << "+") : (cout << " ");
        }
        cout << endl;
    }
}
bool sort_verti(const vector<float> &a, const vector<float> &b)
{
    return a[0] < b[0];
}
bool sort_horiz(const vector<float> &a, const vector<float> &b)
{
    return a[0] < b[0];
}
