#include  <GL/glut.h> 
#include  <stdlib.h>
#include  <math.h>

void init(void)
{
	/* 设置显示窗口的背景颜色为白色 */
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

void triangle()
{
	glBegin(GL_TRIANGLES);
	glVertex2f(100.0, 100.0);
	glVertex2f(200.0, 100.0);
	glVertex2f(100.0, 270.0);
	glEnd();
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT); 	//清空显示窗口
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); 	//清空变换矩阵为单位矩阵，恢复原始坐标系环境

	glColor3f(1.0, 0.0, 0.0);
	triangle();

	glTranslatef(100, 100, 0);
	glRotatef(30, 0, 0, 1);
	glTranslatef(-100, -100, 0);

	glColor3f(0.0, 1.0, 0.0);
	triangle();

	glFlush();
}

void myReshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
}

int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(600, 600);
	glutCreateWindow("二维几何变换实例");

	init();
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(myReshape);

	glutMainLoop();

	return 0;
}