#include <regex>
#include "./TokenTypeList.h"

inline void isNegativeNumber(string &s, string &tokenType){
    if(tokenType == "NEGATIVE_NUMBER"){
        tokenType = "NUMBER";

        s = "0-" + s.substr(1, s.size() - 1);
    }
}

vector <vector <string>> lexer(vector <string> lines){
    vector <vector <string>> res;
    string s = "";
    cmatch result;
    bool openpar = false; // открыта ли "
    bool  isTrue = false; // для проверки следущ символа
    bool isComment = false;
    string tokenType = "";

    // run by lines
    for(int i = 0; i < lines.size(); i++){
        isComment = false;
        // run by line
        for(int j = 0; j < lines[i].size(); j++){
            if(isTrue == true){
                s += lines[i][j];

                for(int k = 0; k < tokenTypesList.size(); k++){
                    regex regular(tokenTypesList[k][1]);

                    if(s.size() == 0) break;

                    if(regex_match(s.c_str(), result, regular)){
                        isTrue = true;
                        break;
                    }

                    isTrue = false;
                }


                if(isTrue) continue;

                if(s.size() > 0){
                    isNegativeNumber(s, tokenType);
                    vector <string> vec = {tokenType, s.substr(0, s.size() - 1)};
                    res.push_back(vec);
                    s = "";
                    j--;
                    continue;
                }
            }


            if(lines[i][j] == '"'){
                if(openpar) {
                    openpar = false;
                }
                else {
                    openpar = true;
                }
            }

            
            if((lines[i][j] != ' ' || openpar) && lines[i][j] != ';') {
                s += lines[i][j];

                bool exit = false; // если комментарий был встречен, то переход на другую строчку
                
                for(int k = 0; k < tokenTypesList.size(); k++){
                    regex regular(tokenTypesList[k][1]);

                    if(s.size() == 0) break;

                    if(regex_match(s.c_str(), result, regular)){
                        isTrue = true;
                        tokenType = tokenTypesList[k][0];
                        if(tokenTypesList[k][0] == "COMMENT") {
                            exit = true;
                            isComment = true;
                            break;
                        }
                    }
                }

                if(exit) {
                    s = "";
                    isTrue = false;
                    exit = false;
                    break;
                }
            }
        }

        if(s.size() != 0 && isTrue){
            isNegativeNumber(s, tokenType);

            vector <string> vec = {tokenType, s};
            res.push_back(vec);
            isTrue = false;
            s = "";
            vector <string>().swap(vec);
        }

        if(lines[i].size() > 0 && !isComment){
            vector <string> v = {"SEMICOLON"};
            res.push_back(v);
        }

    }

    return res;
}