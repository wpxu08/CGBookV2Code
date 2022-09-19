// 提示：写完代码请保存之后再进行评测
#include<windows.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <algorithm>
#include <stdio.h>
#include <math.h>
using namespace std;


void Line(int x1, int y1, int xn, int yn)
{
	// 请在此添加你的代码
	/********** Begin ********/

	glColor3f(1.0f, 0.0f, 0.0f);
	glPointSize(1);

	/* 原来网上的代码 **未通过**
	int dx =  abs (x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = -abs (y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = dx + dy, e2;

	for (;;){  //循环
		glBegin(GL_POINTS);
		glPointSize(1); //点的大小为一个像素
		glVertex2i(x0, y0); //(x, y)为点的坐标
		glEnd();
		if (x0 == x1 && y0 == y1) break;
		e2 = 2 * err;
		if (e2 >= dy) { err += dy; x0 += sx; }
		if (e2 <= dx) { err += dx; y0 += sy; }
	}

	*/


	//DDA
	int dm = 0;
	if (abs(xn - x1) >= abs(yn - y1))
		dm = abs(xn - x1);//斜率绝对值<1，以x确定步长
	else
		dm = abs(yn - y1);//斜率绝对值>1，以y确定步长
	float dx = (float)(xn - x1) / dm;
	//当x是步长方向时，dx=1，也就是在x轴的步长是1
	//如果x为负方向，那么dx=-1
	float dy = (float)(yn - y1) / dm;
	float x = x1;
	float y = y1;
	for (int i = 0; i < dm; i++) {

		glBegin(GL_POINTS);
		glPointSize(1); //点的大小为一个像素
		glVertex2i((int)(x + 0.5), (int)(y + 0.5)); //(x, y)为点的坐标
		glEnd();

		x += dx;//如果是以x步长，进行绘制。
			  //绘制的时候dx的符号决定朝正方向还是负方向绘制。
		y += dy;
	}

	//太难了，代码经由参考，请见谅。
   /********** End **********/
}

void myDisplay(void)
{

	glClear(GL_COLOR_BUFFER_BIT);
	// 请在此添加你的代码用来测试直线绘制代码
	/********** Begin ********/

	int ab[30] = { 0,261, 215, 344, 275,429, 213, 398, 319, 477, 384, 378, 385,344, 491, 310, 384, 209, 382, 292, 319 };
	for (int i = 1; i <= 9; i++) {
		Line(ab[i * 2 - 1], ab[i * 2], ab[i * 2 + 1], ab[i * 2 + 2]);
	}
	Line(292, 319, 261, 215);


	/********** End **********/
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
	int width = 800;
	int height = 600;

	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Hello Line!");
	Init();
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(myReshape);
	glutMainLoop();


	return 0;
}
