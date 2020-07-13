#ifndef CLASS_HPP
#define CLASS_HPP

#pragma once
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
class Arc
{
public:
    float x1, x2, y1, y2, cx, cy;
    bool direction; //true for CW; false for CCW
    Arc(float x1, float y1, float x2, float y2, float cx, float cy, bool direction) : x1(x1), y1(y1), x2(x2), y2(y2), cx(cx), cy(cy), direction(direction){};
};

class Point2D{
    public:
    float x;
    float y;
    Point2D(float x, float y) : x(x), y(y){};
    Point2D(Point2D &point) : x(point.x), y(point.y){};
    Point2D operator+(const Point2D &p){
        return Point2D(this->x + p.x, this->y + p.y);
    }
    Point2D operator-(const Point2D &p){
        return Point2D(this->x - p.x, this->y - p.y);
    }
    Point2D operator*(const double &p){
        return Point2D(this->x * p, this->y * p);
    }
    double operator*(const Point2D &p){
        return this->x * p.x + this->y * p.y;
    }
    double operator*=(const Point2D &p){
        return this->x * p.y - this->y * p.x;
    }
};
#endif