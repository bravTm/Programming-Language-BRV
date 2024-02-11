#include <fstream>

using namespace std;


vector <string> readFile(string filename){
    ifstream file;
    file.open(filename);

    if(file.bad() == true || !file.is_open()) {
        cout << "File wasn't opened  -> LOGOUT";
        exit(-1);
    }

    string line;
    vector <string> lines;

    if (file.is_open())
    {
        while (getline(file, line)){
            lines.push_back(line);
        }
    }

    file.close();

    return lines;
}