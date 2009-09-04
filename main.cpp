////////////////////////////////////////////////////
// Mark Sands
// September, 4 2009
// SAT - Simple Audio Toolkit
// OpenAL/OpenGL test using alut and glut to render
//	different polygons with sound.
////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <cmath>
#include "glut.h"
#include "SAT.h"

using namespace std;

typedef GLfloat point3D[3];
typedef GLfloat point2D[2];

GLfloat pi = 3.141592653;

enum SHAPES_TO_DRAW { CIRCLE = 1, EMPTY = 2, TRIANGLE = 3, SQUARE = 4, PENTAGON = 5, HEXAGON = 6, HEPTAGON = 7, OCTOGON = 8,
					  NONAGON, DECAGON, UNDECAGON, DODECAGON, TRIDECAGON, TETRADECAGON, PENTADECAGON = 15 };
int Shape_to_draw = CIRCLE;

// glut prototypes
void Myinit();
void Display();
void Menu(int id);
void ColorMenu(int id);
void ShapeMenu(int id);
void CreateMenus();

// shapes
void Shape(GLfloat sides, GLfloat radius);

int main( int argc, char* argv[])
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500,500);
	glutInitWindowPosition(0,0);
	glutCreateWindow("OpenGL Shapes");
	glutDisplayFunc(Display);
	Myinit();
	CreateMenus();
	glutMainLoop();

	return 0;
}

// ----------------------
// GLUT INIT
// --

void Myinit() {

	InitSound("test.wav");
	PlaySound();

	glClearColor(1.0,1.0,1.0,1.0);
	glColor3f(0.0,1.0,0.0);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 50.0, 0.0, 50.0);
	glMatrixMode(GL_MODELVIEW);
}

void Display() {
	
	point3D t1 = { 0.0,   0.0, 0.0 };
	point3D t2 = { 25.0, 50.0, 0.0 };
	point3D t3 = { 50.0,  0.0, 0.0 };

	point3D s1 = { 5.0,   5.0, 0.0 };
	point3D s2 = { 45.0, 5.0, 0.0 };
	point3D s3 = { 45.0,  45.0, 0.0 };
	point3D s4 = { 5.0,  45.0, 0.0 };
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	switch ( Shape_to_draw )
	{
		case CIRCLE:
			Shape(360.0, 25.0);
			break;
			
		case TRIANGLE:
			Shape(3, 25.0);
			break;
			
		case SQUARE:
			Shape(4, 25.0);
			break;

		case PENTAGON:
			Shape(5, 25.0);
			break;

		case HEXAGON:
			Shape(6, 25.0);
			break;

		case HEPTAGON:
			Shape(7, 25.0);
			break;

		case OCTOGON:
			Shape(8, 25.0);
			break;	
			
		case NONAGON:
			Shape(9, 25.0);
			break;

		case DECAGON:
			Shape(10, 25.0);
			break;

		case UNDECAGON:
			Shape(11, 25.0);
			break;

		case DODECAGON:
			Shape(12, 25.0);
			break;

		case TRIDECAGON:
			Shape(13, 25.0);
			break;

		case TETRADECAGON:
			Shape(14, 25.0);
			break;		

		case PENTADECAGON:
			Shape(15, 25.0);
			break;		
	}
}


// ----------------------
// MENU & SUBMENU
// --

void ShapeMenu(int id)
{
	glClear(GL_COLOR_BUFFER_BIT);
			
	switch(id)
	{
		case CIRCLE:
			Shape_to_draw = CIRCLE;
			break;
			
		case TRIANGLE:
			Shape_to_draw = TRIANGLE;
			break;
			
		case SQUARE:
			Shape_to_draw = SQUARE;
			break;
			
		case PENTAGON:
			Shape_to_draw = PENTAGON;			
			break;

		case HEXAGON:
			Shape_to_draw = HEXAGON;			
			break;

		case HEPTAGON:
			Shape_to_draw = HEPTAGON;			
			break;

		case OCTOGON:
			Shape_to_draw = OCTOGON;
			break;
			
		case NONAGON:
			Shape_to_draw = NONAGON;
			break;

		case DECAGON:
			Shape_to_draw = DECAGON;
			break;

		case UNDECAGON:
			Shape_to_draw = UNDECAGON;			
			break;

		case DODECAGON:
			Shape_to_draw = DODECAGON;			
			break;

		case TRIDECAGON:
			Shape_to_draw = TRIDECAGON;			
			break;

		case TETRADECAGON:
			Shape_to_draw = TETRADECAGON;
			break;

		case PENTADECAGON:
			Shape_to_draw = PENTADECAGON;
			break;
	}
	
	glutPostRedisplay();
}

void ColorMenu(int id)
{
	switch(id)
	{
		case 1: //RED
			glColor3f(1.0,0.0,0.0);
			break;
			
		case 2: //GREEN
			glColor3f(0.0,1.0,0.0);
			break;
			
		case 3: //BLUE
			glColor3f(0.0,0.0,1.0);
			break;
			
		case 4: //Cyan
			glColor3f(0.0,1.0,1.0);
			break;
			
		case 5: //Magenta
			glColor3f(1.0,0.0,1.0);
			break;
			
		case 6: //Yellow
			glColor3f(1.0,1.0,0.0);
			break;
	}
	
	glutPostRedisplay();
}

void Menu(int id)
{
	switch(id)
	{
		case 1: //EXIT
			DeleteSound();
			exit(1);
			break;
	}
}

void CreateMenus()
{
	GLint c_submenu, s_submenu;

	c_submenu = glutCreateMenu(ColorMenu);
		glutAddMenuEntry("Red",1);
		glutAddMenuEntry("Green",2);
		glutAddMenuEntry("Blue",3);
		glutAddMenuEntry("Cyan",4);
		glutAddMenuEntry("Magenta",5);
		glutAddMenuEntry("Yellow",6);		

	s_submenu = glutCreateMenu(ShapeMenu);	
		glutAddMenuEntry("Circle",1);
		glutAddMenuEntry("Triangle",3);
		glutAddMenuEntry("Square",4);
		glutAddMenuEntry("Pentagon",5);
		glutAddMenuEntry("Hexagon",6);
		glutAddMenuEntry("Heptagon",7);
		glutAddMenuEntry("Octogon",8);
		glutAddMenuEntry("Nonagon",9);
		glutAddMenuEntry("Decagon",10);
		glutAddMenuEntry("Undecagon",11);
		glutAddMenuEntry("Dodecagon",12);
		glutAddMenuEntry("Tridecagon",13);
		glutAddMenuEntry("Tetradecagon",14);
		glutAddMenuEntry("Pentadecagon",15);
		
	glutCreateMenu(Menu);
		glutAddSubMenu("Shape", s_submenu);
		glutAddSubMenu("Color", c_submenu);
		glutAddMenuEntry("Exit",1);	
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// ----------------------
// CUSTOM SHAPES
// --

// SHAPE
void Shape(GLfloat sides, GLfloat radius)
{	
	GLfloat x, y, z = 1.0;
	
	glBegin(GL_POLYGON);
	for(GLfloat i = 0.0; i < 360; i += 360/sides )
		{
			x = radius + radius*cos( i*pi/180.0 );
			y = radius + radius*sin( i*pi/180.0 );
			glVertex3f(x, y, z);
		}
	glEnd();
	glFlush();
}
