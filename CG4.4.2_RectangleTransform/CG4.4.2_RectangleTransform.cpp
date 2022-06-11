// CG4.4.2_RectangleTransform.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include  <GL/glut.h> 
#include  <stdlib.h>
#include  <math.h>

/* 初始化显示窗口大小 */
GLsizei winWidth = 600, winHeight = 600;
/* 设置世界坐标系的显示范围 */
GLfloat xwcMin = -300.0, xwcMax = 300.0;
GLfloat ywcMin = -300.0, ywcMax = 300.0;
void init(void)
{
	/* 设置显示窗口的背景颜色为白色 */
	glClearColor(1.0, 1.0, 1.0, 0.0);
}
class wcPt3D
{
public:
	GLfloat x, y, z;
};
/* 三维旋转变换，参数：旋转轴（由点p1和p2定义）和旋转角度（thetaDegrees）*/
void rotate3D(wcPt3D p1, wcPt3D p2, GLfloat thetaDegrees)
{
	/* 设置旋转轴的矢量 */
	float vx = (p2.x - p1.x);
	float vy = (p2.y - p1.y);
	float vz = (p2.z - p1.z);

	/* 通过平移-旋转-平移复合变换序列完成任意轴的旋转（注意OpenGL中的反序表示）*/
	glTranslatef(p1.x, p1.y, p1.z); 	//③移动p1到原始位置 
	/* ②关于通过坐标原点的坐标轴旋转*/
	glRotatef(thetaDegrees, vx, vy, vz);
	glTranslatef(-p1.x, -p1.y, -p1.z); 	//①移动p1到原点位置
}

/* 三维比例缩放变换，参数：比例系数sx、sy、sz和固定点fixedPt */
void scale3D(GLfloat sx, GLfloat sy, GLfloat sz, wcPt3D fixedPt)
{
	/* 通过平移-缩放-平移复合变换序列完成任意点为中心点的比例缩放*/
	/* ③反平移到原始位置*/
	glTranslatef(fixedPt.x, fixedPt.y, fixedPt.z);
	glScalef(sx, sy, sz);	//②基于原点的比例缩放变换
	/* ① 移动固定点到坐标原点*/
	glTranslatef(-fixedPt.x, -fixedPt.y, -fixedPt.z);
}
void myDisplay()
{
	/* 设置变换中心点位置 */
	wcPt3D centroidPt, R_p1, R_p2;

	centroidPt.x = 50;
	centroidPt.y = 100;
	centroidPt.z = 0;

	R_p1 = centroidPt;
	R_p2.x = 50;
	R_p2.y = 100;
	R_p2.z = 1;

	/* 设置几何变换参数*/
	wcPt3D p1, p2, fixedPt;
	p1 = R_p1;
	p2 = R_p2;
	fixedPt = centroidPt;

	GLfloat tx = 0.0, ty = 100.0, tz = 0;
	GLfloat sx = 0.5, sy = 0.5, sz = 1;
	GLdouble thetaDegrees = 90;

	glClear(GL_COLOR_BUFFER_BIT); 	//清空显示窗口
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); 	//清空变换矩阵为单位矩阵，恢复原始坐标系环境

	/* 显示变换前几何对象 */
	glColor3f(0.0, 0.0, 1.0); 	//设置前景色为蓝色
	glRecti(50, 100, 200, 150);	//显示蓝色矩形（变换前）

	/* 执行几何变换（注意以反序形式写出）*/
	glTranslatef(tx, ty, tz);	//③平移变换
	scale3D(sx, sy, sz, fixedPt);	//②比例缩放变换
	rotate3D(p1, p2, thetaDegrees);	//①旋转变换

	/* 显示变换后几何对象 */
	glColor3f(1.0, 0.0, 0.0); 	//重新设置前景色为红色
	glRecti(50, 100, 200, 150);	//显示红色矩形（变换后）

	glFlush();
}
void Reshape(GLint newWidth, GLint newHeight)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(xwcMin, xwcMax, ywcMin, ywcMax);
	glClear(GL_COLOR_BUFFER_BIT);
}
void main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("二维几何变换实例-OpenGL版复合变换");

	init();
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(Reshape);

	glutMainLoop();
}