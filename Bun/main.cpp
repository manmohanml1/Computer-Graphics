//Name: Manmohan Lonawat
//CSCI 272 Project 5 - Object Model Loader
//Fresno State Id: 300698412

#include <GL/gl.h>
#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <SOIL.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <ctime>

#define PI 3.14159

using namespace std;
int Diff_Obj=0;//Flag for different objects.
bool WireFrame= false;
float i =0;
const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

float Wdth,Hght;//Screen width and height
GLuint tex,vbo,ibo;// For Texture mapping, VBO and IBO declaration.
float Camera_x=0.01, Camera_y= 0.01;// Camera's angle x, y position
float Cam_x=0.0, Cam_y=0.0, Cam_z=50.0;// Camera's x, y and z position
float Lght_Angle = 0.0;//Light settings angle.
int factor;  //factor
float c=50; // for look at functiom
float Trnslt_z = 50;// Translation of z axis
float X_rotate = 0, Y_rotate = 0, Z_rotate = 0;// Rotation around x, y, z axis
const float slices = 100.0;// Controls how far the ball will move each frame

typedef struct{
float x,y,z;
}vec3;//Vector taking x, y, z coordinates.

// Taking vertex, texture and normals of an object.
typedef struct {
int v1, v2, v3;
int t1, t2, t3;
int n1, n2, n3;
}faces;

int vert=0,norm=0,textr=0,fcs=0;// Values of initial Vertex, Normal, Texture, and Faces.

float Ww,Wh;

vec3* vertex;// Vertex structure
vec3* normal;// Normal structure
vec3* texture;// Texture structure
faces* face;// Face structure

//Function prototypes
void Load_Texture(char *, GLuint);
float z_Value_Generator();// z coordinate value generator.

