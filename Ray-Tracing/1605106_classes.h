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
#define MINUS_INFINITY (-1000)
#define pi (2*acos(0.0))
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
        //Point norm = *this/temp;
        Point norm = Point(x, y, z);
        norm = norm/temp;
        //cout << x.absolute() << endl;
        return norm;
    }

    double absolute() const {
        double temp = sqrt(x*x + y*y + z*z);
        return temp;
    }

    Point& operator=(const Point &point) {
        this->x = point.x;
        this->y = point.y;
        this->z = point.z;
        return *this;
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
        red = red < 0 ? 0 : (red > 255 ? 255 : red);
        green = green < 0 ? 0 : (green > 255 ? 255 : green);
        blue = blue < 0 ? 0 : (blue > 255 ? 255 : blue);
    }

    Color operator*(double scalingConstant) const {
        Color temp;
        temp.red = this->red * scalingConstant;
        temp.green = this->green * scalingConstant;
        temp.blue = this->blue * scalingConstant;
        return temp;
    }

    Color operator+(Color &color) const {
        Color temp;
        temp.red = this->red + color.red;
        temp.green = this->green + color.green;
        temp.blue = this->blue + color.blue;
        return temp;
    }

    Color& operator=(const Color &color) {
        this->red = color.red;
        this->green = color.green;
        this->blue = color.blue;
        return *this;
    }

    void print() {
        cout << "(" << this->red << ", " << this->green << ", " << this->blue << ")" << endl;
    }


};

class Light {
public:
    Point position;
    Color color;
    Light(Point position, Color color) {
        this->position =  position;
        this->color = color;
    }

