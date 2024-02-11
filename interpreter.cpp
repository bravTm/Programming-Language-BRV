#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <cmath>
#include <string>
#include <regex>

#include <thread>

using namespace std;
using namespace this_thread;

vector <vector <string>> tokenTypesList = {
    {"NUMBER", "[0-9]+[.]?[0-9]*"},
    {"STRING", "\".*\""},
    {"NEGATIVE_NUMBER", "~{1}[0-9]+[.]?[0-9]*"},
    {"ASSIGN", "=>"},
    {"LOG", "Log"},
    {"INDEX", "\\[(\\d{1,}|@[a-zA-Z_]{1,})\\]"},
    {"COMPARISON", "=="},
    {"UNEQUAL", "!="},
    {"AND", "and"},
    {"OR", "or"},
    {"NOT", "not"},
    {"MORE", ">"},
    {"LESS", "<"},
    {"OPERATOR", "\\+"},
    {"OPERATOR", "\\-"},
    {"OPERATOR", "\\*"},
    {"OPERATOR", "\\^"},
    {"OPERATOR", "\\%"},
    {"OPERATOR", "/"},
    {"OPERATOR", "\\("},
    {"OPERATOR", "\\)"},
    {"FUNCTION", "[A-Za-z_]+[A-Za-z_0-9]*\\(.*\\)"},
    {"VARIABLE", "\\$[a-zA-Z_]{1,}"},
    {"VARIABLEUSE", "@[a-zA-Z_]{1,}"},
    {"COMMENT", "#"},
    {"METHODSTR", ".length"},
    {"METHODSTR", ".to_lower"},
    {"METHODSTR", ".to_upper"},
    {"METHODSTR", ".reverse"},
    {"IF", "if"},
    {"ELSE", "else"},
    {"ENDIF", "endif"},
    {"WHILE", "while"},
    {"ENDWHILE", "endwhile"},
    {"FOR", "for"},
    {"TO", "to"},
    {"DO", "do"},
    {"STEP", "step"},
    {"ENDFOR", "endfor"},
    {"THEN", "then"},
    {"SEMICOLON", ";"},
};


///////////////////////////// SECONDARY FUNCTIONS -----------------------------------------
inline bool isNumber(const std::string& s) {
    try {
        std::stoi(s);
    } catch (std::exception& e) {
        return false;
    }
    return true;
}


////////////////  EVAL --------------------------------------------------
float s2d(string s)
{
      int i,f=0;
      char a;
      float ce=0,fr=0,pw=1.0;
      for (i=0; i<s.size(); i++)
      {
          a=s[i];
          if (a=='.' || a==',') 
             f=1;
          else
             if (f==0)
                ce=ce*10.0+(float)(a-'0');
             else
             {
                fr=fr*10.0+(float)(a-'0');
                pw=pw*10.0;
             }        
      }
      return ce+fr/pw;
}
 
// Проверка: символ a присутствует в строке s
 
int isIn(char a, string s)
{
    int i,n=s.size();
    for (i=0; i<n; i++)
        if (s[i]==a) return i;
    return -1;
}    
 
string * parse(string formula)
{
       int n=formula.size();
       string *res = new string[n+1]; // один лишний элемент - на случай, если каждый символ есть лексема.
       string tmp;                    // это аккумулятор
       char a;                        // это очередной символ
       int i,ptr;                     // ptr - номер очередной позиции в res
       
       ptr=0;
       tmp="";
       
       for (i=0; i<=n; i++) res[i]=""; // чистка res
       
       for (i=0; i<n; i++)             // цикл по символам формулы
       {
           a=formula[i];               // очередной символ
           if (isIn(a,"+-*/^%()") >= 0) // это разделитель???
           {
                                       // да                    
             if (tmp.size()>0) res[ptr++]=tmp; // если аккумулятор непуст - сбросим его в res
             res[ptr++]=string(1,a);           // а в следующую позицию - найденный разделитель
             tmp="";                           // аккумулятор чистим                 
           }
           else                        // это не разделитель
           {
             tmp=tmp+string(1,a);      // приклеим его к аккумулятору
           }  
       }
       // если в аккумуляторе что-то осталось - сбросим
       if (tmp.size()>0) res[ptr++]=tmp;
       return res;
}
 
int prty(char op)
{
    switch (op)
    {
         case '(':
            return 0;
         case '+':
         case '-':
            return 1;
         case '*':
         case '/':
            return 2;
         case '^':
            return 3;
         case '%':
            return 3;
         default:
            return -1;        
    }
}            
 
float exec(char op, float a1, float a2)
{
    float r;   
    switch (op)
    {
         case '+':
            r=a1+a2;
            return r;
         case '-':
            r=a1-a2;
            return r;
         case '*':
            r=a1*a2;
            return r;
         case '/':
            r=a1/a2;
            return r;
         case '^':
            r=pow(a1,a2);
            return r;
         case '%':
            r=(int)a1%(int)a2;
            return r;
         default:
            return r;
    }            
}                        
 
