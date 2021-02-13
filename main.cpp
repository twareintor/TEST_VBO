/**************************
 * Includes
 *
 **************************/

#include <windows.h>
// application specific:
#include <fstream>
#include <gl/gl.h>
#include <string.h>
// #include <gl/glu.h>					// 

#include <gl/glext.h>					// 

////////////////////////////////////////////////////////////////////////////////////////////////////

PFNGLGENBUFFERSPROC 				glGenBuffers;
PFNGLBINDBUFFERPROC 				glBindBuffer;
PFNGLBUFFERDATAPROC 				glBufferData;
PFNGLVERTEXATTRIBPOINTERPROC 		glVertexAttribPointer;
PFNGLENABLEVERTEXATTRIBARRAYPROC 	glEnableVertexAttribArray;

PFNGLCREATESHADERPROC 				glCreateShader;
PFNGLSHADERSOURCEPROC 				glShaderSource;
PFNGLCOMPILESHADERPROC 				glCompileShader;
PFNGLCREATEPROGRAMPROC 				glCreateProgram;
PFNGLATTACHSHADERPROC 				glAttachShader;
PFNGLBINDATTRIBLOCATIONPROC 		glBindAttribLocation;
PFNGLLINKPROGRAMPROC 				glLinkProgram;
PFNGLUSEPROGRAMPROC 				glUseProgram;
PFNGLGETSHADERINFOLOGPROC			glGetShaderInfoLog;

void *GetAnyGLFuncAddress(const char *name);


void InitVBO(void);


int TestVBO(void);

int TestShaders(void);

const char* filetobuf(const char* szFNm);

void buftofile(const char* szFNm, const char* szBuf);



/**************************
 * Function Declarations
 *
 **************************/

LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
WPARAM wParam, LPARAM lParam);
void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC);
void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC);


/**************************
 * WinMain
 *
 **************************/

int WINAPI WinMain (HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine,
                    int iCmdShow)
{
    WNDCLASS wc;
    HWND hWnd;
    HDC hDC;
    HGLRC hRC;        
    MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;

    /* register window class */
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "GLSample";
    RegisterClass (&wc);

    /* create main window */
    hWnd = CreateWindow (
      "GLSample", "OpenGL Sample", 
      WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
      20, 20, 256, 256,
      NULL, NULL, hInstance, NULL);

    /* enable OpenGL for the window */
    EnableOpenGL (hWnd, &hDC, &hRC);

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage (&msg);
                DispatchMessage (&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */
            TestVBO();
            SwapBuffers(hDC);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL (hWnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow (hWnd);

    return msg.wParam;
}


/********************
 * Window Procedure
 *
 ********************/

LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
                          WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_CREATE:
        return 0;
    case WM_CLOSE:
        PostQuitMessage (0);
        return 0;

    case WM_DESTROY:
        return 0;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            return 0;
        }
        return 0;

    default:
        return DefWindowProc (hWnd, message, wParam, lParam);
    }
}


/*******************
 * Enable OpenGL
 *
 *******************/

void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC)
{
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC (hWnd);

    /* set the pixel format for the DC */
    ZeroMemory (&pfd, sizeof (pfd));
    pfd.nSize = sizeof (pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | 
      PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    iFormat = ChoosePixelFormat (*hDC, &pfd);
    SetPixelFormat (*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext( *hDC );
    wglMakeCurrent( *hDC, *hRC );

}


/******************
 * Disable OpenGL
 *
 ******************/

void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC)
{	
    wglMakeCurrent (NULL, NULL);
    wglDeleteContext (hRC);
    ReleaseDC (hWnd, hDC);
}


#include "testvbo.h"


int TestVBO(void)
{
	InitVBO();
	TestShaders();
	return 0;
}


void *GetAnyGLFuncAddress(const char *name)
{
  void *p = (void *)wglGetProcAddress(name);
  if(p == 0 ||
    (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) ||
    (p == (void*)-1) )
  {
    HMODULE module = LoadLibraryA("opengl32.dll");
    p = (void *)GetProcAddress(module, name);
  }

  return p;
}


void InitVBO(void)
{
	// // // // // // // // // // // // // // // // //
	
	HMODULE module = LoadLibraryA("opengl32.dll");
	
	glGenBuffers = 				(PFNGLGENBUFFERSPROC)				GetAnyGLFuncAddress("glGenBuffers");
	glBindBuffer = 				(PFNGLBINDBUFFERPROC)				GetAnyGLFuncAddress("glBindBuffer");
	glBufferData = 				(PFNGLBUFFERDATAPROC)				GetAnyGLFuncAddress("glBufferData");
	glVertexAttribPointer = 	(PFNGLVERTEXATTRIBPOINTERPROC)		GetAnyGLFuncAddress("glVertexAttribPointer");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)	GetAnyGLFuncAddress("glEnableVertexAttribArray");
	
	glCreateShader = 			(PFNGLCREATESHADERPROC)				GetAnyGLFuncAddress("glCreateShader");
	glShaderSource = 			(PFNGLSHADERSOURCEPROC)				GetAnyGLFuncAddress("glShaderSource");
	glCompileShader = 			(PFNGLCOMPILESHADERPROC)			GetAnyGLFuncAddress("glCompileShader");
	glCreateProgram = 			(PFNGLCREATEPROGRAMPROC)			GetAnyGLFuncAddress("glCreateProgram");
	glAttachShader = 			(PFNGLATTACHSHADERPROC)				GetAnyGLFuncAddress("glAttachShader");
	glBindAttribLocation = 		(PFNGLBINDATTRIBLOCATIONPROC)		GetAnyGLFuncAddress("glBindAttribLocation");
	glLinkProgram = 			(PFNGLLINKPROGRAMPROC)				GetAnyGLFuncAddress("glLinkProgram");
	glUseProgram = 				(PFNGLUSEPROGRAMPROC)				GetAnyGLFuncAddress("glUseProgram");

	glGetShaderInfoLog = 		(PFNGLGETSHADERINFOLOGPROC)			GetAnyGLFuncAddress("glGetShaderInfoLog");

	return;
}
	