    void draw(int slices, int stacks) const {
        glColor3f(this->color.red, this->color.green, this->color.blue);
        glPushMatrix();
        {
            glTranslatef(this->position.x, this->position.y, this->position.z);
            glutSolidSphere(2, slices, stacks);
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

class Object;
vector<Object *> allObjects;

class Object {
public:
    Point reference_point;
    Color color;
    vector<double> coefficients; //ambient, diffuse, specular, reflection
    int shine;

    virtual void draw() {}
    void setColor(Color c) {
        this->color.red = c.red;
        this->color.green = c.green;
        this->color.blue = c.blue;
    }
    void setShine(int s) {
        this->shine = s;
    }
    void setCoefficient(vector<double> coeffs) {
        this->coefficients.push_back(coeffs[0]);
        this->coefficients.push_back(coeffs[1]);
        this->coefficients.push_back(coeffs[2]);
        this->coefficients.push_back(coeffs[3]);
    }

    virtual Color getColor(Point point) {
        return color;
    }

    virtual Point findNormal(Point point) {
        return Point(0, 0, 1);
    }

    virtual double getT(Ray r) {
        return -1;
    }

    Color illuminate(int level, Point pointOfInt, Ray ray) {
        Color newColor = getColor(pointOfInt) * this->coefficients[0];
        //getColor(pointOfInt).print();
        //cout << coefficients[0] << endl;
        //newColor.print();
        Point normal = findNormal(pointOfInt);
        //cout << "abs " << normal.absolute() << endl;
        double dotNormal = normal.dotMultiplication(ray.direction);

        if(dotNormal > 0) normal = -normal;

        for (auto light: allLights) {

            Point lightDir = light.position - pointOfInt;
            double dist = lightDir.absolute();
            lightDir = lightDir.normalizePoint();

            Point lightStart = pointOfInt + lightDir;
            Ray lightRay(lightStart, lightDir);

            bool isIntercept = false;

            for (auto obj : allObjects) {

                Ray rayIntercept = obj->intersect(lightRay, 0);
                if (rayIntercept.t < dist && rayIntercept.t > 0) {
                    //cout << "is intercept" << endl;
                    isIntercept = true;
                    break;
                }
            }


            if (!isIntercept) {

                double lightDotNormal = lightDir.dotMultiplication(normal);
                double lambert = max(0.0, lightDotNormal);
                //cout << lambert << endl;
                Point reflectedRayScaled =  normal * (2.0 * lightDotNormal);
                //normal.print();
                //cout << "abs " << normal.absolute() << endl;
                //cout << lightDotNormal << endl;
                //reflectedRayScaled.print();
                Point reflectedRay = reflectedRayScaled - lightDir ;

                double rayDotReflect = ray.direction.dotMultiplication(reflectedRay);
                //cout << rayDotReflect << endl;
                double phongValue = max(rayDotReflect, 0.0);
                //cout << phongValue << endl;

                Color addedColor = getColor(pointOfInt) * lambert * coefficients[1];
                //addedColor.print();
                //cout << "1...." << endl;


                //std::cout << newColor.red << ", " << newColor.green << ", " << newColor.blue;
                //std::cout << "-----" << addedColor.red << ", " << addedColor.green << ", " << addedColor.blue << std::endl;
                newColor = newColor + addedColor;
//              light.color.print();
//                if (shine != 1) {
//                    cout << shine << "::" << pow(phongValue, shine) << "::" << coefficients[2];
//                    light.color.print();
//                }
                //std::cout << newColor.red << ", " << newColor.green << ", " << newColor.blue;
                //std::cout << "----" << coefficients[2] << std::endl;
                addedColor = light.color * pow(phongValue, shine) * coefficients[2];
//              std::cout << newColor.red << ", " << newColor.green << ", " << newColor.blue;
//                if (shine != 1) {
//                    std::cout << "-----" << addedColor.red << ", " << addedColor.green << ", " << addedColor.blue
//                              << std::endl;
//                }
                //addedColor.print();
                newColor = newColor + addedColor;
                //std::cout << "----" <<newColor.red << ", " << newColor.green << ", " << newColor.blue << endl;

            }
            //newColor.print();
        }


        if (level > 0) {
            //cout << "Now level greater than 0 ----- " << level << endl;
            double rayDotNormal = ray.direction.dotMultiplication(normal);
            Point scaledRayDotNormal = normal * (rayDotNormal * 2.0);
            Point rayReflectedDir = ray.direction - scaledRayDotNormal;
            rayReflectedDir = rayReflectedDir.normalizePoint();
            Point rayReflectedStart = pointOfInt + rayReflectedDir;
            Ray rayReflected(rayReflectedStart, rayReflectedDir);
            int indexOfMin = MINUS_INFINITY;
            double minT = INF;
            int count = -1;
            for (auto object : allObjects) {
                count++;
                Ray temp = object->intersect(rayReflected, 0);
                //cout << temp.t << "------ TEMP T ----- " << endl;
                if (temp.t < minT && temp.t > 0) {
                    //cout << temp.t << "------ Inside IF ----- " << indexOfMin << endl;
                    minT = temp.t;
                    indexOfMin = count;

                }

            }

            if (indexOfMin != MINUS_INFINITY)  {
                //cout << "Index of min not equal NEG INF ----- " << indexOfMin << endl;
                Ray next = allObjects[indexOfMin]->intersect(rayReflected, level-1);
                Color tempNext = next.color * coefficients[3];
                newColor = newColor + tempNext;


            }
        }


        return newColor;
    }

    Ray intersect(Ray r, int level) {

        Ray newRay;
        newRay.t = getT(r);
        if(newRay.t < 0 || level < 1) return newRay;
        //cout << newRay.t << endl;
        Point dirScale = r.direction * newRay.t;
        Point pointOfInt = r.start + dirScale;
        //cout << "MY COLOR " << this->color.red << ", " << this->color.green << ", " << this->color.blue << endl;
        newRay.color = getColor(pointOfInt) * this->coefficients[0]; //multiplying ambient

        newRay.color.clip();
        //cout << this->coefficients[0] << " ::::: MY COLOR :::::" << newRay.color.red << ", " << newRay.color.green << ", " << newRay.color.blue << endl;


        Color changedColor = this->illuminate(level, pointOfInt, r);
        //changedColor.print();
        //cout << "Changed " << changedColor.red << ", " << changedColor.green << ", " << changedColor.blue << endl;
        newRay.color = changedColor;

        newRay.color.clip();

        return newRay;

    }
};

class Sphere : public Object {
public:
    double radius;
    Sphere(){};

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

    double getT(Ray r) override {
        double t;
        Point startMinusCenter = r.start - this->reference_point;
        //double a = r.direction.dotMultiplication(r.direction);
        double a = 1.0;

        double b = 2.0 * r.direction.dotMultiplication(startMinusCenter);
        double c = startMinusCenter.dotMultiplication(startMinusCenter) - this->radius * this->radius;
        double discriminant = b * b - 4.0 * a * c; //b^2 - 4ac

        if (discriminant >= 0) {
            //cout << "a " << a << "  b  " << b << "  c   " << c << endl;
            //cout << " INSIDE GET T :: Discriminant " << discriminant << endl;
            double rootDiscriminant = sqrt(discriminant);
            double tPlus = (-b + rootDiscriminant) / (2.0 * a);
            double tMinus = (-b - rootDiscriminant) / (2.0 * a);
            if (tPlus < 0 && tMinus < 0) t = -1;
            else if (tMinus >= 0 && tPlus >= 0) t = tMinus;
            else if (tMinus < 0) t = tPlus;
            else return -1;
        } else t = - 1.0;
        //cout << " INSIDE GET T " << t << endl;
        return t;
    }

    Color getColor(Point point) override {
        return color;
    }

    Point findNormal(Point point) override {
        Point temp;
        temp = point - reference_point;
        temp = temp.normalizePoint();
        //cout << "abs " << temp.absolute() << endl;
        return temp;
    }


};


class Floor : public Object {
public:
    double myX, myY;
    Floor() {
        reference_point = Point(-FLOOR_WIDTH/2.0, -FLOOR_WIDTH/2.0, 0);
        myX = -FLOOR_WIDTH/2.0;
        myY = -FLOOR_WIDTH/2.0;
    }


    void draw() override {
        int nTile = FLOOR_WIDTH/TILE_WIDTH;
        if (nTile % 2 != 0) nTile = nTile + 1;
        for (int i = 0; i < nTile; ++i) {
            for (int j = 0; j < nTile; ++j) {
                double tempX = i * TILE_WIDTH + myX;
                double tempY = j * TILE_WIDTH + myY;
                int r1 = i % 2;
                int r2 = j % 2;
                if ((r1 == 1 && r2 == 1) || (r1 == 0 && r2 == 0)) glColor3f(1.0, 1.0, 1.0);
                else glColor3f(0, 0, 0);

                glPushMatrix(); {
                    glBegin(GL_QUADS);{
                    glVertex3f(tempX, tempY, 0);
                    glVertex3f(tempX, tempY+TILE_WIDTH, 0);
                    glVertex3f(tempX+TILE_WIDTH, tempY+TILE_WIDTH, 0);
                    glVertex3f(tempX+TILE_WIDTH, tempY, 0);
                }glEnd();
                } glPopMatrix();
            }
        }
    }

    double getT(Ray r) override {
        double zStart = r.start.z;
        double zDir = r.direction.z;
        return -(zStart/zDir);
    }
    Point findNormal(Point point) override {
        Point temp(0, 0, 1);
        return temp;
    }
    Color getColor(Point point) override {
        double horizontal = point.x + (FLOOR_WIDTH/2.0);
        horizontal = horizontal/TILE_WIDTH;
        double vertical = point.y + (FLOOR_WIDTH/2.0);
        vertical = vertical/TILE_WIDTH;
        Point p(horizontal, vertical, 0);

        int r1 = (int)p.x % 2;
        int r2 = (int)p.y % 2;

        if (r1 < 0) r1 = (-1) * r1;
        if (r2 < 0) r2 = (-1) * r2;
        Color c;
        if ((r1 == 1 && r2 == 1) || (r1 == 0 && r2 == 0)) c = Color(1, 1,1);
        else c = Color(0, 0, 0);

        return c;
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


