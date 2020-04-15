#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
class linear{
    public:
        float x1;
        float x2;
        float y1;
        float y2;
        int type;//0 for verti , 1 for horiz ,2 for slash
        linear(float x1,float y1 ,float x2,float y2) : x1(x1),y1(y1),x2(x2),y2(y2){};
        
};
class Arc{
    public:
        float x1, x2, y1, y2, cx, cy;
        bool direction; //true for CW; false for CCW
        Arc(float x1,float y1,float x2,float y2,float cx,float cy,bool direction):x1(x1),y1(y1),x2(x2),y2(y2),cx(cx),cy(cy),direction(direction){};
};
int stringtoint(string str){
    return str.compare("Line");
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
    while (true)
    {
        getline(cin, input);
        input.pop_back(); //get rid of ';'
        istringstream ss(input);
        string token;
        float x1, y1, x2, y2;
        getline(ss, token, ',');
        if(token == "Data"){
            getline(ss, data_index);
        }
        else if(token=="Expand"){
            getline(ss, _expand,',');
            expand = stof(_expand);
        }
        else if(token=="NotchSize"){
            getline(ss, _NotchSize,',');
            NotchSize = stof(_NotchSize);
        }
        else if(token=="Line"){
            string temp;
            getline(ss, temp, ',');
            x1 = stof(temp);
            getline(ss, temp, ',');
            x2 = stof(temp);
            getline(ss, temp, ',');
            y1 = stof(temp);
            getline(ss, temp, ',');
            y2 = stof(temp);
            linear obj(x1, y1, x2, y2);
            arr.push_back(obj);
        }
        else if(token=="Arc"){
            string temp;
            getline(ss, temp, ',');
            x1 = stof(temp);
            getline(ss, temp, ',');
            x2 = stof(temp);
            getline(ss, temp, ',');
            y1 = stof(temp);
            getline(ss, temp, ',');
            y2 = stof(temp);
        }
        }
    }

    return 0;
}