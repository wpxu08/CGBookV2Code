// CG4.4.3_RectangleRotating.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <gl\glut.h>
#include <cmath>

GLfloat cx = 0.0f;     //正方形中心点坐标
GLfloat cy = 0.0f;
GLfloat length = 0.5f; //正方形边长
GLfloat theta = 0.0f;   //旋转初始角度值
void myDisplay()
{
	glClearColor(0.8f, 0.8f, 0.8f, 0.0f);//设置绘图背景颜色
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 0.0f, 0.0f);
	glLoadIdentity();//如果去掉这一行代码，会怎样？原因是什么？
	glRotated(theta, 0.0, 0.0, 1.0);
	glRectf(cx - length / 2, cy - length / 2, cx + length / 2, cy + length / 2);
	glutSwapBuffers();//交换双缓存
}

void myIdle() //在空闲时调用，达到动画效果
{
	theta += 0.1f;//旋转角度增加
	if (theta >= 360) //如果旋转角度大于360度，则清零
		theta = 0.0f;
	glutPostRedisplay();//重画，相当于重新调用Display()
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);//初始化GLUT库；
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);//设置显示窗口大小
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);//设置显示模式为双缓冲和RGB彩色模式）
	glutCreateWindow("旋转的正方形");// 创建显示窗口
	glutDisplayFunc(myDisplay);//注册显示回调函数
	glutIdleFunc(myIdle);//注册闲置回调函数
	glutMainLoop();//进入事件处理循环   

	return 0;
}
