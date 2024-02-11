#include "../Functions/exception.h"

void writeVariableuse(int i, vector <vector <string>> variables, vector <vector <string>>& tokens){
    vector <string> vec;
    cmatch result;

    regex regularNum("[0-9]+[.]?[0-9]*");
    regex regularNeg("~{1}[0-9]+[.]?[0-9]*");

    bool isFind = false;
    for(int j = 0; j < variables.size(); j++){
        if(variables[j][0] == tokens[i][1].substr(1, tokens[i][1].size() - 1)){
            if(regex_match(variables[j][1].c_str(), result, regularNum)){
                vec.push_back("NUMBER");
            }
            else if(regex_match(variables[j][1].c_str(), result, regularNeg)){
                vec.push_back("NEGATIVE_NUMBER");
            }
            else if(variables[j].size() == 3){
                vec.push_back("ARRAY");
            }
            else vec.push_back("STRING");
            
            if(vec[0] == "STRING"){
                vec.push_back("\"" + variables[j][1] + "\"");
            }
            if(vec[0] == "ARRAY"){
                vec.push_back(variables[j][1]);
                vec.push_back(variables[j][2]);
            }
            else vec.push_back(variables[j][1]);


            vec.push_back(variables[j][0]);
            vec.push_back(to_string(j));
            
            isFind = true;
            break;
        }
    }

    tokens[i] = vec;

    if(!isFind){ 
        except("Variable " + tokens[i][1] + " was not found\n\n" + 
    "!Initialize Variable!    Examples below\n\n" + "$" + tokens[i][1].substr(1, tokens[i][1].size()-1) +
    " => 100 * 20\t" + "$" + tokens[i][1].substr(1, tokens[i][1].size()-1) + " => \"Hello, World!!!\"" + "\n\n" + "Good luck!");
    }
}

void upgradeVariables(int index, vector <vector <string>>& tokens, vector <vector <string>> variables){
    for(int i = index; i < tokens.size(); i++){
        if(tokens[i][0] == "INDEX"){
            upgradeIndex(i, tokens, variables);
        }
        if(tokens[i].size() != 4) continue;

        for(int j = 0; j < variables.size(); j++){
            if(variables[j][0] == tokens[i][2]){;
                tokens[i][1] = variables[j][1];
            }
        }
    }
}   