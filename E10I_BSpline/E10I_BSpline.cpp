#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

struct Point
{
	int c[2];
	int& x = c[0];
	int& y = c[1];

	Point() { x = 0, y = 0; }
	Point(int x0, int y0) { x = x0, y = y0; }
	Point(const Point& pt) { x = pt.x, y = pt.y; }
	int& operator[](const int i) { return c[i]; }
};

vector<Point> GctrlPt, GbsCurvePt;
int GnumSegment = 10;

void CalcBSPoints()
{
	int numCtrPt = GctrlPt.size();
	float F0[2], F1[2], F2[2], F3[2];
	for (int i = 0; i < 2; i++)
	{
		F0[i] = (GctrlPt[numCtrPt - 4][i] + 4 * GctrlPt[numCtrPt - 3][i] + GctrlPt[numCtrPt - 2][i]) / 6;
		F1[i] = (-3 * GctrlPt[numCtrPt - 4][i] + 3 * GctrlPt[numCtrPt - 2][i]) / 6;
		F2[i] = (3 * GctrlPt[numCtrPt - 4][i] - 6 * GctrlPt[numCtrPt - 3][i] + 3 * GctrlPt[numCtrPt - 2][i]) / 6;
		F3[i] = (-GctrlPt[numCtrPt-4][i] + 3 * GctrlPt[numCtrPt-3][i] - 3 * GctrlPt[numCtrPt-2][i] + GctrlPt[numCtrPt-1][i]) / 6;
	}

	float t = 0;
	float dt = 1 / (float)GnumSegment;

	//第一段NumSubSegment + 1个点，第二段之后每段为NumSubSegment个点,因为第二段之后的起点为前一段的终点
	int numCurvePoint = (GnumSegment + 1) + GnumSegment * (numCtrPt - 4);
	GbsCurvePt.resize(numCurvePoint);

	int startIdx = GnumSegment * (numCtrPt - 4);
	//第一段起点编号为0，第二段之后的为1，原因见上
	for (int i = (numCtrPt == 4)? 0:1; i < GnumSegment + 1; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			GbsCurvePt[i + startIdx][j] = F0[j] + F1[j] * t + F2[j] * t*t + F3[j] * t*t*t;
		}
		t += dt;
	}
}

void ControlPoint(const vector<Point>& vpt)
{
	glPointSize(3);
	glBegin(GL_POINTS);
	for (unsigned int i = 0; i < vpt.size(); i++)
	{
		glVertex2f(vpt[i].x, vpt[i].y);
	}
	glEnd();
}

void PolylineGL(const vector<Point>& vpt)
{
	glBegin(GL_LINE_STRIP);
	for (unsigned int i = 0; i < vpt.size(); i++)
	{
		glVertex2i(vpt[i].x, vpt[i].y);
	}
	glEnd();
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 0.0f, 0.0f);
	ControlPoint(GctrlPt);

	glColor3f(0.0f, 1.0f, 0.0f);
	PolylineGL(GctrlPt);

	if (GctrlPt.size() >= 4)
	{
		glColor3f(1.0f, 1.0f, 1.0f);
		CalcBSPoints();
		PolylineGL(GbsCurvePt);
	}

	glFlush();
}

void Init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);

	printf("Please Click left button of mouse to input control point of B-Spline Curve.\n");
	printf("Please Click right button of mouse to new a B-Spline Curve.\n");
}

void myReshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
}

void myMouse(int button, int state, int x, int y)
{
	Point pt;
	int numCtrPt = GctrlPt.size();
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			pt.x = x;
			pt.y = 480 - y;

			GctrlPt.push_back(pt);
			glutPostRedisplay();
		}
		break;
	default:
		break;
	}
}

void processMenuEvents(int option) 
{
	switch (option) {
	case 1:
		GctrlPt.clear();
		glutPostRedisplay();
		break;
	}
}

void createGLUTMenus(){
	int menu;

	// 创建菜单并告诉GLUT，processMenuEvents处理菜单事件。
	menu = glutCreateMenu(processMenuEvents);

	//	给菜单增加条目
	glutAddMenuEntry("New B-Spline Curve", 1);

	// 把菜单和鼠标右键关联起来。
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 480);
	glutCreateWindow("B-Spline Curve");

	Init();
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(myReshape);
	glutMouseFunc(myMouse);
	createGLUTMenus();
	glutMainLoop();
	return 0;
}

