// 1 - ARRAY type,  2 - Content,   3 - Size of array

void createArray(vector <string> params, vector <string>& ans){
    string s = "";
    int length = 0;
    bool isSemi = false;

    vector <vector <string>> vec = lexer(params);

    for(int i = 0; i < vec.size(); i++){
        if(vec[i][0] == "SEMICOLON") {
            isSemi = false;
            continue;
        }
        if(vec[i][0] == "NUMBER" && !isSemi){
            s += to_string(calc(searchExpression(i, vec))) + ";";
            length++;
            isSemi = true;
        }
        else if(vec[i][0] == "STRING" && !isSemi){
            s += searchString(i, vec) + ";";
            length++;
            isSemi = true;
        }
        else if(vec[i][0] == "FUNCTION" || vec[i][0] == "FOR" || vec[i][0] == "WHILE" || vec[i][0] == "IF" || vec[i][0] == "ASSIGN"){
            except("ARRAYERROR: Array can only contain numbers and strings");
        }
    }


    ans.push_back("ARRAY");
    ans.push_back(s);
    ans.push_back(to_string(length));
}



void lengthArrayOrString(vector <string> params, vector <string>& ans, string type){
    ans.push_back("NUMBER");

    if(type == "ARRAY"){
        int len = 0;
        for(int i = 0; i < params[0].size(); i++){
            if(params[0][i] == ';') len++;
        }
        ans.push_back(to_string((double)len));
    }
    else if(type == "STRING"){
        ans.push_back(to_string((double)params[0].size() - 2));
    }
}