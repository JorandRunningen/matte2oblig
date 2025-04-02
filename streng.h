#pragma once
#include "std_lib_facilities.h"
#include "funksjon.h"

const double pi = 3.14159265;
struct CoefAndFrequency;
class Function;

struct PointValues{
    double x;
    double y;
};

class GuitarString{
private:
    vector<PointValues> string;
    double c;
    void addPoint(double x, double y);
    double solution(double x, double t);
public:
    vector<CoefAndFrequency> coefficients;
    GuitarString(int n, double c, Function& function);
    vector<PointValues> getString() const;
    void printString() const;
    void updateString(double t);
    void editPoint(int index, double value);
    double getSpeed() const;
};