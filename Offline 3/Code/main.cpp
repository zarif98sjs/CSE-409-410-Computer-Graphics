#include<bits/stdc++.h>
#include <GL/glut.h>
using namespace std;
#include "classes.h"
#include <windows.h>
#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;


vector <Object*> objects;
vector <Light*> lights;


// position of camera
PT pos(200,0,10);

// up, right and look direction
PT up(0,0,1);
PT rightV(-1 / sqrt(2), 1 / sqrt(2), 0);
PT look(-1 / sqrt(2), -1 / sqrt(2), 0);

double ROT_ANG = pi/180;
int numSegments;

// draw axes
void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);
		{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}
		glEnd();
	}
}

// draws grid
void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);
		{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}
		glEnd();
	}
}

void rotate3D(PT &vec,PT &axis,double ang)
{
	// vec = vec*cos(ang)+(vec*axis)*sin(ang);
	vec = vec*cos(ang)+(axis^vec)*sin(ang);
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			// rotate LEFT 
			rotate3D(rightV,up,ROT_ANG);
			rotate3D(look,up,ROT_ANG);
			break;
		case '2':
			// rotate right
			rotate3D(rightV,up,-ROT_ANG);
			rotate3D(look,up,-ROT_ANG);
			break;
		case '3':
			// rotate UP
			rotate3D(up,rightV,ROT_ANG);
			rotate3D(look,rightV,ROT_ANG);
			break;
		case '4':
			// rotate DOWN
			rotate3D(up,rightV,-ROT_ANG);
			rotate3D(look,rightV,-ROT_ANG);
			break;
		case '5':
			// tilt CLKWISE
			rotate3D(rightV,look,ROT_ANG);
			rotate3D(up,look,ROT_ANG);
			break;
		case '6':
			// tilt COUNTER CLKWISE
			rotate3D(rightV,look,-ROT_ANG);
			rotate3D(up,look,-ROT_ANG);
			break;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			pos = pos - look * 3;
			break;
		case GLUT_KEY_UP:		// up arrow key
			pos = pos + look * 3;
			break;
		case GLUT_KEY_RIGHT:
			pos = pos + rightV * 3;
			break;
		case GLUT_KEY_LEFT:
			pos = pos - rightV * 3;
			break;
		case GLUT_KEY_PAGE_UP:
			pos = pos + up * 3;
			break;
		case GLUT_KEY_PAGE_DOWN:
			pos = pos - up * 3;
			break;
		case GLUT_KEY_INSERT:
			break;
		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;
		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
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



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
	//  gluLookAt(0,0,200,	0,0,0,	0,1,0);
	//gluLookAt(0,100,0,	0,0,0,	0,0,1);

	gluLookAt(pos.x, pos.y, pos.z, 
			pos.x + look.x, pos.y + look.y, pos.z + look.z, 
			up.x, up.y, up.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

    for (int i=0; i<objects.size(); i++){
		Object *object = objects[i];
		object->draw();
	}

	for (int i=0; i<lights.size(); i++){
		lights[i]->draw();
	}

	glutSwapBuffers();
}


void animate(){
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}
int recursionLevel,imageHeight;
void loadData()
{
	ifstream in("scene.txt");
	in >> recursionLevel >> imageHeight;

	int objCount;
	in >> objCount;

	for(int i=0;i<objCount;i++)
	{
		string objType;
		in >> objType;

		Object *obj;

		if(objType == "sphere"){
			obj = new Sphere();
			in >> *((Sphere *)obj);
		}
		else if(objType == "triangle"){
			obj = new Triangle();
			in >> *((Triangle *)obj);
		}
		else if(objType == "general"){
			// obj = new Plane();
		}
		else{
			cout<<objType<<" is not a valid object type"<<endl;
		}
		objects.push_back(obj);
	}

	int lightCount;
	in >> lightCount;

	for(int i=0;i<lightCount;i++){
		Light *light = new Light();
		in >> *light;
		lights.push_back(light);
	}

	Object *floor;
	floor = new Floor(400, 10);
	objects.push_back(floor);
}

void init(){
	//codes for initialization
	drawgrid=1;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;
	numSegments = 36;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){

	loadData();
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Ray Tracing");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