int TestShaders(void)
{
	GLuint triangleVBO;
	GLuint shaderProgram;
	char* vertexSource = (char*)GlobalAlloc(GPTR, 0x400); 
	char* fragmentSource = (char*)GlobalAlloc(GPTR, 0x400);
	GLuint vertexShader, fragmentShader;
	const unsigned int shaderAttribute = 0;
    GLfloat data[6][3] = {
                           { -0.5, -1.0,  0.0  },
                           {  0.0,  1.0,  0.0  },
                           {  0.5, -1.0,  0.0  },
                           {  1.0,  1.0,  0.0  },
                           { -2.0, -0.7,  1.0  },
                           {  1.2, -0.3,  0.0  }
                       };
    GLuint idx[4][3] = {
                           { 0,  1,  2 },
                           { 1,  2,  3 },
                           { 2,  3,  4 },
                           { 3,  4,  5 }
                        };
	

	/*---------------------- Initialise VBO - (Note: do only once, at start of program) ---------------------*/
	glGenBuffers(1, &triangleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
	glVertexAttribPointer(shaderAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(shaderAttribute);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	/*-------------------------------------------------------------------------------------------------------*/

	/*--------------------- Load Vertex and Fragment shaders from files and compile them --------------------*/
	strcat(vertexSource, filetobuf("..\\_shaders\\Shader001.vert"));
	strcat(fragmentSource, filetobuf("..\\_shaders\\Shader001.frag"));
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	GlobalFree(vertexSource);
	GlobalFree(fragmentSource);
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);
	/*-------------------------------------------------------------------------------------------------------*/

	/*-------------------- Create shader program, attach shaders to it and then link it ---------------------*/
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindAttribLocation(shaderProgram, shaderAttribute, "in_Position");
	glLinkProgram(shaderProgram);
	/*-------------------------------------------------------------------------------------------------------*/

	glUseProgram(shaderProgram);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, (sizeof(data) / 3) / sizeof(GLfloat));
	// glDrawElements(GL_TRIANGLES, sizeof(idx)/sizeof(unsigned int), GL_UNSIGNED_INT, idx);

	/*------------------------------- Info log --------------------------------------------------------------*/
	int nLen;
	char* szInfoLog = (char*)GlobalAlloc(GPTR, 0x400);
	glGetShaderInfoLog(vertexShader, 0x400, &nLen, (GLchar*)szInfoLog);
	buftofile("..\\_logs\\logv.txt", szInfoLog);
	ZeroMemory(szInfoLog, 0x400);
	glGetShaderInfoLog(fragmentShader, 0x400, &nLen, (GLchar*)szInfoLog);
	buftofile("..\\_logs\\logf.txt", szInfoLog);
	ZeroMemory(szInfoLog, 0x400);
	GlobalFree(szInfoLog);
	/*------------------------------- ------- --------------------------------------------------------------*/
	
	return 0;
}


const char* filetobuf(const char* szFNm)
{
	std::fstream fs(szFNm);
	std::string strBuf;
	std::string strLin;
	while(getline(fs, strLin))
	{
		strBuf+=strLin;
		strBuf+=std::string("\n");
	}
	fs.close();
	return strBuf.c_str();

}


void buftofile(const char* szFNm, const char* szBuf)
{
	std::ofstream fs(szFNm);
	if(fs.is_open())
	{
		fs<<szBuf;
		fs.close();

	}
	return;
}




