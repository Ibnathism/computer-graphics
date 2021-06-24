#include <GL/glut.h>
#include<bits/stdc++.h>
#include "functions.h"

Point position = Point(150, 150, 50);
Point rightDir = Point(-1/sqrt(2.0), 1/sqrt(2.0), 0);
Point upDir = Point(0,0,1);
Point lookDir = upDir.crossMultiplication(rightDir);

Floor baseFloor;
vector<Sphere> allSpheres;
double pixels;
int recursionLevel, objectCount;
void keyboardListener(unsigned char key, int x,int y){
    double positiveAngle = 3.0;
    double negativeAngle = -3.0;

    switch(key){
        case '1':
            lookDir = rotateOneAlongAnother(lookDir, upDir, positiveAngle);
            rightDir = lookDir.crossMultiplication(upDir);
            //std::cout << "Position: " << position.x << " , " << position.y << " , " << position.z << std::endl;
            break;
        case '2':
            lookDir = rotateOneAlongAnother(lookDir, upDir, negativeAngle);
            rightDir = lookDir.crossMultiplication(upDir);
            //std::cout << "Position: " << position.x << " , " << position.y << " , " << position.z << std::endl;
            break;
        case '3':
            lookDir = rotateOneAlongAnother(lookDir, rightDir, positiveAngle);
            upDir = rightDir.crossMultiplication(lookDir);
            //std::cout << "Position: " << position.x << " , " << position.y << " , " << position.z << std::endl;
            break;
        case '4':
            lookDir = rotateOneAlongAnother(lookDir, rightDir, negativeAngle);
            upDir = rightDir.crossMultiplication(lookDir);
            //std::cout << "Position: " << position.x << " , " << position.y << " , " << position.z << std::endl;
            break;
        case '5':
            rightDir = rotateOneAlongAnother(rightDir, lookDir, negativeAngle);
            upDir = rightDir.crossMultiplication(lookDir);
            //std::cout << "Position: " << position.x << " , " << position.y << " , " << position.z << std::endl;
            break;
        case '6':
            rightDir = rotateOneAlongAnother(rightDir, lookDir, positiveAngle);
            upDir = rightDir.crossMultiplication(lookDir);
            //std::cout << "Position: " << position.x << " , " << position.y << " , " << position.z << std::endl;
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
            std::cout << "Position: " << position.x << " , " << position.y << " , " << position.z << std::endl;
            break;
        case GLUT_KEY_UP:
            position = position.summation(lookDir);
            std::cout << "Position: " << position.x << " , " << position.y << " , " << position.z << std::endl;
            break;
        case GLUT_KEY_RIGHT:
            position = position.summation(rightDir);
            std::cout << "Position: " << position.x << " , " << position.y << " , " << position.z << std::endl;
            break;
        case GLUT_KEY_LEFT:
            position = position.subtraction(rightDir);
            std::cout << "Position: " << position.x << " , " << position.y << " , " << position.z << std::endl;
            break;
        case GLUT_KEY_PAGE_UP:
            position = position.summation(upDir);
            std::cout << "Position: " << position.x << " , " << position.y << " , " << position.z << std::endl;
            break;
        case GLUT_KEY_PAGE_DOWN:
            position = position.subtraction(upDir);
            std::cout << "Position: " << position.x << " , " << position.y << " , " << position.z << std::endl;
            break;
        default:
            break;
    }
}
void mouseListener(int button, int state, int x, int y){
    switch(button){
        case GLUT_LEFT_BUTTON:
            //........
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
    //glColor3f(1, 1, 1);
    //drawAxes();
    baseFloor.draw();
    for (int i = 0; i < allSpheres.size(); ++i) {
        allSpheres[i].draw(100, 100);
    }
    glutSwapBuffers();
}
void animate(){
    glutPostRedisplay();
}
void loadData() {
    fstream in;
    in.open("scene.txt");
    in >> recursionLevel;
    in >> pixels;
    in >> objectCount;
    string type;
    for (int i = 0; i < objectCount; ++i) {
        in >> type;
        Point center;
        double radius;
        double red, green, blue;
        double ambient, diffuse, specular, reflection;
        int shine;
        vector<double> color;
        vector<double> coefficients;
        if (type == "sphere") {
            in >> center.x >> center.y >> center.z;
            in >> radius;
            in >> red >> green >> blue;
            color.push_back(red);
            color.push_back(green);
            color.push_back(blue);
            in >> ambient >> diffuse >> specular >> reflection;
            coefficients.push_back(ambient);
            coefficients.push_back(diffuse);
            coefficients.push_back(specular);
            coefficients.push_back(reflection);
            in >> shine;
            Sphere sphere(center, radius);
            sphere.setColor(color);
            sphere.setCoefficient(coefficients);
            sphere.setShine(shine);
            allSpheres.push_back(sphere);
        }
    }
}
void init(){
    clear();
    loadData();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80,	1,	1,	1000.0);

}

int main(int argc, char **argv){
    glutInit(&argc,argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Ray Tracing");
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