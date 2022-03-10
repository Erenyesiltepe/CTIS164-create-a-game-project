/*********
   CTIS164 - Template Source Program
----------
STUDENT Eren Yeþiltepe
SECTION :2
HOMEWORK: 1
----------
PROBLEMS:
----------
ADDITIONAL FEATURES:
1)When the program started it shows an instuctions screen until you click somewhere and set truck position
2)Clouds are randomly created every time the program executed
3)Sky color changes while you are playing
4)Truck spins when direction changes
5)In manual mode when truck is higher than ground it falls
6)Manual mode like a game and shows balls that you can jump on them. If they hits you the program ends and shows a game over screen
that shows the last level you passed
7)You can enable and disable gravity in manual mode, if you enable the game starts, if you disable you can travel around freely
*********/


//baþtaki boþ kýsma instructions koy code cleanup and submit


#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT 720

#define TIMER_PERIOD  50 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer


#define D2R 0.0174532

#define TRspeed 12 //truck speed setter


//direction constants
#define Dright 1
#define Dleft -1
#define Dup 2
#define Ddown -2



int groundTop = -280;

double skytime = 0;//changes sky and cloud colors
double cloudPlaces[10]; //array that keeps cloud x values

double grtime = 0;// keeps time to calculate gravity force


//Truck position variables
int TRPx = 0, TRPy = 200, TRdir = 1, TRYdir = 0;


bool jumpPressed = false;

int truckdisp = 0;

int GAMEmode = -1; //-1 manual 1 auto

int level = 1; //levels in manual mode

int win = 0; // 0: nothing 1:lose
bool graDis = true; // shows if gravity is on or off




/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

int enemy[20] = { 650,650 };//enemy array includes their x positions



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

void bolts(int x, int y) {
    glColor3f(0.9, 0.9, 0.9);

    int dist = 8;

    glBegin(GL_POLYGON);
    glVertex2f(x - dist, y + dist);
    glVertex2f(x + 6 - dist, y + dist);
    glVertex2f(x + 9 - dist, y - 5 + dist);
    glVertex2f(x + 6 - dist, y - 10 + dist);
    glVertex2f(x - dist, y - 10 + dist);
    glVertex2f(x - 3 - dist, y - 5 + dist);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(x + dist, y + dist);
    glVertex2f(x + 6 + dist, y + dist);
    glVertex2f(x + 9 + dist, y - 5 + dist);
    glVertex2f(x + 6 + dist, y - 10 + dist);
    glVertex2f(x + dist, y - 10 + dist);
    glVertex2f(x - 3 + dist, y - 5 + dist);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(x + dist, y - dist);
    glVertex2f(x + 6 + dist, y - dist);
    glVertex2f(x + 9 + dist, y - 5 - dist);
    glVertex2f(x + 6 + dist, y - 10 - dist);
    glVertex2f(x + dist, y - 10 - dist);
    glVertex2f(x - 3 + dist, y - 5 - dist);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(x - dist, y - dist);
    glVertex2f(x + 6 - dist, y - dist);
    glVertex2f(x + 9 - dist, y - 5 - dist);
    glVertex2f(x + 6 - dist, y - 10 - dist);
    glVertex2f(x - dist, y - 10 - dist);
    glVertex2f(x - 3 - dist, y - 5 - dist);
    glEnd();





}

void clouds()
{

    glColor3f(1 - skytime * 2 / 3, 1 - skytime * 2 / 3, 1 - skytime * 2 / 3);

    int x, y;
    for (int i = 0; i < 10; i += 2)
    {
        x = cloudPlaces[i];
        y = cloudPlaces[i + 1];

        circle(x, y, 40);
        circle(x - 30, y + 8, 25);
        circle(x + 25, y - 2, 30);
    }
}

