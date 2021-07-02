//
// Created by user on ২৪/৬/২১.
//

#ifndef RAY_TRACING_CLASSES_H
#define RAY_TRACING_CLASSES_H

#endif //RAY_TRACING_CLASSES_H
#include <bits/stdc++.h>
#include <utility>
#include <vector>
#define pi (2*acos(0.0))
#define FAR 1000
#define FLOOR_WIDTH 1000
#define TILE_WIDTH 20

using namespace std;

class Point
{
public:
    double x,y,z;

    Point() {
        x=0;
        y=0;
        z=0;
    }

    Point(double x, double y, double z) : x(x), y(y), z(z) {}
    Point operator-() const {
        return {-this->x, -this->y, -this->z};
    }
    Point negatePoint() const {
        return {-this->x, -this->y, -this->z};
    }

    Point operator+(Point &temp) const {
        double ansX = this->x + temp.x;
        double ansY = this->y + temp.y;
        double ansZ = this->z + temp.z;
        return {ansX, ansY, ansZ};
    }

    Point operator-(Point &temp) const {
        double ansX = this->x - temp.x;
        double ansY = this->y - temp.y;
        double ansZ = this->z - temp.z;
        return {ansX, ansY, ansZ};
    }

    Point operator*(double scalingConstant) const {
        if (scalingConstant==0) return {0, 0,0};
        double ansX = scalingConstant * this->x;
        double ansY = scalingConstant * this->y;
        double ansZ = scalingConstant * this->z;
        return {ansX, ansY, ansZ};
    }

    Point operator/(double divisionConstant) const {
        double ansX = this->x / divisionConstant;
        double ansY = this->y / divisionConstant;
        double ansZ = this->z / divisionConstant;
        return {ansX, ansY, ansZ};
    }

    Point operator*(Point &temp) const {
        double ansX = this->x * temp.x;
        double ansY = this->y * temp.y;
        double ansZ = this->z * temp.z;
        return {ansX, ansY, ansZ};
    }

    double dotMultiplication(Point &temp) const {
        double dot = this->x*temp.x + this->y*temp.y + this->z*temp.z;
        return dot;
    }

    Point constantScale(double scalingConstant) const {
        if (scalingConstant==0) return {0, 0,0};
        double ansX = scalingConstant * this->x;
        double ansY = scalingConstant * this->y;
        double ansZ = scalingConstant * this->z;
        return {ansX, ansY, ansZ};
    }

    Point crossMultiplication(Point &temp) const {
        double ansX = this->y*temp.z - this->z*temp.y;
        double ansY = this->z*temp.x - this->x*temp.z;
        double ansZ = this->x*temp.y - this->y*temp.x;
        return {ansX, ansY, ansZ};
    }

    Point normalizePoint() const {
        double temp = sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
        return *this/temp;
    }

    void print() const {
        std::cout << this->x <<" " << this->y << " " << this->z << std::endl;
    }

};
class Color {
public:
    double red;
    double green;
    double blue;

    Color() {
        red = 0.0;
        green = 0.0;
        blue = 0.0;
    }

    Color(double red, double green, double blue) {
        this->red = red;
        this->green = green;
        this->blue = blue;
    }
};

class Ray {
public:
    Color color;
    Point start;
    Point direction;
    double t;

    Ray() {
        this->t = -1;
    }
    Ray(Point start, Point direction) {
        this->t = -1;
        this->start = start;
        this->direction = direction;
    }
};

class Object {
public:
    Point reference_point;
    Color color;
    vector<double> coefficients;
    int shine;
    Object () {}

    virtual void draw() {}
    void setColor() {}
    void setShine() {}
    void setCoefficient() {}

    virtual double intersect(Ray *r, double *color, int level){
        return -1.0;
    }
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
    void setColor(Color color) {
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
        glColor3f(color.red, color.green, color.blue);
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

    double getT(Ray r) {
        double t;
        Point startMinusCenter = r.start - this->reference_point;
        double a = r.direction.dotMultiplication(r.direction);
        double b = 2.0 * r.direction.dotMultiplication(startMinusCenter);
        double c = startMinusCenter.dotMultiplication(startMinusCenter) - this->radius * this->radius;
        double discriminant = b * b - 4.0 * a * c; //b^2 - 4ac

        if (discriminant >= 0) {
            double rootDiscriminant = sqrt(discriminant);
            double tPlus = (-b + rootDiscriminant) / (2.0 * a);
            double tMinus = (-b - rootDiscriminant) / (2.0 * a);
            if (tPlus >= 0 && tMinus >= 0) t = min(tPlus, tMinus);
            else t = max(tPlus, tMinus);
        } else t = - 1.0;
        return t;
    }

    Ray intersect(Ray r, int level) {
        Ray newRay;
        newRay.t = getT(r);
        Point dirScale = r.direction * r.t;
        Point pointOfInt = r.start + dirScale;

    }
};

class Triangle : public Object {
    Point a, b, c;

    Triangle(Point a, Point b, Point c) {
        a = a;
        b = b;
        c = c;
    }
    void setColor(Color color) {
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
    Color color;
    Light(Point position, Color color) {
        position =  position;
        color = color;
    }

    void draw(int slices, int stacks) {
        glColor3f(this->color.red, this->color.green, this->color.blue);
        glPushMatrix();
        {
            glTranslatef(this->position.x, this->position.y, this->position.z);
            glutSolidSphere(0.5, slices, stacks);
        }
        glPopMatrix();
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

