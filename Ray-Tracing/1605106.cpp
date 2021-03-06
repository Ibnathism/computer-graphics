#include <GL/glut.h>
#include<bits/stdc++.h>
#include "1605106_classes.h"
#include "bitmap_image.h"
#define WINDOW_HEIGHT 500
#define WINDOW_WIDTH 500
#define VIEW_ANGLE 80

Point position = Point(150, 150, 50);
Point rightDir = Point(-1/sqrt(2.0), 1/sqrt(2.0), 0);
Point upDir = Point(0,0,1);
Point lookDir = upDir.crossMultiplication(rightDir);


//Point position, rightDir, lookDir, upDir;

double pixels;
int recursionLevel, objectCount, lightCount;

Point rotateOneAlongAnother(Point &toBeRotated, Point &respective, double angleOfRotation) {
    Point temp = respective.crossMultiplication(toBeRotated);
    double angleInRadian = angleOfRotation*pi / 180.0;
    Point component1 = toBeRotated.constantScale(cos(angleInRadian));
    Point component2 = temp.constantScale(sin(angleInRadian));
    Point answer = component1 + component2;
    return answer;
}

void clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,0);	//color black

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void capture() {
    double planeDistance = (WINDOW_HEIGHT/2.0)/tan((pi/180.0)*(VIEW_ANGLE/2.0));
    Point topLeft_l = lookDir * planeDistance;
    Point topLeft_r = rightDir * (WINDOW_WIDTH/2.0);
    Point topLeft_u = upDir * (WINDOW_HEIGHT/2.0);
    Point topLeft = position + topLeft_l - topLeft_r + topLeft_u;

    double du = (double) WINDOW_WIDTH/pixels;
    double dv = (double) WINDOW_HEIGHT/pixels;

    Point currentPixel, rayDirection, rayStart;
    vector<vector<Color>> nearPlaneColors;
    for (int i = 0; i < pixels; ++i) {
        nearPlaneColors.emplace_back();
        for (int j = 0; j < pixels; ++j) {
            Point cp_r =  rightDir * (j * du);
            Point cp_u = upDir * (i * dv);
            currentPixel = topLeft + cp_r - cp_u;

            rayStart = position;
            rayDirection = currentPixel - position;
            rayDirection = rayDirection.normalizePoint();

            Ray eyeToDir(rayStart, rayDirection);
            //Color temp = calculateColor(eyeToDir);
            //std::cout << "Now i will calculate color" << std::endl;
            Color color;
            int minIndex = MINUS_INFINITY;
            double minT = INF;
            int count = -1;
            for (auto object: allObjects) {
                count++;
                Ray temp = object->intersect(eyeToDir, 0);
                if (temp.t < minT && temp.t > 0) {
                    minT = temp.t;
                    minIndex = count;
                }

            }
            if (minIndex != MINUS_INFINITY) {
                Ray final = allObjects[minIndex]->intersect(eyeToDir, recursionLevel);

                color = final.color;

            }
            //cout << "MY COLOR " << color.red << ", " << color.green << ", " << color.blue << endl;
            color.clip();

            //if (temp.red > 0 || temp.green > 0 || temp.blue > 0) temp.print();
            nearPlaneColors[i].push_back(color);

        }
    }

    bitmap_image bitmapImage(pixels, pixels);
    for (int i = 0; i < pixels; ++i) {
        for (int j = 0; j < pixels; ++j) {
            //if (plane[i][j].red > 0 || plane[i][j].green > 0 || plane[i][j].blue > 0) plane[i][j].print();
            bitmapImage.set_pixel(j, i, nearPlaneColors[i][j].red*255, nearPlaneColors[i][j].green*255, nearPlaneColors[i][j].blue*255);
        }
    }
    bitmapImage.save_image("1605106.bmp");
    bitmapImage.clear();
    for (int i = 0; i < nearPlaneColors.size(); ++i) {
        nearPlaneColors[i].clear();
    }
    nearPlaneColors.clear();
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
            upDir = rightDir.crossMultiplication(lookDir);
            //std::cout << "Position: " << position.x << " , " << position.y << " , " << position.z << std::endl;
            break;
        case '6':
            rightDir = rotateOneAlongAnother(rightDir, lookDir, positiveAngle);
            upDir = rightDir.crossMultiplication(lookDir);
            //std::cout << "Position: " << position.x << " , " << position.y << " , " << position.z << std::endl;
            break;
        case '7':
            position = position + upDir;
            //std::cout << "Position: " << position.x << " , " << position.y << " , " << position.z << std::endl;
            break;
        case '8':
            position = position - upDir;
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
    //baseFloor.draw();
    for (auto obj: allObjects) {
        obj->draw();
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
        if (type == "sphere") {
            Point center;
            double radius;
            double ambient, diffuse, specular, reflection;
            int shine;
            Color color;
            vector<double> coefficients;
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
    vector<double> floorCoeffs;
    floorCoeffs.push_back(0.4);
    floorCoeffs.push_back(0.1);
    floorCoeffs.push_back(0.2);
    floorCoeffs.push_back(0.4);
    object = new Floor();
    object->setCoefficient(floorCoeffs);
    object->setShine(1);

    allObjects.push_back(object);


    in >> lightCount;
    for (int i = 0; i < lightCount; ++i) {
        Point lightPosition;
        Color lightColor;
        in >> lightPosition.x >> lightPosition.y >> lightPosition.z;
        in >> lightColor.red >> lightColor.green >> lightColor.blue;
        Light light(lightPosition, lightColor);
        allLights.push_back(light);
    }
//    for (int i = 0; i < lightCount; ++i) {
//        allLights[i].color.print();
//        allLights[i].position.print();
//    }
    //std::cout << allLights.size() <<std::endl;
    in.close();
//    Point p(125, 223, 65.7);
//    double x = p.dotMultiplication(p);
//    std::cout << x << std::endl;
//    Color c1(100, 100, 50);
//    Color c2 = c1 * 5 * 2.0;
//    c2 = c2 + c1;
//    std::cout << c2.red << ", " << c2.green << ", " << c2.blue << std::endl;
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