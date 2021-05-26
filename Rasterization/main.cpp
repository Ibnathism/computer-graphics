#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <string>
#include <stack>
#include <iomanip>
#include "bitmap_image.h"
#define PI (2*acos(0.0))


class Screen {
public:
    int screenWidth{}, screenHeight{};
    double leftLimitX{}, rightLimitX{}, bottomLimitY{}, topLimitY{}, frontLimitZ{}, rearLimitZ{};
    double dx{}, dy{}, topY{}, leftX{}, bottomY{}, rightX{}, zMax{};
    std::vector<std::vector<double>> zBuffer;

    void allocateBuffer(int width, int height) {
        for (int i = 0; i < zBuffer.size(); ++i) {
            zBuffer[i].clear();
        }
        zBuffer.clear();
        this->screenWidth = width;
        this->screenHeight = height;
        std::vector<double> row(width, 0);
        for (int i = 0; i < height; ++i) {
            zBuffer.push_back(row);
        }
    }

    void initializeBuffers() {
        dx = (rightLimitX-leftLimitX)/screenWidth;
        dy = (topLimitY-bottomLimitY)/screenHeight;

        topY = topLimitY - dy/2;
        leftX = leftLimitX + dx/2;
        bottomY = bottomLimitY + dy/2;
        rightX = rightLimitX - dx/2;

        this->zMax = rearLimitZ;
        for (int i = 0; i < screenWidth; ++i) {
            for (int j = 0; j < screenHeight; ++j) {
                zBuffer[i][j] = zMax;
            }
        }
    }

};
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
        std::cout << this->x <<" " << this->y << " " << this->z << std::endl;
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

class Triangle {
public:
    Point point[3];
    int color[3];
    Triangle(Point a, Point b, Point c) {
        point[0] = a;
        point[1] = b;
        point[2] = c;
        int min = 0;
        int max = 255;
        color[0] = min + ( std::rand() % ( max - min + 1 ) );
        color[1] = min + ( std::rand() % ( max - min + 1 ) );
        color[2] = min + ( std::rand() % ( max - min + 1 ) );
    }

    void print() {
        for (Point p:point) {
            p.print();
        }
    }

    double getTopScanline(Screen &screen) {
        double max = std::max(this->point[0].y, this->point[1].y);
        max = std::max(max, this->point[2].y);
        double topRow = screen.topY;
        if (max > topRow) return topRow;
        while (max < topRow) {
            topRow = topRow - screen.dy;
        }
        return topRow + screen.dy;
    }

    double getBottomScanline(Screen &screen) {
        double min = std::min(this->point[0].y, this->point[1].y);
        min = std::min(min, this->point[2].y);
        double bottomRow = screen.bottomY;
        if (min < bottomRow) return bottomRow;
        while (min > bottomRow) {
            bottomRow = bottomRow + screen.dy;
        }
        return bottomRow - screen.dy;
    }

    double getLeftColumn(Screen &screen) {
        double min = std::min(this->point[0].x, this->point[1].x);
        min = std::min(min, this->point[2].x);
        double leftCol = screen.leftX;
        if (min < leftCol) return leftCol;
        while (min > leftCol) {
            leftCol = leftCol + screen.dx;
        }
        return leftCol - screen.dx;
    }