float calc(string formula)
{
    stack <float> s1;
    stack <char>   s2;
 
    int i,ilex=0,p1,p2;
    float v,a1,a2,r;
    string curr; 
 
    string *lex=parse(formula);
 
    while(lex[ilex] != "")
    {
       curr=lex[ilex++];        
       
       if (curr[0]>='0' && curr[0]<='9')    // Число - в s1
       {
          v=s2d(curr);
          s1.push(v);
       }
       else                                 // разделитель
       {                 
          if ((curr[0]=='(') || s2.empty()) // левая скобка или первая операция 
          {    
             s2.push(curr[0]);
          }
          else 
          {
             if (curr[0]==')')              // опустошение до открывающей скобки
             {
                while(1)   
                {
                  if (s2.top()=='(')         
                  {
                     s2.pop();
                     break;
                   }                        
                   a2=s1.top();
                   s1.pop();
                   a1=s1.top();
                   s1.pop();
                   r=exec(s2.top(),a1,a2);
                   s1.push(r);
                   s2.pop(); 
                } 
             }
             else 
             {
                p1=prty(s2.top());          // приоритет вершины стека
                p2=prty(curr[0]);           // приоритет новой операции
             
                if (p2>p1)                  // более приоритетная операция 
                {
                   s2.push(curr[0]);
                }
                else
                {
                   a2=s1.top();
                   s1.pop();
                   a1=s1.top();
                   s1.pop();
                   r=exec(s2.top(),a1,a2);
                   s1.push(r);
                   s2.pop(); 
                   s2.push(curr[0]);
                }
             }   
          }             
       }      
    }           
 
    // Финишное опустошение стека
     
    while(1)
    {
       if (s2.empty())  break;
       a2=s1.top();
       s1.pop();
       a1=s1.top();
       s1.pop();
       r=exec(s2.top(),a1,a2);
       s1.push(r);
       s2.pop(); 
    }
 
    delete [] lex;
     
    return s1.top();
}


inline void except(string text){
    cout << text;
    exit(-1);
}


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


/////////////////////////// LEXER ----------------------------------------
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

///////////////////////////////// FUNCTIONS ---------------------------------
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

// ---------------------------------- ARRAY --------------------------------------------

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

/// --------------------------------- INPUT --------------------------------------

void input(vector <string> params, vector <string>& ans){
    if(params.size() > 1) except("FUNCTIONERROR: Input - Parameters must be 1 or 0");
    string s;
    cout << params[0] + " ";
    cin >> s;

    if(isNumber(s)){
        ans.push_back("NUMBER");
        ans.push_back(s);
    }
    else{
        ans.push_back("STRING");
        ans.push_back("\"" + s + "\"");
    }
}

/// ------------------------- MATH ---------------------

float fpow(long double a, int n){
    if(n == 0) return 1;
    else if(n % 2 == 0) return fpow(a * a, n / 2);
    else if(n < 0) return fpow(1.0 / a, -n);
    else return a * fpow(a, n - 1);
}

unsigned long long factorial(unsigned long long i)
{
    if(i > 1)
        return i * factorial(i - 1);
    else   return i;
}

void quickPow(vector <string>& params, vector <string>& ans){
    ans.push_back("NUMBER");
    ans.push_back(to_string(fpow(stof(params[0]), stoi(params[1]))));
}

void quickRoot(vector <string> params, vector <string>& ans){
    ans.push_back("NUMBER");
    ans.push_back(to_string(pow(stof(params[0]), 1 / stof(params[1]))));
}

void quickExp(vector <string> params, vector <string>& ans){
    ans.push_back("NUMBER");
    ans.push_back(to_string(exp(stof(params[0]))));
}

void quickCeil(vector <string> params, vector <string>& ans){
    ans.push_back("NUMBER");
    ans.push_back(to_string(ceil(stof(params[0]))));
}

void quickFloor(vector <string> params, vector <string>& ans){
    ans.push_back("NUMBER");
    ans.push_back(to_string(floor(stof(params[0]))));
}

void quickAbs(vector <string> params, vector <string>& ans){
    ans.push_back("NUMBER");
    ans.push_back(to_string(abs(stof(params[0]))));
}

void quickFactorial(vector <string> params, vector <string>& ans){
    ans.push_back("NUMBER");
    ans.push_back(to_string(factorial(stoi(params[0]))));
}

void quickLog10(vector <string> params, vector <string>& ans){
    ans.push_back("NUMBER");
    ans.push_back(to_string(log10(stoi(params[0]))));
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
                if(!isNumber(params[i]) && !isNumber(variables[j][1]) && variables[j].size() != 3){
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
            if(!isNumber(params[i])) return false;
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


////////////////////////////////// WORK ARRAY -----------------------------------------
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

    if(isNumber(idx)){
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

//////////////////////////////// IF-ELSE ----------------------------------------
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

/////////////////////////////// FOR ----------------------------------------------------------
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

////////////////////////////////// WHILE -----------------------------------------
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


///////////////////////////   VARIABLES -------------------------------------------------------
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
        else if(tokens[i][0] == "FUNCTION"){
            vector <string> params = checkOnVariables(tokens[i][1], i, variables);
            vector <string> vec = functions(tokens[i][1], params);
            tokens[i] = vec;
            continue;
        }
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
}

///////////////////////////////////////////////// READ FILE ----------------------------
vector <string> readFile(string filename){
    ifstream file;
    file.open(filename);

    if(file.bad() == true || !file.is_open()) {
        cout << "File wasn't opened  -> LOGOUT";
        exit(-1);
    }

    string line;
    vector <string> lines;

    if (file.is_open())
    {
        while (getline(file, line)){
            lines.push_back(line);
        }
    }

    file.close();

    return lines;
}

////////////////////////////////////////////// MAIN FUNCTION ------------------------------------------
int main(int argc, char** argv){
    if(!argv[1]) {
        cout << "File wasn't opened-> LOGOUT";
        return -1;
    }

    vector <vector <string>> vec = lexer(readFile(argv[1]));

    parser(vec);

    return 0;
}