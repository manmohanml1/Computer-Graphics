// Name: Manmohan Mahavir Lonawat
//Id: 300698412
//CSCI 272 Assignment- N-Degree Bézier Curve Movement of spheres in a cube


#include <string.h> // header file required for string functions

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h> //header for the General Purpose Standard Library of C programming language
#include <iostream>

#include <math.h> // library for mathematical operations
using namespace std;

bool WireFrame= false;

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f }; // for lighting and shine
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

GLfloat angle=0;//for rotating the screen over y-axis
double a=1.0f,b=1.0f,c=15.0f;//for zoom in and zoom out the screen over y-axis
double mpointx, mpointy;// mouse pointer coordinates on the x and y axis
double tx, ty;// to calculate x and y position of t
double spherex, spherey;// sphere's x and y position
double upspherex, upspherey;// We update sphere's center positions.
float t1;// t points between the required curve, whose value ranges from 0 to 1
int dir_of_sphere=1;// determines the direction of sphere's movement
int flag1=0, flag2=0;//Used for toggling show and hide control points on screen
float wdth,hght;// width and height of the screen
double c_pnts[15][3];//Control points between which sphere must be drawn.
int counter = 0;//Counter
void keyboard(unsigned char, int x,  int y);
/* GLUT callback Handlers */

static void resize(int width, int height)
{
     double Ratio;

     wdth=(float)width;
     hght=(float)height;

      glViewport(0 ,0 ,(GLsizei) width,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);//specify which matrix is the current matrix , in this case it is projection

    glLoadIdentity();
	gluPerspective (50.0f,1,0.1f, 100.0f);//set up a perspective projection matrix

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
 }

int fact(int n)//Recursive function to calculate factorial
{
    if (n<=1)
        return(1);
    else
        n=n*fact(n-1);
    return n;
}

float binomial_coeff(float n,float k)//Binomial Coefficient
{
    float result;
    result = fact(n) / (fact(k)*fact(n-k));
    return result;
}
double updatespherecenter(double x, double y)// To update the sphere's central x and y axis coordinates.
{
    upspherex=x;
    upspherey=y;
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear buffers to preset values
    glMatrixMode(GL_MODELVIEW); // specify which matrix is the current matrix , in this case it is modelview
    glLoadIdentity();

    gluLookAt(0,0,c,0.0,0.0,0.0,0.0,1.0,100.0);// define a viewing transformation

    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

    glPushMatrix();
    glRotatef(angle,0,1,0); //for rotating the wireframe

    // code for solid sphere
    if(counter>2)//sphere visible only after 3rd control point is selected.
    {
    glPushMatrix();
    glColor3f(0.0f,0.0f,1.0f);// color red
    glTranslatef (spherex,spherey,0); // setting center to some point in the plan
    glScalef(5.0,5.0,5.0);// sets the scale of sphere
    glutSolidSphere(0.1,20,20);  // declaring solid sphere and give its specifications
    glPopMatrix ();
    }
    if(flag1==1){// for showing the control points(toggle between visible and hidden)
        glPointSize(5);
        glBegin(GL_POINTS);//For drawing control points
        glColor3f(0.0f, 0.0f, 1.0f);//Control Points with color Blue
        for(int j=0;j<counter;j++)
             glVertex3f(c_pnts[j][0],c_pnts[j][1],0);// displays different control points(max = 15)
        glEnd();
    }
    if(counter >= 3 && counter <= 15){//the max number of control points is 15 (program stops drawing after 15 clicks)
            for(float t=0;t<1;t=t+0.00002){ //t points between the required curve, whose value ranges from 0 to 1, with 0.00002 increase every iteration
                for(int i=0;i<counter;i++){
                    tx = tx + binomial_coeff((counter - 1), i) * pow(t, i) * pow((1 - t), (counter - 1 - i)) * c_pnts[i][0];// for x-coordinate
                    ty = ty + binomial_coeff((counter - 1), i) * pow(t, i) * pow((1 - t), (counter - 1 - i)) * c_pnts[i][1];// for y-coordinate
                }
                glPointSize(3);
                glBegin(GL_POINTS);//for drawing the curve

                glColor3f(0.0,0.0,1.0);//Curve with color Blue
                glVertex3f(tx,ty,0.0);//Curve coordinates.
                tx=0; ty=0;//Resetting curve coordinates.
                glEnd();
            }
    }
    if(counter>15){// after 15 control points are drawn it resets back to 0
        counter=0;
    }
    glPopMatrix ();
    glutSwapBuffers();  // swaps the buffers of the current window if double buffered.
}


