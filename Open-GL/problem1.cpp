#include <GL/glut.h>
#include "functions.h"

Point position = Point(150, 150, 50);
Point rightDir = Point(-1/sqrt(2.0), 1/sqrt(2.0), 0);
Point upDir = Point(0,0,1);
Point lookDir = upDir.crossMultiplication(rightDir);

void keyboardListener(unsigned char key, int x,int y){
    double positiveAngle = 3.0;
    double negativeAngle = -3.0;

    switch(key){
        case '1':
            lookDir = rotateOneAlongAnother(lookDir, upDir, positiveAngle);
            rightDir = lookDir.crossMultiplication(upDir);
            break;
        case '2':
            lookDir = rotateOneAlongAnother(lookDir, upDir, negativeAngle);
            rightDir = lookDir.crossMultiplication(upDir);
            break;
        case '3':
            lookDir = rotateOneAlongAnother(lookDir, rightDir, positiveAngle);
            upDir = rightDir.crossMultiplication(lookDir);
            break;
        case '4':
            lookDir = rotateOneAlongAnother(lookDir, rightDir, negativeAngle);
            upDir = rightDir.crossMultiplication(lookDir);
            break;
        case '5':
            rightDir = rotateOneAlongAnother(rightDir, lookDir, negativeAngle);
            upDir = rightDir.crossMultiplication(lookDir);
            break;
        case '6':
            rightDir = rotateOneAlongAnother(rightDir, lookDir, positiveAngle);
            upDir = rightDir.crossMultiplication(lookDir);
            break;
        case 'q':
            applyQRotation();
            break;
        case 'w':
            applyWRotation();
            break;
        case 'e':
            applyERotation();
            break;
        case 'r':
            applyRRotation();
            break;
        case 'a':
            applyARotation();
            break;
        case 's':
            applySRotation();
        case 'd':
            applyDRotation();
            break;
        case 'f':
            applyFRotation();
            break;
        default:
            break;
    }
}
void specialKeyListener(int key, int x,int y){
    switch(key){
        case GLUT_KEY_DOWN:
            position = position.subtraction(lookDir);
            break;
        case GLUT_KEY_UP:
            position = position.summation(lookDir);
            break;
        case GLUT_KEY_RIGHT:
            position = position.summation(rightDir);
            break;
        case GLUT_KEY_LEFT:
            position = position.subtraction(rightDir);
            break;
        case GLUT_KEY_PAGE_UP:
            position = position.summation(upDir);
            break;
        case GLUT_KEY_PAGE_DOWN:
            position = position.subtraction(upDir);
            break;
        default:
            break;
    }
}
void mouseListener(int button, int state, int x, int y){
    switch(button){
        case GLUT_LEFT_BUTTON:
            if(state == GLUT_DOWN){
                fireBullets();
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
    gluLookAt(position.x, position.y, position.y, position.x+lookDir.x, position.y+lookDir.y, position.y+lookDir.z, upDir.x, upDir.y, upDir.z);
    glMatrixMode(GL_MODELVIEW);
    glColor3f(1, 1, 1);
    drawAxes();
    drawGun(18, 80, 6, 9, 1.2);
    glColor3f(0.5, 0.5, 0.5);
    drawSolidRectangle(Point(-100, 100, 200), Point(-100, -100, 200), Point(100, 100, 200), Point(100, -100, 200));
    glColor3f(1, 0, 0);
    drawBulletsOnPlane();
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
    glutMouseFunc(mouseListener);
    glutMainLoop();
    return 0;
}