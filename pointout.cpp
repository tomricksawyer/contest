#include "class.hpp"
using namespace std;
bool isFirsttime = true;
vector<Line *> Point2Line(vector<Point> &vec)
{
    vector<Line *> P2L;
    P2L.clear();
    for (int i = 1; i < vec.size(); i++)
    {
        P2L.push_back(new Line(vec[i - 1].x, vec[i - 1].y, vec[i].x, vec[i].y, 2));
    }
    P2L.push_back(new Line(vec[vec.size() - 1].x, vec[vec.size() - 1].y, vec[0].x, vec[0].y, 2));
    return P2L;
}
void output(vector<Line *> &line_ptr, const int choice)
{
    ofstream fout;
    string path = __FILE__;
    path = path.substr(0, 1 + path.find_last_of('\\'));  //removes file name
    path += "output.txt";                                //adds input file to path
    fout.open(path, ios::out | ios::app);
    fout << "Data,Q" << choice << ";" << endl;
    for (int i = 0; i < line_ptr.size(); i++)
    {
        auto lptr = line_ptr[i];
        float xs = lptr->x1;
        float xb = lptr->x2;
        float ys = lptr->y1;
        float yb = lptr->y2;
        fout << "Line," << xs << "," << ys << "," << xb << "," << yb << ";" << endl;
    }
    fout << "End,Q" << choice << ";";
}
void output(vector<Point>&vec, const int choice)
{
    vector<Line *> line_ptr = Point2Line(vec);
    ofstream fout;
    string path = __FILE__;
    path = path.substr(0, 1 + path.find_last_of('\\'));  //removes file name
    path += "output.txt";                                //adds input file to path
    fout.open(path, ios::out | ios::app);
    fout << "\nData,Q" << choice << ";" << endl;
    for (int i = 0; i < line_ptr.size(); i++)
    {
        auto lptr = line_ptr[i];
        float xs = lptr->x1;
        float xb = lptr->x2;
        float ys = lptr->y1;
        float yb = lptr->y2;
        fout << "Line," << xs << "," << ys << "," << xb << "," << yb << ";" << endl;
    }
    fout << "End,Q" << choice << ";"<<"\n";
}
