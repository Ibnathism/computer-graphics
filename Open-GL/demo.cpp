//
// Created by ashiq on 3/14/21.
//

#include "util.h"

double cameraHeight = 150;
double cameraAngle = 1.0;
double angle = 0;
int DRAW_GRID = 0;
int DRAW_AXIS = 1;

void keyboardListener(unsigned char key, int x, int y) {

    switch (key) {
        case '1':
            DRAW_GRID = 1 - DRAW_GRID;
            break;

        default:
            break;
    }

}


void specialKeyListener(int key, int x, int y) {

    switch (key) {

        case GLUT_KEY_DOWN:
            cameraHeight -= 3.0;
            break;
        case GLUT_KEY_UP:
            cameraHeight += 3.0;
            break;

        case GLUT_KEY_RIGHT:
            cameraAngle += 0.03;
            break;
        case GLUT_KEY_LEFT:
            cameraAngle -= 0.03;
            break;

        /* case GLUT_KEY_PAGE_UP:
            break;
        case GLUT_KEY_PAGE_DOWN:
            break;

        case GLUT_KEY_INSERT:
            break;

        case GLUT_KEY_HOME:
            break;
        case GLUT_KEY_END:
            break;

        default:
            break;
        */
    }

}


void mouseListener(int button, int state, int x, int y) {    //x, y is the x-y of the screen (2D)

    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN) {        // 2 times?? in ONE click? -- solution is checking DOWN or UP
                DRAW_AXIS = 1 - DRAW_AXIS;
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

void init() {


    //clear the screen
    clearDisplay();

    /************************
    / set-up projection here
    ************************/
    //load the PROJECTION matrix
    glMatrixMode(GL_PROJECTION);

    //initialize the matrix
    glLoadIdentity();

    //give PERSPECTIVE parameters
    gluPerspective(80, 1, 1, 1000.0);
    //field of view in the Y (vertically)
    //aspect ratio that determines the field of view in the X direction (horizontally)
    //near distance
    //far distance
}

void drawSS() {
    glColor3f(1, 0, 0);
    drawSquare(20);

    glRotatef(angle, 0, 0, 1);
    glTranslatef(110, 0, 0);
    glRotatef(2 * angle, 0, 0, 1);
    glColor3f(0, 1, 0);
    drawSquare(15);

    glPushMatrix();

    {
        glRotatef(angle, 0, 0, 1);
        glTranslatef(60, 0, 0);
        glRotatef(2 * angle, 0, 0, 1);
        glColor3f(0, 0, 1);
        drawSquare(10);
    }

    glPopMatrix();

    glRotatef(3 * angle, 0, 0, 1);
    glTranslatef(40, 0, 0);
    glRotatef(4 * angle, 0, 0, 1);
    glColor3f(1, 1, 0);
    drawSquare(5);
}

void animate() {
    angle += 0.2;

    glutPostRedisplay();
}

void display() {

    clearDisplay();

    /********************
    / set-up camera here
    ********************/
    //load the correct matrix -- MODEL-VIEW matrix
    glMatrixMode(GL_MODELVIEW);

    //initialize the matrix
    glLoadIdentity();

    //now give three info
    //1. where is the camera (viewer)?
    //2. where is the camera looking?
    //3. Which direction is the camera's UP direction?

    //gluLookAt(100,100,100,	0,0,0,	0,0,1);
    //gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
    gluLookAt(0, 0, 200, 0, 0, 0, 0, 1, 0);


    //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);


    /****************************
    / Add your objects from here
    ****************************/
    //add objects

    if (DRAW_AXIS == 1) drawAxes();
    if (DRAW_GRID == 1) drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

    drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

    //drawSphere(30,24,20);




    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}


int main(int argc, char **argv) {

    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);    //Depth, Double buffer, RGB color

    glutCreateWindow("My OpenGL Program");

    init();

    glEnable(GL_DEPTH_TEST);    //enable Depth Testing

    glutDisplayFunc(display);    //display callback function
    glutIdleFunc(animate);        //what you want to do in the idle time (when no drawing is occuring)

    registerKeyboardListener(keyboardListener);
    registerSpecialKeysListener(specialKeyListener);
    registerMouseListener(mouseListener);

    glutMainLoop();        //The main loop of OpenGL

    return 0;
}

