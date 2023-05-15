

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

#define TIMER_PERIOD  10 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532
#define MAX 5

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

// To store the coordinates of more than 1 target
typedef struct {
    int x, y;  //center of target
}figure_t;

// cannon ball global variables
figure_t cannon = { 0,300 };

// global variable for weapon 
figure_t weapon = { cannon.x,cannon.y};



// initial center of target.
figure_t target;

// creating structure Array to store 5 targets
figure_t targetArray[MAX];

// to make the targets move if it is true
bool weaponmove = false; 

// time global variable
int stime = 19;
int mstime = 59;

//vertical firing speed. constant
float Vo = 120;
float Vcar = 40;

// initial game start boolean
bool gameStart = false;
bool pauseGame = false;


//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

void DrawRoad()
{
    int i;
    glColor3ub(160, 160, 160);
    glRectf(-400, -280, 400, -80);
    for (i = -380; i < 400; i += 40)
    {
        glLineWidth(2);
        glColor3f(0, 0, 0);
        glBegin(GL_LINES);
        glVertex2f(i, -180);
        glVertex2f(i + 15, -180);
        glEnd();
    }

}


void InformativeMessage()
{
    glColor3ub(0, 0, 0);
    glRectf(-400, -300, 400, -270);

    //writing instructions for the players
    glColor3ub(0,255,255);
    vprint(-90, -285, GLUT_BITMAP_8_BY_13, "<space> Fire, <F1> Pause/Restart");
    glColor3ub(0, 0, 0);
    vprint(320, 280, GLUT_BITMAP_8_BY_13, "Remaining");
    glColor3ub(0, 0, 0);
    vprint(335, 265, GLUT_BITMAP_8_BY_13, "Time");
    glColor3ub(0, 0, 0);
    vprint(325, 250, GLUT_BITMAP_8_BY_13, "%d : %d",stime,mstime);

}



void drawWeapon()
{
    
    glColor3ub(0, 0, 0);
    circle(weapon.x, weapon.y - 15, 10);
    glColor3ub(96,96,96);
    circle(weapon.x+3, weapon.y - 10, 3);

}

void drawCannon()
{
    
    glColor3ub(153,76,0);
    glBegin(GL_POLYGON);
    glVertex2f(cannon.x + 30, cannon.y);
    glVertex2f(cannon.x - 30, cannon.y);
    glVertex2f(cannon.x - 25, cannon.y - 30);
    glVertex2f(cannon.x + 25, cannon.y-30);
    glEnd();

    glColor3ub(102,0, 0);
    glBegin(GL_POLYGON);
    glVertex2f(cannon.x + 15, cannon.y -15);
    glVertex2f(cannon.x - 15, cannon.y-15);
    glVertex2f(cannon.x - 12, cannon.y - 50);
    glVertex2f(cannon.x + 12, cannon.y - 50);
    glEnd();

    glColor3ub(0, 0, 0);
    circle(cannon.x, cannon.y - 15, 10);

}

void drawtarget(int xi, int yi)
{

    glColor3ub(255, 0, 127);
    glRectf(xi - 50, yi + 20, xi + 50, yi - 20);

    glColor3ub(128, 128, 128);
    circle(xi - 20, yi - 20, 12);
    glColor3ub(128, 128, 128);
    circle(xi + 20, yi - 20, 12);

    glColor3ub(0, 0, 0);
    circle(xi - 20, yi - 20, 5);
    glColor3ub(0, 0, 0);
    circle(xi + 20, yi - 20, 5);

    glColor3ub(255, 102, 178);
    glBegin(GL_POLYGON);
    glVertex2f(xi - 40, yi + 20);
    glVertex2f(xi + 40, yi + 20);
    glVertex2f(xi + 35, yi + 45);
    glVertex2f(xi - 35, yi + 45);
    glEnd();

}

void InitialSettings()
{
    targetArray[0].x = -460;
    targetArray[0].y = -110;

    targetArray[1].x = -620;
    targetArray[1].y = -180;

    targetArray[2].x = -790;
    targetArray[2].y = -150;

    targetArray[3].x = -940;
    targetArray[3].y = -240;

    targetArray[4].x = -1120;
    targetArray[4].y = -190;
}


