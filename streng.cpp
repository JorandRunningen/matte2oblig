#include "streng.h"
void GuitarString::addPoint(double x, double y){
    string.push_back(PointValues{x, y});
}

void GuitarString::printString() const{
    for (int i = 0; i < string.size(); i++){
        cout << "(" << string.at(i).x << ", " << string.at(i).y << ")\n";
    }    
}

vector<PointValues> GuitarString::getString() const{
    return string;
}

GuitarString::GuitarString(int n, double c, Function& function){
    for (int i = 0; i < n; i++){
        addPoint(i*pi/(n-1), 0);
    }
    this->coefficients = function.none();
    this->c = c;
}

double GuitarString::solution(double x, double t){
    // løsning av bølgeligninigen til strengen
    //u(x,t) = sum(0, n)(an*cos(c*n*t) + bn*sin(c*n*t)) * sin(n*x)
    
    double y = 0;
    int n;
    double an;
    double bn;
    for (int i = 0; i < coefficients.size(); i++){
        n = coefficients.at(i).n;
        an = coefficients.at(i).a;
        bn = coefficients.at(i).b;
        y += (an*cos(c*n*t) + bn*sin(c*n*t)) * sin(n*x);
    }
    return y;
}

void GuitarString::updateString(double t){
    for (int i = 0; i < string.size(); i++){
        string.at(i).y = solution(string.at(i).x, t);
    }
}

void GuitarString::editPoint(int index, double value){
    string.at(index).y = value;
}

double GuitarString::getSpeed() const{
    return c;
}

