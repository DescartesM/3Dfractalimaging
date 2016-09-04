#include <windows.h>
#include "GL\glut.h"

GLfloat colors[7][3]={{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0},{0.5,0.1,0.1},{0.2,0.5,0.7},{0.2,0.9,0.1},{0.4,0.6,0.1}};  
float x1 = -1.5;
float y1 = 1.5;
float x2 = 1.5; 
float y2 = -1.5;

void rectangle( GLfloat *a, GLfloat *b, GLfloat *c,GLfloat *d)
{
	glVertex2fv(a);
	glVertex2fv(b);
	glVertex2fv(c);
	glVertex2fv(d);
}

void trans(float x1, float x2, float y1, float y2)
{
	GLfloat v0[2], v1[2], v2[2],v3[2];
	float L = x2 - x1;
	float W = y2 - y1;
	v0[0] = x1 + L/3;
	v0[1] = y1 + W/3;

	v1[0] = x1 + L/3;
	v1[1] = y2 - W/3;

	v2[0] = x2 - L/3;
	v2[1] = y2 - W/3;

	v3[0] = x2 - L/3;
	v3[1] = y1 + W/3;

	rectangle(v0, v1, v2, v3);
}
 
void divide_rectangle(float x1, float x2, float y1, float y2, int m)
{
 
/* triangle subdivision using vertex numbers */
	float L = x2 - x1;
	float W = y2 - y1;
	int i;

	trans(x1, x2, y1, y2);
    //int j;
if(m>0) 
    {
	glColor3fv(colors[7]);  
 	divide_rectangle(x1, x1 + L/3,y1, y1 + W/3, m-1);
 	glColor3fv(colors[0]);  
 	divide_rectangle(x1 + L/3, x2 - L/3, y1, y1 + W/3, m-1);
 	glColor3fv(colors[1]);  
 	divide_rectangle(x2 - L/3, x2, y1, y1 + W/3, m-1);
	glColor3fv(colors[2]); 

	divide_rectangle(x1, x1 + L/3, y1 + W/3, y2 - W/3, m-1);
	glColor3fv(colors[3]);
	divide_rectangle(x2 - L/3, x2, y1 + W/3, y2 - W/3, m-1);
	glColor3fv(colors[4]);

	divide_rectangle(x1, x1 + L/3, y2 - W/3, y2, m-1);
	glColor3fv(colors[5]);
	divide_rectangle(x1 + L/3, x2 - L/3, y2 - W/3, y2, m-1);
	glColor3fv(colors[6]);
 	divide_rectangle(x2 - L/3, x2, y2 - W/3, y2, m-1);
}
    else
       trans(x1, x2, y1, y2); /* draw triangle at end of recursion */
 
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);
	divide_rectangle(x1, x2, y1, y2, 5);
	glEnd();
	glFlush();
}

void myinit()
{
 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-2.0, 2.0, -2.0, 2.0);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(0.3,0.2,0.4);
 
}

 
void  main(int argc, char **argv)
{
 
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Rectangle");
    glutDisplayFunc(display);
	myinit();
    glutMainLoop();
 
}



