#pragma once
#ifndef CLASS_HPP
#define CLASS_HPP
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
using std::map;
using std::multimap;
using std::pair;
using std::vector;
using namespace std;
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
    Line *deepCopy()
    {
        return new Line(this->x1, this->y1, this->x2, this->y2, this->type);
    }
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
    Index *copy()
    {
        return new Index(this->prev, this->index, this->low, this->big, this->next, this->Lptr);
    }
    Index *deepCopy()
    {
        Line *nLptr = this->Lptr->deepCopy();
        Index *nptr = new Index(this->index, this->low, this->big, nLptr);
        Index *new_next_ptr = this->next->deepCopy(nptr);
        nptr->next = new_next_ptr;
        return nptr;
    }
    Index *deepCopy(Index *new_front)
    {
        if (this->next != nullptr)
        {
            return new Index(new_front, this->index, this->low, this->big, this->next->deepCopy(this), this->Lptr->deepCopy());
        }
        else
        {
            return new Index(new_front, this->index, this->low, this->big, nullptr, this->Lptr->deepCopy());
        }
    }
    Index(Index *prev, float index, float low, float big, Index *next, Line *Lptr) : index(index), low(low), big(big), next(next), prev(prev), Lptr(Lptr){};
    Index(float index, float low, float big) : index(index), low(low), big(big), next(nullptr), prev(nullptr){};
    Index(float index, float low, float big, Line *Lptr) : index(index), low(low), big(big), Lptr(Lptr){};
    Index(float index, float low, float big, Index *next) : index(index), low(low), big(big), next(next), prev(nullptr){};
    Index(Index *prev, float index, float low, float big) : index(index), low(low), big(big), next(nullptr), prev(prev){};
};
class Point
{
public:
    float x, y;
    Point(int x, int y) : x(x), y(y){};
    Point() : x(0), y(0){};
    Point(float x, float y) : x(x), y(y){};
    void doceil(){
        if (this->x > 0)
        {
            this->x = ceil(this->x);
        }
        else
        {
            this->x = floor(this->x);
        }
        if (this->y > 0)
        {
            this->y = ceil(this->y);
        }
        else
            this->y = floor(this->y);
    }
    void doRoundUp(){
        /*if(this->x >0){
            this->x = ((int)(this->x * 10000 + 0.5))/10000;
        }
        else{
            this->x = ((int)(this->x * 10000 - 0.5))/10000;
        }
        if(this->y >0){
            this->y = ((int)(this->y * 10000 + 0.5))/10000;
        }
        else{
            this->y = ((int)(this->y * 10000 - 0.5))/10000;
        }
        */
        this->x = roundf(this->x * 10000) / 10000;
        this->y = roundf(this->y * 10000) / 10000;
    }
    /*Point(float _x, float _y)
    {
        if (_x > 0)
        {
            x = ceil(_x);
        }
        else
        {
            x = floor(_x);
        }
        if (_y > 0)
        {
            y = ceil(_y);
        }
        else
            y = floor(_y);
    }*/
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
    Point(const Point &point) : x(point.x), y(point.y){};
    Point operator+(const Point &p)
    {
        return Point(this->x + p.x, this->y + p.y);
    }
    Point operator-(const Point &p)
    {
        return Point(this->x - p.x, this->y - p.y);
    }
    Point operator*(const double &p)
    {
        return Point((float)(this->x * p), (float)(this->y * p));
    }
    double operator*(const Point &p)
    {
        return this->x * p.x + this->y * p.y;
    }
    double operator*=(const Point &p)
    {
        return this->x * p.y - this->y * p.x;
    }
};
class Arc
{
public:
    float x1, x2, y1, y2, cx, cy;
    bool direction; //true for CW; false for CCW
    Arc(float x1, float y1, float x2, float y2, float cx, float cy, bool direction) : x1(x1), y1(y1), x2(x2), y2(y2), cx(cx), cy(cy), direction(direction){};
};

class Point2D
{
public:
    float x;
    float y;
    Point2D(float x, float y) : x(x), y(y){};
    Point2D(const Point2D &point) : x(point.x), y(point.y){};
    Point2D operator+(const Point2D &p)
    {
        return Point2D(this->x + p.x, this->y + p.y);
    }
    Point2D operator-(const Point2D &p)
    {
        return Point2D(this->x - p.x, this->y - p.y);
    }
    Point2D operator*(const double &p)
    {
        return Point2D(this->x * p, this->y * p);
    }
    double operator*(const Point2D &p)
    {
        return this->x * p.x + this->y * p.y;
    }
    double operator*=(const Point2D &p)
    {
        return this->x * p.y - this->y * p.x;
    }
};
//some useful universal func
void main_contest();
inline void yprint(Index *&toPrint);
inline void xprint(Index *&);
vector<Point> getChain(vector<Point> &point_vec);
void ysolve(int i, multimap<float, Line *> &x2ymptr, multimap<float, Line *> &yleft, Index *&root, int xbegin, int xsize, int ybegin, int ysize);
void xsolve(int i, multimap<float, Line *> &x2ymptr, multimap<float, Line *> &yleft, Index *&root, int xbegin, int xsize, int ybegin, int ysize);
Line *ycutfront(Line *temp, Index *a);
Line *xcutfront(Line *temp, Index *a);
multimap<float, Line *> yleft_cal(bool left, multimap<float, Line *> &x2ymptr, int xbegin, int xsize, int ybegin, int ysize);
multimap<float, Line *> xleft_cal(bool left, multimap<float, Line *> &x2ymptr, int xbegin, int xsize, int ybegin, int ysize);
void ycutback(Line *temp, Index *a);
void print(vector<Line *> &line_ptr, const int x, const int y, const int xmin, const int ymin);
void print(vector<Line *> &);
void IndexLptrPrint(Index *&);
void arcpts();
void printALL();
bool ycheck(vector<Line> &yChecked, int ysize, int yfirst);
bool ydfs(multimap<float, Line *> &y, int ysize, int yfirst, int ypos);
bool xyfs(multimap<pair<float, float>, Line *> &xy, pair<float, float> &xystart, pair<float, float> &xypos, pair<float, float> xysize);
void mmapPrint(multimap<float, Line *> &);
vector<Line *> mmaptovec(multimap<float, Line *> &);
void yIndexptrprint(Index *&yptr);
void xIndexptrprint(Index *&yptr);
//void vec_print(vector<Line*>&);
//vector<Line*> combine(vector<Line*>&a,multimap<float,Line*>&);
bool compare(const Point &a, const Point &b);
void combine(vector<Line *> &a, multimap<float, Line *> &);
void update(int, int, int, int);
void cleanAllIndex();
void search();
void cleanAll();
vector<Point> doExpand(float dist, vector<Point> &vec);
void output(vector<Line *> &line_ptr, const int choice);
void output(vector<Point> &vec, const int choice,string outpath);
//some gloal variable here
extern Index *v_left;
extern Index *v_right;
extern Index *h_up;
extern Index *h_down;
extern multimap<float, Line *> yleft;
extern multimap<float, Line *> yright;
extern multimap<float, Line *> xup;
extern multimap<float, Line *> xdown;
extern int _xsize;
extern int _ysize;
extern int _x;
extern int _y;
#endif