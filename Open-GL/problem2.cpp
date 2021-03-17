#include <GL/glut.h>
#include "functions.h"

#define TOTAL 5
#define IN_RECTANGLE 0
#define IN_CIRCLE 1

double circleRadius = 60;
double bubbleRadius = 10.0;
double rectangleWidth = 400;
double rectangleHeight = 400;
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
        double w = width/4.0;
        double h = height/4.0;
        topLeft = Point(-w, h, 0);
        bottomLeft = Point(-w, -h, 0);
        topRight = Point(w, h, 0);
        bottomRight = Point(w, -h, 0);
    }
};

class Circle2D {
public:
    Point center;
    double radius;

    Circle2D() {}

    Circle2D(const Point &center, double radius) : center(center), radius(radius) {}

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
        region = IN_CIRCLE;
        appeared = false;
    }
};

Bubble2D * bubble2DList[TOTAL];
Circle2D circle;
Rect2D rectangle;

void keyboardListener(unsigned char key, int x,int y){
    switch(key){
        case 'p':
            break;
        default:
            break;
    }
}


void specialKeyListener(int key, int x,int y){
    switch(key){
        case GLUT_KEY_DOWN:

            break;
        case GLUT_KEY_UP:

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
    glColor3f(0, 1, 0);
    draw2DRectangle(rectangle.topLeft, rectangle.bottomLeft, rectangle.topRight, rectangle.bottomRight);
    glColor3f(1, 0, 0);
    draw2DCircle(circle.center.x, circle.center.y, circle.radius, 80);
    for (int i = 0; i < TOTAL; ++i) {
        glColor3f(1.0, 1.0, 0);
        draw2DCircle(bubble2DList[i]->center.x, bubble2DList[i]->center.y, bubble2DList[i]->radius, 80);
    }
    glutSwapBuffers();
}


void animate(){
    glutPostRedisplay();
}

void init(){
    clear();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80,	1,	1,	1000.0);
    rectangle = Rect2D(rectangleWidth, rectangleHeight);
    Point center(0, 0, 0);
    circle = Circle2D(center, circleRadius);

    for (auto & i : bubble2DList) {
        double bCenterX = rectangle.bottomLeft.x + bubbleRadius;
        double bCenterY = rectangle.bottomLeft.y + bubbleRadius;
        auto *temp = new Bubble2D(Point(bCenterX, bCenterY, 0));
        i = temp;
    }
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