//
// Created by Subangkar on 24-Jun-19.
//

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#include <vector>

using std::vector;
using namespace std;

#define PI (2*acos(0.0))
#define RAD(deg) (deg * PI / 180)

#define AXIS_LENGTH 200
#define GRID_DIFF 15
#define GRID_LENGTH (AXIS_LENGTH-GRID_DIFF)
#define CAMERA_INIT_POS point(120, 120, 40)
#define CAMERA_INIT_L point(-1 / sqrt(2), -1 / sqrt(2), 0)
#define CAMERA_INIT_R point(-1 / sqrt(2), 1 / sqrt(2), 0)
#define CAMERA_INIT_U point(0, 0, 1)
#define CAMERA_MOVE_STEP 2
#define CAMERA_ANGLE_STEP 2
#define X_AXIS point(1,0,0)
#define Y_AXIS point(0,1,0)
#define Z_AXIS point(0,0,1)


enum color_t {
	RED, GREEN, BLUE, GREY, WHITE, BLACK, GOLDEN
};

int drawgrid;
int drawaxes;
double angle;
double speed;
double dspeed;

struct point {
	double x, y, z;

	point() { x = y = z = 0; }

	point(double x, double y, double z) : x(x), y(y), z(z) {}

	point operator-() const {
		return {-x, -y, -z};
	}

	point operator+(const point &right) const {
		return {x + right.x, y + right.y, z + right.z};
	}

	point operator-(const point &right) const {
		return {x - right.x, y - right.y, z - right.z};
	}

	point operator*(const point &right) const {
		return {x * right.x, y * right.y, z * right.z};
	}

	point operator*(const double constant) const {
		return {x * constant, y * constant, z * constant};
	}

	point operator/(const double constant) const {
		return {x / constant, y / constant, z / constant};
	}

	point operator+=(const point &inc) {
		return *this = *this + inc;
	}

	point operator-=(const point &dec) {
		return *this = *this - dec;
	}

	friend point operator*(double constant, point const &rhs);
};

inline point operator*(double constant, point const &rhs) {
	return rhs * constant;
}

double cameraHeight;
double cameraAngle;

void setColor(color_t color) {
	switch (color) {
		case RED:
			glColor3f(1, 0, 0);
			break;
		case GREEN:
			glColor3f(0, 1, 0);
			break;
		case BLUE:
			glColor3f(0, 0, 1);
			break;
		case GREY:
			glColor3f(0.7, 0.7, 0.7);
			break;
		case WHITE:
			glColor3f(1, 1, 1);
			break;
		case BLACK:
			glColor3f(0, 0, 0);
			break;
		case GOLDEN:
			glColor3f(0.9, 0.6, 0.1);
			break;
		default:
			break;
	}
}

void translate(const point &vector) {
	glTranslated(vector.x, vector.y, vector.z);
}

void rotate(double angle, const point &axis) {
	glRotated(angle, axis.x, axis.y, axis.z);
}

void drawAxes() {
	if (drawaxes == 1) {
		glBegin(GL_LINES);
		{
			// x-axis
			setColor(RED);
			glVertex3f(AXIS_LENGTH, 0, 0);
			glVertex3f(-AXIS_LENGTH, 0, 0);

			// y-axis
			setColor(GREEN);
			glVertex3f(0, -AXIS_LENGTH, 0);
			glVertex3f(0, AXIS_LENGTH, 0);

			// z-axis
			setColor(BLUE);
			glVertex3f(0, 0, AXIS_LENGTH);
			glVertex3f(0, 0, -AXIS_LENGTH);
		}
		glEnd();
	}
}


void drawGrid() {
	if (drawgrid == 1) {
		glBegin(GL_LINES);
		{
			for (auto i = -(GRID_LENGTH / GRID_DIFF); i <= (GRID_LENGTH / GRID_DIFF); i++) {
				//lines parallel to Y-axis
				glVertex3f(i * GRID_DIFF, -GRID_LENGTH, 0);
				glVertex3f(i * GRID_DIFF, GRID_LENGTH, 0);

				//lines parallel to X-axis
				glVertex3f(-GRID_LENGTH, i * GRID_DIFF, 0);
				glVertex3f(GRID_LENGTH, i * GRID_DIFF, 0);
			}
		}
		glEnd();
	}
}