    double getRightColumn(Screen &screen) {
        double max = std::max(this->point[0].x, this->point[1].x);
        max = std::max(max, this->point[2].x);
        double rightCol = screen.rightX;
        if (max > rightCol) return rightCol;
        while (max < rightCol) {
            rightCol = rightCol - screen.dx;
        }
        return rightCol + screen.dx;
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
        temp.m[3][0] = 1;
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

void modelingTransformation() {
    std::fstream infile;
    infile.open("scene.txt");
    if (!infile.is_open()) {
        std::cout << "Cant open file" << std::endl;
        return;
    }
    std::ofstream modelOut, viewOut, projOut;
    modelOut.open("stage1.txt");
    modelOut << std::setprecision(7) << std::fixed;
    viewOut.open("stage2.txt");
    viewOut << std::setprecision(7) << std::fixed;
    projOut.open("stage3.txt");
    projOut << std::setprecision(7) << std::fixed;
    ///GluLookAt
    Point eye, look, up;
    infile >> eye.x >> eye.y >> eye.z;
    infile >> look.x >> look.y >> look.z;
    infile >> up.x >> up.y >> up.z;
    Matrix_2D viewT = viewTransformation(eye, look, up);
    //viewT.print();

    ///GluPerspective
    double fovY, aspectRatio, near, far;
    infile >> fovY >> aspectRatio >> near >> far;
    Matrix_2D projT = projectionTransformation(fovY, aspectRatio, near, far);
    //projT.print();

    ///Modeling Transformation
    std::stack<Matrix_2D> stack;
    std::stack<std::stack<Matrix_2D>> all;
    Matrix_2D I(Functions::getIdentityMatrix(4, 4));
    stack.push(I);
    std::string strCommand;
    while (true) {

        infile >> strCommand;
        std::cout << strCommand << std::endl;
        if (strCommand=="triangle") {
            Point triangleCorners[3];
            //Point transformerCorners[3];
            std::cout << "Found triangle" << std::endl;
            for (auto & triangleCorner : triangleCorners) {
                infile >> triangleCorner.x >> triangleCorner.y >> triangleCorner.z;
                //std::cout << "Point" << std::endl;
                //triangleCorners[i].print();
                Point model = Functions::transformPoint(triangleCorner, stack.top());
                std::cout << "model" << std::endl;
                model.print();
                modelOut << model.x << " " << model.y << " " << model.z << std::endl;
                Point view = Functions::transformPoint(model, viewT);
                Point proj = Functions::transformPoint(view, projT);
                std::cout << "view" << std::endl;
                view.print();
                viewOut << view.x << " " << view.y << " " << view.z << std::endl;
                std::cout << "projection" << std::endl;
                proj.print();
                projOut << proj.x << " " << proj.y << " " << proj.z << std::endl;

            }
            modelOut << std::endl;
            viewOut << std::endl;
            projOut << std::endl;
        } else if (strCommand=="translate") {
            std::cout << "Found translate" << std::endl;
            Point translationAmount;
            infile >> translationAmount.x >> translationAmount.y >> translationAmount.z;
            Matrix_2D transMat = Functions::getTranslationMatrix(translationAmount);
            stack.push(stack.top()*transMat);
        } else if (strCommand=="scale") {
            std::cout << "Found scale" << std::endl;
            Point scaleAmount;
            infile >> scaleAmount.x >> scaleAmount.y >> scaleAmount.z;
            Matrix_2D scaleMat = Functions::getScalingMatrix(scaleAmount);
            stack.push(stack.top()*scaleMat);
        } else if (strCommand=="rotate") {
            std::cout << "Found rotate" << std::endl;
            double rotationAngle;
            Point rotationPoint;
            infile >> rotationAngle >> rotationPoint.x >> rotationPoint.y >> rotationPoint.z;
            rotationPoint = rotationPoint.normalizePoint();

            Point xAxis(1, 0, 0);
            Point yAxis(0, 1, 0);
            Point zAxis(0, 0, 1);

            Point firstColumn = Functions::getRodrigues(rotationAngle, xAxis, rotationPoint);
            Point secondColumn = Functions::getRodrigues(rotationAngle, yAxis, rotationPoint);
            Point thirdColumn = Functions::getRodrigues(rotationAngle, zAxis, rotationPoint);

            Point rotationL(-firstColumn.z, -secondColumn.z, -thirdColumn.z);
            Point rotationR(firstColumn.x, secondColumn.x, thirdColumn.x);
            Point rotationU(firstColumn.y, secondColumn.y, thirdColumn.y);

            Matrix_2D rotateMat = Functions::getRotationalMatrix(rotationL, rotationR, rotationU);
            stack.push(stack.top()*rotateMat);

        } else if (strCommand=="push") {
            std::cout << "Found Push" << std::endl;
            all.push(stack);
        } else if (strCommand=="pop") {
            std::cout << "Found Pop" << std::endl;
            stack = all.top();
            all.pop();
        } else if (strCommand=="end") {
            std::cout << "Found end" << std::endl;
            break;
        }
    }
    infile.close();
    modelOut.close();
    viewOut.close();
    projOut.close();
}

Screen screen;
std::vector<Triangle> triangles;

void readData() {
    std::fstream configFile, stage3File;
    configFile.open("config.txt");
    stage3File.open("stage3.txt");
    if (!configFile.is_open() || !stage3File.is_open()) {
        std::cout << "Cant open file" << std::endl;
        return;
    }

    int width, height;

    configFile >> width >> height;
    configFile >> screen.leftLimitX;
    screen.rightLimitX = -screen.leftLimitX;
    configFile >> screen.bottomLimitY;
    screen.topLimitY = -screen.bottomLimitY;
    configFile >> screen.frontLimitZ >> screen.rearLimitZ;

    while (!stage3File.eof()){
        Point a, b, c;
        stage3File >> a.x >> a.y >> a.z;
        stage3File >> b.x >> b.y >> b.z;
        stage3File >> c.x >> c.y >> c.z;
        Triangle t(a, b, c);
        triangles.push_back(t);
    }
    screen.allocateBuffer(width, height);
    configFile.close();
    stage3File.close();
}

void clip() {

}


void applyProcedure(bitmap_image &image) {
    for (int i = 0; i < triangles.size()-1; ++i) {
        Triangle t = triangles[i];
        std::cout << "Triangle" << std::endl;
        t.print();
        //Find top_scanline and bottom_scanline after necessary clipping
        double topScanline = t.getTopScanline(screen);
        double bottomScanline = t.getBottomScanline(screen);
        double leftColumn = t.getLeftColumn(screen);
        double rightColumn = t.getRightColumn(screen);
        std::cout << topScanline << " -------row------ " << bottomScanline << std::endl;
        std::cout << leftColumn << " -----col----- " << rightColumn << std::endl;

        //for row_no from top_scanline to bottom_scanline
        for (double row = topScanline; row > bottomScanline; row = row - screen.dy) {
            //Find left_intersecting_column and right_intersecting_column after necessary clipping
            //For col_no from left_intersecting_column to right_intersecting_column
            for (double col = leftColumn; col < rightColumn ; col = col + screen.dx) {
                image.set_pixel(row, col, t.color[0], t.color[1], t.color[2]);
                //Calculate z values
                //Compare with z-buffer and z_front_limit and update if required
                //Update pixel information if required
            }


        }

    }
}
void clippingAndScanConversion() {
    readData();
    screen.initializeBuffers();

    bitmap_image image(screen.screenWidth, screen.screenHeight);
    applyProcedure(image);
    image.save_image("test.bmp");

}


int main() {
    modelingTransformation();
    clippingAndScanConversion();
    return 0;
}