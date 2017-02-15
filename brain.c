//
//  main.c
//  brain structure
//
//  Created by Xiangzhen Sun on 2/11/17.
//  Copyright © 2017 Xiangzhen Sun. All rights reserved.
//
/* Keyboard Controls:
   	x/X:	Positive/negative X-axis shift of the model
	y/Y:	Positive/negative Y-axis shift of the model
    z/Z:    Positive/negative Z-axis shift of the model
	UP/DOWN ARROWS:	(zoom) Z-axis shift of the model
	
	LEFT/RIGHT ARROWS:	Y-axis rotations
	PAGE UP/DOWN:	X-axis rotations
	ESC:	exit
 
 */

#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif
#define ESC 27

GLfloat light_diffuse[] = {1.0, 0.0, 0.0, 1.0};  // red diffuse light
GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0}; // infinite light position


char title[] = "Brain Structure";
// title of command window

struct vertex {
    // define a structure to store xyz-components of each vertex
    float x;
    float y;
    float z;
};

struct triangle {
    // define a structure to store the reference vortices of a triangle
    int i; // first vertex
    int j; // second vertex
    int k; // third vertex
};

GLfloat normalVect[382241][3]; // n[][] contains the normal of each triangle facets

struct vertex v[191724];  // declare a vertex struct "v"
struct triangle t[382241];  // declare a triangle struct "t"
static GLfloat green[] = {0.75f, 1.0f, 0.0f};
static int xrot = 0, yrot = 0, zrot = 0;
// rotation vars

static GLfloat xshift = 0.0, yshift = 0.0, zoom = 0.0;
// shifting vars

void CalculateNormalVector(GLfloat fVert1[], GLfloat fVert2[], GLfloat fVert3[], GLfloat normalVect[]) {
    // this function helps calculate the normal vector of a triangle
    GLfloat Qx, Qy, Qz, Px, Py, Pz;
    Qx = fVert2[0]-fVert1[0];
    Qy = fVert2[1]-fVert1[1];
    Qz = fVert2[2]-fVert1[2];
    Px = fVert3[0]-fVert1[0];
    Py = fVert3[1]-fVert1[1];
    Pz = fVert3[2]-fVert1[2];
    normalVect[0] = Py*Qz - Pz*Qy;
    normalVect[1] = Pz*Qx - Px*Qz;
    normalVect[2] = Px*Qy - Py*Qx;
}
// read in vortices from file
void readFile() {
    FILE * f = fopen("Cort_lobe_poly.vtk", "r");
    if(f == NULL) {
        printf("The file cannot be openned!");
        exit(0);
    }

    // skip line 1 - line 5
    char foo[100];
    int index;
    for(int i = 0; i < 5; i++) {
        fgets(foo, 100, f);
    }
    // read vertex data in v
    for(int i = 0; i < 191724; i++) {
        fscanf(f, "%f %f %f\n", &(v[i].x), &(v[i].y), &(v[i].z));
    }
    
    /*
    // test read file
    for(int i = 0; i < 191724; i++) {
        printf("%lf %lf %lf\n", v[i].x, v[i].y, v[i].z);
     }
     */

    fgets(foo, 100, f);
    for(int i = 0; i < 382241; i++) {
        // read triangle referencing points
        fscanf(f, "%i %i %i %i\n", &index, &(t[i].i), &(t[i].j), &(t[i].k));
    }
}

/* Initialize OpenGL Graphics */
void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClearDepth(1.0f);                   // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
    glShadeModel(GL_SMOOTH);   // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
    
    /* Enable a single OpenGL light. */
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
}

