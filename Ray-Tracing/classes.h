//
// Created by user on ২৪/৬/২১.
//

#ifndef RAY_TRACING_CLASSES_H
#define RAY_TRACING_CLASSES_H

#endif //RAY_TRACING_CLASSES_H
#include <bits/stdc++.h>
#include <utility>
#include <vector>
#define INF INT_MAX
#define NEG_INF (-100)
#define pi (2*acos(0.0))
#define FAR 1000
#define FLOOR_WIDTH 1000
#define TILE_WIDTH 20

using namespace std;

class Point {
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

    double absolute() {
        double temp = sqrt(x*x + y*y + z*z);
        return temp;
    }

    void operator=(const Point &point) {
        this->x = point.x;
        this->y = point.y;
        this->z = point.z;
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

    void clip() {
        if (red < 0) red = 0;
        if (red > 255) red = 255;
        if (green < 0) green = 0;
        if (green > 255) green = 255;
        if (blue < 0) blue = 0;
        if (blue > 255) blue = 255;
    }

    Color operator*(double scalingConstant) const {
        Color temp;
        temp.red = (double) this->red * scalingConstant;
        temp.green = (double) this->green * scalingConstant;
        temp.blue = (double) this->blue * scalingConstant;
        return temp;
    }

    Color operator+(Color &color) const {
        Color temp;
        temp.red = this->red + color.red;
        temp.green = this->green + color.green;
        temp.blue = this->blue + color.blue;
        return temp;
    }

    void operator=(const Color &color) {
        this->red = color.red;
        this->green = color.green;
        this->blue = color.blue;
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
vector<Light> allLights;

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
    vector<double> coefficients; //ambient, diffuse, specular, reflection
    int shine{};
    Object () = default;

    virtual void draw() {}
    void setColor(Color c) {
        this->color = c;
    }
    void setShine(int s) {
        this->shine = s;
    }
    void setCoefficient(vector<double> coeffs) {
        this->coefficients = std::move(coeffs);
    }

    virtual Ray intersect(Ray &r, int level){
        return {};
    }

    virtual Color getColor(Point point) {
        return color;
    }

    virtual Point findNormal(Point point) {
        return Point(0, 0, 1);
    }
};
vector<Object *> allObjects;
class Sphere : public Object {
public:
    double radius{};
    Sphere() = default;

    Sphere(Point center, double radius) {
        reference_point = center;
        this -> radius = radius;
    }

    void draw() override {
        glPushMatrix();
        glTranslatef(reference_point.x, reference_point.y, reference_point.z);
        glColor3f(color.red, color.green, color.blue);
        struct Point points[100][100];
        int i,j;
        int slices = 80;
        int stacks = 80;
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
            if (tPlus < 0 && tMinus < 0) t = -1;
            else if (tMinus >= 0 && tPlus >= 0) t = tMinus;
            else if (tMinus < 0) t = tPlus;
            else return -1;
        } else t = - 1.0;
        return t;
    }

    Color getColor(Point point) override {
        return color;
    }

    Point findNormal(Point point) override {
        Point temp;
        temp = point - reference_point;
        return temp.normalizePoint();
    }
    Color illuminate(int level, Point pointOfInt, Ray ray) {
        Color newColor = getColor(pointOfInt) * this->coefficients[0];

        Point normal = findNormal(pointOfInt);
        double dotNormal = normal.dotMultiplication(ray.direction);

        if(dotNormal > 0) normal = normal * (-1.0);

        for (auto light: allLights) {
            Point lightDir = light.position - pointOfInt;
            double dist = lightDir.absolute();
            lightDir.normalizePoint();

            Point lightStart = pointOfInt + lightDir;
            Ray lightRay(lightStart, lightDir);

            bool isIntercept = false;

            for (auto obj : allObjects) {

                Ray rayIntercept = obj->intersect(lightRay, 0);
                if (rayIntercept.t < dist && rayIntercept.t > 0) {
                    isIntercept = true;
                    break;
                }
            }


            if (!isIntercept) {
                double lf = 1;
                double lightDotNormal = lightDir.dotMultiplication(normal);
                double lambert = max(0.0, lightDotNormal);
                Point reflectedRayScaled =  normal * (2.0 * lightDotNormal);
                Point reflectedRay = reflectedRayScaled - lightDir;

                double rayDotReflect = ray.direction.dotMultiplication(reflectedRay);

                double phongValue = max(rayDotReflect, 0.0);

                Color addedColor = getColor(pointOfInt) * lf * lambert * coefficients[1];

                //std::cout << newColor.red << ", " << newColor.green << ", " << newColor.blue << std::endl;
                newColor = newColor + addedColor;

                addedColor = Color(255, 255, 255) * lf * pow(phongValue, shine) * coefficients[2];

                newColor = newColor + addedColor;

            }

        }

        if (level > 0) {
            double rayDotNormal = ray.direction.dotMultiplication(normal);
            Point scaledRayDotNormal = normal * (rayDotNormal * 2.0);
            Point rayReflectedDir = ray.direction - scaledRayDotNormal;
            rayReflectedDir.normalizePoint();
            Point rayReflectedStart = pointOfInt + rayReflectedDir;
            Ray rayReflected(rayReflectedStart, rayReflectedDir);

            int indexOfMin = NEG_INF;
            double minT = INF;
            int count = 0;
            for (auto object : allObjects) {

                Ray temp = object->intersect(rayReflected, 0);
                if (temp.t < minT && temp.t > 0) {
                    minT = temp.t;
                    indexOfMin = count;

                }
                count++;
            }

            if (indexOfMin != NEG_INF)  {
                Ray next = allObjects[indexOfMin]->intersect(rayReflected, level-1);
                Color tempNext = next.color * coefficients[3] * 1.0;
                newColor = newColor + tempNext;


            }
        }


        return newColor;
    }

    Ray intersect(Ray &r, int level) override {

        Ray newRay;
        newRay.t = getT(r);
        Point dirScale = r.direction * r.t;
        Point pointOfInt = r.start + dirScale;
        //cout << "MY COLOR " << this->color.red << ", " << this->color.green << ", " << this->color.blue << endl;
        newRay.color = getColor(pointOfInt) * this->coefficients[0]; //multiplying ambient

        newRay.color.clip();

        if (newRay.t < 0 || level < 1) return newRay;

        Color changedColor = this->illuminate(level, pointOfInt, r);
        //cout << "Changed " << changedColor.red << ", " << changedColor.green << ", " << changedColor.blue << endl;
        newRay.color = changedColor;

        newRay.color.clip();

        return newRay;

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

Floor baseFloor;

