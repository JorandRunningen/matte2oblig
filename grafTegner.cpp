#include "grafTegner.h"
GraphDrawer::GraphDrawer(double xMin, double xMax, double yMin, double yMax, string title) :
    AnimationWindow{100, 100, windowWidth, windowHeight, title}
{
    this->xMin = xMin;
    this->xMax = xMax;
    this->yMin = yMin;
    this->yMax = yMax;
}


vector<int> GraphDrawer::mathPointToPixelPoint(const double& x, const double& y){ 
    return vector<int> {static_cast<int>(((x - xMin)/(xMax - xMin))*windowWidth), 
                        static_cast<int>(windowHeight - ((y - yMin)/(yMax - yMin)) * windowHeight)};
}

vector<double> GraphDrawer::pixelPointToMathPoint(const Point& point){
    return vector<double> {xMin + (xMax-xMin)*(static_cast<double>(point.x)/windowWidth),
                           yMin + (yMax-yMin)*(static_cast<double>(windowHeight - point.y)/windowHeight)};
}

void GraphDrawer::drawGraph(const vector<double>& xPoints , const vector<double>& yPoints, Color color){
    // kaster et unntak om det blir lakt inn forskjellig antall x og y verdier
    try {
        if (xPoints.size() != yPoints.size()){throw vector<unsigned long long> {xPoints.size(), yPoints.size()};}
    }
    catch (vector<unsigned long long> &e){
        cout << e.at(0) << "x-verdier og " << e.at(1) << "y-verdier" << endl;  
    }

    for (int i = 1; i < xPoints.size(); i++){
        vector<int> point1 = mathPointToPixelPoint(xPoints.at(i-1), yPoints.at(i-1));
        vector<int> point2 = mathPointToPixelPoint(xPoints.at(i), yPoints.at(i));
        draw_line({point1.at(0), point1.at(1)}, {point2.at(0), point2.at(1)}, color);
    }
}

void GraphDrawer::drawGraphPoint(const vector<PointValues>& points, Color color){
    for (int i = 1; i < points.size(); i++){
        vector<int> point1 = mathPointToPixelPoint(points.at(i-1).x, points.at(i-1).y);
        vector<int> point2 = mathPointToPixelPoint(points.at(i).x, points.at(i).y);
        draw_line({point1.at(0), point1.at(1)}, {point2.at(0), point2.at(1)}, color);
    }
}

void GraphDrawer::grid(){
    int v = 8; // antall linjer vertikalt
    int h = 5; // antall linjer horisontalt
    double dx = (xMax-xMin) / v;
    double dy = (yMax-yMin) / h;
    
    int X = mathPointToPixelPoint(10, 1).at(0);
    int Y = mathPointToPixelPoint(10, 1).at(1);
    draw_circle({X, Y}, 10);

    // vertikale linjer
    for (int i = 0; i < v; i++){
        drawGraph({xMin + dx*i, xMin + dx*i}, {yMin, yMax}, Color::gray);
        if (i != 0){
            int X = mathPointToPixelPoint(xMin + dx*i, yMin).at(0);
            int Y = mathPointToPixelPoint(xMin + dx*i, yMin).at(1) - 30;
            draw_text({X,Y}, to_string(xMin + dx*i));
        }
    }

    // horisontale linjer
    for (int i = 0; i < h; i++){
        drawGraph({xMin, xMax}, {yMin + dy*i, yMin + dy*i}, Color::gray);
        
        if (i != 0){
            int X = mathPointToPixelPoint(xMin, yMin + dy*i).at(0);
            int Y = mathPointToPixelPoint(xMin, yMin + dy*i).at(1) - 30;
            draw_text({X,Y}, to_string(yMin + dy*i));
        }
    }
}

