//
// Created by user on ২৪/৬/২১.
//

#ifndef RAY_TRACING_CLASSES_H
#define RAY_TRACING_CLASSES_H

#endif //RAY_TRACING_CLASSES_H
#include "point.h"
#include <vector>
#define FAR 1000
#define FLOOR_WIDTH 1000
#define TILE_WIDTH 20

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
public:
    Floor() {
        reference_point = Point(-FLOOR_WIDTH/2, -FLOOR_WIDTH/2, 0);
    }

    void draw() {
        glPushMatrix();
        double start = -FAR;
        double end = FAR;
        double color = 0;
        for (int i = start; i < end; i = i + TILE_WIDTH) {
            for (int j = start; j < end; j = j + TILE_WIDTH) {
                glColor3f(color, color, color);
                glBegin(GL_QUADS);{
                    glVertex2f(i, j);
                    glVertex2f(i, j+TILE_WIDTH);
                    glVertex2f(i+TILE_WIDTH, j+TILE_WIDTH);
                    glVertex2f(i+TILE_WIDTH, j);
                }glEnd();
                color = 1 - color;
            }
            color = 1 - color;
        }
        glPopMatrix();
    }
};
