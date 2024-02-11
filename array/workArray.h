vector <string> arrayConversion(string item){
    vector <string> arr;
    string s = "";

    for(int i = 0; i < item.size(); i++){
        if(item[i] == ';') {
            arr.push_back(s);
            s = "";
            continue;
        }

        s += item[i];
    }

    return arr;
}

inline int itemByIndex(int length, string item){
    string idx;
    for(int j = 1; j < item.size(); j++){
        if(item[j] == ']') break;

        idx += item[j];
    }

    if(isNum(idx)){
        if(stoi(idx) >= length || stoi(idx) < 0) except("ARRAYERROR: Index out of line");
    }
    else except("ARRAYERROR: Index is invalid");

    return stoi(idx);
}


string workArray(int idx, vector <vector <string>> tokens){
    vector <string> array = arrayConversion(tokens[idx][1]);

    if(tokens[idx + 1][0] == "INDEX"){
        return array[itemByIndex(array.size(), tokens[idx + 1][1])];
    }

    return tokens[idx][1];
}