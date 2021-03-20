#ifndef OPEN_GL_FUNCTIONS_H
#define OPEN_GL_FUNCTIONS_H

#endif //OPEN_GL_FUNCTIONS_H

#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include "point.h"
#include <random>

#define pi (2*acos(0.0))
double planeDistance = 200;

Point axisQW(1, 0, 0);
double angleQW = 0;
double maxAngleQW = 15;
Point axisER(0, 1, 0);
double angleER = 0;
double maxAngleER = 20;
Point axisAS(0, 1, 0);
double angleAS = 0;
double maxAngleAS = 15;
//Point axisDF(0, 0, 0);
double angleDF = 0;
double maxAngleDF = 45;

double angleToChange = 1.0;
Point headCenter(0, 0, 80);
Point updatedCenter(0, 0, 80);
Point totalBullets[1000];
int bulletCount = 0;

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

void draw2DRectangle(Point topLeft, Point bottomLeft, Point topRight, Point bottomRight) {
    glBegin(GL_LINES);{
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

Point rotateOneAlongAnother(Point &toBeRotated, Point &respective, double angleOfRotation) {
    Point temp = respective.crossMultiplication(toBeRotated);
    double angleInRadian = angleOfRotation*pi / 180.0;
    Point component1 = toBeRotated.constantScale(cos(angleInRadian));
    Point component2 = temp.constantScale(sin(angleInRadian));
    Point answer = component1.summation(component2);
    return answer;
}

void setHeadCenter(){
    updatedCenter = rotateOneAlongAnother(headCenter, axisQW, angleQW);
    updatedCenter = rotateOneAlongAnother(updatedCenter, axisER, angleER);
    updatedCenter = rotateOneAlongAnother(updatedCenter, axisAS, angleAS);
}

void applyQRotation() {
    if (std::abs(angleQW + angleToChange) > maxAngleQW) return;
    angleQW = angleQW + angleToChange;
    setHeadCenter();
}

void applyWRotation() {
    if (std::abs(angleQW - angleToChange) > maxAngleQW) return;
    angleQW = angleQW - angleToChange;
    setHeadCenter();
}

void applyERotation() {
    if (std::abs(angleER + angleToChange) > maxAngleER) return;
    angleER = angleER + angleToChange;
    setHeadCenter();
}

void applyRRotation() {
    if (std::abs(angleER - angleToChange) > maxAngleER) return;
    angleER = angleER - angleToChange;
    setHeadCenter();
}

void applyARotation() {
    if (std::abs(angleAS + angleToChange) > maxAngleAS) return;
    angleAS = angleAS + angleToChange;
    setHeadCenter();
}

void applySRotation() {
    if (std::abs(angleAS - angleToChange) > maxAngleAS) return;
    angleAS = angleAS - angleToChange;
    setHeadCenter();
}

void applyDRotation() {
    if (std::abs(angleDF + angleToChange) > maxAngleDF) return;
    angleDF = angleDF + angleToChange;
}

void applyFRotation() {
    if (std::abs(angleDF - angleToChange) > maxAngleDF) return;
    angleDF = angleDF - angleToChange;
}

void drawGun(double handleRadius, double bodyHeight, double bodyRadius, double headRadius, double headOffset) {

    int slices = 50;
    int stacks = 200;

    Point handlePoints[stacks+1][slices+1];
    double handleH, handleR;
    for(int i=0;i<=stacks;i++) {
        handleH = handleRadius*sin(((double)i/(double)stacks)*(pi/2));
        handleR = handleRadius*cos(((double)i/(double)stacks)*(pi/2));
        for(int j=0;j<=slices;j++) {
            handlePoints[i][j].x = handleR * cos(((double)j/(double)slices)*2*pi);
            handlePoints[i][j].y = handleR * sin(((double)j/(double)slices)*2*pi);
            handlePoints[i][j].z = handleH;

            handlePoints[i][j] = rotateOneAlongAnother(handlePoints[i][j], axisQW, angleQW);
            handlePoints[i][j] = rotateOneAlongAnother(handlePoints[i][j], axisER, angleER);

        }
    }
    bool isWhite = false;
    for(int i=0;i<stacks;i++) {
        for(int j=0;j<slices;j++) {
            glBegin(GL_QUADS);{
                if (isWhite) {
                    glColor3f(1, 1, 1);
                    isWhite = false;
                }
                else {
                    glColor3f(0, 0, 0);
                    isWhite = true;
                }
                glVertex3f(handlePoints[i][j].x,handlePoints[i][j].y,handlePoints[i][j].z);
                glVertex3f(handlePoints[i][j+1].x,handlePoints[i][j+1].y,handlePoints[i][j+1].z);
                glVertex3f(handlePoints[i+1][j+1].x,handlePoints[i+1][j+1].y,handlePoints[i+1][j+1].z);
                glVertex3f(handlePoints[i+1][j].x,handlePoints[i+1][j].y,handlePoints[i+1][j].z);

                glVertex3f(handlePoints[i][j].x,handlePoints[i][j].y,-handlePoints[i][j].z);
                glVertex3f(handlePoints[i][j+1].x,handlePoints[i][j+1].y,-handlePoints[i][j+1].z);
                glVertex3f(handlePoints[i+1][j+1].x,handlePoints[i+1][j+1].y,-handlePoints[i+1][j+1].z);
                glVertex3f(handlePoints[i+1][j].x,handlePoints[i+1][j].y,-handlePoints[i+1][j].z);
            }glEnd();
        }
    }

    Point bodyPoints[stacks+1][slices+1];
    double bodyH, bodyR;
    for(int i=0;i<=stacks;i++) {
        bodyH = bodyHeight*sin(((double)i/(double)stacks)*(pi/2));
        bodyR = bodyRadius;
        for(int j=0;j<=slices;j++) {
            int joint = (int) (0.2 * stacks);
            if (i > joint) {
                bodyPoints[i][j].x = bodyR*cos(angleDF + ((double)j/(double)slices)*2*pi);
                bodyPoints[i][j].y = bodyR*sin(angleDF + ((double)j/(double)slices)*2*pi);
                bodyPoints[i][j].z = bodyH;

                bodyPoints[i][j] = rotateOneAlongAnother(bodyPoints[i][j], axisQW, angleQW);
                bodyPoints[i][j] = rotateOneAlongAnother(bodyPoints[i][j], axisAS, angleAS);
                bodyPoints[i][j] = rotateOneAlongAnother(bodyPoints[i][j], axisER, angleER);
            } else {
                double angleOnJoint = (double)i/(double)joint;
                bodyR = bodyRadius * cos(angleOnJoint * (pi/2));
                bodyH = (bodyHeight*0.2) * sin( angleOnJoint * (pi/2));

                bodyPoints[i][j].x = bodyR*cos(angleDF + ((double)j/(double)slices)*2*pi);
                bodyPoints[i][j].y = bodyR*sin(angleDF + ((double)j/(double)slices)*2*pi);
                bodyPoints[i][j].z = bodyH;

                Point temp(0, 0, 1);
                Point addMore(0, 0, bodyHeight*0.2*2);
                bodyPoints[i][j] = (rotateOneAlongAnother(bodyPoints[i][j], temp, 180)).summation(addMore);
                bodyPoints[i][j] = rotateOneAlongAnother(bodyPoints[i][j], axisQW, angleQW);
                bodyPoints[i][j] = rotateOneAlongAnother(bodyPoints[i][j], axisAS, angleAS);
                bodyPoints[i][j] = rotateOneAlongAnother(bodyPoints[i][j], axisER, angleER);
            }
        }
    }
    isWhite = false;
    for(int i=0;i<stacks;i++) {
        for(int j=0;j<slices;j++) {
            glBegin(GL_QUADS);{
                if (isWhite) {
                    glColor3f(1, 1, 1);
                    isWhite = false;
                }
                else {
                    glColor3f(0, 0, 0);
                    isWhite = true;
                }
                glVertex3f(bodyPoints[i][j].x,bodyPoints[i][j].y,bodyPoints[i][j].z);
                glVertex3f(bodyPoints[i][j+1].x,bodyPoints[i][j+1].y,bodyPoints[i][j+1].z);
                glVertex3f(bodyPoints[i+1][j+1].x,bodyPoints[i+1][j+1].y,bodyPoints[i+1][j+1].z);
                glVertex3f(bodyPoints[i+1][j].x,bodyPoints[i+1][j].y,bodyPoints[i+1][j].z);
            }glEnd();
        }
    }

    Point headPoints[stacks+1][slices+1];
    double headH, headR;
    for(int i=0;i<=stacks;i++) {
        headH = bodyHeight*sin(((double)i/(double)stacks)*(pi/2));
        headR = headRadius - headRadius*cos(((double)i/(double)stacks)*(pi/2)) + headOffset;
        for(int j=0;j<=slices;j++) {
            headPoints[i][j].x = headR*cos(angleDF + ((double)j/(double)slices)*2*pi);
            headPoints[i][j].y = headR*sin(angleDF + ((double)j/(double)slices)*2*pi);
            headPoints[i][j].z = headH;

            headPoints[i][j] = rotateOneAlongAnother(headPoints[i][j], axisQW, angleQW);
            headPoints[i][j] = rotateOneAlongAnother(headPoints[i][j], axisAS, angleAS);
            headPoints[i][j] = rotateOneAlongAnother(headPoints[i][j], axisER, angleER);
        }
    }
    isWhite = false;
    for(int i=0;i<stacks;i++) {
        for(int j=0;j<slices;j++) {
            glBegin(GL_QUADS);{
                if (isWhite) {
                    glColor3f(1, 1, 1);
                    isWhite = false;
                }
                else {
                    glColor3f(0, 0, 0);
                    isWhite = true;
                }
                if (headPoints[i][j].z >= bodyHeight*0.2 && headPoints[i][j+1].z >= bodyHeight*0.2) {
                    if (headPoints[i+1][j+1].z >= bodyHeight*0.2 && headPoints[i+1][j].z >= bodyHeight*0.2) {
                        glVertex3f(headPoints[i][j].x,headPoints[i][j].y,headPoints[i][j].z);
                        glVertex3f(headPoints[i][j+1].x,headPoints[i][j+1].y,headPoints[i][j+1].z);
                        glVertex3f(headPoints[i+1][j+1].x,headPoints[i+1][j+1].y,headPoints[i+1][j+1].z);
                        glVertex3f(headPoints[i+1][j].x,headPoints[i+1][j].y,headPoints[i+1][j].z);
                    }
                }
            }glEnd();
        }
    }
}

void drawBulletsOnPlane() {
    for (int i = 0; i < bulletCount; ++i) {
        Point temp = totalBullets[i];
        drawSolidRectangle(Point(temp.x-2, temp.y+2, temp.z-20), Point(temp.x-2, temp.y-2, temp.z-20), Point(temp.x+2, temp.y+2, temp.z-20), Point(temp.x+2, temp.y-2, temp.z-20));
    }
}
void fireBullets() {
    double temp = std::abs(planeDistance/updatedCenter.z);
    Point bulletPosition = updatedCenter.constantScale(temp);
    totalBullets[bulletCount] = bulletPosition;
    bulletCount = bulletCount + 1;
}