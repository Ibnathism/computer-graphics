//
// Created by user on ২৪/৬/২১.
//

#ifndef RAY_TRACING_CLASSES_H
#define RAY_TRACING_CLASSES_H

#endif //RAY_TRACING_CLASSES_H
#include "point.h"
#include <vector>
#define pi (2*acos(0.0))
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
public:
    double radius;
    Sphere() {
    }

    Sphere(Point center, double radius) {
        reference_point = center;
        this -> radius = radius;
    }
    void setColor(vector<double> color) {
        this -> color = color;
    }
    void setCoefficient(vector<double> coefficients) {
        this -> coefficients = coefficients;
    }
    void setShine(int shine) {
        this -> shine = shine;
    }

    void draw(int slices,int stacks)
    {
        glPushMatrix();
        glTranslatef(reference_point.x, reference_point.y, reference_point.z);
        glColor3f(color[0], color[1], color[2]);
        struct Point points[100][100];
        int i,j;
        double h,r;
        //generate points
        for(i=0;i<=stacks;i++)
        {
            h=radius*sin(((double)i/(double)stacks)*(pi/2));
            r=radius*cos(((double)i/(double)stacks)*(pi/2));
            for(j=0;j<=slices;j++)
            {
                points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
                points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
                points[i][j].z=h;
            }
        }
        //draw quads using generated points
        for(i=0;i<stacks;i++)
        {
            for(j=0;j<slices;j++)
            {
                glBegin(GL_QUADS);{
                    //upper hemisphere
                    glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                    //lower hemisphere
                    glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
                }glEnd();
            }
        }
        glPopMatrix();
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
public:
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
