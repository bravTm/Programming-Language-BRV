#include <iostream>
using namespace std;

///////////////////////////   SEARCH STRING   ----------------------------------------------
inline void symbolByIndex(string &metstr, string item){
    string idx;
    for(int j = 1; j < item.size(); j++){
        if(item[j] == ']') break;

        idx += item[j];
    }

    if(isNumber(idx)){
        if(stoi(idx) > metstr.size() - 1 || stoi(idx) < 0) {
            cout << "STRINGERROR: Index out of line";
            exit(-1);
        }
    }
    else {
        cout << "STRINGERROR: Index out of line";
        exit(-1);
    }

    metstr = metstr[stoi(idx)];
}

// для методов
void methodDo(string &metstr, string item){
    int i = 0;
    if(item == ".to_upper"){
        while (i < metstr.length()) {
            metstr[i] = toupper(metstr[i]);
            i = i + 1; // optimization
        }
    }
    else if(item == ".to_lower"){
        while (i < metstr.length()) {
            metstr[i] = tolower(metstr[i]);
            i = i + 1; // optimization
        }
    }
    else if(item == ".reverse"){
        i = metstr.length();
        string s = "";
        while (i >= 0) {
            s += metstr[i];
            i = i - 1; // optimization
        }

        metstr = s;
    }
    else {
        cout << "STRINGERROR: Not found method";
        exit(-1);
    }
}

// определяет все строки
string searchString(int index, vector <vector <string>> tokens){
    string res = "", prevTokenType = "";
    string metstr = ""; // для работы с методами
    bool error = false;
    int idx = index;

    for(int i = index; i < tokens.size() - 1; i++){

        if(tokens[i][0] == "SEMICOLON" || 
            (tokens[i][0] != "STRING" && tokens[i][0] != "METHODSTR" && tokens[i][0] != "OPERATOR" && tokens[i][0] != "INDEX")){
            break;
        }
        idx = idx + 1;

        if(tokens[i+1][0] == "METHODSTR" && tokens[i][0] == "STRING") {
            metstr += tokens[i][1].substr(1, tokens[i][1].size() - 2);
            continue;
        }
        else if(tokens[i+1][0] == "INDEX" && tokens[i][0] == "STRING") {
            metstr += tokens[i][1].substr(1, tokens[i][1].size() - 2);
            continue;
        }
        else if(tokens[i][0] == "INDEX"){
            symbolByIndex(metstr, tokens[i][1]);
        }
        else if(tokens[i][0] == "METHODSTR"){
            methodDo(metstr, tokens[i][1]);
        }

        prevTokenType = tokens[i][0];

        if(tokens[i][0] == "OPERATOR"){
            if(tokens[i][1] == "+") continue;
            else break;
        }

        if(tokens[i][0] == "METHODSTR" && metstr.size() > 0) {
            res += metstr;
            metstr = "";
        }
        else if(tokens[i][0] == "INDEX" && metstr.size() > 0) {
            res += metstr;
            metstr = "";
        }
        else res += tokens[i][1].substr(1, tokens[i][1].size() - 2);
    }


    if(tokens[tokens.size() - 1][0] == "METHODSTR" && idx == tokens.size() - 1){
        methodDo(metstr, tokens[tokens.size() - 1][1]);
        res += metstr;
        metstr = "";
    }
    else if(tokens[tokens.size() - 1][0] == "INDEX" && idx == tokens.size() - 1){
        symbolByIndex(metstr, tokens[idx][1]);
        res += metstr;
        metstr = "";
    }
    else if(tokens[tokens.size() - 1][0] == "STRING" && idx == tokens.size() - 1) {  
        res += tokens[tokens.size() - 1][1].substr(1, tokens[tokens.size() - 1][1].size() - 2);
    }

    return res;
}