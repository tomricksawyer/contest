#include "class.hpp"
#include <iostream>
#include <vector>

std::vector<Point2D> pList;
std::vector<Point2D> dpList;
std::vector<Point2D> ndpList;
std::vector<Point2D> newList;
void initPList(Index *res)
{
    /*Index *last = res->prev;
    Index *cur = res;
    Index *next = res ->next;
    while(true){
        pList.push_back(Point2D(cur->Lptr->x1,cur->Lptr->y1));
        if(cur->next != nullptr){
            last = cur;
            cur = next;
            next = next->next;
        }
        else{
            break;
        }
    }
    */
    //for test 1 manualy
    /*
    pList.push_back(Point2D(-44,9));
    pList.push_back(Point2D(-44,27));
    pList.push_back(Point2D(-41,30));
    pList.push_back(Point2D(-29,30));
    pList.push_back(Point2D(-26,27));
    pList.push_back(Point2D(-26,9));
    pList.push_back(Point2D(-29,6));
    pList.push_back(Point2D(-41,6));
    */
}

void initDPList()
{
    std::cout << "dpList Calculate" << std::endl;
    int index;
    for (index = 0; index < pList.size(); ++index)
    {
        dpList.push_back(pList[index == pList.size() - 1 ? 0 : index + 1] - pList[index]);
    }
}

void initNDPList()
{
    std::cout << "calculate ndpList" << std::endl;
    int index = 0;
    for (; index < dpList.size(); ++index)
    {
        ndpList.push_back(dpList[index] * (1.0 / sqrt(dpList[index] * dpList[index])));
    }
}

void computeLine(int dist)
{
    std::cout << "Calculate New Vertex" << std::endl;
    int index = 0;
    for (; index < pList.size(); ++index)
    {
        int startIndex = index == 0 ? pList.size() - 1 : index - 1;
        int endIndex = index;
        auto sina = ndpList[startIndex] *= ndpList[endIndex];
        auto length = dist / sina;
        auto vec = ndpList[endIndex] - ndpList[startIndex];
        auto point = pList[index] + vec * length;
        newList.push_back(point);
        std::cout << "newList: " << index << " x= " << newList[newList.size() - 1].x << " ,y= " << newList[newList.size() - 1].y << std::endl;
    }
}