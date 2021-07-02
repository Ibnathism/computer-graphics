#ifndef OPEN_GL_FUNCTIONS_H
#define OPEN_GL_FUNCTIONS_H

#endif //OPEN_GL_FUNCTIONS_H
#include <bits/stdc++.h>
#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include "classes.h"
#include <random>
#include <vector>

#define pi (2*acos(0.0))

double initOffset = 5.0;

double maxAngle = 45;

Point axisQW(1, 0, 0);
double angleQW = 0;
Point axisER(0, 1, 0);
double angleER = 0;
Point axisAS(0, 1, 0);
double angleAS = 0;
double angleDF = 0;

double angleToChange = 1.0;
Point headCenter(0, 0, 80);
Point updatedCenter(0, 0, 80);


void clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,0);	//color black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}



void drawAxes() {
    glBegin(GL_LINES);{
        glColor3f(1, 0, 0);
        glVertex3f(100, 0, 0);
        glVertex3f(-100, 0, 0);
        glColor3f(0, 1, 0);
        glVertex3f(0, -100, 0);
        glVertex3f(0, 100, 0);
        glColor3f(0, 0, 1);
        glVertex3f(0, 0, 100);
        glVertex3f(0, 0, -100);
    }glEnd();
}




void drawSolidRectangle(Point topLeft, Point bottomLeft, Point topRight, Point bottomRight) {
    glBegin(GL_QUADS);{
        glVertex3f(topLeft.x, topLeft.y, topLeft.z);
        glVertex3f(bottomLeft.x, bottomRight.y, bottomLeft.z);
        glVertex3f(topRight.x, topRight.y, topRight.z);
        glVertex3f(bottomRight.x, bottomRight.y, bottomRight.z);
        glVertex3f(topLeft.x, topLeft.y, topLeft.z);
        glVertex3f(topRight.x, topRight.y, topRight.z);
        glVertex3f(bottomLeft.x, bottomLeft.y, bottomLeft.z);
        glVertex3f(bottomRight.x, bottomRight.y, bottomRight.z);
    }glEnd();
}

void draw2DCircle(double centerX, double centerY, double radius,int segments) {
    int i;
    Point points[100];
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].x = points[i].x + centerX;
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
        points[i].y = points[i].y + centerY;
    }
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);{
            glVertex3f(points[i].x,points[i].y,0);
            glVertex3f(points[i+1].x,points[i+1].y,0);
        }glEnd();
    }
}



double getRandom(double a, double b) {
    std::random_device randomDevice;
    std::mt19937 mt19937(randomDevice());
    std:: uniform_real_distribution<double> distribution(a, b);
    return distribution(mt19937);
}


double getMinimum(double v1, double v2, double v3, double v4) {
    double min = std::min(v1, v2);
    min = std::min(min, v3);
    return std::min(min, v4);
}

double getMaximum(double v1, double v2, double v3, double v4) {
    double max = std::max(v1, v2);
    max = std::max(max, v3);
    return std::max(max, v4);
}

double centerToCenterDistance(Point & p1, Point & p2) {
    double x = p1.x - p2.x;
    double y = p1.y - p2.y;
    double z = p1.z - p2.z;
    double temp = sqrt(x*x + y*y + z*z);
    return temp;
}



Point rotateOneAlongAnother(Point &toBeRotated, Point &respective, double angleOfRotation) {
    Point temp = respective.crossMultiplication(toBeRotated);
    double angleInRadian = angleOfRotation*pi / 180.0;
    Point component1 = toBeRotated.constantScale(cos(angleInRadian));
    Point component2 = temp.constantScale(sin(angleInRadian));
    Point answer = component1.summation(component2);
    return answer;
}