//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(153 / 255.0, 204.0 / 255, 255.0 / 255, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    DrawRoad();
    InformativeMessage();
    drawWeapon();
    drawCannon();
    
   for(int i=0;i<MAX;i++)
       drawtarget(targetArray[i].x, targetArray[i].y);

   if (gameStart == false)
   {
       glColor3f(0, 0, 0);
       print(-150, 100, "-=- Left Click to Start Game -=-", GLUT_BITMAP_TIMES_ROMAN_24);

   }

   if (stime == 0)
   {
       glColor3f(0, 0, 0);
       print(-100, 100, "-=- GAME OVER -=-", GLUT_BITMAP_TIMES_ROMAN_24);
       print(-40, 50, "Press F1 to Restart...", GLUT_BITMAP_HELVETICA_12);
   }

    glutSwapBuffers();
}


//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    if (key == ' ')
        weaponmove = true;

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
    case GLUT_KEY_F1:
        pauseGame = !(pauseGame);
        if (stime == 0)
        {
            stime = 19;
            mstime = 59;
            pauseGame = false;
        }
        break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }


    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.
    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)
    {
        gameStart = true;
        if (stime == 0 || stime == 19)
            InitialSettings();
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}




//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}


void hitTarget()
{
    //this function will test if the cannon hit the cars or if the car is not hit and reaches the end of the window


    if ((((weapon.y - 5) < targetArray[0].y + 40) && ((weapon.y - 5) > targetArray[0].y - 20) && (weapon.x > targetArray[0].x - 50) && (weapon.x < targetArray[0].x + 50)) 
        || (targetArray[0].x > 390))
    {
        
        targetArray[0].x = -450;
        targetArray[0].y = -110;
        
    }
    if ((((weapon.y - 5) < targetArray[1].y + 40) && ((weapon.y - 5) > targetArray[1].y - 20) && (weapon.x > targetArray[1].x - 50) && (weapon.x < targetArray[1].x + 50))
        || (targetArray[1].x > 390))
    {
        
        targetArray[1].x = -620;
        targetArray[1].y = -180;
     
    }
    if ((((weapon.y - 5) < targetArray[2].y + 40) && ((weapon.y - 5) > targetArray[2].y - 20) && (weapon.x > targetArray[2].x - 50) && (weapon.x < targetArray[2].x + 50))
        || (targetArray[2].x > 390))
    {
        
        targetArray[2].x = -750;
        targetArray[2].y = -150;
       
    }
     if ((((weapon.y - 5) < targetArray[3].y + 40) && ((weapon.y - 5) > targetArray[3].y - 20) && (weapon.x > targetArray[3].x - 50) && (weapon.x < targetArray[3].x + 50))
         || (targetArray[3].x > 390))
    {
        targetArray[3].x = -940;
        targetArray[3].y = -240;
    
    }
     if ((((weapon.y - 5) < targetArray[4].y + 20) && ((weapon.y - 5) > targetArray[4].y - 20) && (weapon.x > targetArray[4].x - 50) && (weapon.x < targetArray[4].x + 50))
         || (targetArray[4].x > 390))
    {
        targetArray[4].x = -1050;
        targetArray[4].y = -190;
    }

}



void gameTime()
{
    mstime--;
    if (mstime == 0)
    {
        stime--;
        mstime = 59;
    }
    if (stime == 0)
    {
        mstime = 0;
        InitialSettings();
        weaponmove = false;
        weapon.x = cannon.x;
        weapon.y = cannon.y;
    }

      
}



#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.
    if (gameStart && stime >0 && !pauseGame) 
    {
        
        for (int i = 0; i < MAX; i++)
        {
            if (targetArray[i].x < 400)
            {
                targetArray[i].x += Vcar * TIMER_PERIOD / 1000 * 5;
            }
            hitTarget();

        }

        if (right && cannon.x < 280) {
            cannon.x += 10;
            if (!weaponmove)
                weapon.x = cannon.x;
        }

        if (left && cannon.x > -280) {
            cannon.x -= 10;
            if (!weaponmove)
                weapon.x = cannon.x;
        }

        if (weaponmove)
        {
            weapon.y -= Vo * TIMER_PERIOD / 1000 * 5;

            if (weapon.y < -290)
            {
                weaponmove = false;
                weapon.x = cannon.x;
                weapon.y = cannon.y;
            }
        }  
        gameTime();

    }

    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(200, 30);
    glutCreateWindow("Ayesha Nizam");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}