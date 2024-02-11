vector <int> condit(vector <vector <string>> condition){
    if(condition[0][0] != "STRING" && condition[0][0] != "NUMBER" && condition[0][0] != "NOT"){
        except("Condition invalid");
    }

    string prev_item, oper;
    vector <string> last;
    
    if(condition.size() > 1){

        for(int i = 0; i < condition.size(); i++){
            if((condition[i][0] == "AND" || condition[i][0] == "OR" || condition[i][0] == "NOT") && i != 0){
                last.push_back(condition[i][0]);
                prev_item = "";
                oper = "";
                continue;
            }

            if(prev_item.size() > 0 && oper.size() > 0){
                if(oper == "COMPARISON"){
                    if(prev_item == condition[i][1]) last.push_back("1");
                    else last.push_back("0");
                }
                else if(oper == "UNEQUAL"){
                    if(prev_item != condition[i][1]) last.push_back("1");
                    else last.push_back("0");
                }
                else if(oper == "MORE"){
                    if(isNumber(prev_item) && isNumber(condition[i][1])){
                        if(stoi(prev_item) > stoi(condition[i][1])) last.push_back("1");
                        else last.push_back("0");
                    }
                }
                else if(oper == "LESS"){
                    if(isNumber(prev_item) && isNumber(condition[i][1])){
                        if(stoi(prev_item) < stoi(condition[i][1])) last.push_back("1");
                        else last.push_back("0");
                    }
                }

                continue;
            }

            if(prev_item == "" && (condition[i][0] == "STRING" || condition[i][0] == "NUMBER")){
                prev_item = condition[i][1];
            }
            else if(condition[i][0] == "COMPARISON" || condition[i][0] == "UNEQUAL" || condition[i][0] == "MORE" || condition[i][0] == "LESS"){
                oper = condition[i][0];
            }
        }

    }
    else if(condition.size() == 1){
        if(isNumber(condition[0][1])){
            if(stoi(condition[0][1]) != 0) last.push_back("1");
            else last.push_back("0");
        }else {
            if(condition[0][1].size() > 0) last.push_back("1");
            else last.push_back("0");
        }
    }

    vector<vector<string>>().swap(condition);

    bool answer = stoi(last[0]);

    if(last.size() > 1){
        for(int i = 1; i < last.size() - 1; i++){
            if(last[i] == "AND") answer = answer && stoi(last[i+1]);
            else if(last[i] == "OR") answer = answer || stoi(last[i+1]);
            else if(last[i] == "NOT") answer = !answer;
        }
    }

    vector<string>().swap(last);

    vector <int> ret = {answer};

    return ret;
}



vector <int> workIf(int index, vector <vector <string>> tokens){
    vector <vector <string>> condition;
    bool isThen = false;
    int idxend = -1;
    int idxElse = -1;
    bool isEl = false;

    for(int i = index + 1; i < tokens.size(); i++){
        if(tokens[i][0] == "THEN") isThen = true;
        if(!isThen){
            vector <string> temp;
            if((tokens[i][0] == "NUMBER" || tokens[i][1] == "(") && !isEl) {
                temp.push_back("NUMBER");
                temp.push_back(to_string(calc(searchExpression(i, tokens))));
            }
            else if(tokens[i][0] == "STRING" && !isEl){
                string item = searchString(i, tokens);
                
                if(isNumber(item)) temp.push_back("NUMBER");
                else temp.push_back("STRING");
                temp.push_back(item);
            }
            else if(isEl && (tokens[i][0] != "STRING" && tokens[i][0] != "STRING")) {
                condition.push_back(tokens[i]);
                isEl = false;
            }

            if((tokens[i][0] == "NUMBER" || tokens[i][1] == "(" || tokens[i][0] == "STRING") && !isEl){
                condition.push_back(temp);
                isEl = true;  
            }
        }
        
        if(isThen && tokens[i][0] == "ENDIF"){
            idxend = i;
            break;
        }
        else if(isThen && tokens[i][0] == "ELSE"){
            idxElse = i;
        }
    }

    if(idxend == -1) except("IFERROR: Add 'endif'");
    else if(!isThen) except("IFERROR: Add 'then'");
    else if(condition.size() == 0) except("IFERROR: Condition is empty");

    vector <int> ret = condit(condition);
    ret.push_back(idxElse);
    ret.push_back(idxend);

    return ret;
}