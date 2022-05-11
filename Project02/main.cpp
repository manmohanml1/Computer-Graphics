
#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>

#include <math.h>
using namespace std;

bool WireFrame= false;

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

static int slices = 16;
static int stacks = 16;
float x2=0.0f,y2=1.0f,z2=0.0f, angle=0.0f;//angle used for rotation over y-axis in glRotatef
/* GLUT callback Handlers */
float x[3],y[3],z[3];// the coordinate matrix for 4 spheres using x,y,z coordinate points.
float c=20.0f, a=0.0f, b=5.0f;// a,b,c are eyeX,eyeY, eyeZ coordinates in gluLookAt
int dir_x[3],dir_y[3],dir_z[3];// flags used to determine directions of sphere's movements
float spheresqr = 0.3*0.3;// square of sphere's radius
float distX[3], distY[3], distZ[3];//distance between 2 spheres in X,Y,Z axises.
float sqrDist[3];//sum of squares of distX,distY and distZ -> distX^2 + distY^2 + distZ^2

static void resize(int width, int height)
{
     double Ratio;

   if(width<=height)
            glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
    else
          glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
	gluPerspective (40.0f,1,1.0f, 100.0f);
 }

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(a,b,c,0.0,0.0,0.0,0.0,1.0,0.0);// taking in values of eyeX, eyeY, eyeZ

    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

    // your code here
    //glutSolidSphere (1.3, 10, 10);
    //glTranslatef(x,y,z);

    glPushMatrix();
    glRotatef(angle,x2,y2,z2);// used to rotate scene over y-axis
    glutWireCube(8);// used to make an imaginary cube inside which our sphere's move

    glPopMatrix();
   for(int i=0;i<4;i++)// used to display all the 4 spheres in a for loop
   {
      glPushMatrix();

        glTranslatef(x[i],y[i],z[i]);// assigning the positions for spheres using glTranslate
        glutSolidSphere(0.3,slices,stacks);// for displaying solidspheres
        glPopMatrix();
   }

    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;
    }
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_UP:// in the keyboard if we click up button it should zoom in the eyeZ in the glLookAt by 0.5 increment
        c-=0.5;
    break;
    case GLUT_KEY_DOWN:// in the keyboard if we click down button it should zoom out the eyeZ in the glLookAt by 0.5 increment
        c+=0.5;
        break;
    case GLUT_KEY_LEFT:// in the keyboard if we click left button it should rotate the scene/imaginary cube over y-axis by 5 degrees increment
        angle+=0.5;
        break;
    case GLUT_KEY_RIGHT:// in the keyboard if we click left button it should rotate the scene/imaginary cube over y-axis by 5 degrees decrement
        angle-=0.5;
        break;
   }
  glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();

}

static void init(void)
{
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);

    x[0]=2.0f;x[1]=-2.0f;x[2]=-2.0f;x[3]=2.0f;// position of sphere in X-axis
    y[0]=2.0f;y[1]=2.0f;y[2]=-2.0f;y[3]=-2.0f;// position of sphere in Y-axis
    z[0]=2.0f;z[1]=2.0f;z[2]=-2.0f;z[3]=-2.0f;// position of sphere in Z-axis
    for(int i = 0; i<4; i++)// assigning the flag for direction of sphere movement
    {
        dir_x[i]=0;
        dir_y[i]=0;
        dir_z[i]=0;
    }


}