//------------------------------------------------------------------------------------------
/* GLUT callback Handlers */
static void resize(int width, int height)
{
     double Ratio;

     Wdth = (float)width;
     Hght = (float)height;

     Ratio= (double)width /(double)height;

    glViewport(0,0,(GLsizei) width,(GLsizei) height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (45.0f,Ratio,0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

 }

//--------------I tried doing the VBO structure but wasn't succesful and hence I pasted the tried code here.------------------
/*
 void createVBO()// Creating VBO
{
    unsigned int Size = v+n;
    glGenBuffers(1,&ibo);
    glGenBuffersARB(1,&vbo);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glBufferDataARB(GL_ARRAY_BUFFER_ARB,Size,0,GL_DYANAMIC_DRAW);
    glBufferSubDataARB(GL_ARRAY_BUFFER_ARB,0,sizeof(vertex),vertex);
    glBufferSubDataARB(GL_ARRAY_BUFFER_ARB,sizeof(vertex),sizeof(normal),normal);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces),faces,GL_DYANAMIC_DRAW);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void draw_vbo()// Drawing VBO
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, (void*)0);
    glNormalPointer(GL_FLOAT, 0,(void*)sizeof(vertex));

    glDrawElements(GL_TRAINGLES,
                   sizeof(faces),
                   GL_UNSIGNED_INT, (void*)0);

     glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}
*/

//------------------------------------------------------------------------------------------
static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(Cam_x,Cam_y,Cam_z,0.0,0.0,0.0,0.0,1.0,100.0);//Camera positions are being assigned using Cam_x, Cam_y, Cam_z.

    //Move the cube around
    glPushMatrix();
    glDisable(GL_LIGHTING);//Disabling the lighting.
    glTranslatef(0,0, 0);
    glScalef(20.0f, 20.0f, 20.0f);
//------------------------------Draw the front face of a cube--------
    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    // Order of tex cords: TL -> TR -> BR -> BL
    glTexCoord2f(0.25f, 0.33f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.50f, 0.33f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.50f, 0.66f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(0.25f, 0.66f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glEnd();
//-----------------------------------------Draw the backface---------
    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    // Order: TR -> BR -> BL -> TL
    glTexCoord2f(1.0f, 0.33f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.66f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.75f, 0.66f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.75f, 0.33f); glVertex3f(1.0f, 1.0f, -1.0f);
    glEnd();

//---------------------------------------Drawing the Top face--------
    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0.50f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.50f, 0.33f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.25f, 0.33f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.25f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glEnd();
//--------------------------------------Drawing the Bottom face 'Z'-------
    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.25f, 0.66f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(0.50f, 0.66f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(0.50f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.25f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();

//------------------------------------Drawing the Right Face Y--------

    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.75f, 0.33f); glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.75f, 0.66f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.50f, 0.66f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(0.50f, 0.33f); glVertex3f(1.0f, 1.0f, 1.0f);
    glEnd();
//--------------------------------------Drawing the Left Face Y-------
    glBindTexture(GL_TEXTURE_2D, tex);//
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.33f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.25f, 0.33f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.25f, 0.66f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.66f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();
    glPopMatrix();
//-------------------------------Drawing the sphere--------
    glPushMatrix();
        glEnable(GL_LIGHTING);
        glRotatef(X_rotate, 1.0f, 0.0f, 0.0f);
       glRotatef(Y_rotate, 0.0f, 1.0f, 0.0f);
       glRotatef(Z_rotate, 0.0f, 0.0f, 1.0f);
//-------------------------------Drawing the triangles-----
      for(int i=0;i<fcs;i++)
      {
            glBegin(GL_TRIANGLES);
            glNormal3f(normal[face[i].n1].x,normal[face[i].n1].y,normal[face[i].n1].z);
            glNormal3f(normal[face[i].n2].x,normal[face[i].n2].y,normal[face[i].n2].z);
            glNormal3f(normal[face[i].n3].x,normal[face[i].n3].y,normal[face[i].n3].z);

            glVertex3f(vertex[face[i].v1].x,vertex[face[i].v1].y,vertex[face[i].v1].z);
            glVertex3f(vertex[face[i].v2].x,vertex[face[i].v2].y,vertex[face[i].v2].z);
            glVertex3f(vertex[face[i].v3].x,vertex[face[i].v3].y,vertex[face[i].v3].z);
            glEnd();
      }
    glPopMatrix();

    glPushMatrix();
        glRotatef(Lght_Angle,1,0,0);
        glLightfv(GL_LIGHT0,GL_POSITION, light_position);
        Lght_Angle+=0.1;
    glPopMatrix();

    if(WireFrame)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//Draw Our Mesh In Wireframe Mesh
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//Toggle WIRE FRAME

    glPopMatrix();
    glutSwapBuffers();
}
//-------------------------------Camera Angle movements.-----------------
void updateAngles()
{
    Cam_x= (sin(Camera_x)* cos(Camera_y))*50;//Camera x-position settings.
    Cam_y = (sin(Camera_y))*50;//Camera y-position settings.
    Cam_z = (cos(Camera_x) * cos(Camera_y))*50;//Camera z-position settings.
}
//-------------------------------Load objects into the screen.-----------
void loadObj(char *fname)
{
    float temporary[4];
    char lines[62000];// 62000 lines
    const char* c;

    FILE *fp; // declarition of file pointer
    fp = fopen(fname,"r"); // open file in read mode
    if(fp)                  // if file is opened
    {
        face =(faces*)malloc(sizeof(faces));  //initial memory allocation for faces
        vertex =(vec3*)malloc(sizeof(vec3));   //initial memory allocation for vertex
        normal=(vec3*)malloc(sizeof(vec3));     //initial memory allocation for normal
        texture=(vec3*)malloc(sizeof(vec3));    //initial memory allocation for texture

          while( fgets(lines, sizeof(lines), fp) != NULL )   // we read each of line until we reach end of the file
         {
            int i =0;
            c=NULL;
            c = strtok (lines," ");//
            if(strcmp(c,"v")==0){
              for(int i=0;i<4;i++){
                temporary[i]=atof(c);
                c = strtok (NULL, " \n"); // break the string
                }
                normal[vert].x= 0.0;//Initializing Normal values for x coordinates
                normal[vert].y= 0.0;//Initializing Normal values for y coordinates
                normal[vert].z= 0.0;//Initializing Normal values for z coordinates

                vertex[vert].x= temporary[1]/factor;//Scaling the Vertice Points for x coordinates.
                vertex[vert].y= temporary[2]/factor;//Scaling the Vertice Points for y coordinates.
                vertex[vert].z= temporary[3]/factor;//Scaling the Vertice Points for z coordinates.
                vert++;

                normal =(vec3*)realloc(normal,(vert+1)*sizeof(vec3)); //memory reallocation
                vertex =(vec3*)realloc(vertex,(vert+1)*sizeof(vec3)); //memory reallocation for vertex

                }
                else if(strcmp(c,"vn")==0) //Reading Vertice Normals of object
            {

              for(int i=0;i<4;i++)
                {
                    temporary[i]=atof(c);
                    c = strtok (NULL, " \n");
                }

                normal[norm].x= temporary[1];//Initializing Normal values x coordinates with temporaray values.
                normal[norm].y= temporary[2];//Initializing Normal values y coordinates with temporaray values.
                normal[norm].z= temporary[3];//Initializing Normal values z coordinates with temporaray values.
                norm++;

                 normal =(vec3*)realloc(normal,(norm+1)*sizeof(vec3)); //Reallocate memory for Vertex Normals

            }

                else if(strcmp(c,"vt")==0) //Reading Vertice Textures of object
                {
                   for(int i=0;i<4;i++)
                    {
                        temporary[i]=atof(c);
                        c = strtok (NULL, " \n");
                    }

                    texture[textr].x= temporary[1];//Initializing Texture values x coordinates with temporaray values.
                    texture[textr].y= temporary[2];//Initializing Texture values y coordinates with temporaray values.
                    texture[textr].z= 0;
                    textr++;
                    texture =(vec3*)realloc(texture,(textr+1)*sizeof(vec3)); //memory reallocation
                }
                 else if(strcmp(c,"f")==0) //Read the Vertice texture and normal values from the faces
                 {

                    int str[12];
                      for(int i=0;i<11;i++)
                    {
                        str[i]=atoi(c);
                        c = strtok (NULL, " /");   // dividing the string based on /
                    }


                    face[fcs].v1 = str[1] - 1; face[fcs].v2 = str[4] - 1; face[fcs].v3 = str[7] - 1;//Faces
                    face[fcs].t1 = str[2] - 1; face[fcs].t2 = str[5] - 1; face[fcs].t3 = str[8] - 1;//Texture
                    face[fcs].n1 = str[3] - 1; face[fcs].n2 = str[6] - 1; face[fcs].n3 = str[9] - 1;//Normals
                    fcs++;
                      face =(faces*)realloc(face,(fcs+1)*sizeof(faces));//Memory Reallocation for faces
                }
             }
       }
       else
       {
            cout << "Error: can't open file " << fname << endl;//Error handling
       }
        fclose(fp);
}
void calculateNormal()
{
     for(int i=0;i<fcs;i++){

    vec3 vert1 = vertex[face[i].v1];//Vertex faces v1
    vec3 vert2 = vertex[face[i].v2];//Vertex faces v2
    vec3 vert3 = vertex[face[i].v3];//Vertex faces v3

    float x1 = vert2.x - vert1.x;//Preparing for cross product
    float x2 = vert1.x - vert3.x;
    float y1 = vert2.y - vert1.y;
    float y2 = vert1.y - vert3.y;
    float z1 = vert2.z - vert1.z;
    float z2 = vert1.z - vert3.z;

    //Cross product
    float x3 = (y2 * z1) - (z2 * y1);
    float y3 = (z2 * x1) - (x2 * z1);
    float z3 = (x2 * y1) - (y2 * x1);

    float n[3];
      n[0] = x3/sqrt( x3 * x3 + y3 * y3 + z3 * z3);//Normalizing these
      n[1] = y3/sqrt( x3 * x3 + y3 * y3 + z3 * z3);
      n[2] = z3/sqrt( x3 * x3 + y3 * y3 + z3 * z3);

        normal[face[i].v1].x = (normal[face[i].v1].x + n[0])/2; //Need our normals to share and to be on each vertex
        normal[face[i].v1].y = (normal[face[i].v1].y + n[1])/2;
        normal[face[i].v1].z = (normal[face[i].v1].z + n[2])/2;
        normal[face[i].v2].x = (normal[face[i].v2].x + n[0])/2;
        normal[face[i].v2].y = (normal[face[i].v2].y + n[1])/2;
        normal[face[i].v2].z = (normal[face[i].v2].z + n[2])/2;
        normal[face[i].v3].x = (normal[face[i].v3].x + n[0])/2;
        normal[face[i].v3].y = (normal[face[i].v3].y + n[1])/2;
        normal[face[i].v3].z = (normal[face[i].v3].z + n[2])/2;
    }
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q'://to quit program
            exit(0);
            break;

      case 'w':
        WireFrame =!WireFrame;//Wire frame settings
           break;

      case 'l':
        Camera_x+=0.05;//Moving camera view towards right on x-axis
      if(Camera_x > PI) Camera_x -= 2*PI;
        updateAngles();
        break;

     case 'j':
        Camera_x-=0.05;//Moving camera view towards left on x-axis
       if(Camera_x < -PI) Camera_x += 2*PI;
        updateAngles();
        break;
    case 'i':
        Camera_y+=0.05;
      if(Camera_x > PI) Camera_y -= 2*PI;//Moving camera view towards upside on y-axis
        updateAngles();
        break;
    case 'k':
        Camera_y-=0.05;
       if(Camera_x < -PI) Camera_y += 2*PI;//Moving camera view towards downside on y-axis
        updateAngles();
        break;
    case 'a'://Changing the object to previous ones.
            Diff_Obj=Diff_Obj+1;
            if(Diff_Obj>2)//if  flag of objects exceeds the total objects then goes back to starting object.
            {
                Diff_Obj=1;
            }
            if(Diff_Obj==1)//Bunny object
            {
                vert=0; norm=0; textr=0; fcs=0;////initial vertex, normal, texture, face value is assigned
                factor=200;//factor
                loadObj("bunny.obj");//Load Object
                calculateNormal();//Calculate Normal
            }
            else if(Diff_Obj==2)//Aetneam object
            {

                vert=0; norm=0; textr=0; fcs=0;////initial vertex, normal, texture, face value is assigned
                factor=1000;//factor
                loadObj("ateneam.obj");//Load Object

            }
            /*
            else if(Diff_Obj==3)//Ven object
            {

                vert=0; norm=0; textr=0; fcs=0;////initial vertex, normal, texture, face value is assigned
                factor=1000;//factor
                loadObj("ven.obj");//Load Object

            }
            */
            break;
    case 'd'://Changing the object to next ones.
            Diff_Obj=Diff_Obj-1;
            if(Diff_Obj<1)//if the flag of objects is going before the first it goes to the last object.
            {
                Diff_Obj=2;
            }
            if(Diff_Obj==1)
            {

                vert=0; norm=0; textr=0; fcs=0;////initial vertex, normal, texture, face value is assigned
                factor=200;//factor
                loadObj("bunny.obj");//Load Object
                calculateNormal();//Calculate Normal
            }
            else if(Diff_Obj==2)
            {

                vert=0; norm=0; textr=0; fcs=0;////initial vertex, normal, texture, face value is assigned
                factor=1000;//factor
                loadObj("ateneam.obj");//Load Object

            }
            /*
            else if(Diff_Obj==3)//Ven object
            {

                vert=0; norm=0; textr=0; fcs=0;////initial vertex, normal, texture, face value is assigned
                factor=1000;//factor
                loadObj("ven.obj");//Load Object

            }
            */
            break;

    }
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_END:
      Cam_z+= (float) 1.0f;
        break;
    case GLUT_KEY_HOME:
      Cam_z-= (float) 1.0f;
        break;
    case GLUT_KEY_UP:
        X_rotate = ( (int)X_rotate + 5) % 360;//Rotate screen towards right
        break;
    case GLUT_KEY_DOWN:
        X_rotate = ( (int)X_rotate - 5) % 360;//Rotate screen towards left
        break;
    case GLUT_KEY_LEFT:
        Y_rotate = ( (int)Y_rotate + 5) % 360;//Rotate screen towards up
        break;
    case GLUT_KEY_RIGHT:
        Y_rotate = ( (int)Y_rotate - 5) % 360;//Rotate screen towards down
        break;
    default:
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
    if(Diff_Obj==0)//Initial object bind
    {

        vert=0; norm=0; textr=0; fcs=0;////initial vertex, normal, texture, face value is assigned
        factor=200;//factor
        loadObj("bunny.obj");//Load Object
        calculateNormal();//Calculate Normal
    }


    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClearColor(0.5f, 0.5f, 1.0f, 0.0f);                 // assign a color you like

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

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    //Enable lightning for objects in display for that specific object
    glEnable(GL_LIGHTING);

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &tex);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    Load_Texture("images/r.png", tex);//Loading skybox image and applying texture wrap on object.
}

void Load_Texture(char *file, GLuint tex){// Loading Texture for wrapping on object and skybox

     glBindTexture(GL_TEXTURE_2D, tex); // images are 2D arrays of pixels, bound to the GL_TEXTURE_2D target.

     int width, height; //Width & Height for the image reader
     unsigned char* image;

     //change file?
     image = SOIL_load_image(file, &width, &height, 0, SOIL_LOAD_RGB);
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    // binding image data
     SOIL_free_image_data(image);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
}

/* Program entry point */

int main(int argc, char *argv[])
{
    // To make each mouse click different every time the program
    // is run
    srand(time(0));

    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Project #5 - Object Model Loader");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);

    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);

    glutMainLoop();

    return EXIT_SUCCESS;
}
