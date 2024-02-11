#include <iostream>

using namespace std;

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