void clearDisplay() {
	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);    //color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void drawSquare(double a) {
	glBegin(GL_QUADS);
	{
//		glVertex3f(a, a, 0);
//		glVertex3f(a, -a, 0);
//		glVertex3f(-a, -a, 0);
//		glVertex3f(-a, a, 0);
		glVertex3f(a, a, 0);
		glVertex3f(a, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, a, 0);
	}
	glEnd();
}

#define SQLEN 100

void
drawCubeFace(color_t color, GLdouble rotAngle = 0.00, point rotAxis = point(0, 0, 0), point transVect = point(0, 0, 0),
             bool rotBef = false) {
	glPushMatrix();
	{
		setColor(color);
		if (rotBef) {
			rotate(rotAngle, rotAxis);
			translate(transVect);
		} else {
			translate(transVect);
			rotate(rotAngle, rotAxis);
		}
		drawSquare(SQLEN);
	}
	glPopMatrix();
}


void drawHexagon(double radius) {
	int i;
	int segments = 6;
	struct point points[100];
	glColor3f(1.0, 1.0, 0.0);
	//generate points
	for (i = 0; i <= segments; i++) {
		points[i].x = radius * cos(((double) i / (double) segments) * 2 * PI);
		points[i].y = radius * sin(((double) i / (double) segments) * 2 * PI);
	}
	//draw segments using generated points
	for (i = 0; i < segments; i++) {
		glBegin(GL_LINES);
		{
			glVertex3f(points[i].x, points[i].y, 0);
			glVertex3f(points[i + 1].x, points[i + 1].y, 0);
		}
		glEnd();
	}
}

void drawCraddle(double length, double width, double height) {
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	{
		glVertex3f(0, 0, 0);
		glVertex3f(length / 2, 0, -height);


		glVertex3f(0, 0, 0);
		glVertex3f(-length / 2, 0, -height);
	}
	glEnd();

	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	{
		glVertex3f(length / 2, width / 2, -height);
		glVertex3f(length / 2, -width / 2, -height);
		glVertex3f(-length / 2, -width / 2, -height);
		glVertex3f(-length / 2, width / 2, -height);
	}
	glEnd();

}

void drawSwing() {
	drawHexagon(50);
	for (int i = 0; i < 6; i++) {
		glRotated(60, 0, 0, 1);
		glPushMatrix();
		{
			glTranslated(50, 0, 0);
			glRotated(speed*90, 0, -1, 0);
			drawCraddle(10, 5, 50);
		}
		glPopMatrix();
	}
}

GLdouble faceAngleDir[10] = {0, -1, 1, 1, -1, -1};
GLdouble angles[10] = {0, 90, 90, 90, 90, 90};
point rotAxis[10] = {X_AXIS, Y_AXIS, X_AXIS, Y_AXIS, X_AXIS, Y_AXIS};
color_t colors[10] = {GREY, RED, BLUE, GREEN, GOLDEN, WHITE};
vector<point> transVectors = {point(0, 0, 0), point(SQLEN, 0, 0), point(0, SQLEN, 0), point(-SQLEN, 0, 0),
                              point(0, -SQLEN, 0), point(SQLEN, 0, 0)};

