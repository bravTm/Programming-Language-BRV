#include <iostream>
#include <string>
#include <stack>
#include <cmath>
 
using namespace std;

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