#include <iostream>
#include <vector>

using namespace std;

void input(vector <string> params, vector <string>& ans){
    if(params.size() > 1) except("FUNCTIONERROR: Input - Parameters must be 1 or 0");
    string s;
    cout << params[0] + " ";
    cin >> s;

    if(isNum(s)){
        ans.push_back("NUMBER");
        ans.push_back(s);
    }
    else{
        ans.push_back("STRING");
        ans.push_back("\"" + s + "\"");
    }
}