void drawObjects() {

	drawAxes();
	setColor(GREY);
	drawGrid();

//	translate(point(-SQLEN / 2.00, -SQLEN / 2.00, 0));

//	for (const auto& trans:transVectors) {
//		drawCubeFace(RED, 0, X_AXIS, trans);
//	}
//	for (int i = 0; i < 1; ++i) {
//		drawCubeFace(colors[i], angles[i], rotAxis[i], transVectors[i]);
//	}

	glRotated(angle, 0, 0, 1);
	drawSwing();

//
//	drawCubeFace(colors[0], faceAngleDir[0] * angles[0], rotAxis[0], transVectors[0], false);
////	drawCubeFace(colors[1], faceAngleDir[1] * angles[1], rotAxis[1], transVectors[1], false);
//	drawCubeFace(colors[2], faceAngleDir[2] * angles[2], rotAxis[2], transVectors[2], false);
//	drawCubeFace(colors[3], faceAngleDir[3] * angles[3], rotAxis[3], transVectors[3], true);
//	drawCubeFace(colors[4], faceAngleDir[4] * angles[4], rotAxis[4], transVectors[4], true);
////	drawCubeFace(colors[5], faceAngleDir[5] * angles[5], rotAxis[5], transVectors[5], false);
//
//	glPushMatrix();
//	{
//		translate(transVectors[1]);
//		rotate(faceAngleDir[1] * angles[1], rotAxis[1]);
//		drawCubeFace(colors[1]);
//		translate(transVectors[5]);
//		rotate(faceAngleDir[5] * angles[5], rotAxis[5]);
//		drawCubeFace(colors[5]);
//
//	}
//	glPopMatrix();
}


void setupCamera();

void display() {

	clearDisplay();

	setupCamera();

	drawObjects();


	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}

void animate() {
	//codes for any changes in Models, Camera
	angle += speed;
	glutPostRedisplay();
}

void keyboardListener(unsigned char key, int x, int y);

void specialKeyListener(int key, int x, int y);

void mouseListener(int button, int state, int x, int y);//x, y is the x-y of the screen (2D)

void init() {

	//codes for initialization
	drawgrid = 0;
	drawaxes = 1;
	cameraHeight = 150.0;
	cameraAngle = 1.0;

	angle = 0;
	speed = 0;
	dspeed = 0.01;
	//clear the screen
	glClearColor(0, 0, 0, 0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80, 1, 1, 1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);    //Depth, Double buffer, RGB color

	glutCreateWindow("Sphere to/from Cylinder Simulation");

	init();

	glEnable(GL_DEPTH_TEST);    //enable Depth Testing

	glutDisplayFunc(display);    //display callback function
	glutIdleFunc(animate);        //what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();        //The main loop of OpenGL

	return 0;
}


#pragma region IOHandler

void mouseListener(int button, int state, int x, int y) {    //x, y is the x-y of the screen (2D)
	switch (button) {
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN) {        // 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes = 1 - drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_DOWN) {        // 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawgrid = !drawgrid;
			}
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}

void specialKeyListener(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_DOWN:        //down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:        // up arrow key
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

void keyboardListener(unsigned char key, int x, int y) {
	double diff = +1;
	GLdouble *ref = nullptr;
	switch (key) {

		case '1':
			speed = min(speed + dspeed, 1.00);
			return;
		case '2':
			speed = max(speed - dspeed, 0.00);
			return;
//		case '1':
//			ref = &angles[1];
//			diff = +1;
//			break;
//		case '2':
//			ref = &angles[1];
//			diff = -1;
//			break;

		case '3':
			ref = &angles[2];
			diff = +1;
			break;
		case '4':
			ref = &angles[2];
			diff = -1;
			break;

		case '5':
			ref = &angles[3];
			diff = +1;
			break;
		case '6':
			ref = &angles[3];
			diff = -1;
			break;

		case '7':
			ref = &angles[4];
			diff = +1;
			break;
		case '8':
			ref = &angles[4];
			diff = -1;
			break;

		case '9':
			ref = &angles[5];
			diff = +1;
			break;
		case '0':
			ref = &angles[5];
			diff = -1;
			break;

		default:
			break;
	}

	*ref = (diff > 0) ? min(*ref + 5.00, 90.00) : max(*ref - 5.00, 0.00);
}

#pragma endregion


void setupCamera() {
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
	gluLookAt(200 * cos(cameraAngle), 200 * sin(cameraAngle), cameraHeight, 0, 0, 0, 0, 0, 1);
	//	gluLookAt(0, 0, 200, 0, 0, 0, 0, 1, 0);
//	gluLookAt(camera.position().x, camera.position().y, camera.position().z, camera.position().x + camera.left().x,
//	          camera.position().y + camera.left().y, camera.position().z + camera.left().z, camera.up().x,
//	          camera.up().y, camera.up().z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);
}
