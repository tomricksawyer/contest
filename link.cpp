#include "class.hpp"
using namespace std;
int timesComb = 0;
int timesSingle = 0;
int checkroot(Index *&p)
{
    if (p == h_up)
        return 1;
    else if (p == h_down)
        return 2;
    else if (p == v_left)
        return 3;
    else if (p == v_right)
        return 4;
    else
        return 0;
}
void ptrupdate()
{
    ;
}
void popIndex(Index *&ptr)
{
    if (ptr->next != nullptr)
    {
        ptr->prev->next = ptr->next;
        ptr->next->prev = ptr->prev;
    }
    else
    {
        ptr->prev->next = nullptr;
    }
    delete (ptr);
}
void cleanIndex(Index *&ptr)
{
    auto cur = ptr;
    Index *prev = nullptr;
    while (cur != nullptr)
    {
        //if exist cur->next
        auto next = cur->next;
        if (cur->next != nullptr)
        {
            if (next->index == cur->index)
            {
                if (cur->low == cur->big && (cur->low >= next->low && cur->big <= next->big))
                {
                    timesSingle++;
                    auto a = checkroot(cur);
                    if (a != 0)
                    {
                        switch (a)
                        {
                        case 1:
                            h_up = h_up->next;
                            break;
                        case 2:
                            h_down = h_down->next;
                            break;
                        case 3:
                            v_left = v_left->next;
                            break;
                        case 4:
                            v_right = v_right->next;
                            break;
                        default:
                            break;
                        }
                        delete (cur);
                        cur = next;
                        continue;
                    }
                    else
                    {
                        popIndex(cur);
                    }
                }
                else if (next->low == next->big && (next->low >= cur->low && next->big <= cur->big))
                {
                    auto a = checkroot(next);
                    if (a != 0)
                    {
                        switch (a)
                        {
                        case 1:
                            h_up = h_up->next;
                            break;
                        case 2:
                            h_down = h_down->next;
                            break;
                        case 3:
                            v_left = v_left->next;
                            break;
                        case 4:
                            v_right = v_right->next;
                            break;
                        default:
                            break;
                        }
                        cur = next;
                        continue;
                    }
                    else
                    {
                        auto a = next->next;
                        popIndex(next);
                        next = a;
                    }
                }
            }
        }
        if (next == nullptr)
        {
            cur = next;
            continue;
        }
        if (next->index == cur->index)
        {
            if (cur->big == next->low)
            {
                auto type = cur->Lptr->type;
                if (next->next == nullptr)
                {
                    cur = next;
                    continue;
                }
                else
                {
                    timesComb++;
                    if (type == 0)
                    {
                        Line *a = new Line(cur->low, cur->index, next->big, next->index, type);
                        next->low = cur->low;
                        next->prev = prev;
                        prev->next = next;
                        next->Lptr = a;
                    }
                    else
                    { //type == 1
                        Line *a = new Line(cur->index, cur->low, next->index, next->big, type);
                        next->low = cur->low;
                        next->prev = prev;
                        prev->next = next;
                        next->Lptr = a;
                    }
                }
            }
            next = next->next;
        }
        prev = cur;
        cur = next;
    }
}
void cleanAllIndex()
{
    cleanIndex(h_up);
    cleanIndex(h_down);
    IndexLptrPrint(h_down);
    cleanIndex(v_left);

    cleanIndex(v_right);
    cleanIndex(h_up);
    cleanIndex(h_down);
    cleanIndex(v_left);
    cleanIndex(v_right);
    cout << "clean Complete";
    printALL();
}
Index *findIndex(Index *ptr, int target, int upper, int lower)
{
    auto cur = ptr;
    if (lower > upper)
    {
        swap(lower, upper);
    }
    while (cur != nullptr)
    {
        if (cur == nullptr)
        {
            break;
        }
        if (cur->index == target)
        {
            if (cur->low == lower && cur->big == upper)
            {
                return cur;
            }
            else if (cur->low >= lower && cur->big == upper)
            {
                ;
            }
        }
        else if (target - 2 <= cur->index && cur->index <= target + 2)
        {
            if (cur->low <= lower && upper <= cur->big)
            {
                auto type = ptr->Lptr->type;
                Line *l;
                Index *n;
                if (type == 1)
                {
                    l = new Line(cur->index, lower, cur->index, upper, type);
                    n = new Index(cur->index, lower, upper, l);
                }
                else if (type == 0)
                {
                    l = new Line(lower, cur->index, upper, cur->index, type);
                    n = new Index(cur->index, lower, upper, l);
                }
                return n;
            }
        }
        cur = cur->next;
    }
    auto type = ptr->Lptr->type;
    Line *l;
    Index *n;
    if (type == 1)
    {
        l = new Line(target, lower, target, upper, type);
        n = new Index(target, lower, upper, l);
    }
    else if (type == 0)
    {
        l = new Line(lower, target, upper, target, type);
        n = new Index(target, lower, upper, l);
    }
    return n;
}
void insert(Index *&front, Index *&toin)
{
    if (toin == nullptr)
        return;
    auto toins = toin->copy();
    if (front->next == nullptr)
    {
        front->next = toins;
        toins->prev = front;
        toins->next = nullptr;
    }
    else if (front->next != nullptr)
    {
        auto origin_next = front->next;
        toins->next = front->next;
        front->next = toins;
        toins->prev = front;
        origin_next->prev = toins;
    }
}
void Indexdel(Index *&a)
{
    if (a->next != nullptr)
    {
        a->prev->next = a->next;
    }
    else
    {
        a->prev->next = nullptr;
    }
}
void search_left()
{
    auto tptr = v_left;
    auto nptr = h_up;
    auto xhalf = _x + _xsize / 2;
    auto yhalf = _y + _ysize / 2;
    auto cur = tptr;
    while (cur != nullptr)
    {
        auto next = cur->next;
        if (next == nullptr)
        {
            break;
        }
        if (cur->big == next->low)
        {
            if (cur->index != next->index)
            {
                cur->big < yhalf ? nptr = h_down : nptr = h_up;
                Index *tofind = findIndex(nptr, cur->big, cur->index, next->index);
                insert(cur, tofind);
            }
        }
        cur = next;
    }
    cout << "v_left finish" << endl;
    IndexLptrPrint(tptr);
    ;
}
void search_up()
{
    auto tptr = h_up;
    auto nptr = v_right;
    auto xhalf = _x + _xsize / 2;
    auto yhalf = _y + _ysize / 2;
    auto cur = tptr;

    while (cur != nullptr)
    {
        auto next = cur->next;
        if (next == nullptr)
        {
            break;
        }
        if (cur->big == next->low)
        {
            if (cur->index != next->index)
            {
                cur->big < xhalf ? nptr = v_left : nptr = v_right;
                Index *tofind = findIndex(nptr, cur->big, cur->index, next->index);
                insert(cur, tofind);
            }
        }
        cur = next;
    }
    cout << "h_up finish" << endl;
    IndexLptrPrint(tptr);
    search_left();
}
void search_right()
{
    auto tptr = v_right;
    auto nptr = h_up;
    auto xhalf = _x + _xsize / 2;
    auto yhalf = _y + _ysize / 2;
    auto cur = tptr;
    while (cur != nullptr)
    {
        auto next = cur->next;
        if (next == nullptr)
        {
            break;
        }
        if (cur->big == next->low)
        {
            if (cur->index != next->index)
            {
                cur->big < yhalf ? nptr = h_down : nptr = h_up;
                Index *tofind = findIndex(nptr, cur->big, cur->index, next->index);
                insert(cur, tofind);
            }
        }
        cur = next;
    }
    cout << "v_right finish" << endl;
    IndexLptrPrint(tptr);
    search_up();
}
void search()
{
    cout << "\n---------searching------------\n";
    //auto tptr = h_down;
    auto tptr = h_down;
    auto nptr = v_right;
    auto xhalf = _x + _xsize / 2;
    auto yhalf = _y + _ysize / 2;
    auto cur = tptr;

    while (cur != nullptr)
    {
        auto next = cur->next;
        if (next == nullptr)
        {
            break;
        }
        if (cur->big == next->low)
        {
            if (cur->index != next->index)
            {
                cur->big < xhalf ? nptr = v_left : nptr = v_right;
                Index *tofind = findIndex(nptr, cur->big, cur->index, next->index);
                insert(cur, tofind);
                IndexLptrPrint(tptr);
            }
        }
        cur = next;
    }
    cout << "xdown finish" << endl;
    IndexLptrPrint(tptr);
    //find nextone
    search_right();
    return;
    auto nextptr = v_right;
    while (nextptr != nullptr)
    {
        if (nextptr->index == cur->big)
        {
            if (nextptr->low <= cur->index && nextptr->big > cur->index)
                break;
        }
        nextptr = nextptr->next;
    }
    cur->next = nextptr;
    cout << "connect!!!" << endl;
    IndexLptrPrint(h_down);

    printALL();

    ;
    //get next
}