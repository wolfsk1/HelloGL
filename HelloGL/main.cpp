#include <Windows.h>
#include <glew.h>
#include <glut.h>  

HGLRC           hRC = NULL;							// 窗口着色描述表句柄

HDC             hDC = NULL;							// OpenGL渲染描述表句柄

HWND            hWnd = NULL;							// 保存我们的窗口句柄

HINSTANCE       hInstance;							// 保存程序的实例

bool	keys[256];								// 保存键盘按键的数组

bool	active = TRUE;								// 窗口的活动标志，缺省为TRUE

bool	fullscreen = TRUE;							// 全屏标志缺省，缺省设定成全屏模式

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// 调整窗口用
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)
{
	// 设置height防止零除
	if (height == 0) 
	{
		height = 1;
	}
	// 重置当前窗口
	glViewport(0, 0, width, height);
	// 告知OpenGL即将操作投影矩阵
	glMatrixMode(GL_PROJECTION);
	// 重置投影矩阵
	glLoadIdentity();

	// 设置窗口大小
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

// 初始化用
int InitGL(GLvoid) 
{
	// 启用阴影平滑
	glShadeModel(GL_SMOOTH);
	// 清理屏幕
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// 设置深度缓存
	glClearDepth(1.0f);
	// 启用深度测试
	glEnable(GL_DEPTH_TEST);
	// 所做深度测试的类型 小于或等于则通过测试
	glDepthFunc(GL_LEQUAL);
	// 指定GL进行最好的投影修正
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	return TRUE;

}

// 实际屏幕绘制用
int DrawGLScene(GLvoid)
{
	// 清理屏幕及深度缓存
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 重置当前的模型观察矩阵
	glLoadIdentity();
	return TRUE;
}

// 程序推出前调用，应一词释放着色描述表，设备描述表及窗口的处理器
GLvoid KillGLWindow(GLvoid)
{
	// 检查是否全屏，如果是的话，将显示系统调整至桌面的方式显示
	if (fullscreen) 
	{
		// 使用默认的参数类回复原始分辨率并使鼠标重新可见
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);

	}

	// 检查我们是否拥有着色描述表
	if (hRC) 
	{
		// 尝试释放 将RC与DC分离
		if (!wglMakeCurrent(NULL, NULL))
		{
			MessageBox(NULL, TEXT("释放DC或RC失败。"), TEXT("关闭错误"), MB_OK | MB_ICONINFORMATION);
		}

		// 尝试删除RC
		if (!wglDeleteContext(hRC)) 
		{
			MessageBox(NULL, TEXT("释放RC失败。"), TEXT("关闭错误"), MB_OK | MB_ICONINFORMATION);
		}

		hRC = NULL;

	}
	// 查看DC是否存在并尝试释放
	if (hDC && !ReleaseDC(hWnd, hDC))
	{
		MessageBox(NULL, TEXT("释放DC失败。"), TEXT("关闭错误"), MB_OK | MB_ICONINFORMATION);

		hDC = NULL;
	}
	// 注销窗口类
	if (!UnregisterClass(TEXT("OPenG"), hInstance)) 
	{
		MessageBox(NULL, "不能注销窗口类。", "关闭错误", MB_OK | MB_ICONINFORMATION);

		hInstance = NULL;
	}
}

// 创建GL窗口
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullScreenFlag)
{
	// 像素格式
	GLuint PixelFormat;
	// 窗口类结构
	WNDCLASS wc;
	// 扩展的窗口的风格信息
	DWORD dwExStylr;
	// 窗口风格
	DWORD dwStyle;
	// 窗口矩形相关
	RECT WindowRect;							// 取得矩形的左上角和右下角的坐标值

	WindowRect.left = (long)0;						// 将Left   设为 0

	WindowRect.right = (long)width;						// 将Right  设为要求的宽度

	WindowRect.top = (long)0;							// 将Top    设为 0

	WindowRect.bottom = (long)height;						// 将Bottom 设为要求的高度

	fullscreen = fullScreenFlag;						// 设置全局全屏标志

	// 取得窗口实例
	hInstance = GetModuleHandle(NULL);
	// 移动时重绘，为窗口创建私有DC 
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;		// 移动时重画，并为窗口取得DC

	wc.lpfnWndProc = (WNDPROC)WndProc;				// WndProc处理消息

	wc.cbClsExtra = 0;						// 无额外窗口数据

	wc.cbWndExtra = 0;						// 无额外窗口数据

	wc.hInstance = hInstance;					// 设置实例

	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			// 装入缺省图标

	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// 装入鼠标指针

	wc.hbrBackground = NULL;						// GL不需要背景

	wc.lpszMenuName = NULL;						// 不需要菜单

	wc.lpszClassName = "OpenG";					// 设定类名字

	
}

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0f, 1.0f, 0.0f);
	glRectf(-0.5f, -0.5f, 0.5f, 0.5f);
	glFlush();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 480);
	glutCreateWindow("First_GL!");
	glutDisplayFunc(myDisplay);
	glutMainLoop();
}