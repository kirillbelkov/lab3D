#include <windows.h>
#include <gl/gl.h>
#include <math.h>
#include <iostream>

#include "camera.h"
#include "light.h"

using namespace std;

bool isPlaying = true;

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

float vert[] = {1,1,0, 1,-1,0, -1,-1,0, -1,1,0};
float bulb[] = {1,1,10, 1,-1,10, -1,-1,10, -1,1,10};

void ShowWorld(){
    glNormal3f(0,0,1);

    glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &vert);
        for (int i = -5; i < 5; i++){
            for(int j = -5; j < 5; j++){
                glPushMatrix();
                    if((i+j)%2 == 0) glColor3f(0.4,0.8,0);
                    else glColor3f(0.2,0.4,0);
                    glTranslatef(i*2, j*2, 0);
                    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                glPopMatrix();
            }
        }
    glDisableClientState(GL_VERTEX_ARRAY);
}

void drawPrisma(float weight, float height, int storon, float x, float y, float z, float transparency) {
    float angle = (2 * M_PI) / storon;

    glPushMatrix();

        glTranslatef(x, y, z);
        glEnable(GL_NORMALIZE);
        for (int i = 0; i < storon; ++i) {
            GLfloat mat_diffuse[] = { 1.0f, 1.0f, 1.0f, transparency };
            GLfloat mat_ambient[] = { 1.0f, 1.0f, 1.0f, transparency };
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
            glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
            glColor4f(0.7, 0.2, 0.1, transparency);

            float x1 = (weight * cos(i * angle));
            float y1 = (weight * sin(i * angle));

            float x2 = (weight * cos((i + 1) * angle));
            float y2 = (weight * sin((i + 1) * angle));

            glBegin(GL_TRIANGLES);
            // ������� ����������� 1
            glNormal3f(x1, y1, 0);
            glVertex3f(x1*1.5, y1*1.5, 0);
            glNormal3f(x2, y2, 0);
            glVertex3f(x2*1.5, y2*1.5, 0);
            glNormal3f(x1, y1, 0);
            glVertex3f(x1/1.5, y1/1.5, height);
            glEnd();

            glBegin(GL_TRIANGLES);
            // ������� ����������� 2
            glNormal3f(x1, y1, 0);
            glVertex3f(x1/1.5, y1/1.5, height);
            glNormal3f(x2, y2, 0);
            glVertex3f(x2*1.5, y2*1.5, 0);
            glNormal3f(x2, y2, 0);
            glVertex3f(x2/1.5, y2/1.5, height);
            glEnd();

            glBegin(GL_TRIANGLES);
            // ���
            glNormal3f(0.0f, 0.0f, 1.0f);
            glVertex3f(0.0f, 0.0f, 0);
            glVertex3f(x2*1.5, y2*1.5, 0);
            glVertex3f(x1*1.5, y1*1.5, 0);
            glEnd();

            glBegin(GL_TRIANGLES);
            // ����
            glNormal3f(0.0f, 0.0f, 1.0f);
            glVertex3f(0.0f, 0.0f, height);
            glVertex3f(x1/1.5, y1/1.5, height);
            glVertex3f(x2/1.5, y2/1.5, height);
            glEnd();

            glColor4f(1, 1, 1, transparency);

            glBegin(GL_LINE_LOOP);
            // ������� ����������� 1
            glVertex3f(x1*1.5, y1*1.5, 0);
            glVertex3f(x2*1.5, y2*1.5, 0);
            glVertex3f(x1/1.5, y1/1.5, height);
            glEnd();

            glBegin(GL_LINE_LOOP);
            // ������� ����������� 2
            glVertex3f(x1/1.5, y1/1.5, height);
            glVertex3f(x2*1.5, y2*1.5, 0);
            glVertex3f(x2/1.5, y2/1.5, height);
            glEnd();


            glBegin(GL_LINE_LOOP);
            // ���
            glVertex3f(0.0f, 0.0f, 0);
            glVertex3f(x2*1.5, y2*1.5, 0);
            glVertex3f(x1*1.5, y1*1.5, 0);
            glEnd();

            glBegin(GL_LINE_LOOP);
            // ����
            glVertex3f(0.0f, 0.0f, height);
            glVertex3f(x1/1.5, y1/1.5, height);
            glVertex3f(x2/1.5, y2/1.5, height);
            glEnd();

        }

    glPopMatrix();
}

void DrawLine(){
    glDisable(GL_LIGHTING);
    glLineWidth(2);
    glBegin(GL_LINES);
        glColor3f(1,0,0);
        glVertex3f(1,1,1);
        glVertex3f(1,1,10);

        glColor3f(0,1,0);
        glVertex3f(1,1,1);
        glVertex3f(1,10,1);

        glColor3f(0,0,1);
        glVertex3f(1,1,1);
        glVertex3f(10,1,1);
    glEnd();
    glEnable(GL_LIGHTING);
}

void draw(){
    glDisable(GL_LIGHTING);
    glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &bulb);
        glDrawArrays(GL_QUADS, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glEnable(GL_LIGHTING);
}

void MoveCamera() {
    Camera_MoveDirectional(
        GetKeyState('W') < 0 ? 1 : GetKeyState('S') < 0 ? -1 : 0,
        GetKeyState('D') < 0 ? 1 : GetKeyState('A') < 0 ? -1 : 0,
        0.1);
    Camera_AutoMoveByMouse(400, 400, 0.1);
}

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
    float theta = 1.0f;
    float radius = 3;
    float bulb_x;
    float bulb_y;
    float angle;
    int cnt;
    float prism_x;
    float prism_y;
    float transparency;

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

    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          1280,
                          720,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    ShowCursor(FALSE);

    EnableOpenGL(hwnd, &hDC, &hRC);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-2,2, -2,2, 2,80);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);


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

            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glPushMatrix();
            if (isPlaying && GetForegroundWindow() == hwnd) {
                    MoveCamera();
            }
                Camera_Apply();
                ShowWorld();
                cnt = 8;
                transparency = 1.0;
                glPushMatrix();
                    glTranslatef(-1,-1,0.1);
                    DrawLine();

                    for (int i = 0; i <= cnt; i++)
                    {
                        prism_x = cos((2*M_PI/cnt)*i)*5;
                        prism_y = sin((2*M_PI/cnt)*i)*5;
                        drawPrisma(1,1.5,15,prism_x,prism_y,0,transparency);
                        transparency -= 0.1;
                    }

                glPopMatrix();

                glPushMatrix();
                    bulb_x = radius * cos(theta*M_PI/180);
                    bulb_y = radius * sin(theta*M_PI/180);

                    glPushMatrix();
                    glTranslatef(bulb_x, bulb_y, 0);
                    angle = atan2(bulb_y, bulb_x) * 180 / M_PI + 90;
                    glRotatef(angle, 0,0,1);
                    glRotatef(40, 1, 0, 0);
                    on_light();
                    glColor3f(1,1,1);
                    draw();
                    glPopMatrix();
                glPopMatrix();

            glPopMatrix();

            SwapBuffers(hDC);

            Sleep (1);
            theta++;
        }
    }

    DisableOpenGL(hwnd, hDC, hRC);


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

        case WM_MOUSEMOVE:

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

    *hDC = GetDC(hwnd);

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

    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