static void key(unsigned char key, int x, int y)
{
    switch (key) // q key to exit
    {
        case 27 :
        case 'q':
            exit(0);
            break;
        case 32://space button on keyboard resets control points back to 0 and angle to original
            counter=0;
            angle=0;
            break;
        case 13:// enter button
            spherex=0; spherey=0;
    }
}

void Specialkeys(int key, int x, int y)
{
    switch(key){
    case GLUT_KEY_UP: // key-up to zoom out
            c=c+0.5;
            break;
    case GLUT_KEY_DOWN: // key down to zoom in
            c=c-0.5;
            break;
    case GLUT_KEY_RIGHT: // In the keyboard if we click right button it should rotate the scene over y-axis by 5 degrees increment
            angle+=5;
            break;
     case GLUT_KEY_LEFT: // In the keyboard if we click left button it should rotate the scene over y-axis by 5 degrees decrement
            angle-=5;
            break;
   }
  glutPostRedisplay();

}

void mymouse(int key,int state, int x , int y)
{
     mpointx=(float)(x-wdth/2)/55 * (wdth/hght);// to get mouse coordinates for x-axis
     mpointy=(float)(hght/2-y)/55 * (wdth/hght);// to get mouse coordinates for y-axis

    if(key== GLUT_LEFT_BUTTON && state == GLUT_DOWN ){
        flag1=1;
        c_pnts[counter][0]= mpointx;//determines number of control points position
        c_pnts[counter][1]= mpointy;//determines number of control points position
        counter++;
    }
    if(key== GLUT_RIGHT_BUTTON && state == GLUT_DOWN ){
        if(flag1%2==0)
         {
             flag2=0;
         }
         else{ flag1=1; }
         flag1++;
    }
     if(key== GLUT_MIDDLE_BUTTON && state == GLUT_DOWN ){// counter gets reset back to 0
        counter=0;
     }
}

void timer(int)                // timer function
{
    if(counter >= 3 && counter <= 15){// between 3 to 15 control points
            spherex=0;spherey=0;//sphere initial x and y axis coordinates
            if (t1>0.98){
                dir_of_sphere=-1;//direction of sphere movement should reverse
            }
            if (t1<0.02){
                dir_of_sphere=1;//direction of sphere movement
            }
            t1=t1+(dir_of_sphere)*(0.02);// for movements of sphere
            for(int i=0;i<counter;i++){
                spherex = spherex + binomial_coeff((counter - 1), i) * pow(t1, i) * pow((1 - t1), (counter - 1 - i)) * c_pnts[i][0];
                spherey = spherey + binomial_coeff((counter - 1), i) * pow(t1, i) * pow((1 - t1), (counter - 1 - i)) * c_pnts[i][1];
            }
    }
     glutPostRedisplay();
     glutTimerFunc(100,timer,0);
}

static void idle(void)
{
    glutPostRedisplay();
}

static void init(void)
{

    glClearColor(1.0,1.0,1.0,0.0);
    glColor3f(0.0,0.0,0.0);
    glPointSize(3);

    glEnable(GL_NORMALIZE);// enables gl -normalize , gl-normalize--> normal vectors are normalized to unit length after transformation and before lighting
    glEnable(GL_COLOR_MATERIAL); // enables color_material

    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//specify implementation-specific hints
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);// set light source parameters
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient); // specifies material parameters for the lighting model.
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
}


/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv); // used to initialize the GLUT library

    glutInitWindowSize(640,500);// window size
    glutInitWindowPosition(100,150); // position
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); // display mode

    glutCreateWindow("GLUT Shapes");// window name and creation
    init();
    glutReshapeFunc(resize); // resize function
    glutDisplayFunc(display); // display function
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);// special functions
    glutMouseFunc(mymouse);//mouse pointers
    glutTimerFunc(0,timer,0);  // timer function
    glutIdleFunc(idle);
    glutMainLoop();

    return EXIT_SUCCESS;
}
