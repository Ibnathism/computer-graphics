
#include <GL/glut.h>
#include "functions.h"

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

class Cam {
public:
    Point position;
    Point lookDir;
    Point upDir;
    Point rightDir;

    Cam() {
        position = Point(150, 150, 50);
        rightDir = Point(-1/sqrt(2.0), 1/sqrt(2.0), 0);
        upDir = Point(0,0,1);
        lookDir = upDir.crossMultiplication(rightDir);
    }
};

Cam myCamera;



void keyboardListener(unsigned char key, int x,int y){
    switch(key){

        case '1':
            drawgrid=1-drawgrid;
            break;

        default:
            break;
    }
}


void specialKeyListener(int key, int x,int y){
    switch(key){
        case GLUT_KEY_DOWN:
            myCamera.position = myCamera.position.subtraction(myCamera.lookDir);
            break;
        case GLUT_KEY_UP:
            myCamera.position = myCamera.position.summation(myCamera.lookDir);
            break;
        case GLUT_KEY_RIGHT:
            myCamera.position = myCamera.position.summation(myCamera.rightDir);
            break;
        case GLUT_KEY_LEFT:
            myCamera.position = myCamera.position.subtraction(myCamera.rightDir);
            break;
        case GLUT_KEY_PAGE_UP:
            myCamera.position = myCamera.position.summation(myCamera.upDir);
            break;
        case GLUT_KEY_PAGE_DOWN:
            myCamera.position = myCamera.position.subtraction(myCamera.upDir);
            break;
        default:
            break;
    }
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
    switch(button){
        case GLUT_LEFT_BUTTON:
            if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
                drawaxes=1-drawaxes;
            }
            break;

        case GLUT_RIGHT_BUTTON:
            //........
            break;

        case GLUT_MIDDLE_BUTTON:
            //........
            break;

        default:
            break;
    }
}



void display(){
    clear();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    double tempX = myCamera.position.x;
    double tempY = myCamera.position.y;
    double tempZ = myCamera.position.z;
    gluLookAt(tempX, tempY, tempZ, tempX+myCamera.lookDir.x, tempY+myCamera.lookDir.y, tempZ+myCamera.lookDir.z, myCamera.upDir.x, myCamera.upDir.y, myCamera.upDir.z);
    glMatrixMode(GL_MODELVIEW);

    drawAxes();
    //drawGrid();

    glColor3f(1,0,0);
    //drawSquare(10);

    //drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

    drawSphere(30,24,20);

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
}

int main(int argc, char **argv){
    glutInit(&argc,argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

    glutCreateWindow("Problem 1");

    init();

    glEnable(GL_DEPTH_TEST);	//enable Depth Testing

    glutDisplayFunc(display);	//display callback function
    glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    //glutMouseFunc(mouseListener);

    glutMainLoop();		//The main loop of OpenGL

    return 0;
}