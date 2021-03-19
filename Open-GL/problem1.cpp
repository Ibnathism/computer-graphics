#include <GL/glut.h>
#include "functions.h"


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
    double positiveAngle = 3.0;
    double negativeAngle = -3.0;

    switch(key){
        case '1':
            myCamera.lookDir = rotateOneAlongAnother(myCamera.lookDir, myCamera.upDir, positiveAngle);
            myCamera.rightDir = myCamera.lookDir.crossMultiplication(myCamera.upDir);
            break;
        case '2':
            myCamera.lookDir = rotateOneAlongAnother(myCamera.lookDir, myCamera.upDir, negativeAngle);
            myCamera.rightDir = myCamera.lookDir.crossMultiplication(myCamera.upDir);
            break;
        case '3':
            myCamera.lookDir = rotateOneAlongAnother(myCamera.lookDir, myCamera.rightDir, positiveAngle);
            myCamera.upDir = myCamera.rightDir.crossMultiplication(myCamera.lookDir);
            break;
        case '4':
            myCamera.lookDir = rotateOneAlongAnother(myCamera.lookDir, myCamera.rightDir, negativeAngle);
            myCamera.upDir = myCamera.rightDir.crossMultiplication(myCamera.lookDir);
            break;
        case '5':
            myCamera.rightDir = rotateOneAlongAnother(myCamera.rightDir, myCamera.lookDir, negativeAngle);
            myCamera.upDir = myCamera.rightDir.crossMultiplication(myCamera.lookDir);
            break;
        case '6':
            myCamera.rightDir = rotateOneAlongAnother(myCamera.rightDir, myCamera.lookDir, positiveAngle);
            myCamera.upDir = myCamera.rightDir.crossMultiplication(myCamera.lookDir);
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
    glColor3f(1, 1, 1);
    drawAxes();
    //drawGunHandle(18,50,200);
    //drawGunBody(80, 6, 50, 200);
    //drawGunHead(80, 9, 1.2, 50, 200);

    drawGun(18, 0, 0);
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
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Problem 1");
    init();
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(display);
    glutIdleFunc(animate);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    //glutMouseFunc(mouseListener);
    glutMainLoop();
    return 0;
}