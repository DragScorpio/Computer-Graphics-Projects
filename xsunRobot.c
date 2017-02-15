//
//  main.c
//  RoboticArm Project
//
//  Created by Xiangzhen Sun on 1/30/17.
//  Copyright © 2017 Xiangzhen Sun. All rights reserved.
//

/*
	keyboard controls:
	s/S:	Positive/negative shoulder rotation
	e/E:	Positive/negative elbow rotation
	O/C:	Open/Close the hand -- YOU GET TO IMPLEMENT THIS
	
	x/X:	Positive/negative X-axis shift of the model
	y/Y:	Positive/negative Y-axis shift of the model
	UP/DOWN ARROWS:	(zoom) Z-axis shift of the model
	
	LEFT/RIGHT ARROWS:	Y-axis rotations
	PAGE UP/DOWN:	X-axis rotations
	ESC:	exit
 */
#include<stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif
// ensure the program work in both macOS and Windows
#define ESC 27

static GLfloat red[]={1,0,0},green[]={0,1,0},blue[]={0,0,1};  // color constants
static int shoulder=0,elbow=0;  // shoulder, and elbow rotation: default rotations are both 0
static int upperFinger=0,lowerFinger=0; // fingers are closed by default
static int xrot=0,yrot=0,zrot=0;  // the rotation of the whole frame in Cartesian coordinate
static GLfloat xshift=-1.0,yshift=0.0,zoom=-3.0;  // the shift of the whole frame in Cartesian coordinate

/* some basic GL initialization */
void init()
{
    glClearColor(0.0,0.0,0.0,0.0);
    // specify clear values for the color buffers
    glShadeModel(GL_FLAT);
    // select flat or smooth shading
    glEnable(GL_DEPTH_TEST);
    // enable server-side GL capabilities
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    // back-facing facets are culled
}

/* The display callback */
void draw_stuff()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    // clear buffers to preset values
    // set bitplane area of the window as indicated in init()
    
    /*------------------------ Loading Identity Matrix -----------------------*/
    glLoadIdentity();
    // replace the current matrix, if any, with the identity matrix
    
    
    /*----------------- Defining Origin of Modeling Coordinate ---------------*/
    glPushMatrix();
    // push the current matrix down by one, duplicating it at the same time
    // :the matrix on top of the stack is identical to the matrix below it
    // the stack of matrix keeps a record of matrix transformations in each step
    glTranslatef(xshift,yshift,zoom);
    // post multiplies the current matrix with a matrix that moves the
    // current object to (xshift, yshift, zoom); float type entries only
    glRotatef((GLfloat)xrot,1.0,0.0,0.0);
    glRotatef((GLfloat)yrot,0.0,1.0,0.0);
    glRotatef((GLfloat)zrot,0.0,0.0,1.0);
    // specify (GLfloat) rotation angle
    // specify a space vector (x, y, z)
    // rotate the current object CCW around the space vector
    
    
    /*---------------------------- Drawing Shoulder --------------------------*/
    glColor3fv(red);
    // set current color
    // using 3-dimension vector of float type: "red"
    
    // the following transformations define the center of shoulder block
    glPushMatrix();
    glTranslatef(-1.0,0.0,0.0);
    glRotatef((GLfloat)shoulder,0.0,0.0,1.0);
    glTranslatef(1.0,0.0,0.0);
    
    // the following transformations define the shape of shoulder block
    glPushMatrix();
    glScalef(2.0,0.4,1.0);
    // specifies the scale factors along x, y, z axis respectively
    glutSolidCube(1.0);
    // renders a solid cube that is centered at the origin of modeling coordinate
    // the length of sides is 1.0
    
    glPopMatrix();
    // pop out the shoulder matrix, which leads us back to the geometry center of the shoulder
    
    
    /*---------------------------- Drawing Elbow --------------------------*/
    glColor3fv(green);
    
    // the following transformations define the center of elbow block
    glTranslatef(1.0,0.0,0.0);
    glRotatef((GLfloat)elbow,0.0,0.0,1.0);
    glTranslatef(1.0,0.0,0.0);
    
    // the following transformations define the shape of elbow block
    glPushMatrix();
    glScalef(2.0,0.4,1.0);
    glutSolidCube(1.0);
    
    glPopMatrix(); // return to the geometry center of elbow
    
    
    /*---------------------------- Drawing Fingers --------------------------*/
    glColor3fv(blue);
    glPushMatrix(); // make a copy of the elbow center
    
    // the following transformations define the rotating center of upper finger
    glTranslatef(1.0,0.1,0.0);
    glRotatef((GLfloat)upperFinger,0.0,0.0,1.0);
    glTranslatef(0.6,-0.05,0.0);
    
    // the following transformations define the shape of upper finger
    glScalef(1.2,0.1,0.2);
    glutSolidCube(1.0);
    
    glPopMatrix(); // return to the geometry center of elbow
    
    // the following transformations define the rotating center of lower finger
    glTranslatef(1.0,-0.1,0.0);
    glRotatef((GLfloat)lowerFinger,0.0,0.0,1.0);
    glTranslatef(0.6,0.05,0.0);
    
    // the following transformations define the shape of lower finger
    glScalef(1.2,0.1,0.2);
    glutSolidCube(1.0);

    // clean up the matrix stack
    glPopMatrix();
    glPopMatrix();
    glutSwapBuffers();
}