//include all the clouds, name, surname, id, direction
void background(void) {
    //plain blue background changes color according to time
    glColor3f(252. / 255 - skytime / 3, 163. / 255 - skytime / 3, 3. / 255 - skytime / 3);
    glRectf(-600, groundTop, 600, -360);


    clouds();

    //statistics screen and background
    glColor3f(50. / 255, 157. / 255, 168. / 255);
    glRectf(10 - winWidth / 2, winHeight / 2 - 10, 300 - winWidth / 2, winHeight / 2 - 120);
    glColor3f(1, 1, 1);
    vprint2(30 - winWidth / 2, winHeight / 2 - 30, 0.1, "Eren Yesiltepe-22002527");



}

void truck(int trX, int trY, int dir)
{
    double size = 4;

    //backside
    glColor3ub(252, 186, 3);
    glRectf(trX - 11 * size * dir, trY + 7 * size, trX + 4 * size * dir, trY + 25 * size);

    //middle and right rectangles
    glColor3ub(126, 93, 2);
    glRectf(trX - 4 * size * dir, trY + 5.5 * size, trX + 3.6 * size * dir, trY + 13.5 * size);

    glRectf(trX - 1 * size * dir, trY + 6 * size, trX + 12 * size * dir, trY + 19 * size);

    glColor3ub(126, 93, 2);
    float angle;

    glBegin(GL_POLYGON);
    for (int i = 0; i < 25; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(trX + 10 * size * dir + 12 * size * dir * cos(angle), trY + 7 * size + 12 * size * sin(angle));
    }
    glVertex2f(trX + 11 * size * dir, trY + 7 * size);
    glEnd();


    //wheels
    glColor3f(0, 0, 0);
    circle(trX + 14 * size * dir, trY + 6 * size, 8 * size);
    circle(trX - 11 * size * dir, trY + 7 * size, 9 * size);

    //Wheel centers
    glColor3ub(5, 114, 98);
    circle(trX + 14 * size * dir, trY + 6 * size, 4 * size);
    circle(trX - 11 * size * dir, trY + 7 * size, 4.5 * size);

    //bolts
    bolts(trX - 0.5 + 14 * size * dir, trY + 6.5 * size);
    bolts(trX - 0.5 - 11 * size * dir, trY + 7.5 * size);

    //steering wheel holder
    glColor3f(0, 0, 0);
    glBegin(GL_QUAD_STRIP);
    glVertex2f(trX - 1 * size * dir, trY + 18 * size);
    glVertex2f(trX + 0 * size * dir, trY + 18 * size);
    glVertex2f(trX - 1.5 * size * dir, trY + 19 * size);
    glVertex2f(trX - 2 * size * dir, trY + 19 * size);
    glEnd();

    //steering wheel
    glColor3ub(3, 119, 52);
    circle(trX - 3 * size * dir, trY + 20 * size, 2 * size);

    //The black strip
    glColor3f(0, 0, 0);

    glBegin(GL_POLYGON);

    glVertex2f(trX - 11 * size * dir, trY + 16 * size);
    glVertex2f(trX - 9 * size * dir, trY + 16 * size);

    glVertex2f(trX - 9 * size * dir, trY + 25 * size);

    glVertex2f(trX + 4 * size * dir, trY + 25 * size);
    glVertex2f(trX + 4 * size * dir, trY + 27 * size);

    glVertex2f(trX - 11 * size * dir, trY + 27 * size);


    glEnd();


}



