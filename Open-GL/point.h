//
// Created by user on ১৬/৩/২১.
//

#ifndef OPEN_GL_POINT_H
#define OPEN_GL_POINT_H

#endif //OPEN_GL_POINT_H

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

    Point negatePoint() {
        Point temp;
        temp.x = -this->x;
        temp.y = -this->y;
        temp.z = -this->z;
        return temp;
    }

    Point summation(Point &temp) {
        Point answer;
        answer.x = this->x + temp.x;
        answer.y = this->y + temp.y;
        answer.z = this->z + temp.z;
        return answer;
    }

    Point subtraction(Point &temp) {
        Point answer;
        answer.x = this->x - temp.x;
        answer.y = this->y - temp.y;
        answer.z = this->z - temp.z;
        return answer;
    }

    Point constantScale(double scalingConstant) {
        Point temp;
        if (scalingConstant==0) return temp;
        temp.x = scalingConstant * this->x;
        temp.y = scalingConstant * this->y;
        temp.z = scalingConstant * this->z;
        return temp;
    }

    Point crossMultiplication(Point &temp) {
        Point answer;
        answer.x = this->y*temp.z - this->z*temp.y;
        answer.y = this->z*temp.x - this->x*temp.z;
        answer.z = this->x*temp.y - this->y*temp.x;
        return answer;
    }

};