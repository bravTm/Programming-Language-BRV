// импорты все идут из других файлов и main.cpp
#include "./Functions/eval.h"

// tokens[i][0] - TokenType     tokens[i][1] - Content

///////////////////////////// SECONDARY FUNCTIONS -----------------------------------------
inline bool isNumber(const std::string& s) {
    try {
        std::stoi(s);
    } catch (std::exception& e) {
        return false;
    }
    return true;
}

#include "./String/allString.h" // isNumber используется в allString.h

void upgradeIndex(int i, vector <vector <string>>& tokens, vector <vector <string>> variables){
    string idx, item = tokens[i][1];
    for(int j = 1; j < item.size(); j++){
        if(item[j] == ']') break;

        idx += item[j];
    }

    if(!isNumber(idx)) {
        tokens[i].push_back(idx.substr(1, idx.size() - 1));
        for(int j = 0; j < variables.size(); j++){
            if(idx.substr(1, idx.size() - 1) == variables[j][0]){
                tokens[i][1] = "[" + to_string(stoi(variables[j][1])) + "]";
                break;
            }
        }
    }
    else if(tokens[i].size() == 3){
        for(int j = 0; j < variables.size(); j++){
            if(tokens[i][2] == variables[j][0]){
                tokens[i][1] = "[" + to_string(stoi(variables[j][1])) + "]";
                break;
            }
        }
    }
}

#include "./Variable/variable.h" // upgradeIndex используется в variable.h

///////////////////////////   SEARCH EXPRESSION   ----------------------------------------------

// определяет все мат. выражение
string searchExpression(int index, vector <vector <string>> tokens){
    string res = "", prevTokenType = "";
    int i = index;

    while(i < tokens.size()){
        if((tokens[i][0] != "NUMBER" && tokens[i][0] != "OPERATOR")) {
            break;
        }
        res += tokens[i][1];

        prevTokenType = tokens[i][0];
        i = i + 1; // optimization
    }

    return res;
}

/////////////////////////////// IF ---------------------------


#include "./if_else/if.h" // чтобы searchExpression попадал в if.h
#include "./while/while.h" 
#include "./for/for.h" 
#include "./Functions/functions.h" // в array используется searchExpession
#include "./array/workArray.h" // isNum используется в workArray.h


///////////////////////////   VARIABLES -------------------------------------------------------

vector <string> recognizeVariable(int index, vector <vector <string>>& tokens, vector <vector <string>> variables){
    vector <string> res = {tokens[index][1].substr(1, tokens[index][1].size() - 1)};
    bool isAssign = false;

    int idx = index + 1;

    // Для обнаружения в данной строке VARIABLEUSE и записи в нее значения, для использования ниже
    while(tokens[idx][0] != "SEMICOLON"){
        if(tokens[idx][0] == "VARIABLEUSE"){
            writeVariableuse(idx, variables, tokens);
        }
        else if(tokens[idx][0] == "FUNCTION"){
            vector <string> params = checkOnVariables(tokens[idx][1], idx, variables);
            vector <string> vec = functions(tokens[idx][1], params);
            tokens[idx] = vec;
        }
        idx++;
    }

    for(int i = index + 1; i < tokens.size(); i++){
        if(tokens[i][0] == "SEMICOLON") break;

        if(tokens[i][0] == "ASSIGN") {
            isAssign = true;
            continue;
        }

        if(isAssign) {
            if(tokens[i][0] == "NUMBER") {
                res.push_back(to_string(calc(searchExpression(i, tokens))));
                break;
            }
            else if(tokens[i][0] == "STRING"){
                res.push_back(searchString(i, tokens));
                break;
            }
            else if(tokens[i][0] == "ARRAY"){
                res.push_back(tokens[i][1]);
                res.push_back(tokens[i][2]);
                break;
            }
        }
    }

    if(!isAssign) {
        except("Assignment was not successful. Variable - " + res[0]);
    }

    return res;
}


void overwriteVariable(int index, int indexVar, vector <vector <string>>& variables, vector <vector <string>>& tokens){
    vector <string> vec = recognizeVariable(index, tokens, variables);
    variables[indexVar][1] = vec[1];
}


///////////////////////////   MAIN PARSER ----------------------------------------------------------

