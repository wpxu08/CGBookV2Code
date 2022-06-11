// CG4.4.1_TriangleTransform.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include  <GL/glut.h> 
#include  <stdlib.h>
#include  <math.h>
/*  初始化显示窗口大小 */
GLsizei winWidth = 600, winHeight = 600;
/* 设置世界坐标系的显示范围 */
GLfloat xwcMin = 0.0, xwcMax = 225.0;
GLfloat ywcMin = 0.0, ywcMax = 225.0;
/* 定义二维点数据结构 */
class wcPt2D
{
public:
	GLfloat  x, y;
};
typedef GLfloat Matrix3x3[3][3];
Matrix3x3 matComposite;  	//定义复合矩阵
const GLdouble pi = 3.14159;
void init(void)
{
	/* 设置显示窗口的背景颜色为白色 */
	glClearColor(1.0, 1.0, 1.0, 0.0);
}
/* 构建3*3的单位矩阵 */
void matrix3x3SetIdentity(Matrix3x3 matIdent3x3)
{
	GLint row, col;

	for (row = 0; row < 3; row++)
		for (col = 0; col < 3; col++)
			matIdent3x3[row][col] = (row == col);
}
/* 变换矩阵m1前将m1与矩阵m2相乘,并将结果存放到m2中 */
void  matrix3x3PreMultiply(Matrix3x3 m1, Matrix3x3 m2)
{
	GLint  row, col;
	Matrix3x3  matTemp;

	for (row = 0; row < 3; row++)
		for (col = 0; col < 3; col++)
			matTemp[row][col] = m1[row][0] * m2[0][col] + m1[row][1] * m2[1][col] + m1[row][2] * m2[2][col];
	for (row = 0; row < 3; row++)
		for (col = 0; col < 3; col++)
			m2[row][col] = matTemp[row][col];
}

/* 平移变换函数，平移量tx，ty */
void translate2D(GLfloat tx, GLfloat ty)
{
	Matrix3x3  matTransl;
	/* 初始化平移矩阵为单位矩阵 */
	matrix3x3SetIdentity(matTransl);

	matTransl[0][2] = tx;
	matTransl[1][2] = ty;
	/*  将平移矩阵前乘到复合矩阵matComposite中 */
	matrix3x3PreMultiply(matTransl, matComposite);
}

/* 旋转变换函数，参数为中心点pivotPt和旋转角度theta */
void rotate2D(wcPt2D pivotPt, GLfloat theta)
{
	Matrix3x3  matRot;

	/*  初始化旋转矩阵为单位矩阵 */
	matrix3x3SetIdentity(matRot);
	matRot[0][0] = cos(theta);
	matRot[0][1] = -sin(theta);
	matRot[0][2] = pivotPt.x*(1 - cos(theta)) + pivotPt.y*sin(theta);
	matRot[1][0] = sin(theta);
	matRot[1][1] = cos(theta);
	matRot[1][2] = pivotPt.y*(1 - cos(theta)) - pivotPt.x*sin(theta);

	/*  将旋转矩阵前乘到复合矩阵matComposite中 */
	matrix3x3PreMultiply(matRot, matComposite);
}

/* 比例变换函数，参数为基准点fixedPt和缩放比例sx、sy */
void scale2D(GLfloat sx, GLfloat sy, wcPt2D fixedPt)
{
	Matrix3x3  matScale;

	/* 初始化缩放矩阵为单位矩阵  */
	matrix3x3SetIdentity(matScale);

	matScale[0][0] = sx;
	matScale[0][2] = (1 - sx)*fixedPt.x;
	matScale[1][1] = sy;
	matScale[1][2] = (1 - sy)*fixedPt.y;

	/*  将缩放矩阵前乘到复合矩阵matComposite中 */
	matrix3x3PreMultiply(matScale, matComposite);
}

/*  利用复合矩阵计算变换后坐标 */
void  transformVerts2D(GLint nVerts, wcPt2D * verts)
{
	GLint  k;
	GLfloat  temp;
	for (k = 0; k < nVerts; k++)
	{
		temp = matComposite[0][0] * verts[k].x + matComposite[0][1] * verts[k].y + matComposite[0][2];
		verts[k].y = matComposite[1][0] * verts[k].x + matComposite[1][1] * verts[k].y + matComposite[1][2];
		verts[k].x = temp;
	}
}

/*  三角形绘制函数 */
void triangle(wcPt2D * verts)
{
	GLint  k;

	glBegin(GL_TRIANGLES);
	for (k = 0; k < 3; k++)
		glVertex2f(verts[k].x, verts[k].y);
	glEnd();
}

void myDisplay()
{
	/* 定义三角形的初始位置 */
	GLint nVerts = 3;
	wcPt2D verts[3] = { {50.0,25.0},{150.0,25.0},{100.0,100.0} };
	/*  计算三角形中心位置 */
	wcPt2D centroidPt;

	GLint k, xSum = 0, ySum = 0;
	for (k = 0; k < nVerts; k++)
	{
		xSum += verts[k].x;
		ySum += verts[k].y;
	}
	centroidPt.x = GLfloat(xSum) / GLfloat(nVerts);
	centroidPt.y = GLfloat(ySum) / GLfloat(nVerts);

	/*  设置几何变换参数*/
	wcPt2D pivPt, fixedPt;
	pivPt = centroidPt;
	fixedPt = centroidPt;

	GLfloat tx = 0.0, ty = 100.0;
	GLfloat sx = 0.5, sy = 0.5;
	GLdouble theta = pi / 2.0;

	glClear(GL_COLOR_BUFFER_BIT); 	//清空显示窗口

	glColor3f(0.0, 0.0, 1.0); 	//设置前景色为蓝色
	triangle(verts);       	//显示蓝色三角形（变换前）

	/* 初始化复合矩阵为单位矩阵 */
	matrix3x3SetIdentity(matComposite);

	/* 根据变换序列重建复合矩阵  */
	scale2D(sx, sy, fixedPt); 	//变换序列1:缩放变换  
	rotate2D(pivPt, theta);	//变换序列2:旋转变换
	translate2D(tx, ty);    	//变换序列3:平移变换

	/* 应用复合矩阵到三角形 */
	transformVerts2D(nVerts, verts);

	glColor3f(1.0, 0.0, 0.0); 	//重新设置前景色为红色
	triangle(verts);       	//显示红色三角形（变换后）

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
	glutCreateWindow("二维几何变换实例-复合变换");

	init();
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(Reshape);

	glutMainLoop();
}
