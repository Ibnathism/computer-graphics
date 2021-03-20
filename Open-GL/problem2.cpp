#include <GL/glut.h>
#include "functions.h"

#define TOTAL 5
#define IN_RECTANGLE 0
#define IN_CIRCLE 1
#define NO_SIDE 0
#define HORIZONTAL_SIDE 1
#define VERTICAL_SIDE 2

double circleRadius = 60;
double bubbleRadius = 10.0;
double bubbleSpeed = 0.1;
double offset = 11.0;
bool isPlay = true;

class Rect2D {
public:
    Point topLeft, bottomLeft, topRight, bottomRight;

    Rect2D(){
        topLeft = Point();
        bottomLeft = Point();
        topRight = Point();
        bottomRight = Point();
    }
    Rect2D(double width, double height){
        double w = width/5.0;
        double h = height/5.0;
        topLeft = Point(-w, h, 0);
        bottomLeft = Point(-w, -h, 0);
        topRight = Point(w, h, 0);
        bottomRight = Point(w, -h, 0);
    }

    int pointState(Point &temp) {
        double minimumX = getMinimum(this->topLeft.x, this->bottomLeft.x, this->topRight.x, this->bottomRight.x);
        double minimumY = getMinimum(this->topLeft.y, this->bottomLeft.y, this->topRight.y, this->bottomRight.y);
        double maximumX = getMaximum(this->topLeft.x, this->bottomLeft.x, this->topRight.x, this->bottomRight.x);
        double maximumY = getMaximum(this->topLeft.y, this->bottomLeft.y, this->topRight.y, this->bottomRight.y);
        if (temp.x-offset < minimumX) return VERTICAL_SIDE;
        else if (temp.x+offset > maximumX) return VERTICAL_SIDE;
        else if (temp.y+offset > maximumY) return HORIZONTAL_SIDE;
        else if (temp.y-offset < minimumY) return HORIZONTAL_SIDE;
        else return NO_SIDE;
    }
};

class Circle2D {
public:
    Point center;
    double radius;

    Circle2D() {}

    Circle2D(const Point &center, double radius) : center(center), radius(radius) {}

    bool isInside(Point & bubbleCenter, double r) {
        return centerToCenterDistance(this->center, bubbleCenter) < (this->radius - r);
    }
};

class Bubble2D {
public:
    Point vectorDir;
    Point center;
    double radius;
    int region;
    bool appeared;

    Bubble2D() {
        vectorDir = getNormalizedPoint(Point(getRandom(-1.0, 1.0), getRandom(-1.0, 1.0), 0));
        center = Point();
        radius = bubbleRadius;
        region = IN_RECTANGLE;
        appeared = false;
    }

    Bubble2D(const Point &center) : center(center) {
        vectorDir = getNormalizedPoint(Point(getRandom(-1.0, 1.0), getRandom(-1.0, 1.0), 0));
        radius = bubbleRadius;
        region = IN_RECTANGLE;
        appeared = false;
    }


};

Bubble2D * bubble2DList[TOTAL];
Circle2D circle;
Rect2D rectangle;

void keyboardListener(unsigned char key, int x,int y){
    switch(key){
        case 'p':
            if (isPlay) isPlay = false;
            else isPlay = true;
            break;
        default:
            break;
    }
}


void specialKeyListener(int key, int x,int y){
    switch(key){
        case GLUT_KEY_DOWN:
            if (bubbleSpeed - 0.02 >= 0) bubbleSpeed = bubbleSpeed - 0.02;
            else bubbleSpeed = 0;
            break;
        case GLUT_KEY_UP:
            if (bubbleSpeed + 0.02 <= 0.7) bubbleSpeed = bubbleSpeed + 0.02;
            else bubbleSpeed = 0.7;
            break;
        default:
            break;
    }
}


void display(){
    clear();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0, 0, 150, 0, 0, 0, 0, 1, 0);
    glMatrixMode(GL_MODELVIEW);
    glColor3f(1, 0, 0);
    draw2DCircle(circle.center.x, circle.center.y, circle.radius, 80);
    glColor3f(0, 1, 0);
    draw2DRectangle(rectangle.topLeft, rectangle.bottomLeft, rectangle.topRight, rectangle.bottomRight);
    for (int i = 0; i < TOTAL; ++i) {
        if (!bubble2DList[i]->appeared) continue;
        glColor3f(1.0, 1.0, 0);
        draw2DCircle(bubble2DList[i]->center.x, bubble2DList[i]->center.y, bubble2DList[i]->radius, 80);
    }
    glutSwapBuffers();
}

