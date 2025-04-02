#pragma once
#include "std_lib_facilities.h"
#include <filesystem>
#include "streng.h"
class GuitarString;

struct CoefAndFrequency{
    int n;
    double a;
    double b;
};

class Function{
private:
    vector<CoefAndFrequency> newFunctionCoef; // inneholder koeffisientene til en fynksjon som ikke er lagret
    map<string, vector<CoefAndFrequency>> allFunctions; // inneholder alle lagrede funksjoner
    string onlyLettersAndNumbers(const string& word);
public:
    Function();
    vector<CoefAndFrequency> none();
    vector<CoefAndFrequency> getNewFunction();
    vector<CoefAndFrequency> getCoef(string name);
    void fourier(const GuitarString& guitarstring, const GuitarString& speed);
    vector<string> allFunctionsName;
    bool addFunction(string name);
    void removeFunction(string name);
    bool saveAllFunctions();
    void loadAllFunctions();
};