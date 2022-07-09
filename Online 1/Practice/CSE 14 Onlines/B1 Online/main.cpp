#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <math.h>
#include <GL/glut.h>
using namespace std;
#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angleTurn;
double angleFwd;

double wheelH = 5, wheelR = 15;

double angle = 0;

struct PT
{
	PT() {}
	double x,y,z;

	PT(double x, double y, double z) : x(x), y(y), z(z) {}
    PT(const PT &p) : x(p.x), y(p.y), z(p.z) {}

	// arithemtic operations
	PT operator +(PT b)  {return PT(x+b.x,y+b.y, z+b.z);}
    PT operator -(PT b)  {return PT(x-b.x,y-b.y, z-b.z);}
	PT operator *(double b)  {return PT(x*b,y*b, z*b);}
	PT operator /(double b)  {return PT(x/b,y/b, z/b);}
	PT operator *(PT b)  {return PT(y*b.z-z*b.y, z*b.x-x*b.z, x*b.y-y*b.x);}
};

// position of wheel
PT pos(0,0,0);

void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				// if(i==0)
				// 	continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,0);
		glVertex3f( a,-a,0);
		glVertex3f(-a,-a,0);
		glVertex3f(-a, a,0);
	}glEnd();
}

void drawRectangle(double w,double h)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( w, h,0);
		glVertex3f( w,-h,0);
		glVertex3f(-w,-h,0);
		glVertex3f(-w, h,0);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct PT points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }

	struct PT pointsLine[100];

	segments = 6;

	//generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }

	//draw segments using generated points
    for(i=0;i<segments/2;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+3].x,points[i+3].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct PT points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks)
{
	struct PT points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawCylinder(double radius,double height,int segments)
{
	struct PT points[100];
	int i;
	double shade;

	// glRotatef(angle,0,0,1);
	

	//generate points
	for(i=0;i<=segments;i++)
	{
		points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
		points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
	}
	//draw quads using generated points
	for(i=0;i<segments;i++)
	{
		//create shading effect
		if(i<segments/2)shade=2*(double)i/(double)segments;
		else shade=2*(1.0-(double)i/(double)segments);
		
		glColor3f(shade,shade,shade);

		glBegin(GL_QUADS);{
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i].x,points[i].y,height);
			glVertex3f(points[i+1].x,points[i+1].y,height);
			glVertex3f(points[i+1].x,points[i+1].y,0);
		}glEnd();
	}
}


void drawSS()
{
    glColor3f(1,0,0);
    drawSquare(20);

    glRotatef(angleTurn,0,0,1);
    glTranslatef(110,0,0);
    glRotatef(2*angleTurn,0,0,1);
    glColor3f(0,1,0);
    drawSquare(15);

    glPushMatrix();
    {
        glRotatef(angleTurn,0,0,1);
        glTranslatef(60,0,0);
        glRotatef(2*angleTurn,0,0,1);
        glColor3f(0,0,1);
        drawSquare(10);
    }
    glPopMatrix();

    glRotatef(3*angleTurn,0,0,1);
    glTranslatef(40,0,0);
    glRotatef(4*angleTurn,0,0,1);
    glColor3f(1,1,0);
    drawSquare(5);
}

double distanceTrav;

void moveForward()
{
	angleFwd += 2;
	distanceTrav = 2 * pi * wheelR * (double)(2)/(double)(360);
	pos.x -= cos(angleTurn*pi/(double)180) * distanceTrav;
	pos.y -= sin(angleTurn*pi/(double)180) * distanceTrav;
}

void moveBackward()
{
	angleFwd -= 2;
	distanceTrav = 2 * pi * wheelR * (double)(-2)/(double)(360);
	pos.x -= cos(angleTurn*pi/(double)180) * distanceTrav;
	pos.y -= sin(angleTurn*pi/(double)180) * distanceTrav;
}

double ROT_B = 0, ROT_G = 0, ROT_SKY = 0;

double max_(double a,double b)
{
	if(a>b)return a;
	else return b;
}

double min_(double a,double b)
{
	if(a<b)return a;
	else return b;
}
void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			// drawgrid=1-drawgrid;
			ROT_B = min_(90,ROT_B+1);
			break;
		case '2':
			// drawgrid=1-drawgrid;
			ROT_B = max_(0,ROT_B-1);
			break;
		case '7':
			// drawgrid=1-drawgrid;
			ROT_G = max_(-90,ROT_G-1);
			// ROT_G = ROT_G-1;
			break;
		case '8':
			// drawgrid=1-drawgrid;
			// ROT_G = ROT_G+1;
			ROT_G = min_(0,ROT_G+1);
			break;
		case '9':
			ROT_SKY = max_(-90,ROT_SKY-1);
			break;
		case '0':
			ROT_SKY = min_(0,ROT_SKY+1);
			break;
		case 'w':
			// pos.x -= 3;
			moveForward();
			break;
		case 's':
			moveBackward();
			break;
		case 'a':
			angleTurn += 1.5;
			break;
		case 'd':
			angleTurn -= 1.5;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
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



void drawHexagon(double hexR = 10)
{

	double intAng = 60 * acos(-1) / 180;

	glBegin(GL_LINES);{
		glVertex3f( hexR,0,0);
		glVertex3f(hexR*cos(intAng),hexR*sin(intAng),0);
	}
	glEnd();
}

void drawLines(PT p1,PT p2)
{
	glBegin(GL_LINES);
	{
		glVertex3f(p1.x,p1.y,p1.z);
		glVertex3f(p2.x,p2.y,p2.z);
	}
	glEnd();
}

void drawDolna(PT p,double ang)
{
	glPushMatrix();
    {
		glTranslatef(p.x,p.y,p.z);
		glRotatef(ang,0,0,1);

        PT p(0,0,0);
		PT p1(0,-2,-10);
		PT p2(0,2,-10);
		drawLines(p,p1);
		drawLines(p,p2);

		glTranslatef(0,0,-10);
		drawRectangle(0.5,4);
    }
    glPopMatrix();
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

	// gluLookAt(0,70,20,	0,0,0,	0,0,1);
	double cameraDistance = 100;
	// gluLookAt(cameraDistance*cos(cameraAngle), cameraDistance*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(0,0,80,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

	double r1 = 10;
	double r2 = r1/2;
	double r3 = r2/2;

	drawCircle(r1,40);

	glPushMatrix();
    {
        glRotatef(angle,0,0,1);
		glTranslatef(r1+r2,0,0);
		drawCircle(r2,40);

		glPushMatrix();
		{
			glRotatef(angle,0,0,1);
			glTranslatef(r2+r3,0,0);
			drawCircle(r3,40);
		}
		glPopMatrix();
    }
    glPopMatrix();

	
	
	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=1;
	drawaxes=0;
	cameraHeight=100.0;
	cameraAngle=1.0;
	angleTurn=0;
	angleFwd=0;

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
	gluPerspective(60,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

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
