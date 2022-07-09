
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
double radius;
double position;
struct point
{
	double x,y,z;
};

//global variables

struct point pos;
struct point u;
struct point r;
struct point l;


struct point center;
double wheelstacks;
double length,breadth;
double leftright;
double ang,rectTraverse,st;
double distRect;
double angles,angles2,angles3;

void vectorInit(){
    angles = 0;
    angles2 = 0;
    angles3 = 0;
    st = 0;
    radius = 20;
    rectTraverse = 0;
    position = 0;
    distRect = 0;
    //rectTraverse = 360/(2*pi*radius)

    center.x = 0;
    center.y = 0;
    center.z = radius;

    double sqrt2 = sqrt(2);
    ang = 3;
    wheelstacks = 30;
    length = 20;
    breadth = 3;
    leftright = 0;

    pos.x = 100.0;
    pos.y = 100.0;
    pos.z = 0.0;

    u.x = 0.0;
    u.y = 0.0;
    u.z = 1.0;

    r.x = -1/sqrt2;
    r.y = 1/sqrt2;
    r.z = 0.0;

    l.x = -1/sqrt2;
    l.y = -1/sqrt2;
    l.z = 0.0;
}


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

				//if(i==0)
					//continue;	//SKIP the MAIN axes

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


void drawRectangle(double a,double b)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, b,0);
		glVertex3f( a,-b,0);
		glVertex3f(-a,-b,0);
		glVertex3f(-a, b,0);
	}glEnd();
}




void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
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
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
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
	struct point points[100][100];
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








