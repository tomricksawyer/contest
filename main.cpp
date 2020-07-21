#include "class.hpp"
using namespace std;
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
int main()
{
    int dotimes;
    string operation;
    getline(cin, operation);
    dotimes = count(operation.begin(), operation.end(), ',');
    ofstream fout;
    string path = __FILE__;
    path = path.substr(0, 1 + path.find_last_of('\\'));  //removes file name
    path += "output.txt";                                //adds input file to path
    fout.open(path, ios::out | ios::trunc);
    fout << operation << "\n";
    fout.close();
    for (int i = 1; i <= dotimes;++i){
        main_contest();
        cleanAll();
    }
}
