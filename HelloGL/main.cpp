#include <Windows.h>
#include <glew.h>
#include <glut.h>  

HGLRC           hRC = NULL;							// ������ɫ��������

HDC             hDC = NULL;							// OpenGL��Ⱦ��������

HWND            hWnd = NULL;							// �������ǵĴ��ھ��

HINSTANCE       hInstance;							// ��������ʵ��

bool	keys[256];								// ������̰���������

bool	active = TRUE;								// ���ڵĻ��־��ȱʡΪTRUE

bool	fullscreen = TRUE;							// ȫ����־ȱʡ��ȱʡ�趨��ȫ��ģʽ

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// ����������
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)
{
	// ����height��ֹ���
	if (height == 0) 
	{
		height = 1;
	}
	// ���õ�ǰ����
	glViewport(0, 0, width, height);
	// ��֪OpenGL��������ͶӰ����
	glMatrixMode(GL_PROJECTION);
	// ����ͶӰ����
	glLoadIdentity();

	// ���ô��ڴ�С
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

// ��ʼ����
int InitGL(GLvoid) 
{
	// ������Ӱƽ��
	glShadeModel(GL_SMOOTH);
	// ������Ļ
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// ������Ȼ���
	glClearDepth(1.0f);
	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);
	// ������Ȳ��Ե����� С�ڻ������ͨ������
	glDepthFunc(GL_LEQUAL);
	// ָ��GL������õ�ͶӰ����
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	return TRUE;

}

// ʵ����Ļ������
int DrawGLScene(GLvoid)
{
	// ������Ļ����Ȼ���
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// ���õ�ǰ��ģ�͹۲����
	glLoadIdentity();
	return TRUE;
}

// �����Ƴ�ǰ���ã�Ӧһ���ͷ���ɫ�������豸���������ڵĴ�����
GLvoid KillGLWindow(GLvoid)
{
	// ����Ƿ�ȫ��������ǵĻ�������ʾϵͳ����������ķ�ʽ��ʾ
	if (fullscreen) 
	{
		// ʹ��Ĭ�ϵĲ�����ظ�ԭʼ�ֱ��ʲ�ʹ������¿ɼ�
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);

	}

	// ��������Ƿ�ӵ����ɫ������
	if (hRC) 
	{
		// �����ͷ� ��RC��DC����
		if (!wglMakeCurrent(NULL, NULL))
		{
			MessageBox(NULL, TEXT("�ͷ�DC��RCʧ�ܡ�"), TEXT("�رմ���"), MB_OK | MB_ICONINFORMATION);
		}

		// ����ɾ��RC
		if (!wglDeleteContext(hRC)) 
		{
			MessageBox(NULL, TEXT("�ͷ�RCʧ�ܡ�"), TEXT("�رմ���"), MB_OK | MB_ICONINFORMATION);
		}

		hRC = NULL;

	}
	// �鿴DC�Ƿ���ڲ������ͷ�
	if (hDC && !ReleaseDC(hWnd, hDC))
	{
		MessageBox(NULL, TEXT("�ͷ�DCʧ�ܡ�"), TEXT("�رմ���"), MB_OK | MB_ICONINFORMATION);

		hDC = NULL;
	}
	// ע��������
	if (!UnregisterClass(TEXT("OPenG"), hInstance)) 
	{
		MessageBox(NULL, "����ע�������ࡣ", "�رմ���", MB_OK | MB_ICONINFORMATION);

		hInstance = NULL;
	}
}

// ����GL����
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullScreenFlag)
{
	// ���ظ�ʽ
	GLuint PixelFormat;
	// ������ṹ
	WNDCLASS wc;
	// ��չ�Ĵ��ڵķ����Ϣ
	DWORD dwExStylr;
	// ���ڷ��
	DWORD dwStyle;
	// ���ھ������
	RECT WindowRect;							// ȡ�þ��ε����ϽǺ����½ǵ�����ֵ

	WindowRect.left = (long)0;						// ��Left   ��Ϊ 0

	WindowRect.right = (long)width;						// ��Right  ��ΪҪ��Ŀ��

	WindowRect.top = (long)0;							// ��Top    ��Ϊ 0

	WindowRect.bottom = (long)height;						// ��Bottom ��ΪҪ��ĸ߶�

	fullscreen = fullScreenFlag;						// ����ȫ��ȫ����־

	// ȡ�ô���ʵ��
	hInstance = GetModuleHandle(NULL);
	// �ƶ�ʱ�ػ棬Ϊ���ڴ���˽��DC 
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;		// �ƶ�ʱ�ػ�����Ϊ����ȡ��DC

	wc.lpfnWndProc = (WNDPROC)WndProc;				// WndProc������Ϣ

	wc.cbClsExtra = 0;						// �޶��ⴰ������

	wc.cbWndExtra = 0;						// �޶��ⴰ������

	wc.hInstance = hInstance;					// ����ʵ��

	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			// װ��ȱʡͼ��

	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// װ�����ָ��

	wc.hbrBackground = NULL;						// GL����Ҫ����

	wc.lpszMenuName = NULL;						// ����Ҫ�˵�

	wc.lpszClassName = "OpenG";					// �趨������

	
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