void drawWheel(double radius,int slices,int stacks)
{
    glRotated(90,1,0,0);

        glTranslated(0,radius,0);
	struct point points[100][100];
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
	for(i=0;i<stacks/6;i++)
	{
	    glColor3f(255,255,255);
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
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





void drawSS()
{

     glColor3f(1,1,1);
    glRotated(angles,0,0,1);
    //glRotated(90,0,1,1);
    drawCircle(30,30);
    glBegin(GL_LINES);
        glVertex2f(20, 20);
        glVertex2f(-20, -20);
    glEnd();
    glBegin(GL_LINES);
        glVertex2f(-20, 20);
        glVertex2f(20, -20);
    glEnd();
    glTranslatef(50,0,0);


    glRotated(angles2,0,0,1);
    drawCircle(20,30);
    glBegin(GL_LINES);
        glVertex2f(14, 14);
        glVertex2f(-14, -14);
    glEnd();
    glBegin(GL_LINES);
        glVertex2f(-14, 14);
        glVertex2f(14, -14);
    glEnd();
    glTranslatef(30,0,0);
    glRotated(angles3*3,0,0,1);
    glBegin(GL_LINES);
        glVertex2f(7, 7);
        glVertex2f(-7, -7);
    glEnd();
    glBegin(GL_LINES);
        glVertex2f(7, -7);
        glVertex2f(-7, 7);
    glEnd();
    drawCircle(10,30);

    //double angleOfRotation = ang* pi/180;
    /*double angleOfRotation = ang;

    double rectX = leftright * cos(rectTraverse);
    double rectY = leftright * sin(rectTraverse);



    //glTranslated(st,st,0);

    glTranslatef(center.x,center.y,0);
    //printf("The center is: %d %d",center.x,center.y);
    glRotated(angleOfRotation,0,0,1);

    //glTranslatef(center.x,center.y,0);
        //glTranslated();
    //glTranslated(leftright,0,0);

    //glTranslated(center.x,center.y,0);
    glPushMatrix();{
        drawWheel(radius,30,wheelstacks);
    }glPopMatrix();


    glPushMatrix();{
        glColor3f(255,255,255);
        //glRotated(45,0,1,0);
        glTranslated(0,0,radius);


        //glRotated(angleOfRotation,0,0,1);


        glRotated(rectTraverse,0,1,0);


        drawRectangle(length,breadth);
    }glPopMatrix();

    glPushMatrix();{
        glColor3f(255,255,255);
        //glRotated(45,0,1,0);
        glTranslated(0,0,radius);
        glRotated(90,0,1,0);
        //glRotated(-angleOfRotation,1,0,0);

        glRotated(rectTraverse,0,1,0);
        //glTranslated(0,0,-radius/2);
        drawRectangle(length,breadth);
    }glPopMatrix();
*/

    //drawSphere(25,100,60);
    /*glPushMatrix();
    {
        glRotatef(angle,0,0,1);
        glTranslatef(60,0,0);
        glRotatef(2*angle,0,0,1);
        glColor3f(0,0,1);
        drawSquare(10);
    }
    glPopMatrix();

    glRotatef(3*angle,0,0,1);
    glTranslatef(40,0,0);
    glRotatef(4*angle,0,0,1);
    glColor3f(1,1,0);
    drawSquare(5);*/
}




void keyboardListener(unsigned char key, int x,int y){
    double angle = 3.0 * 3.1416/180;
	switch(key){

		case '1':
			//drawgrid=1-drawgrid;
            //int tempx,tempy;
            //LookLeftOrRight(angle);
            //l.x += 0.1;
            angles+=1;
            if(angles2<=125){
                angles2 += 1;
                angles3 +=1;
            }else angles3-=1;
			break;
        case '2':
            angles-=1;
            //l.x += l.x * cos(30);
            //LookLeftOrRight(-angle);
            //l.x -= 0.1;
            if(angles2>=-125){
                angles2 -= 1;
                angles3 -= 1;
            }else angles3 += 1;
			break;
        case '3':
            //LookUpOrDown(-angle);
            //l.z += 0.1;
            //u.z += 0.1;
			break;
		case '4':
            //LookUpOrDown(angle);
            //l.z -= 0.1;
            //u.z -= 0.1;
			break;
        case '5':
            //CLOCKorCOUNTERCLOCK(-angle);
            //u.x += l.y * cos(30);
            //u.x -= 0.1;
			break;
        case '6':
            //CLOCKorCOUNTERCLOCK(angle);
            //u.x -= l.y * cos(30);
            //u.x += 0.1;
			break;
		case 's':
            leftright += 1;
            rectTraverse += 360/(2*pi*radius);
            //double tempX = cos(ang*pi/180);
            //double tempY = sin(ang*pi/180);
            center.x += cos(ang*pi/180);
            center.y += sin(ang*pi/180);

            position = sqrt(pow(center.x,2) + pow(center.y,2));
			break;
        case 'w':
            leftright -= 1;
            rectTraverse -= 360/(2*pi*radius);
            //double tempXx = cos(ang*pi/180);
            //double tempYy = sin(ang*pi/180);
            center.x -= cos(ang*pi/180);
            center.y -= sin(ang*pi/180);
            position = sqrt(pow(center.x,2) + pow(center.y,2));
			break;
        case 'a':
            ang += 2;
           // st = (position * ang * pi)/360;

            break;
        case 'd':
            ang -=2;
            //st = (position * ang * pi)/360;
            break;

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
		    //pos.x += ux;
			//pos.y += uy;
			//pos.z += uz;

			break;
		case GLUT_KEY_PAGE_DOWN:
		    //pos.x -= ux;
			//pos.y -= uy;
			//pos.z -= uz;
			break;

		case GLUT_KEY_INSERT:
		    //pos.y -= 100.0;
			break;

		case GLUT_KEY_HOME:
		    //pos.y -= 100.0;
			break;
		case GLUT_KEY_END:
		    //pos.y -= 100.0;
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

    //vectorInit();

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

	gluLookAt(0,0,100,	0,0,0,	0,1,0);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(0,0,200,	0,0,0,	0,1,0);
    //gluLookAt(pos.x,pos.y,pos.z,	pos.x + l.x,pos.y + l.y,pos.z + l.z,	u.x,u.y,u.z);

    //gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		pos.x + l.x,pos.y + l.y, pos.z+ l.z,		u.x,u.y,u.z);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects
   // glPushMatrix();{
   // drawgrid = 1;
	drawAxes();
	//drawGrid();
    //}glPopMatrix();

    //glColor3f(1,0,0);
    //drawSquare(10);

    drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);




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
	vectorInit();
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

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
	gluPerspective(80,	1,	1,	500.0);
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

