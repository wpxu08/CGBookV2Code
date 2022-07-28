#include  <GL/glut.h> 
#include <algorithm> 

using namespace std;

void line(int x0, int y0, int x1, int y1)
{
	// Please add the code here
	/********** Begin ********/
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_POINTS);
	glPointSize(1);

	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	if (dx > dy) // x方向为步进方向
	{
		if (x0 > x1) //确保x0<x1，这样循环中x=x+1
		{
			swap(x0, x1);
			swap(y0, y1);
		}

		int d = dx - 2 * dy;
		int d1 = 2 * dx - 2 * dy;
		int d2 = -2 * dy;
		int x = x0;
		int y = y0;
		int yIncr = (y1 > y0) ? 1 : -1; // 比较两端点y值大小决定y的增量值
		glVertex2i(x, y);
		for (int x = x0 + 1; x <= x1; x++)
		{
			if (d < 0)
			{
				y = y + yIncr;
				d = d + d1;
			}
			else
				d = d + d2;

			glVertex2i(x, y);
		}
	}
	else
	{
		if (y0 > y1)
		{
			swap(x0, x1);
			swap(y0, y1);
		}

		int d = dy - 2 * dx;
		int d1 = 2 * dy - 2 * dx;
		int d2 = -2 * dx;
		int x = x0;
		int y = y0;
		int xIncr = (x1 > x0) ? 1 : -1;
		glVertex2i(x, y);
		for (int y = y0 + 1; y <= y1; y++)
		{
			if (d < 0)
			{
				x = x + xIncr;
				d = d + d1;
			}
			else
				d = d + d2;

			glVertex2i(x, y);
		}
	}
	glEnd();
	/********** End *********/
}

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	line(261, 215, 344, 275);
	line(344, 275, 429, 213);
	line(429, 213, 398, 319);
	line(398, 319, 477, 384);

	line(477, 384, 378, 385);
	line(378, 385, 344, 491);
	line(344, 491, 310, 384);
	line(310, 384, 209, 382);

	line(209, 382, 292, 319);
	line(292, 319, 261, 215);
	glFlush();
}

void Init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
}
void myReshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Hello Line!");
	Init();
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(myReshape);
	glutMainLoop();

	return 0;
}