vector <int> whileWork(int index, vector <vector <string>> tokens){
    vector <vector <string>> condition;
    bool isThen = false;
    int idxend = -1;
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
        
        if(isThen && tokens[i][0] == "ENDWHILE"){
            idxend = i;
            break;
        }
    }

    if(idxend == -1) except("WHILEERROR: Add 'endwhile'");
    else if(!isThen) except("WHILEERROR: Add 'then'");
    else if(condition.size() == 0) except("WHILEERROR: Condition is empty");

    vector <int> ret = condit(condition);
    ret.push_back(idxend);

    return ret;
}