/* the window-reshape callback */
void reshape(int w, int h)
{
    glViewport(0,0,(GLsizei)w,(GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0,(GLfloat)w/(GLfloat)h,1.0,20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0,0.0,-5.0);
}

/* The basic keyboard controls, callback for glutKeyboardFunc() */
void kb(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 's':
            shoulder=(shoulder+5)%360;
            glutPostRedisplay();
            break;
        case 'S':
            shoulder=(shoulder-5)%360;
            glutPostRedisplay();
            break;
        case 'e':
            elbow=(elbow+5)%360;
            glutPostRedisplay();
            break;
        case 'E':
            elbow=(elbow-5)%360;
            glutPostRedisplay();
            break;
        case 'C':
            /* close the "fingers" */
            // anti-interference: if totally closed, no more locomotion
            if(upperFinger-lowerFinger>1.0) {
                upperFinger=(upperFinger-5)%360;
                lowerFinger=(lowerFinger+5)%360;
            }
            glutPostRedisplay();
            break;
        case 'O':
            /* open the "fingers" */
            // anti-interference: if opened straight up, no more locomotion
            if(upperFinger-lowerFinger<179) {
                upperFinger=(upperFinger+5)%360;
                lowerFinger=(lowerFinger-5)%360;
            }
            glutPostRedisplay();
            break;
        case 'x':
            xshift+=.25;
            glutPostRedisplay();
            break;
        case 'X':
            xshift-=.25;
            glutPostRedisplay();
            break;
        case 'y':
            yshift+=.25;
            glutPostRedisplay();
            break;
        case 'Y':
            yshift-=.25;
            glutPostRedisplay();
            break;
        case 'r':
        case 'R':
            xrot=yrot=zrot=shoulder=elbow=upperFinger=lowerFinger=0;
            xshift=-1.0;
            yshift=0.0;
            zoom=-3.0;
            glutPostRedisplay();
            break;
        case 'z':
            zrot=(zrot-1)%360;
            glutPostRedisplay();
            break;
        case 'Z':
            zrot=(zrot+1)%360;
            glutPostRedisplay();
            break;
        case ESC:
            return;
            break;
        default:
            break;
    }
}

/* The "special" keyboard controls, callback for glutSpecialFunc() */
void skb(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_UP:
            zoom+=1;
            glutPostRedisplay();
            break;
        case GLUT_KEY_DOWN:
            zoom-=1;
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT:
            yrot=(yrot-1)%360;
            glutPostRedisplay();
            break;
        case GLUT_KEY_LEFT:
            yrot=(yrot+1)%360;
            glutPostRedisplay();
            break;
        case GLUT_KEY_PAGE_UP:
            xrot=(xrot-1)%360;
            glutPostRedisplay();
            break;
        case GLUT_KEY_PAGE_DOWN:
            xrot=(xrot+1)%360;
            glutPostRedisplay();
            break;
        default:
            break;
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(700,300);
    glutInitWindowPosition(100,50);
    glutCreateWindow("foo");
    init();
    glutDisplayFunc(draw_stuff);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(kb);
    glutSpecialFunc(skb);
    glutMainLoop();
    return 0;
}
