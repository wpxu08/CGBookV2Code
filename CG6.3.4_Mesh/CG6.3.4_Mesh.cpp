#include <gl/glut.h>  
#include<iostream>
using namespace std;
float	rtri;
float	rquad;
GLfloat points0[5][3] = { { 0, 1,  0}, {-1, -1, 1}, { 1, -1, 1}, {	1, -1, -1},{-1, -1,-1} };
GLfloat points1[8][3] = { { 1, 1, -1 }, {-1, 1, -1}, {-1, 1, 1}, { 1, 1, 1},
	{ 1, -1, 1 }, {-1, -1, 1}, {-1,-1,-1}, { 1, -1, -1} };
GLfloat Colors0[4][3] = { {1,0,0},{0,1,0}, {0,0,1},{1,1,0} };	//四棱锥的颜色
//下行是立方体的颜色
GLfloat Colors1[6][3] = { {0,1,0},{1,0.5,0},{1,0,0},{1,1,0},{0,0,1},{1,0,1} };
//int vertice0[4][3] = { {0,1,2},{0,2,3},{0,3,4},{0,4,1} };	//四棱锥的顶点号序列
int vertice0[4][3] = { {0,2,1},{0,3,2},{0,4,3},{0,1,4} };	//四棱锥的顶点号序列
//下行是立方体的顶点号序列
int vertice1[6][4] = { {0,1,2,3},{4,5,6,7},{3,2,5,4},{7,6,1,0},{2,1,6,5}, {0,3,4,7} };
void InitGL(GLvoid)
{
	glShadeModel(GL_SMOOTH);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_COLOR_MATERIAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glPolygonMode(GL_FRONT, GL_FILL);//控制多边形的正面与背面绘制模式，此处设为正面填充绘制
	glPolygonMode(GL_BACK, GL_LINE);//背面为轮廓线绘制
}
void CreatePyramid()
{
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 4; i++)
	{
		glColor3fv(Colors0[i]);
		for (int j = 0; j < 3; j++)
		{
			int VtxId = vertice0[i][j];
			glVertex3fv(points0[VtxId]);
		}
	}
	glEnd();
	glBegin(GL_QUADS); 	//构建底面
	glColor3f(1.0f, 1.0f, 1.0f);
	for (int i = 0; i < 4; i++)
		glVertex3fv(points0[i]);
	glEnd();
}
void CreateCube()
{
	glBegin(GL_QUADS);
	for (int i = 0; i < 6; i++)
	{
		glColor3fv(Colors1[i]);
		for (int j = 0; j < 4; j++)
		{
			int VtxId = vertice1[i][j];
			glVertex3fv(points1[VtxId]);
		}
	}
	glEnd();
}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(-1.5f, 0.0f, -6.0f);	//平移至左侧
	glRotatef(rtri, 0.0f, 1.0f, 0.0f);	//旋转一个角度
	CreatePyramid();	//创建三角塔

	glLoadIdentity();	//将矩阵归一化回原样    
	glTranslatef(1.5f, 0.0f, -6.0f);	//平移到右侧
	glRotatef(rquad, 1.0f, 0.0f, 0.0f);	//旋转一个角度
	CreateCube(); 	//创建立方体
	glPopMatrix();

	rtri += 0.01f;	//修改三角塔旋转角度
	rquad -= 0.01f;	//修改立方体的旋转角度
	glutSwapBuffers();
}
void reshape(int width, int height)
{
	if (height == 0)
		height = 1;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(600, 400);
	glutCreateWindow("Pyramid and cube");
	InitGL();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(display);
	glutMainLoop();

	return 0;
}