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

vector<Point> GctrlPt, GbzCurvePt;
int GnumSegment = 10;

void CalcBZPoints()
{
	float F0[2], F1[2], F2[2], F3[2];
	for (int i = 0; i < 2; i++)
	{
		F0[i] = GctrlPt[0][i];
		F1[i] = -3 * GctrlPt[0][i] + 3 * GctrlPt[1][i];
		F2[i] = 3 * GctrlPt[0][i] - 6 * GctrlPt[1][i] + 3 * GctrlPt[2][i];
		F3[i] = -GctrlPt[0][i] + 3 * GctrlPt[1][i] - 3 * GctrlPt[2][i] + GctrlPt[3][i];
	}

	float t = 0;
	float dt = 1 / (float)GnumSegment;
	GbzCurvePt.resize(GnumSegment + 1);
	for (int i = 0; i < GnumSegment + 1; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			GbzCurvePt[i][j] = F0[j] + F1[j] * t + F2[j] * t*t + F3[j] * t*t*t;
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

	if (GctrlPt.size() == 4)
	{
		glColor3f(1.0f, 1.0f, 1.0f);
		CalcBZPoints();
		PolylineGL(GbzCurvePt);
	}

	glFlush();
}

void Init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);

	printf("Please Click left button of mouse to input control point of Bezier Curve!\n");
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
	int cptNum = GctrlPt.size();
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			pt.x = x;
			pt.y = 480 - y;

			if (cptNum != 4)
			{
				GctrlPt.push_back(pt);
				glutPostRedisplay();
			}
			else
			{
				GctrlPt.clear();
				GctrlPt.push_back(pt);
				glutPostRedisplay();
			}
		}
		break;
	default:
		break;
	}
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Bezier Curve");

	Init();
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(myReshape);
	glutMouseFunc(myMouse);
	glutMainLoop();
	return 0;
}

