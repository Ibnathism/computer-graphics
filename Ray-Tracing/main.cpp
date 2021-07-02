#include <GL/glut.h>
#include<bits/stdc++.h>
#include "functions.h"
#include "bitmap_image.h"
#define WINDOW_HEIGHT 500
#define WINDOW_WIDTH 500
#define IMAGE_WIDTH 768
#define IMAGE_HEIGHT 768
#define VIEW_ANGLE 80
Point position = Point(150, 150, 50);
Point rightDir = Point(-1/sqrt(2.0), 1/sqrt(2.0), 0);
Point upDir = Point(0,0,1);
Point lookDir = upDir.crossMultiplication(rightDir);


double pixels;
int recursionLevel, objectCount, lightCount;

void updateImage(vector<vector<Color>> &plane) {
    std::cout << "Now i will generate image" << std::endl;
    bitmap_image bitmapImage(IMAGE_WIDTH, IMAGE_HEIGHT);
    for (int i = 0; i < IMAGE_HEIGHT; ++i) {
        for (int j = 0; j < IMAGE_WIDTH; ++j) {
            bitmapImage.set_pixel(j, i, plane[i][j].red, plane[i][j].green, plane[i][j].blue);
        }
    }
    bitmapImage.save_image("1605106.bmp");
}
Color calculateColor(Ray &ray) {
    //std::cout << "Now i will calculate color" << std::endl;
    Color color;
    int minIndex = -99999;
    double minT = 99999;

    for (int i = 0; i < allObjects.size(); ++i) {
        Ray temp = allObjects[i]->intersect(ray, 0);
        if (temp.t < minT && temp.t > 0) {
            minT = temp.t;
            minIndex = i;
        }
    }
    if (minIndex > 0) {
        Ray final = allObjects[minIndex]->intersect(ray, recursionLevel);
        color = final.color;
    }
    return color;
}
void capture() {
    double planeDistance = (WINDOW_HEIGHT/2.0)/tan((pi/180.0)*(VIEW_ANGLE/2.0));
    Point topLeft_l = lookDir * planeDistance;
    Point topLeft_r = rightDir * (WINDOW_WIDTH/2.0);
    Point topLeft_u = upDir * (WINDOW_HEIGHT/2.0);
    Point topLeft = position + topLeft_l - topLeft_r + topLeft_u;

    double du = (double) WINDOW_WIDTH/IMAGE_WIDTH;
    double dv = (double) WINDOW_HEIGHT/IMAGE_HEIGHT;

    ///DO we need this???
//    Point mid_r = rightDir * (0.5 * du);
//    Point mid_u = upDir * (0.5 * dv);
//    topLeft = topLeft + mid_r - mid_u;

    Point currentPixel, rayDirection, rayStart;
    vector<vector<Color>> nearPlaneColors;
    for (int i = 0; i < IMAGE_HEIGHT; ++i) {
        nearPlaneColors.emplace_back();
        for (int j = 0; j < IMAGE_WIDTH; ++j) {
            Point cp_r =  rightDir * (j * du);
            Point cp_u = upDir * (i * dv);
            currentPixel = topLeft + cp_r - cp_u;

            rayStart = position;
            rayDirection = currentPixel - position;
            rayDirection.normalizePoint();

            Ray eyeToDir(rayStart, rayDirection);
            Color c = calculateColor(eyeToDir);
            //std::cout << c.red << ", " << c.green << ", " << c.blue << std::endl;
            nearPlaneColors[i].push_back(calculateColor(eyeToDir));
        }
    }

    updateImage(nearPlaneColors);


}
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
        case '0':
            capture();
            std::cout << "Image Captured" << std::endl;
            break;
        default:
            break;
    }
}
void specialKeyListener(int key, int x,int y){
    switch(key){
        case GLUT_KEY_DOWN:
            position = position - lookDir;
            //std::cout << "Position: " << position.x << " , " << position.y << " , " << position.z << std::endl;
            break;
        case GLUT_KEY_UP:
            position = position + lookDir;
            //std::cout << "Position: " << position.x << " , " << position.y << " , " << position.z << std::endl;
            break;
        case GLUT_KEY_RIGHT:
            position = position + rightDir;
            //std::cout << "Position: " << position.x << " , " << position.y << " , " << position.z << std::endl;
            break;
        case GLUT_KEY_LEFT:
            position = position - rightDir;
            //std::cout << "Position: " << position.x << " , " << position.y << " , " << position.z << std::endl;
            break;
        case GLUT_KEY_PAGE_UP:
            position = position + upDir;
            //std::cout << "Position: " << position.x << " , " << position.y << " , " << position.z << std::endl;
            break;
        case GLUT_KEY_PAGE_DOWN:
            position = position - upDir;
            //std::cout << "Position: " << position.x << " , " << position.y << " , " << position.z << std::endl;
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
    for (int i = 0; i < allObjects.size(); ++i) {
        allObjects[i]->draw();
    }
    for (int i = 0; i < lightCount; ++i) {
        allLights[i].draw(7, 7);
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
    Object *object;
    for (int i = 0; i < objectCount; ++i) {
        in >> type;
        Point center;
        double radius;
        double ambient, diffuse, specular, reflection;
        int shine;
        Color color;
        vector<double> coefficients;
        if (type == "sphere") {
            in >> center.x >> center.y >> center.z;
            in >> radius;
            in >> color.red >> color.green >> color.blue;
            in >> ambient >> diffuse >> specular >> reflection;
            coefficients.push_back(ambient);
            coefficients.push_back(diffuse);
            coefficients.push_back(specular);
            coefficients.push_back(reflection);
            in >> shine;
            object = new Sphere(center, radius);
            object->setColor(color);
            object->setCoefficient(coefficients);
            object->setShine(shine);
            allObjects.push_back(object);
        }
    }
    in >> lightCount;
    for (int i = 0; i < lightCount; ++i) {
        Point lightPosition;
        Color lightColor;
        in >> lightPosition.x >> lightPosition.y >> lightPosition.z;
        in >> lightColor.red >> lightColor.green >> lightColor.blue;
        Light light(lightPosition, lightColor);
        allLights.push_back(light);
    }
    in.close();
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