#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <stack>
#define PI (2*acos(0.0))
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
    static Point getRodrigues(double theta, Point x, Point a) {
        Point third = a.crossMultiplication(x) * sin(convertToRadian(theta));
        Point first = x * cos(convertToRadian(theta));
        Point second = a * a.dotMultiplication(x) * (1-cos(convertToRadian(theta)));
        return first + second + third;
    }
    static Matrix_2D getIdentityMatrix(int nRows, int nColumns) {
        Matrix_2D temp(nRows, nColumns);
        for (int i = 0, j=0; i < nRows && j < nColumns; ++i, ++j) {
            temp.m[i][j] = 1;
        }
        return temp;
    }
    static Matrix_2D alignPointInColumn(Point point) {
        Matrix_2D temp(4, 1);
        temp.m[0][0] = point.x;
        temp.m[1][0] = point.y;
        temp.m[2][0] = point.z;
        return temp;
    }
    static Matrix_2D getTranslationMatrix(Point point) {
        Matrix_2D identity = getIdentityMatrix(4, 4);
        identity.m[0][3] = point.x;
        identity.m[1][3] = point.y;
        identity.m[2][3] = point.z;
        return identity;
    }
    static Matrix_2D getScalingMatrix(Point point) {
        Matrix_2D identity = getIdentityMatrix(4, 4);
        identity.m[0][0] = point.x;
        identity.m[1][1] = point.y;
        identity.m[2][2] = point.z;
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
    static Point transformPoint(Point point, Matrix_2D &matrix2D) {
        Matrix_2D pointMatrix = alignPointInColumn(point);
        Matrix_2D temp = matrix2D * pointMatrix;
        Point tempPoint(temp.m[0][0], temp.m[1][0], temp.m[2][0]);
        double corner = temp.m[temp.m.size()-1][temp.m[0].size()-1];
        return tempPoint/corner;
    }
    static Point readPoint(std::ifstream &infile) {
        std::string str;
        getline(infile, str);
        std::vector<double> vec;
        std::istringstream iss(str);
        std::string token;
        while (std::getline(iss, token, ' ')) {
            vec.push_back(std::stod(token));
        }
        Point temp(vec[0], vec[1], vec[2]);
        return temp;
    }

};

class sp;

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
//    std::ofstream stage1;
//    stage1.open("stage1.txt");
//    std::ofstream stage2;
//    stage2.open("stage2.txt");
//    std::ofstream stage3;
//    stage3.open("stage3.txt");
    std::ifstream infile("scene.txt");
    std::string data;
    if (!infile.is_open()) {
        std::cout << "Cant open file" << std::endl;
        return 0;
    }
    ///GluLookAt
    Point eye = Functions::readPoint(infile);
    Point look = Functions::readPoint(infile);
    Point up = Functions::readPoint(infile);
    Matrix_2D viewT = viewTransformation(eye, look, up);
    //viewT.print();

    ///GluPerspective
    std::string str;
    getline(infile, str);
    std::vector<double> vec;
    std::istringstream iss(str);
    std::string token;
    while (std::getline(iss, token, ' ')) {
        vec.push_back(std::stod(token));
    }

    double fovY = vec[0];
    double aspectRatio = vec[1];
    double near = vec[2];
    double far = vec[3];

    Matrix_2D projT = projectionTransformation(fovY, aspectRatio, near, far);
    //projT.print();

    std::stack<Matrix_2D> stack;
    Matrix_2D I(Functions::getIdentityMatrix(4, 4));
    stack.push(I);


//    while (true) {
//        std::string strCommand;
//
//        getline(infile, strCommand);
//        //std::cout << strCommand << std::endl;
//        //std::cout << (strCommand=="end") << std::endl;
//
//        if (strCommand=="triangle") {
//            //input three points
//            Point triangleCorners[3];
//            Point transformerCorners[3];
//            std::cout << "Found triangle" << std::endl;
//            for (int i = 0; i < 3; ++i) {
//                triangleCorners[i] = Functions::readPoint(infile);
//                triangleCorners[i].print();
//                //transformerCorners[i] = Functions::transformPoint(triangleCorners[i], stack.top());
//                //transformerCorners[i].print();
//            }
//
//                // P’ <- transformPoint(S.top,P)
//                // output P’
//        } else if ((strCommand=="translate") == 0) {
//            //input translation amounts
//            // generate the corresponding translation matrix T
//            // S.push(product(S.top,T))
//        } else if ((strCommand=="scale") == 0) {
//
//        } else if ((strCommand=="rotate") == 0) {
//
//        } else if ((strCommand=="push") == 0) {
//
//        } else if ((strCommand=="pop") == 0) {
//
//        } else if ((strCommand=="end") == 0) {
//            std::cout << "Found end" << std::endl;
//            break;
//        }
//    }
//
//    double theta = 90;
//    Point a(0.0, 0.0, 1.0);
//    Point x(1.0, 0.0, 0.0);
//    Point ans = Functions::getRodrigues(theta, x, a);
//    //ans.print();

//    stage3.close();
//    stage2.close();
//    stage1.close();

    infile.close();
    return 0;
}