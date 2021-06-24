//
// Created by user on ২৪/৬/২১.
//

#ifndef RAY_TRACING_CLASSES_H
#define RAY_TRACING_CLASSES_H

#endif //RAY_TRACING_CLASSES_H
#include "point.h"

using namespace std;
class Object {
public:
    Point reference_point;
    vector<double> color;
    vector<double> coefficients;
    int shine;
    Object () {}

    virtual void draw() {}
    void setColor() {}
    void setShine() {}
    void setCoefficient() {}
};

class Sphere : public Object {
    double radius;

    Sphere(Point center, double radius) {
        reference_point = center;
        radius = radius;
    }
    void setColor(vector<double> color) {
        color = color;
    }
    void setCoefficient(vector<double> coefficients) {
        coefficients = coefficients;
    }
    void setShine(int shine) {
        shine = shine;
    }
};

class Triangle : public Object {
    Point a, b, c;

    Triangle(Point a, Point b, Point c) {
        a = a;
        b = b;
        c = c;
    }
    void setColor(vector<double> color) {
        color = color;
    }
    void setCoefficient(vector<double> coefficients) {
        coefficients = coefficients;
    }
    void setShine(int shine) {
        shine = shine;
    }
};

class Light {
    Point position;
    vector<double> color;
    Light(Point position, vector<double> color) {
        position =  position;
        color = color;
    }
};

class Floor : public Object {
    double tileWidth;
    Floor(double floorWidth, double tileWidth) {
        reference_point = Point(-floorWidth/2, -floorWidth/2, 0);
        tileWidth = tileWidth;
    }
};
