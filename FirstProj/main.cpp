#include <string.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
/* GLUT callback Handlers */

/*
x’ = ax + cy + e

y’ = bx + dy + f
ƒ1: xn + 1 = 0 ; yn + 1 = 0.16 yn where (a= 0, b =0, c =0,d =0.16, e =0, f=0 )

ƒ2: xn + 1 = 0.85 xn + 0.04 yn; yn + 1 = −0.04 xn + 0.85 yn + 1.6

//where (a= 0.85, b = -0.04, c =0.04, d =0.85, e =0, f=1.6 )

ƒ3: xn + 1 = 0.2 xn − 0.26 yn; yn + 1 = 0.23 xn + 0.22 yn + 1.6

//where (a= 0.2, b =0.23, c =-0.26, d =0.22, e =0, f=1.6 )

ƒ4: xn + 1 = −0.15 xn + 0.28 yn; yn + 1 = 0.26 xn + 0.24 yn + 0.44

//where (a= -0.15, b =0.26, c =0.28, d =0.24, e =0, f=0.44 )
*/

float px, py, x, y, a, b, c, d, e, f, num;
int n = 0;


void resize(int width, int height)
{
if(width<=height)
glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);

else
glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);
}

void init()
{
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glOrtho(-2.5,2.75,10.5,0, -1.0, 1.0); // adjusted for suitable viewport

px = py = x = y = 0.5;
n = 200000;
}

void display(void)
{
glClear (GL_COLOR_BUFFER_BIT); // clear display screen
// your code here
glTranslatef(0,10,0);
glRotatef(180.0,1.0f,0.0f,0.0f);
for (int i = 1; i<n; i++)
{
num = rand() % 100;
if(num<7)
{
a= -0.15; b =0.26; c =0.28; d =0.24; e =0; f=0.44;
}
else if(num<14)
{
a= 0.2; b =0.23; c =-0.26; d =0.22; e =0; f=1.6;
}
else if(num<99)
{
a= 0.85; b = -0.04; c =0.04; d =0.85; e =0; f=1.6;
}
else
{
a= 0; b =0; c =0; d =0.16; e =0; f=0;
}

px = a*x + c*y + e;
py = b*x + d*y + f;

glColor3f(0,1,0);

glBegin(GL_POINTS);
glVertex3f(px, py, 0);
glEnd();
x = px;
y = py;
}
glFlush (); // clear buffer
}


void key(unsigned char key, int x, int y)
{
switch (key)
{
case 27 : // esc key to exit
case 'q':
exit(0);
break;
}

glutPostRedisplay();
}


/* Program entry point */

int main(int argc, char *argv[])
{
glutInit(&argc, argv);
glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
glutInitWindowSize (600, 800); //window screen
glutInitWindowPosition (100, 100); //window position
glutCreateWindow ("Program1"); //program title
init();
glutDisplayFunc(display); //callback function for display
glutReshapeFunc(resize); //callback for reshape
glutKeyboardFunc(key); //callback function for keyboard
glutMainLoop(); //loop

return EXIT_SUCCESS;
}
