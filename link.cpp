#include "class.hpp"
using namespace std;
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
    }else
    {
        ptr->prev->next = nullptr;
    }
    delete (ptr);
}
void cleanIndex(Index *&ptr)
{
    auto cur = ptr;
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
        cur = next;
    }
}
void cleanAllIndex()
{
    cleanIndex(h_up);
    cleanIndex(h_down);
    cleanIndex(v_left);
    cleanIndex(v_right);
    cleanIndex(h_up);
    cleanIndex(h_down);
    cleanIndex(v_left);
    cleanIndex(v_right);
}
void search()
{
    auto tptr = h_down;
    auto nptr = v_right;
    auto tmap = xdown;
    auto nmap = yright;

    auto cur = tptr;
    while(cur != nullptr){
        
    }
    //get next
}