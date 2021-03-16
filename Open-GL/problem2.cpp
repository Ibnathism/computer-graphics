#include <GL/glut.h>
#include "functions.h"

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

    double tempX = myCamera.position.x;
    double tempY = myCamera.position.y;
    double tempZ = myCamera.position.z;
    gluLookAt(tempX, tempY, tempZ, tempX+myCamera.lookDir.x, tempY+myCamera.lookDir.y, tempZ+myCamera.lookDir.z, myCamera.upDir.x, myCamera.upDir.y, myCamera.upDir.z);
    glMatrixMode(GL_MODELVIEW);

    drawAxes();
    glColor3f(1,0,0);
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
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Problem 1");
    init();
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(display);
    glutIdleFunc(animate);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMainLoop();
    return 0;
}