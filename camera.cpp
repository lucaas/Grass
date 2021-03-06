
#include "camera.h"
#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif
#include <math.h>

Camera::Camera()
{
    xpos = 0;
    ypos = 3;
    zpos = 15;
    xrot = 20;
    yrot = 0;
}

void Camera::move (void) {
    glRotatef(xrot,1.0,0.0,0.0);  //rotate our camera on the x-axis (left and right)
    glRotatef(yrot,0.0,1.0,0.0);  //rotate our camera on the y-axis (up and down)
    glTranslated(-xpos,-ypos,-zpos); //translate the screen to the position of our camera
}


void Camera::key (unsigned char key, int x, int y) {
    if (key=='q')
    {
        xrot += 1;
        if (xrot >360) xrot -= 360;
    }

    if (key=='z')
    {
        xrot -= 1;
        if (xrot < -360) xrot += 360;
    }



    if (key=='w')
    {
        float xrotrad, yrotrad;
        yrotrad = (yrot / 180 * 3.141592654f);
        xrotrad = (xrot / 180 * 3.141592654f);
        xpos += float(sin(yrotrad)) ;
        zpos -= float(cos(yrotrad)) ;
        ypos -= float(sin(xrotrad)) ;
    }

    if (key=='s')
    {
        float xrotrad, yrotrad;
        yrotrad = (yrot / 180 * 3.141592654f);
        xrotrad = (xrot / 180 * 3.141592654f);
        xpos -= float(sin(yrotrad));
        zpos += float(cos(yrotrad)) ;
        ypos += float(sin(xrotrad));
    }

    if (key=='d')
    {
        float yrotrad;
        yrotrad = (yrot / 180 * 3.141592654f);
        xpos += float(cos(yrotrad)) * 0.2;
        zpos += float(sin(yrotrad)) * 0.2;
    }

    if (key=='a')
    {
        float yrotrad;
        yrotrad = (yrot / 180 * 3.141592654f);
        xpos -= float(cos(yrotrad)) * 0.2;
        zpos -= float(sin(yrotrad)) * 0.2;
    }

    glutPostRedisplay();
}

void Camera::mouseClick(int button, int state, int x, int y) {
    if(state == GLUT_DOWN)
    {
        lastx=x;
        lasty=y;
    }
}

Vector3f Camera::getPosition()
{
    return Vector3f(xpos, ypos, zpos);
}

void Camera::mouseMovement(int x, int y) {
    int diffx=x-lastx; //check the difference between the current x and the last x position
    int diffy=y-lasty; //check the difference between the current y and the last y position
    lastx=x; //set lastx to the current x position
    lasty=y; //set lasty to the current y position
    xrot += (float) diffy*0.15; //set the xrot to xrot with the addition of the difference in the y position
    yrot += (float) diffx*0.15;    //set the xrot to yrot with the addition of the difference in the x position
}
