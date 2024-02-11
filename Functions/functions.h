#include <regex>
// #include "./exception.h"

inline bool isNum(const std::string& s) {
    try {
        std::stoi(s);
    } catch (std::exception& e) {
        return false;
    }
    return true;
}


#include "./Funcs/input.h"
#include "./Funcs/maths.h"
#include "./Funcs/array.h"


vector <string> getParams(int index, string item){
    vector <string> params;
    string s;

    for(int i = index + 1; i < item.size(); i++){
        if(item[i] == ',' || item[i] == ')') {
            params.push_back(s);
            s = "";
            continue;
        }

        s += item[i];
    }

    return params;
}

//////////////////////// CHECK PARAMETRS FUNCTION ON VARIABLE ------------------------
// Проверка есть ли в параметрах переменная и если есть, то заменить значением
vector <string> checkOnVariables(string item, int idxItem, vector <vector <string>> variables){
    int index;

    for(int i = 0; i < item.size(); i++){
        if(item[i] == '(') {
            index = i;
            break;
        }
    }
    vector <string> params = getParams(index, item);

    for(int i = 0; i < params.size(); i++){
        if(params[i][0] != '@') {
            continue;
        }

        for(int j = 0; j < variables.size(); j++){
            if(params[i].substr(1, params[i].size() - 1) == variables[j][0]){
                if(!isNum(params[i]) && !isNum(variables[j][1]) && variables[j].size() != 3){
                    params[i] = "\"" + variables[j][1] + "\"";
                }
                else params[i] = variables[j][1];
            }
        }
    }

    return params;
}


bool validParams(vector <string> params, vector <string> types){
    if(params.size() != types.size()) return false;
    cmatch result;

    for(int i = 0; i < params.size(); i++){
        if(types[i] == "NUM"){
            if(!isNum(params[i])) return false;
        }

        else if(types[i] == "STR"){
            regex regular("\".*\"");
            if(!regex_match(params[i].c_str(), result, regular)) return false;
        }

        else if(types[i] == "ARRAY"){
            regex regular("[A-Za-z_0-9. \\;]+");
            if(!regex_match(params[i].c_str(), result, regular)) return false;
        }

    }

    return true;
}

vector <string> functions(string item, vector <string> params){
    vector <string> ans;
    string nameFunction = "";
    int index;

    for(int i = 0; i < item.size(); i++){
        if(item[i] == '(') {
            index = i;
            break;
        }

        nameFunction += item[i];
    }

    if(nameFunction == "input") {
        vector <string> types = {"STR"};
        if(validParams(params, types)) input(params, ans);
        else except("FUNCTIONERROR: Input invalid");
    }

    /////////////////////////////////// MATH -----------------------------------------------------
    else if(nameFunction == "pow") {
        vector <string> types = {"NUM", "NUM"};
        if(validParams(params, types)) quickPow(params, ans);
        else except("FUNCTIONERROR: Pow invalid");
    }
    else if(nameFunction == "root") {
        vector <string> types = {"NUM", "NUM"};
        if(validParams(params, types)) quickRoot(params, ans);
        else except("FUNCTIONERROR: Root invalid");
    }
    else if(nameFunction == "exp") {
        vector <string> types = {"NUM"};
        if(validParams(params, types)) quickExp(params, ans);
        else except("FUNCTIONERROR: Exp invalid");
    }
    else if(nameFunction == "ceil") {
        vector <string> types = {"NUM"};
        if(validParams(params, types)) quickCeil(params, ans);
        else except("FUNCTIONERROR: Ceil invalid");
    }
    else if(nameFunction == "floor") {
        vector <string> types = {"NUM"};
        if(validParams(params, types)) quickFloor(params, ans);
        else except("FUNCTIONERROR: Floor invalid");
    }
    else if(nameFunction == "abs") {
        vector <string> types = {"NUM"};
        if(validParams(params, types)) quickAbs(params, ans);
        else except("FUNCTIONERROR: Abs invalid");
    }
    else if(nameFunction == "factorial") {
        vector <string> types = {"NUM"};
        if(validParams(params, types)) quickFactorial(params, ans);
        else except("FUNCTIONERROR: Factorial invalid");
    }
    else if(nameFunction == "log10") {
        vector <string> types = {"NUM"};
        if(validParams(params, types)) quickLog10(params, ans);
        else except("FUNCTIONERROR: Log10 invalid");
    }

    /////////////////////// ARRAY -----------------------------------------------------
    else if(nameFunction == "array") {
        createArray(params, ans);
    }
    else if(nameFunction == "len") {
        vector <string> typesArr = {"ARRAY"};
        vector <string> typesStr = {"STRING"};

        if(validParams(params, typesArr)) lengthArrayOrString(params, ans, "ARRAY");
        else if(validParams(params, typesStr)) lengthArrayOrString(params, ans, "STRING");
        else except("FUNCTIONERROR: Len invalid");
    }
    else except("FUNCTIONERROR: Not found function " + nameFunction);

    return ans;
}