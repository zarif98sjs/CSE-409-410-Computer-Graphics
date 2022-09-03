#include<bits/stdc++.h>
#include <GL/glut.h>
using namespace std;
#include "1705010_Classes.h"
#include "bitmap_image.hpp"
#include <windows.h>
#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

int recursionLevel;
int imageHeight,imageWidth;
bitmap_image image;

vector <Object*> objects;
vector <Light*> lights;
vector <SpotLight*> spotlights;

int imageCount = 1;

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

double windowWidth = 500, windowHeight = 500;
double viewAngle = 80;

void capture()
{
	cout<<"Capturing Image"<<endl;

	// initialize bitmap image and set background color to black
	for(int i=0;i<imageWidth;i++)
		for(int j=0;j<imageHeight;j++)
			image.set_pixel(i, j, 0, 0, 0);
	
	// image.save_image("black.bmp");

	double planeDistance = (windowHeight / 2.0) / tan((pi * viewAngle) / (360.0));

	PT topLeft = pos + (look * planeDistance) + (up * (windowHeight / 2.0)) - (rightV * (windowWidth / 2.0));

	double du = windowWidth / (imageWidth*1.0);
	double dv = windowHeight / (imageHeight*1.0);

	// Choose middle of the grid cell
	topLeft = topLeft + (rightV * du / 2.0) - (up * dv / 2.0);

	int nearestObjectIndex = -1;
	double t,tMin;

	for(int i=0;i<imageWidth;i++)
	{
		for(int j=0;j<imageHeight;j++)
		{
			// calculate current pixel
			PT pixel = topLeft + (rightV * du * i) - (up * dv * j);

			// cast ray from EYE to (curPixel-eye) direction ; eye is the position of the camera
			Ray ray(pos,pixel-pos);
			Color color;

			// cout<<"Ray direction "<<ray.dir<<endl;

			// find nearest object
			tMin = -1;
			nearestObjectIndex = -1;
			for(int k=0;k<(int)objects.size();k++)
			{
				t = objects[k]->intersect(ray,color, 0);
				if(t>0 && (nearestObjectIndex == -1 || t<tMin) )
					tMin = t , nearestObjectIndex = k;
			}

			// if nearest object is found, then shade the pixel
			if(nearestObjectIndex != -1)
			{
				// cout<<"Object "<<nearestObjectIndex<<" intersected"<<endl;
				// color = objects[nearestObjectIndex]->color;
				color = Color(0,0,0);
				// cout<<"Before Color "<<color.r<<" "<<color.g<<" "<<color.b<<endl;
				double t = objects[nearestObjectIndex]->intersect(ray,color, 1);

				if(color.r > 1) color.r = 1;
				if(color.g > 1) color.g = 1;
				if(color.b > 1) color.b = 1;

				if(color.r < 0) color.r = 0;
				if(color.g < 0) color.g = 0;
				if(color.b < 0) color.b = 0;
				
				// cout<<"After Color "<<color.r<<" "<<color.g<<" "<<color.b<<endl;
				image.set_pixel(i, j, 255*color.r, 255*color.g, 255*color.b);
			}
		}
	}

	image.save_image("Output_1"+to_string(imageCount)+".bmp");
	imageCount++;
	cout<<"Saving Image"<<endl;		
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){
		case '0':
			capture();
			break;
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

	for(int i=0;i<spotlights.size();i++){
		SpotLight* l = spotlights[i];
		// cout<<"Spotlight "<<spotlight<<endl
		spotlights[i]->draw();
	}

	glutSwapBuffers();
}


void animate(){
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void loadData()
{
	ifstream in("scene.txt");
	in >> recursionLevel >> imageHeight;

	imageWidth = imageHeight;

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
			obj = new General();
			in >> *((General *)obj);
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

	int spotlightCount;
	in >> spotlightCount;

	for(int i=0;i<spotlightCount;i++){
		SpotLight *spotlight = new SpotLight();
		in >> *spotlight;
		spotlights.push_back(spotlight);
	}

	// for(int i=0;i<spotlights.size();i++){
	// 	SpotLight* l = spotlights[i];
	// 	// cout<<"Spotlight "<<spotlight<<endl;

	// 	cout << fixed << setprecision(7) << l->pointLight.pos.x << " " << l->pointLight.pos.y << " " << l->pointLight.pos.z << " " << l->pointLight.color.r << " " << l->pointLight.color.g << " " << l->pointLight.color.b << " " << l->dir.x << " " << l->dir.y << " " << l->dir.z << " " << l->cutoffAngle<<endl;

	// 	// cout<<"Spotlight pos "<<spotlights[i]->pointLight.pos.x<<" "<<spotlights[i]->pointLight.pos.y<<" "<<spotlights[i]->pointLight.pos.z<<endl;
	// 	// cout<<"Spotlight dir "<<spotlights[i]->pointLight.dir.x<<" "<<spotlights[i]->pointLight.dir.y<<" "<<spotlights[i]->pointLight.dir.z<<endl;
	// 	// cout<<"Spotlight cutOff "<<spotlights[i]->pointLight.cutOff<<endl;
	// 	// spotlights[i]->draw();
	// }

        

	Object *floor;
	floor = new Floor(400, 10);
	floor->setColor(Color(0.5, 0.5, 0.5));
	// vector <double> coefficients = {1, 1, 1, 1};
	vector <double> coefficients = {0.4, 0.2, 0.2, 0.2};
	// vector <double> coefficients = {0.2, 0.2, 0.2, 0.2};
	floor->setCoefficients(coefficients);
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

	loadData();
	image = bitmap_image(imageWidth, imageHeight);

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

	objects.clear();
	objects.shrink_to_fit();

	lights.clear();
	lights.shrink_to_fit();

	spotlights.clear();
	spotlights.shrink_to_fit();

	return 0;
}