/* Handler for window-repaint event. Called back when the window first appears and
 whenever the window needs to be re-painted. */
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
    glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
    
    glLoadIdentity();                 // Reset the model-view matrix
    glTranslatef(-130.0f, -100.0f, -145.0f);
    glPushMatrix();
    
    glTranslatef(xshift,yshift,zoom);
    glRotatef((GLfloat)xrot, 1.0, 0.0, 0.0);
    glRotatef((GLfloat)yrot, 0.0, 1.0, 0.0);
    glRotatef((GLfloat)zrot, 0.0, 0.0, 1.0);
    glPushMatrix();

    // glColor3fv(green); // this command was for uniform color test
    glBegin(GL_TRIANGLES);
    for(int m = 0; m < 382241; m++) {
        GLfloat fVert1[3], fVert2[3], fVert3[3];
        fVert1[0] = v[t[m].i].x;
        fVert1[1] = v[t[m].i].y;
        fVert1[2] = v[t[m].i].z;
        fVert2[0] = v[t[m].j].x;
        fVert2[1] = v[t[m].j].y;
        fVert2[2] = v[t[m].j].z;
        fVert3[0] = v[t[m].k].x;
        fVert3[1] = v[t[m].k].y;
        fVert3[2] = v[t[m].k].z;
        
        // calculate the normal vector of each triangle facets
        CalculateNormalVector(fVert1, fVert2, fVert3, normalVect[m]);
        
        // specify the calculated normal vector:
        glNormal3fv(normalVect[m]);
        
        // specify the 3 vortices of a triangle
        glVertex3f(v[t[m].i].x, v[t[m].i].y, v[t[m].i].z);
        glVertex3f(v[t[m].j].x, v[t[m].j].y, v[t[m].j].z);
        glVertex3f(v[t[m].k].x, v[t[m].k].y, v[t[m].k].z);
    }
    glEnd();   // Done drawing the pyramid
    
    glPopMatrix();  // clean up matrix stack
    glPopMatrix();
    glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}

/* Handler for window re-size event. Called back when the window first appears and
 whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    
    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);
    
    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(70.0f, aspect, 60.0f, 100.0f);
    glTranslatef(0.0, -5.0, 25.0);
}

/* keyboard control functions */
void kb(unsigned char key, int x, int y) {
    switch(key) {
        case 'x':
            xshift -= 0.5;
            glutPostRedisplay();
            break;
        case 'X':
            xshift += 0.5;
            glutPostRedisplay();
            break;
        case 'y':
            yshift -= 0.5;
            glutPostRedisplay();
            break;
        case 'Y':
            yshift += 0.5;
            glutPostRedisplay();
            break;
        case 'r':
        case 'R':
            xrot = yrot = zrot = 0;
            xshift = yshift = zoom = 0.0;
            glutPostRedisplay();
            break;
        case 'z':
            zrot = (zrot - 1)%360;
            glutPostRedisplay();
            break;
        case 'Z':
            zrot = (zrot + 1)%360;
            glutPostRedisplay();
            break;
        case ESC:
            return;
            break;
        default:
            break;
            
    }
}

void skb(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_UP:
            zoom += 1;
            glutPostRedisplay();
            break;
        case GLUT_KEY_DOWN:
            zoom -= 1;
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT:
            yrot = (yrot - 1)%360;
            glutPostRedisplay();
            break;
        case GLUT_KEY_LEFT:
            yrot = (yrot + 1)%360;
            glutPostRedisplay();
            break;
        case GLUT_KEY_PAGE_UP:
            xrot = (xrot - 1)%360;
            glutPostRedisplay();
            break;
        case GLUT_KEY_PAGE_DOWN:
            xrot = (xrot + 1)%360;
            glutPostRedisplay();
            break;
        default:
            break;
    }
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
    readFile();
    glutInit(&argc, argv);            // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
    glutInitWindowSize(800, 600);   // Set the window's initial width & height
    glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
    glutCreateWindow(title);        // Create window with the given title
    initGL();                       // Our own OpenGL initialization
    glutDisplayFunc(display);       // Register callback handler for window re-paint event
    glutReshapeFunc(reshape);       // Register callback handler for window re-size event
    glutKeyboardFunc(kb);
    glutSpecialFunc(skb);
    glutMainLoop();                 // Enter the infinite event-processing loop
    return 0;
}
