#include <iostream>
#include <cmath>
#include <vector>

#define PI 3.1416
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

    double dotMultiplication(Point &temp) const {
        double dot = this->x*temp.x + this->y*temp.y + this->z*temp.z;
        return dot;
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
        std::cout << '(' << this->x <<", " << this->y << ", " << this->z << ')' << std::endl;
    }
};

class Matrix_2D {
public:
    std::vector<std::vector<double>> m;

    Matrix_2D(int nRows, int nColumns) {
        std::vector<double> row(nColumns, 0);
        for (int i = 0; i < nRows; ++i) {
            m.push_back(row);
        }
    }

    Matrix_2D operator*(Matrix_2D &temp) const {
        int nRow1 = this->m.size();
        int nRow2 = temp.m.size();
        int nColumn1 = this->m[0].size();
        int nColumn2 = temp.m[0].size();
        if (nColumn1 != nRow2) {
            std::cout << "Matrix Multiplication not possible" << std::endl;
            return Matrix_2D(nRow1, nColumn1);
        }
        Matrix_2D answer(nRow1, nColumn2);
        for (int i = 0; i < nRow1; ++i) {
            for (int j = 0; j < nColumn2; ++j) {
                double value = 0;
                for (int k = 0; k < nRow1; ++k) {
                    value = value + this->m[i][k] * temp.m[k][j];
                }
                answer.m[i][j] = value;
            }
        }
        return answer;
    }

    void print() {
        std::cout << std::endl;
        for (int i = 0; i < this->m.size(); ++i) {
            std::cout << "[";
            for (int j = 0; j < this->m[0].size(); ++j) {
                std::cout << this->m[i][j] << "  ";
            }
            std::cout << "]" << std::endl;
        }
    }


};

class Functions {
public:
    static double convertToRadian(double angle) {
        return angle * (PI/180.0);
    }
    static Matrix_2D getIdentityMatrix(int nRows, int nColumns) {
        Matrix_2D temp(nRows, nColumns);
        for (int i = 0, j=0; i < nRows && j < nColumns; ++i, ++j) {
            temp.m[i][j] = 1;
        }
        return temp;
    }
    static Matrix_2D getTranslationMatrix(Point point) {
        Matrix_2D identity = getIdentityMatrix(4, 4);
        identity.m[0][3] = point.x;
        identity.m[1][3] = point.y;
        identity.m[2][3] = point.z;
        return identity;
    }
    static Matrix_2D getRotationalMatrix(Point l, Point r, Point u) {
        Matrix_2D identity = getIdentityMatrix(4, 4);
        identity.m[0][0] = r.x;
        identity.m[0][1] = r.y;
        identity.m[0][2] = r.z;

        identity.m[1][0] = u.x;
        identity.m[1][1] = u.y;
        identity.m[1][2] = u.z;

        identity.m[2][0] = -l.x;
        identity.m[2][1] = -l.y;
        identity.m[2][2] = -l.z;

        return identity;
    }
    static Matrix_2D getProjectionMatrix(double t, double r, double near, double far) {
        Matrix_2D temp(4, 4);
        temp.m[0][0] = near/r;
        temp.m[1][1] = near/t;
        temp.m[2][2] = -((far+near)/(far-near));
        temp.m[2][3] = -((2*far*near)/(far-near));
        temp.m[3][2] = -1;
        return temp;
    }


};

Matrix_2D viewTransformation(Point eye, Point look, Point up) {
    Point l = look - eye;
    l = l.normalizePoint();
    Point r = l.crossMultiplication(up);
    r = r.normalizePoint();
    Point u = r.crossMultiplication(l);
    Point negativeEye = -eye;
    Matrix_2D T = Functions::getTranslationMatrix(negativeEye);
    Matrix_2D R = Functions::getRotationalMatrix(l, r, u);
    Matrix_2D V = R * T;
    return V;
}

Matrix_2D projectionTransformation(double fovY, double aspectRatio, double near, double far) {
    double fovX = fovY * aspectRatio;
    double t = near * tan(Functions::convertToRadian(fovY/2));
    double r = near * tan(Functions::convertToRadian(fovX/2));
    Matrix_2D P = Functions::getProjectionMatrix(t, r, near, far);
    return P;

}
int main() {
    Point eye(0.0, 0.0, 50.0);
    Point look(5.0, 10.0, 0.0);
    Point up(0.0, 1.0, 0.0);

    Matrix_2D viewT = viewTransformation(eye, look, up);
    viewT.print();

    double fovY = 80.0;
    double aspectRatio = 1.0;
    double near = 1.0;
    double far = 100.0;

    Matrix_2D projT = projectionTransformation(fovY, aspectRatio, near, far);
    projT.print();

    return 0;
}