//
// To display onto window using OpenGL commands
//
void display() {


    glClearColor(3. / 255 - skytime, 227. / 255 - skytime, 252. / 255 - skytime, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    //shows fixed background
    background();


    if (win == 0)
    {
        if (TRYdir == Dleft)
            vprint2(30 - winWidth / 2, winHeight / 2 - 75, 0.1, "Direction: left");
        else if (TRYdir == Dright)
            vprint2(30 - winWidth / 2, winHeight / 2 - 75, 0.1, "Direction: right");
        else if (TRYdir == Dup)
            vprint2(30 - winWidth / 2, winHeight / 2 - 75, 0.1, "Direction: up");
        else if (TRYdir == Ddown)
            vprint2(30 - winWidth / 2, winHeight / 2 - 75, 0.1, "Direction: down");
        else
            vprint2(30 - winWidth / 2, winHeight / 2 - 75, 0.1, "Direction: Not Set");

        //set display of truck
        if (truckdisp == 1)
            truck(TRPx, TRPy, TRdir);

        //shows instuctions page until truck is displayed
        else
        {
            glColor3f(50. / 255, 157. / 255, 168. / 255);

            glRectf(-winWidth / 2, winHeight / 4, winWidth / 2, -winHeight / 4);

            glColor3f(1, 1, 1);

            vprint2(-240, 110, 0.2, "Instructions: ");
            vprint2(-550, 80, 0.2, "F1 :changes game mode (manual or automathic) ");
            vprint2(-550, 50, 0.2, "In automathic mode: you can change trucks direction by pressing arrow keys. ");

            vprint2(-550, 20, 0.2, "In manual mode, you can play the game or travel around ");
            vprint2(-540, -10, 0.2, "You can enable or disable gravity by pressing g ");
            vprint2(-540, -40, 0.2, "If gravity enabled, play the game if not travel around ");
            vprint2(-540, -70, 0.2, "After the game ends the program shows the level you failed ");

        }


        glColor3f(1, 1, 1);
        if (GAMEmode == -1)
            vprint2(30 - winWidth / 2, winHeight / 2 - 50, 0.1, "Mode: MANUAL");
        else
            vprint2(30 - winWidth / 2, winHeight / 2 - 50, 0.1, "Mode: AUTOMATIC");


        if (GAMEmode == -1)
        {
            glColor3f(1, 1, 1);

            //display statistics in manual mode and show enemy and set level
            if (graDis)
                vprint2(30 - winWidth / 2, winHeight / 2 - 90, 0.1, "Gravity: On (change by pressing g)");
            else
                vprint2(30 - winWidth / 2, winHeight / 2 - 90, 0.1, "Gravity: Off (change by pressing g)");

            vprint2(30 - winWidth / 2, winHeight / 2 - 105, 0.1, "Level: %d", level);

            //enemy settings
            if (truckdisp == 1 && graDis)
            {
                glColor3f(219 / 255., 52 / 255., 235 / 255.);
                for (int i = 0; i < level; i++)
                    circle(enemy[i], groundTop + 5, 15);

                //only set enemy 
                if (enemy[level - 1] < -winWidth / 2) {
                    level++;
                    for (int k = 0; k < level; k++)
                        enemy[k] = winWidth / 2 + 10 + k * 4 * (100);

                }

            }


        }



    }// When the game ends
    else {
        glColor3f(50. / 255, 157. / 255, 168. / 255);

        glRectf(-winWidth / 2, winHeight / 4, winWidth / 2, -winHeight / 4);

        glColor3f(1, 1, 1);

        vprint2(-120, 50, 0.3, "GAME OVER");

        vprint2(-300, 0, 0.3, "You have reached the level %d", level);

        vprint2(-500, -50, 0.3, "To continue to play, please restart the program :)");

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

    if (key == 32)
        jumpPressed = true;

    //sets gravity value
    if (key == 'g') {
        if (graDis)
            graDis = false;
        else
            graDis = true;



    }
   

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
    if (GAMEmode == -1)
    {
        //moves the truck
        if (key == GLUT_KEY_LEFT && TRPx > 60 - winWidth / 2)
        {
            left = false;
            TRPx -= TRspeed;
            TRdir = Dleft;
            TRYdir = Dleft;
        }
        else if (key == GLUT_KEY_RIGHT && TRPx < winWidth / 2 - 60)
        {
            right = false;
            TRPx += TRspeed;
            TRdir = Dright;
            TRYdir = Dright;
        }
        else if (key == GLUT_KEY_UP) {
            up = false;
            TRPy += TRspeed;
            TRYdir = Dup;


        }
        else if (key == GLUT_KEY_DOWN) {
            down = false;
            TRPy -= TRspeed;
            TRYdir = Ddown;


        }




    }
    else//change direction of truck in auto mode
        switch (key) {

        case GLUT_KEY_LEFT: left = false;
            TRdir = Dleft;
            TRYdir = Dleft;
            break;
        case GLUT_KEY_RIGHT: right = false;
            TRdir = Dright;
            TRYdir = Dright;
            break;
        case GLUT_KEY_DOWN:
            TRYdir = Ddown;
            break;
        case GLUT_KEY_UP:
            TRYdir = Dup;
            break;
        }


    //change game mode
    if (key == GLUT_KEY_F1)
        GAMEmode *= -1;



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

    //gets mouse position and prevents us from clicking outside
    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN && x < winWidth - 70 && x>70) {
        truckdisp = 1;
        TRPx = x - winWidth / 2;
        TRPy = winHeight / 2 - y;

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

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.

    //*TIMER_PERIOD/500.  makes the animation more smoother

    //auto moves clouds
    for (int i = 0; i < 10; i += 2)
        cloudPlaces[i] += 1.5 * TIMER_PERIOD / 500.;

    //changes cloud and sky color
    if (skytime < 0.8)
        skytime += 0.01 * TIMER_PERIOD / 500.;
    else
        skytime -= 0.01 * TIMER_PERIOD / 500.;

    //check jump condition
    if (jumpPressed)
    {
        if (TRPy < groundTop + 100)
            TRPy += 30;
        else
            jumpPressed = false;
    }

    //changing automathic mode positions automaticly
    if (GAMEmode == 1)
    {
        if (TRYdir == Dright)
            TRPx += 6 * TRspeed * TIMER_PERIOD / 500.;
        else if (TRYdir == Dleft)
            TRPx -= 6 * TRspeed * TIMER_PERIOD / 500.;
        else if (TRYdir == Ddown)
            TRPy -= 6 * TRspeed * TIMER_PERIOD / 500.;
        else if (TRYdir == Dup)
            TRPy += 6 * TRspeed * TIMER_PERIOD / 500.;


        if (TRPy > winHeight / 2 - 60) {
            TRYdir = Ddown;
            TRPy -= 4 * TRspeed * TIMER_PERIOD / 500.;
        }
        else if (TRPy < 60 - winHeight / 2) {
            TRYdir = Dup;
            TRPy += 4 * TRspeed * TIMER_PERIOD / 500.;
        }
        else if (TRPx < 30 - winWidth / 2) {
            TRYdir = Dright;
            TRdir = Dright;
            TRPx += 10 * TRspeed * TIMER_PERIOD / 500.;
        }
        else if (TRPx > winWidth / 2 + 30) {
            TRYdir = Dleft;
            TRdir = Dleft;
            TRPx -= 10 * TRspeed * TIMER_PERIOD / 500.;
        }

    }
    else if (truckdisp == 1 && graDis)
    {
        grtime += TIMER_PERIOD / 500.;



        if (TRPy > groundTop && GAMEmode == -1)
            TRPy -= 4 * powf(grtime, 2);

        else if (!(TRPy > groundTop) && GAMEmode == -1) {
            grtime = 0;
            TRPy = groundTop;

        }

        //changes enemy positions
        for (int k = 0; k < level; k++)
            enemy[k] -= 12 + level / 2.;

        //checks win condition
        for (int i = 0; i < level; i++) {

            if (sqrtf(powf(TRPx - enemy[i], 2) + powf(TRPy - groundTop, 2)) < 20)
                win = 1;
        }
    }







    glColor3f(0, 0, 0);


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
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("My First Game: Truck");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //randomly declaring cloud places
    srand(time(NULL));

    for (int i = 0; i < 10; i += 2) {

        cloudPlaces[i] = rand() % 1151 - 600;
        cloudPlaces[i + 1] = rand() % 561 - 200;
    }

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