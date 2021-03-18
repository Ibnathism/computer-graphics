#ifndef OPEN_GL_FUNCTIONS_H
#define OPEN_GL_FUNCTIONS_H

#endif //OPEN_GL_FUNCTIONS_H

#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include "point.h"
#define pi (2*acos(0.0))
#include <random>

Point rotateOneAlongAnother(Point &toBeRotated, Point &respective, double angleOfRotation) {
    Point temp = respective.crossMultiplication(toBeRotated);
    double angleInRadian = angleOfRotation*pi / 180.0;
    Point component1 = toBeRotated.constantScale(cos(angleInRadian));
    Point component2 = temp.constantScale(sin(angleInRadian));
    Point answer = component1.summation(component2);
    return answer;
}


void clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,0);	//color black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void drawAxes() {
    glBegin(GL_LINES);{
        glVertex3f(100, 0, 0);
        glVertex3f(-100, 0, 0);

        glVertex3f(0, -100, 0);
        glVertex3f(0, 100, 0);

        glVertex3f(0, 0, 100);
        glVertex3f(0, 0, -100);
    }glEnd();
}

void drawGrid() {
    int i;
    glBegin(GL_LINES);{
        for (i = -8; i <= 8; i++) {

            if (i == 0)
                continue;    //SKIP the MAIN axes

            //lines parallel to Y-axis
            glVertex3f(i * 10, -90, 0);
            glVertex3f(i * 10, 90, 0);

            //lines parallel to X-axis
            glVertex3f(-90, i * 10, 0);
            glVertex3f(90, i * 10, 0);
        }
    }glEnd();
}

void drawSquare(double a)
{
    glBegin(GL_QUADS);{
        glVertex3f( a, a,2);
        glVertex3f( a,-a,2);
        glVertex3f(-a,-a,2);
        glVertex3f(-a, a,2);
    }glEnd();
}

void draw2DRectangle(Point topLeft, Point bottomLeft, Point topRight, Point bottomRight) {
    glBegin(GL_LINES);{
        //Left Side
        glVertex3f(topLeft.x, topLeft.y, topLeft.z);
        glVertex3f(bottomLeft.x, bottomRight.y, bottomLeft.z);
        //Right Side
        glVertex3f(topRight.x, topRight.y, topRight.z);
        glVertex3f(bottomRight.x, bottomRight.y, bottomRight.z);
        //Top Side
        glVertex3f(topLeft.x, topLeft.y, topLeft.z);
        glVertex3f(topRight.x, topRight.y, topRight.z);
        //Bottom Side
        glVertex3f(bottomLeft.x, bottomLeft.y, bottomLeft.z);
        glVertex3f(bottomRight.x, bottomRight.y, bottomRight.z);
    }glEnd();
}


void drawCircle(double radius,int segments) {
    int i;
    Point points[100];
    for(i=0;i<=segments;i++) {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    for(i=0;i<segments;i++) {
        glBegin(GL_LINES);{
            glVertex3f(points[i].x,points[i].y,0);
            glVertex3f(points[i+1].x,points[i+1].y,0);
        }glEnd();
    }
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



void drawCone(double radius,double height,int segments){
    int i;
    double shade;
    Point points[100];
    for(i=0;i<=segments;i++) {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    for(i=0;i<segments;i++) {
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);{
            glVertex3f(0,0,height);
            glVertex3f(points[i].x,points[i].y,0);
            glVertex3f(points[i+1].x,points[i+1].y,0);
        }glEnd();
    }
}

void drawSphere(double radius,int slices,int stacks) {
    Point points[100][100];
    int i,j;
    double h,r;
    for(i=0;i<=stacks;i++) {
        h=radius*sin(((double)i/(double)stacks)*(pi/2));
        r=radius*cos(((double)i/(double)stacks)*(pi/2));
        for(j=0;j<=slices;j++) {
            points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
            points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
            points[i][j].z=h;
        }
    }
    for(i=0;i<stacks;i++) {
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
        for(j=0;j<slices;j++) {
            glBegin(GL_QUADS);{
                glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);

                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
            }glEnd();
        }
    }
}

double getRandom(double a, double b) {
    std::random_device randomDevice;
    std::mt19937 mt19937(randomDevice());
    std:: uniform_real_distribution<double> distribution(a, b);
    return distribution(mt19937);
}

Point getNormalizedPoint(Point p) {
    double temp = sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
    return {p.x/temp, p.y/temp, p.z/temp};
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