void parser(vector <vector <string>>& tokens){
    vector <vector <string>> variables;
    bool isCircle = false; // если есть цикл(while or for) и если есть перезапись переменной где-то в теле цикла, то не делать это
    string typeCircle = "";

    for(int i = 0; i < tokens.size(); i++){
        if(tokens[i][0] == "WHILE"){
            isCircle = true;
        }
        if(tokens[i][0] == "ENDWHILE"){
            isCircle = false;
        }
        if(tokens[i][0] == "VARIABLE"){
            bool isNew = true;
            int indexVar;

            for(int j = 0 ; j < variables.size(); j++){
                if(variables[j][0] == tokens[i][1].substr(1, tokens[i][1].size() - 1)){
                    isNew = false;
                    indexVar = j;
                    break;
                }
            }

            if(isNew) variables.push_back(recognizeVariable(i, tokens, variables));
            else {
                if(!isCircle) overwriteVariable(i, indexVar, variables, tokens);
            }
        }
        else if(tokens[i][0] == "VARIABLEUSE" && tokens[i].size() != 4){
            writeVariableuse(i, variables, tokens);
        }
        // else if(tokens[i][0] == "FUNCTION"){
        //     vector <string> params = checkOnVariables(tokens[i][1], i, variables);
        //     vector <string> vec = functions(tokens[i][1], params);
        //     tokens[i] = vec;
        //     continue;
        // }
        else if(tokens[i][0] == "INDEX"){
            upgradeIndex(i, tokens, variables);
        }
    }

    int elseIdx, elseChange;
    int circleStart, circleEnd;
    int countFor = 0;

    for(int i = 0; i < tokens.size() - 1; i++){
        if(i == elseIdx) {
            i = elseChange;
            continue;
        }
        if(circleStart != -1){
            if(i == circleEnd){
                i = circleStart;
            }
        }

        if(tokens[i][0] == "LOG"){
            if(tokens[i+1][0] == "NUMBER" || tokens[i+1][1] == "(") {
                cout << calc(searchExpression(i+1, tokens)) << endl;
                continue;
            }
            else if(tokens[i+1][0] == "STRING"){
                cout << searchString(i+1, tokens) << endl;
                continue;
            }
            else if(tokens[i+1][0] == "ARRAY"){
                cout << workArray(i + 1, tokens) << endl;
                continue;
            }
        }

        else if((tokens[i][0] == "VARIABLE" || tokens[i].size() == 4) && circleStart != -1 && typeCircle == "WHILE"){
            int indexVar;

            for(int j = 0 ; j < variables.size(); j++){
                if(tokens[i].size() == 4 && tokens[i][2] == variables[j][0]) {
                    upgradeVariables(i, tokens, variables);
                    tokens[i][1] = variables[j][1];
                    break;
                }
                else if(variables[j][0] == tokens[i][1].substr(1, tokens[i][1].size() - 1)){
                    indexVar = j;
                    break;
                }
            }

            if(tokens[i][0] == "VARIABLE"){
                overwriteVariable(i, indexVar, variables, tokens);
            }
        }

        else if(tokens[i][0] == "IF"){
            vector <int> vec = workIf(i, tokens);

            if(vec[1] != -1 && vec[0] == 0){
                i = vec[1];
            }

            else if(vec[0] == 1 && vec[1] != -1){
                elseIdx = vec[1];
                elseChange = vec[2];
            }
            
            else if(vec[1] == -1 && vec[0] == 0) i = vec[2];
        }
        else if(tokens[i][0] == "WHILE"){
            vector <int> ret = whileWork(i, tokens);
            typeCircle = "WHILE";

            if(ret[0] == 0){
                circleStart = -1;
                typeCircle = "";
                i = ret[1];
            }else {
                circleStart = i;
                typeCircle = "WHILE";
            }
            circleEnd = ret[1];
        }
        else if(tokens[i][0] == "FOR"){
            vector <int> ret =  workFor(i, tokens, variables, countFor);

            if(ret[0] == 0){
                circleStart = -1;
                typeCircle = "";
                countFor = 0;
                i = ret[1];
            }else {
                typeCircle = "FOR";
                upgradeVariables(i, tokens, variables);
                upgradeIndex(i, tokens, variables);
                circleStart = i;
                countFor += 1;
            }
            circleEnd = ret[1];
        }
    }

    // cout << "\n\n\nVARIABLES\n"; 
    // for(int i = 0; i < variables.size(); i++){
    //     cout << "[";
    //     for(int j = 0; j < variables[i].size(); j++){
    //         cout << variables[i][j] << " ";
    //     }
    //     cout << "],  ";
    // }
    // cout << "\nEND VARIABLE\n\n";
}