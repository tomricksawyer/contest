#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
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
};
Point P[100], CH[101];

int stringtoint(string str)
{
    return str.compare("Line");
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
int _equal(linear a,Point b)
{
    if(a.x1==b.x&&a.y1==b.y)
    {
        return 1;
    }
    else if(a.x2==b.x&&a.y2==b.y)
    {
        return 2;
    }
    else
        return 0;
}

int main()
{
    string input;
    string data_index;
    string _expand;
    string _NotchSize;
    float expand;
    float NotchSize;
    vector<linear> arr;
    vector<Arc> arr2;
    vector<Point> arr3;
    vector<float> xy;
    int type;
    while (true)
    {
        getline(cin, input);
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
            x2 = stof(temp);
            getline(ss, temp, ',');
            y1 = stof(temp);
            getline(ss, temp, ',');
            y2 = stof(temp);
            if (x1 == x2)
                type = 1;
            else if (y1 == y2)
                type = 0;
            else
                type = 2;
            linear obj(x1, y1, x2, y2, type);
            arr.push_back(obj);
            Point obj1(x1, y1);
            Point obj2(x2, y2);
            arr3.push_back(obj1);
            arr3.push_back(obj2);
        }
        else if (token == "Arc")
        {
            string temp;
            getline(ss, temp, ',');
            x1 = stof(temp);
            getline(ss, temp, ',');
            x2 = stof(temp);
            getline(ss, temp, ',');
            y1 = stof(temp);
            getline(ss, temp, ',');
            y2 = stof(temp);
            getline(ss, temp, ',');
            cx = stof(temp);
            getline(ss, temp, ',');
            cy = stof(temp);
            getline(ss, temp, ',');
            if (temp == "CW")
                rotation = 1;
            else if (temp == "CCW")
                rotation = 0;
            arr2.push_back(Arc(x1, x2, y1, y2, cx, cy, rotation));
            Point obj1(x1, y1);
            Point obj2(x2, y2);
            arr3.push_back(obj1);
            arr3.push_back(obj2);
        }
    }
    sort(arr.begin(), arr.end());
    bool inspect = false;
    for (int i = 0; i < 100;i++)
    {
        if(_equal(arr[i],CH[i]) == 1)
        {
            for (int j = 0; j < 100;j++)
            {
                if(_equal(arr[i],CH[j]) == 2)
                {
                    inspect = true;
                }
            }
        }
        if(!inspect)
        {
            
            
        }
    }
        return 0;
}
