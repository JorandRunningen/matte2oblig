#pragma once
#include "std_lib_facilities.h"
#include "AnimationWindow.h"
#include "streng.h"

class GraphDrawer : protected AnimationWindow {
protected:
    static const int windowHeight = 500;
    static const int windowWidth = 1000;
    
    // matematisk størrelse på grafvinduet
    double xMin;
    double xMax;
    double yMin;
    double yMax;

    vector<int> mathPointToPixelPoint(const double& x, const double& y);
    vector<double> pixelPointToMathPoint(const Point& point);
public:
    GraphDrawer(double xMin, double xMax, double yMin, double yMax, string title = "Graf");
    void drawGraph(const vector<double>& xPoints , const vector<double>& yPoints, Color color = Color::black);
    void drawGraphPoint(const vector<PointValues>& points, Color color = Color::black);
    void grid();
    using AnimationWindow::wait_for_close; // setter wait_for_close publick
    using AnimationWindow::should_close; // setter should_close publick
};