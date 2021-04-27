#include <windows.h>
#include <stdio.h>
#include <gl/gl.h>
#include <math.h>
#define SIZE 10

int width = 700;

int height = 500;

double koef;

double gravity = 0.0011;
//double x [4] = {1,2,3,4};


///Making particle as a circle from triangles
/// @see TParticle_Show()

void DrawCircle(int cnt)
{
    double x;
    double y;

    double da = M_PI * 2/cnt;

    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(0,0);

    for(int i = 0; i <= cnt; i++)
    {
        x = sin(da*i);

        y =cos(da*i);

        glVertex2f(x,y);
    }

    glEnd();
}
///SRTUCTERE of the particle

typedef struct
{
    double x;

    double y;

    double dx;

    double dy;

    double r;

    double angular_speed;


}TParticle;


TParticle particle[SIZE];




//====================3=====================INITIALIZATION

/// Initialization
///
/// @param 1 is obj of the structure
/// @param 2 is x coord of the obj
/// @param 3 is y coord of the obj
/// @param 4 is X speed of the obj
/// @param 5 is Y speed of the obj
/// @param 6 is  radius of the obj
/// @see Init()
void TParticle_Init(TParticle *obj, double x1, double y1, double dx1, double dy1, double r1)
{

    obj->x = x1;

    obj->y = y1;

    obj->dx = dx1;

    obj->dy = dy1;

    obj->r = r1;


}

double uniform()
{
    return (double) rand() / RAND_MAX;
}

//=====================4====================INITIALIZATION
/// initialisation of the obj
/// parameters are going to TParticle_Show()
/// @see TParticle_Show()
void Init()
{
    for (int i = 0; i<SIZE; i++)
    {
        //printf("%f", uniform());
        TParticle_Init(&particle[i],uniform(),uniform(),uniform(),0,0.03);
    }

}

//=====================5====================

void TParticle_Show(TParticle obj)
    {
         glColor3f(1, 0.7, 0);
      glPushMatrix();

      glTranslatef(obj.x, obj.y, 0);

      glScalef(obj.r, obj.r, 1);

      DrawCircle(30);

      glPopMatrix();

    }
//======================6===================PARTICLE SHOW



///PHYSICS OF THE PARTICLE
/// @param 1 obj of the structure
/// @param 2  left wall
/// @param 3  right wall
/// @see gravity

void Particle_Fys(TParticle *obj, double wl1, double wl2)
{
    //collision detection
    double Py = 0;
    double Px = 0;

    Py-= gravity;
    Py -=  obj->dy * 0.01;

    Px -= obj->dx * 0.01;
    //
   if(obj->y - obj->r < -1) // the floor
        {
            obj->y = -1 + obj->r;
            Py -= obj->dy;
            obj->dy*=0.1;

        }


    if(obj->x < wl1 + obj->r) //left wall
    {
        obj->x = wl1 + obj->r ;
        Px  -= obj->dx;
        obj->dx*=0.1;

    }


    if(obj->x > wl2 - obj->r)
    {
        obj->x = wl2 - obj->r; // right wall
        Px  -= obj->dx;
    }

    obj->dx += Px;
    obj->dy += Py;


        //integration
    obj->x += obj->dx;
    obj->y += obj->dy;



}
void Particle_Fys2(TParticle *obj)
{
    double index = 0;
    for(int i = 0; i < SIZE; i++)

    {
      index = particle+i   ;





    }





}
//====================================================================================================


void Simulation_Show()
{
    for(int i = 0; i<SIZE; i++)
    {
        Particle_Fys(&particle[i],-koef, koef);
        TParticle_Show(particle[i]);
        glEnd();
    }
}


LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);


int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;
    double theta = 0.0f;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          width,
                          height,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);
    ///
    koef = ((double)width/height);

    glScalef(1/koef,1,1);

    Init();

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */

            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            Simulation_Show();

            SwapBuffers(hDC);


            Sleep (1);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

