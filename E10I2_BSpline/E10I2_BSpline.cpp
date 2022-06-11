#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

struct Point
{
	float c[2];
	float& x = c[0];
	float& y = c[1];

	Point() { x = 0, y = 0; }
	Point(float x0, float y0) { x = x0, y = y0; }
	Point(const Point& pt) { x = pt.x, y = pt.y; }
	float& operator[](const int i) { return c[i]; }
};

vector<Point> GctrlPt, GbsCurvePt;
int GnumSegment = 10;

void CalcBSPoints()
{
	int numCtrPt = GctrlPt.size();
	float F0[2], F1[2], F2[2], F3[2];

	float t = 0;
	float dt = 1 / (float)GnumSegment;

	//第一段NumSubSegment + 1个点，第二段之后每段为NumSubSegment个点,因为第二段之后的起点为前一段的终点
	int numCurvePoint = (GnumSegment + 1) + GnumSegment * (numCtrPt - 4);
	GbsCurvePt.resize(numCurvePoint);

	//第一段起点编号为0，第二段之后的为1，原因见上
	for (int i = 0; i < (numCtrPt - 3); i++)
	{
		for (int j = 0; j < 2; j++)
		{
			F0[j] = (GctrlPt[i][j] + 4 * GctrlPt[1 + i][j] + GctrlPt[2 + i][j]) / 6.0f;
			F1[j] = (-3 * GctrlPt[i][j] + 3 * GctrlPt[2 + i][j]) / 6.0f;
			F2[j] = (3 * GctrlPt[i][j] - 6 * GctrlPt[1 + i][j] + 3 * GctrlPt[2 + i][j]) / 6.0f;
			F3[j] = (-GctrlPt[i][j] + 3 * GctrlPt[1 + i][j] - 3 * GctrlPt[2 + i][j] + GctrlPt[3 + i][j]) / 6.0f;
		}

		t = (i == 0) ? 0 : dt;//第一段起点参数为0，第二段之后的为dt，原因同上
		for (int j = (i == 0) ? 0 : 1; j < GnumSegment + 1; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				GbsCurvePt[j + GnumSegment * i][k] = F0[k] + F1[k] * t + F2[k] * t*t + F3[k] * t*t*t;
			}
			t += dt;
		}
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
		glVertex2f(vpt[i].x, vpt[i].y);
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

	float points[9][3] = { { 10, 5, 0 },	{ 5, 10, 0 },	{ -5, 15, 0 },	{ -10, -5, 0 },	{ 4, -4, 0 },
	{ 10, 5, 0 },	{ 5, 10, 0 },	{ -5, 15, 0 },	{ -10, -5, 0 } };
	Point pt;
	for (int i = 0; i < 9; i++)
	{
		Point pt = Point(points[i][0], points[i][1]);
		GctrlPt.push_back(pt);
	}
}

void myReshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-30.0, 30.0, -30.0*(GLfloat)h / (GLfloat)w, 30.0*(GLfloat)h / (GLfloat)w, 0.0, 100.0);
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
	glutMainLoop();
	return 0;
}