void timer(int)// we use timer function to slower the speed of collisions
{
for(int i=0;i<4;i++)// for all 4 spheres
   {
       if(dir_x[i]==0)// if direction dir = 0 then should move towards +ve x-axis
       {
         x[i]+=0.1;
       }
       if(dir_x[i]==1)// if direction dir = 1 then should move towards -ve x-axis
       {
         x[i]-=0.1;
       }
       if(dir_y[i]==0)// if direction dir = 0 then should move towards +ve y-axis
       {
         y[i]+=0.1;
       }
       if(dir_y[i]==1)// if direction dir = 1 then should move towards -ve y-axis
       {
         y[i]-=0.1;
       }
       if(dir_z[i]==0)// if direction dir = 0 then should move towards +ve z-axis
       {
         z[i]+=0.1;
       }
       if(dir_z[i]==1)// if direction dir = 1 then should move towards -ve z-axis
       {
         z[i]-=0.1;
       }
   }


    for(int i=0;i<4;i++)// for all 4 spheres
   {
       if(x[i]>1.5)// if sphere reaches 1.5 position in the x-axis then direction dir = 1
    {
        dir_x[i]=1;
    }
    if(y[i]>1.5)// if sphere reaches 1.5 position in the y-axis then direction dir = 1
    {
        dir_y[i]=1;
    }if(z[i]>1.5)// if sphere reaches 1.5 position in the z-axis then direction dir = 1
    {
        dir_z[i]=1;
    }
    if(x[i]<-1.5)// if sphere reaches -1.5 position in the x-axis then direction dir = 0
    {
        dir_x[i]=0;
    }
    if(y[i]<1.5)// if sphere reaches -1.5 position in the y-axis then direction dir = 0
    {
        dir_y[i]=0;
    }if(z[i]<1.5)// if sphere reaches -1.5 position in the z-axis then direction dir = 0
    {
        dir_z[i]=0;
    }
   }
   for(int i=0;i<4;i++)// for all 4 spheres
   {
       for(int j=i+1;j<4;j++)// for the remaining 3 spheres
       {
        distX[i] = x[i] - x[j];// distance between 1st and 2nd sphere's in the x-axis
        distY[i] = y[i] - y[j];// distance between 1st and 2nd sphere's in the y-axis
        distZ[i] = z[i] - z[j];// distance between 1st and 2nd sphere's in the y-axis
        distX[i] *= distX[i];// square of distX
        distY[i] *= distY[i];// square of distY
        distZ[i] *= distZ[i];// square of distZ

        sqrDist[i] = (distX[i]+distY[i]+distZ[i]);// sum of the new distX, distY, distZ

        if((2*spheresqr) > sqrDist[i])// if 2*radius of sphere's is greater than sum of the new distX, distY, distZ
        {
            //x-axis They intersect
            if(dir_x[i]==0 && dir_x[j]==0)// if direction of 1st and 2nd sphere's is the same over x-axis
           {
             x[i]+=0.1; x[j]+=0.1;
             y[i]-=0.1; y[j]-=0.1;// the direction of y-axis changes
             z[i]-=0.1; z[j]-=0.1;// the direction of z-axis changes
           }
           if(dir_x[i]==1 && dir_x[j]==1)// if direction of 1st and 2nd sphere's is the same over x-axis
           {
             x[i]-=0.1; x[j]-=0.1;
             y[i]+=0.1; y[j]+=0.1;// the direction of y-axis changes
             z[i]+=0.1; z[j]+=0.1;// the direction of z-axis changes
           }
           if(dir_x[i]==0 && dir_x[j]==1)// if direction of 1st sphere's is 0 and 2nd sphere's is 1 over x-axis
           {
             x[i]-=0.1; x[j]+=0.1;
           }
           if(dir_x[i]==1 && dir_x[j]==0)// if direction of 1st sphere's is 1 and 2nd sphere's is 0 over x-axis
           {
             x[i]+=0.1; x[j]-=0.1;
           }
           //y-axis They intersect
           if(dir_y[i]==0 && dir_y[j]==0)// if direction of 1st and 2nd sphere's is the same over y-axis
           {
             x[i]-=0.1; x[j]-=0.1;// the direction of x-axis changes
             y[i]+=0.1; y[j]+=0.1;
             z[i]-=0.1; z[j]-=0.1;// the direction of z-axis changes
           }
           if(dir_y[i]==1 && dir_y[j]==1)// if direction of 1st and 2nd sphere's is the same over y-axis
           {
             x[i]+=0.1; x[j]+=0.1;// the direction of x-axis changes
             y[i]-=0.1; y[j]-=0.1;
             z[i]+=0.1; z[j]+=0.1;// the direction of z-axis changes
           }
           if(dir_y[i]==0 && dir_y[j]==1)// if direction of 1st sphere's is 0 and 2nd sphere's is 1 over y-axis
           {
             y[i]-=0.1; y[j]+=0.1;
           }
           if(dir_y[i]==1 && dir_y[j]==0)// if direction of 1st sphere's is 1 and 2nd sphere's is 0 over y-axis
           {
             y[i]+=0.1; y[j]-=0.1;
           }
           //z-axis They intersect
           if(dir_z[i]==0 && dir_z[j]==0)// if direction of 1st and 2nd sphere's is the same over z-axis
           {
             x[i]-=0.1; x[j]-=0.1;// the direction of x-axis changes
             y[i]-=0.1; y[j]-=0.1;// the direction of y-axis changes
             z[i]+=0.1; z[j]+=0.1;
           }
           if(dir_z[i]==1 && dir_z[j]==1)// if direction of 1st and 2nd sphere's is the same over z-axis
           {
             x[i]+=0.1; x[j]+=0.1;// the direction of x-axis changes
             y[i]+=0.1; y[j]+=0.1;// the direction of y-axis changes
             z[i]-=0.1; z[j]-=0.1;
           }
           if(dir_z[i]==0 && dir_z[j]==1)// if direction of 1st sphere's is 0 and 2nd sphere's is 1 over z-axis
           {
             z[i]-=0.1; z[j]+=0.1;
           }
           if(dir_z[i]==1 && dir_z[j]==0)// if direction of 1st sphere's is 1 and 2nd sphere's is 0 over z-axis
           {
             z[i]+=0.1; z[j]-=0.1;
           }
        }
       }
   }
    glutPostRedisplay();
    glutTimerFunc(150,timer,0);// speed of timer is 150ms
}

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);
    glutTimerFunc(0,timer,0);

    glutIdleFunc(idle);
    glutMainLoop();

    return EXIT_SUCCESS;
}
