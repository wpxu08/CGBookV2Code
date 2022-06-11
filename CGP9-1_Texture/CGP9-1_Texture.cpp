#define _CRT_SECURE_NO_WARNINGS
#include <GL/glut.h>
#include<GL/glaux.h>
#include<stdio.h>

#pragma comment(lib, "glaux.lib")

static GLfloat theta[] = { 0.0,0.0,0.0 };
static GLint axis = 2;
GLuint texture[1];    //创建一个纹理空间
AUX_RGBImageRec* LoadBMP(CHAR* Filename) //载入位图图像
{
	FILE* File = NULL;                             //文件句柄
	if (!Filename)                                //确保文件名已提供
	{
		return NULL;
	}
	File = fopen(Filename, "r");                   //尝试打开文件
	if (File)
	{
		fclose(File);                            //关闭文件
		return auxDIBImageLoadA(Filename);       //载入位图并返回指针
	}
	return NULL;                              //如果载入失败，返回NULL
}

int LoadGLTextures()  //载入位图并转换成纹理
{
	int Status = FALSE;                         //状态指示器
	AUX_RGBImageRec* TextureImage[1];         //创建纹理的存储空间
	memset(TextureImage, 0, sizeof(void*) * 1);//初始化
//载入位图，检查有无错误，如果位图没找到则退出
	if (TextureImage[0] = LoadBMP((CHAR*)"texture4.bmp"))
	{
		Status = TRUE;
		//使用来自位图数据生成的纹理
		glBindTexture(GL_TEXTURE_2D, 0);
		//指定二维纹理
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	if (TextureImage[0])               //纹理是否存在
	{
		if (TextureImage[0]->data)     //纹理图像是否存在
		{
			free(TextureImage[0]->data);      //释放纹理图像占用的内存
		}
		free(TextureImage[0]);        //释放图像结构
	}
	return Status;                   //返回Status
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glRotatef(theta[0], 1.0, 0.0, 0.0);
	glRotatef(theta[1], 0.0, 1.0, 0.0);
	glRotatef(theta[2], 0.0, 0.0, 1.0);
	glutSolidTeapot(1.0);
	//glutSolidSphere(1.0, 50, 50);
	glutSwapBuffers();
}

void spinCube()
{
	theta[axis] += 0.05;
	if (theta[axis] > 360.0) theta[axis] -= 360.0;
	glutPostRedisplay();
}

void mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) axis = 0;
	if (btn == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) axis = 1;
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) axis = 2;
}

void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-2.0, 2.0, -2.0 * (GLfloat)h / (GLfloat)w,
			2.0 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
	else
		glOrtho(-2.0 * (GLfloat)w / (GLfloat)h,
			2.0 * (GLfloat)w / (GLfloat)h, -2.0, 2.0, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
}

void key(unsigned char k, int x, int y)
{
	if (k == '1') glutIdleFunc(spinCube);
	if (k == '2') glutIdleFunc(NULL);
	if (k == 'q') exit(0);
}
void Init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	LoadGLTextures();
	GLfloat planes[] = { 0.5,0.0,0.0,0.5 };
	GLfloat planet[] = { 0.0,0.5,0.0,0.5 };
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_S, GL_OBJECT_LINEAR, planes);
	glTexGenfv(GL_T, GL_OBJECT_LINEAR, planet);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	printf("Click left, middle or right button of mouse to change rotating axis");

}
int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("a teapot with texture");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutIdleFunc(spinCube);
	glutMouseFunc(mouse);
	Init();

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	glutKeyboardFunc(key);

	glutMainLoop();
}