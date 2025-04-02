#pragma once
#include "std_lib_facilities.h"
#include "AnimationWindow.h"
#include "streng.h"
#include "funksjon.h"
#include "widgets/DropdownList.h"
#include "widgets/TextInput.h"
#include "grafTegner.h"

class Animation: private GraphDrawer{
private:
    double t;
    double dt;
    double dx;  // matematisk avstand mellom punktene
    int nIndex; // antall punkter

    // komandoer
    bool play = true;
    bool keyDown = false;
    bool quit = false;
    bool showInfo = false;
    bool edit = false;
    bool drawGrid = false;
    bool editSpeed = false;

    GuitarString guitarString;
    GuitarString speed;
    Function function;
    void commands();
    void info();
    void userDrawString();

    // lagre/slette funksjon
    TextInput textInput;
    void saveFunction();
    void deleteFunction();

    //velge funksjon
    vector<string> options = {"Ny"};
    DropdownList functionList;
    void updateOptions();
    string currentFunction;
    void chooseFunction(); 

    
public:
    Animation(int n, double bølgefart);
    void playAninmation();
};