void animate(){

    for (int i = 0; i < TOTAL; ++i) {
        Bubble2D *temp = bubble2DList[i];
        if (isPlay) {
            if (temp->appeared && temp->region != IN_CIRCLE) {
                Point p = bubble2DList[i]->vectorDir.constantScale(bubbleSpeed);
                Point toPosition = bubble2DList[i]->center.summation(p);

                if (rectangle.pointState(toPosition) == HORIZONTAL_SIDE) {
                    temp->vectorDir = Point(temp->vectorDir.x, -temp->vectorDir.y, temp->vectorDir.z);
                } else if (rectangle.pointState(toPosition) == VERTICAL_SIDE) {
                    temp->vectorDir = Point(-temp->vectorDir.x, temp->vectorDir.y, temp->vectorDir.z);
                } else if (rectangle.pointState(toPosition) == NO_SIDE) {
                    temp->center = toPosition;
                    if (circle.isInside(temp->center, temp->radius)) temp->region = IN_CIRCLE;
                }
            }
        }
    }
    for (int i = 0; i < TOTAL; ++i) {
        Bubble2D *temp = bubble2DList[i];
        if (isPlay) {
            if (temp->appeared && temp->region != IN_RECTANGLE) {
                Point p = bubble2DList[i]->vectorDir.constantScale(bubbleSpeed);
                Point toPosition = bubble2DList[i]->center.summation(p);

                if (circle.isInside(toPosition, temp->radius)) temp->center = toPosition;
                else {
                    Point norm = getNormalizedPoint(circle.center.subtraction(toPosition));
                    Point scaled = norm.constantScale(2 * norm.dotMultiplication(temp->vectorDir));
                    temp->vectorDir = getNormalizedPoint(temp->vectorDir.subtraction(scaled));
                }
            }
        }
    }
    for (int i = 0; i < TOTAL; ++i) {
        Bubble2D *temp = bubble2DList[i];
        if (isPlay) {
            if (temp->appeared && temp->region != IN_RECTANGLE) {
                for (int j = 0; j < TOTAL; ++j) {
                    if (i != j) {
                        Bubble2D *other = bubble2DList[j];
                        if (other->region != IN_RECTANGLE) {
                            if (isBubblesColliding(temp->center, other->center, bubbleRadius) && temp->region==IN_CIRCLE && other->region==IN_CIRCLE) {
                                Point norm = getNormalizedPoint(temp->center.subtraction(other->center));

                                Point scaled = norm.constantScale(2 * norm.dotMultiplication(temp->vectorDir));
                                temp->vectorDir = getNormalizedPoint(temp->vectorDir.subtraction(scaled));
                                Point centerOffset = temp->vectorDir.constantScale(bubbleSpeed);
                                temp->center = temp->center.summation(centerOffset);

                                scaled = norm.constantScale(2 * norm.dotMultiplication(other->vectorDir));
                                other->vectorDir = getNormalizedPoint(other->vectorDir.subtraction(scaled));
                                centerOffset = other->vectorDir.constantScale(bubbleSpeed);
                                other->center = other->center.summation(centerOffset);
                            }
                        }
                    }
                }
            }
        }
    }
    glutPostRedisplay();
}


void bringBubblesOneByOne(int x) {
    int count = 0;
    for (int i = 0; i < TOTAL; ++i) {
        if (bubble2DList[i]->appeared) continue;
        else {
            count++;
            bubble2DList[i]->appeared = true;
            break;
        }
    }
    if (count<TOTAL) glutTimerFunc(10000, bringBubblesOneByOne, x);
}

void init(){
    clear();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80,	1,	1,	1000.0);
    rectangle = Rect2D(500, 500);
    Point center(0, 0, 0);
    circle = Circle2D(center, circleRadius);

    for (int i = 0; i < TOTAL; ++i) {
        double bCenterX = rectangle.bottomLeft.x + bubbleRadius + initOffset;
        double bCenterY = rectangle.bottomLeft.y + bubbleRadius + initOffset;
        Bubble2D *temp = new Bubble2D(Point(bCenterX, bCenterY, 0));
        bubble2DList[i] = temp;
    }
    bringBubblesOneByOne(0);
}

int main(int argc, char **argv){
    glutInit(&argc,argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Problem 2");
    init();
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(display);
    glutIdleFunc(animate);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMainLoop();
    return 0;
}