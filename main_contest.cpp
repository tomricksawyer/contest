#include "class.hpp"
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
Index *v_left = nullptr;
Index *v_right = nullptr;
Index *h_up = nullptr;
Index *h_down = nullptr;
multimap<float, Line *> yleft;
multimap<float, Line *> yright;
multimap<float, Line *> xup;
multimap<float, Line *> xdown;
bool Point::isSizeUpdated = false;
int Point::__xbegin = INT_MIN;
int Point::__xsize = 0;
int Point::__ybegin = INT_MIN;
int Point::__ysize = 0;
void cleanAll();
int main(int argc, char *argv[])
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    ifstream fin;
    string path = __FILE__;
    path = path.substr(0, 1 + path.find_last_of('\\')); //removes file name
    path += argv[1];
    fin.open(path, ios::in);
    int dotimes;
    string operation;
    getline(fin, operation);
    dotimes = count(operation.begin(), operation.end(), ',');
    ofstream fout;
    path = path.substr(0, 1 + path.find_last_of('\\')); //removes file name
    path += argv[2];                                    //adds input file to path
    fout.open(path, ios::out | ios::trunc);
    fout << operation << "\n";
    fout.close();

    for (int i = 1; i <= dotimes; i++)
    {
        //map store for x -> multiend y point
        multimap<float, Line *> x2ymptr;
        multimap<float, Line *> y2xmptr;
        multimap<float, Line *> slashx2ymptr;
        multimap<float, Line *> slashy2xmptr;
        vector<Point> point_vec;
        Point::init();
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
        //auto buf = cin.rdbuf();
        //ifstream fin;
        //string path = __FILE__;
        int choice = 0;
        string a;
        getline(fin, a);
        //path = path.substr(0, 1 + path.find_last_of('\\')); //removes file name
        //path += "input.txt";                                //adds input file to path
        //fin.open(path, ios::in);
        while (true) //wait for input
        {
            getline(fin, input);
            input.pop_back(); //get rid of ';'
            istringstream ss(input);
            string token;
            float x1, y1, x2, y2, cx, cy;
            int type;
            bool rotation;
            getline(ss, token, ',');
            if (token == "End" || fin.eof())
            {
                break;
            }
            if (token == "Data")
            {
                getline(ss, data_index);
                data_index.erase(data_index.begin());
                choice = stoi(data_index);
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
                    type = 2;  // slash
                if (type == 1) //verti x1==x2
                {
                    if (y1 >= y2)
                        swap(ys, yb);
                    for (int i = ys; i != yb; i++)
                    {
                        point_vec.push_back(Point((int)xs, i));
                    }
                }
                else if (type == 0) // horiz y1==y2
                {
                    if (x1 >= x2)
                        swap(xs, xb);
                    for (int i = xs; i <= xb; i++)
                    {
                        point_vec.push_back(Point(i, (int)ys));
                    }
                }
                else //slash
                {
                    float m = (y1 - y2) / (x1 - x2);
                    for (int a = 0; a <= abs(x1 - x2); ++a)
                    {
                        float i = x1, j = y1;
                        i += a * m;
                        j += a * m;
                        point_vec.push_back(Point(ceil(i), ceil(j)));
                    }
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
                //arc_vec.push_back(Arc(x1, x2, y1, y2, cx, cy, rotation));

                Point obj1(tx2, ty2);
                Point obj2(tx2, cy);
                Point obj3(tx2, ty1);
                Point obj4(cx, ty2);
                Point obj5(cx, ty1);
                Point obj6(tx1, ty2);
                Point obj7(tx1, cy);
                Point obj8(tx1, ty1);
                point_vec.push_back(obj1);
                point_vec.push_back(obj2);
                point_vec.push_back(obj3);
                point_vec.push_back(obj4);
                point_vec.push_back(obj5);
                point_vec.push_back(obj6);
                point_vec.push_back(obj7);
                point_vec.push_back(obj8);
            }
        }
        vector<Point> Andrew_Chain = getChain(point_vec);
        Point::print(point_vec);
        vector<Point> pChain = doExpand(expand, Andrew_Chain);
        output(pChain, choice, string(argv[2]));
        cleanAll();
    }
    return 0;
}
void cleanAll()
{
    v_left = nullptr;
    v_right = nullptr;
    h_up = nullptr;
    h_down = nullptr;
    yleft.clear();
    yright.clear();
    xup.clear();
    xdown.clear();
}
