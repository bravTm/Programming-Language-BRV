#include <iostream>
#include <vector>
#include <thread>

#include "./readfile.h"
#include "./lexer.h"
#include "./parser.h"

using namespace std;
using namespace this_thread;

int main(int argc, char** argv){
    if(!argv[1]) {
        cout << "File wasn't opened-> LOGOUT";
        return -1;
    }

    vector <vector <string>> vec = lexer(readFile(argv[1]));

    parser(vec);
    
    // for(int i = 0; i < vec.size(); i++){
    //     cout << "[";

    //     for(int j = 0; j < vec[i].size(); j++){
    //         cout << vec[i][j] << ", ";
    //     }

    //     cout << "], ";
    // }


    // TODO: СДЕЛАТЬ:
    //     [x] array
    //     [x] Индексы с переменной
    //     [x] Сделать так, чтобы работало ДЛИНА + ЧИСЛА и ЧИСЛА + ДЛИНА
    ///    [x] Сделать функции setArray, которая будет преобразовывать строку в массив(вектор)
    //     [x] Протестировать работу с массивом в for

    // ЕСЛИ ЗАРАБОТАЕТ \n, \t, то убрать endl в cout-ах -----------------------------------
    //     [] Решить проблему с \n, \t и т.д  (ВОЗМОЖНО ЗАРАБОТАЕТ КОГДА БУДЕТ 1 большой файл main.cpp)

    return 0;
}