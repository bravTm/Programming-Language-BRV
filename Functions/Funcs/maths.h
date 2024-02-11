#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

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