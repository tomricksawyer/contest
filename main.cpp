#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <set>
using namespace std;
class non_duplicate
{
    float key;
    list<float> val;
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
Point P[100], CH[101];

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
double cross(Point o, Point a, Point b)
{
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}
bool compare(Point a, Point b)
{
    return (a.x < b.x) || (a.x == b.x && a.y < b.y);
}
void Andrew_monotone_chain()
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

int main()
{
    //multimap<float, float> x2yMap;
    //multimap<float, float> y2xMap;
    map<float, set<float>> x2yMap;
    map<float, set<float>> y2xMap;
    string input;
    string data_index;
    string _expand;
    string _NotchSize;
    float expand;
    float NotchSize;
    vector<linear> line_vec;
    vector<Arc> arc_vec;
    vector<Point> point_vec;
    vector<float> xy;
    int type;
    while (true)
    {
        getline(cin, input);
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
            linear obj(x1, y1, x2, y2, type);
            line_vec.push_back(obj);
            Point obj1(x1, y1);
            Point obj2(x2, y2);
            /*x2yMap.insert(pair<float, float>(x1, y1));
            x2yMap.insert(pair<float, float>(x2, y2));
            y2xMap.insert(pair<float, float>(y1, x1));
            y2xMap.insert(pair<float, float>(y2, x2));
            */
            if(x2yMap.count(x1)==0){
                x2yMap.insert( pair<float,set<float>>(x1, set<float>{y1} ) );
            }
            else{
                x2yMap.find(x1) -> second.insert(y1);
            }
            if(x2yMap.count(x2)==0){
                x2yMap.insert( pair<float,set<float>>(x2, set<float>{y2} ) );
            }
            else{
                x2yMap.find(x2) -> second.insert(y2);
            }
            if(y2xMap.count(y1)==0){
                y2xMap.insert( pair<float,set<float>>(y1, set<float>{x1} ) );
            }
            else{
                y2xMap.find(y1) -> second.insert(x1);
            }
            if(y2xMap.count(y2)==0){
                y2xMap.insert( pair<float,set<float>>(y2, set<float>{x2} ) );
            }
            else{
                y2xMap.find(y2) -> second.insert(x2);
            }
            //case verti
            if(type == 1){
                //deter y1 < y2?
                float yl, ys;
                y1 < y2 ? (yl = y1, ys = y2) : (yl = y2, ys = y1);
                if(ys < *(x2yMap.find(x1) -> second.begin())){
                    //case ys < front < end < yl -> replace end and front (include)
                    auto it = x2yMap.find(x1) -> second;
                    if (yl > *(prev(it.end())) && ys < *(it.begin())){
                        it.erase(prev(it.end()));
                        it.insert(yl);
                        it.erase(it.begin());
                        it.insert(ys);
                    }
                    //case ys < yl < front < end -> ins ys, yl (indepedent)
                    else if(yl < *(it.begin())){
                        it.insert(yl);
                        it.insert(ys);
                    }
                    //case ys < front < yl < end -> replace front with ys (bigger front)
                    else if(ys < *(it.begin()) && *(it.begin()) < yl ){
                        it.erase(it.begin());
                        it.insert(ys);
                    } 
                    //case ys < front < yl < end -> replace front with ys (bigger end)
                    else if(ys < *(it.begin()) && *(it.begin()) < yl ){
                        it.erase(it.begin());
                        it.insert(ys);
                    } 

                    //case yl
                }
            }
            point_vec.push_back(obj1);
            point_vec.push_back(obj2);
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
    vector<Point> xSort(point_vec);
    vector<Point> ySort(point_vec);

    //sort(line_vec.begin(), line_vec.end());
    sort(xSort.begin(), xSort.end(), xcomp);
    sort(ySort.begin(), ySort.end(), ycomp);
    vector<linear> marked;
    //start from xmin for verti
    for (int i = 0; i < xSort.size(); i++)
    {
        for (int j = 0; j < line_vec.size(); i++)
        {
            if (line_vec[j].type == 1)
            {
                if (xSort[i].x == line_vec[j].x1 || xSort[i].x == line_vec[j].x2)
                {
                    marked.push_back(line_vec[j]);
                }
            }
        }
    }
    for (int i = 0; i < ySort.size(); i++)
    {
        for (int j = 0; j < line_vec.size(); i++)
        {
            if (ySort[i].y == line_vec[j].y1 || ySort[i].y == line_vec[j].y2)
            {
                if (line_vec[j].type == 2)
                {
                    marked.push_back(line_vec[j]);
                }
            }
        }
    }

    return 0;
}
