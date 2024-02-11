vector <int> workFor(int index, vector <vector <string>> tokens, vector <vector <string>> &variables, int &countFor){
    vector <vector <string>> content;
    bool isTo = false;
    bool isDo = false;
    bool isStep = false;
    int idxend = -1;

    for(int i = index + 1; i < tokens.size(); i++){
        if(tokens[i][0] == "TO") {
            isTo = true;
            continue;
        }
        else if(tokens[i][0] == "DO") {
            isDo = true;
            continue;
        }
        else if(tokens[i][0] == "STEP") {
            isStep = true;
        }
        else if(tokens[i][0] == "ENDFOR") {
            idxend = i;
            break;
        }

        if((isTo && !isDo) || tokens[i][0] == "VARIABLE"){
            content.push_back(tokens[i]);
        }
    }

    if(idxend == -1) except("FORERROR: add 'endfor'");
    else if(!isDo) except("FORERROR: add 'do' after the range");
    else if(!isTo) except("FORERROR: add 'to' after the variable");
    else if(content.size() < 3) except("FORERROR: condition is empty");
    else if(!isNumber(content[1][1])) except("FORERROR: range must be integer");

    vector <double> vec;

    for(int i = 1; i < content.size(); i++){   // i = 1, потому что content[0] переменная
        if(vec.size() == 0){
            if(content[i][0] == "NUMBER"){
                vec.push_back(calc(searchExpression(i, content)));
            }
            else if(content[i][0] == "STRING"){
                string s = searchString(i, content);

                if(isNumber(s)) vec.push_back(stof(s));
                else except("FORERROR: Range must be a number");
            }
        }
        else if(content[i][0] == "STEP" && vec.size() == 1 && vec.size() - i > 1){
            if(content[i+1][0] == "NUMBER"){
                vec.push_back(calc(searchExpression(i+1, content)));
            }
            else if(content[i+1][0] == "STRING"){
                string s = searchString(i+1, content);

                if(isNumber(s)) vec.push_back(stof(s));
                else except("FORERROR: Step must be a number");
            }
        }
    }

    if(vec.size() != 2) except("FORERROR: Range or step is invalid");

    bool isTrue = false;

    for(int i = 0; i < variables.size(); i++){
        if(content[0][1].substr(1, content[0][1].size() - 1) == variables[i][0]){
            if(!isNumber(variables[i][1])) except("FORERROR: Variable is invalid, must be integer");

            if(stof(variables[i][1]) >= vec[0]){
                isTrue = false;
            }else {
                if(countFor > 0){
                    variables[i][1] = to_string(stof(variables[i][1]) + vec[1]);
                }
                isTrue = true;
            }
            break;
        }
    }

    vector <int> met = {isTrue, idxend};

    return met;
}