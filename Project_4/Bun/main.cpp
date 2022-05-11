//Name: Manmohan M Lonawat
//CSCI 272 Project 4
#include <SOIL.h>
#include <string.h>
#ifdef _APPLE_
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
#include <math.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>


#define PI 3.14159

using namespace std;

GLuint tex;
bool WireFrame= false;  //to toggle Wireframe
int model_factor = 20;  //factor for model

typedef struct{
float xPos;  // x coordinate position
float yPos;  // y coordinate position
float zPos;  // z coordinate position
}vec3;// defining the vector with x, y, z coordinates.

typedef struct {

int vert1;// vertex 1
int vert2;// vertex 2
int vert3;// vertex 3

int tex1;// texture 1
int tex2;// texture 2
int tex3;// texture 3

int normal1;// normal 1
int normal2;// normal 2
int normal3;// normal 3
}face;

float Rotate_X_axis =0, Rotate_Y_axis =0;// Rotate on X-axis and Y-axis
float zoom =-50;// Zoom function
float Wdth,Hight;// Width and Height of the screen and model

vec3* vertex;//Vertex x,y,z coordinates.
vec3* normal;//Normal x,y,z coordinates.
vec3* texture;//Texture x,y,z coordinates.
face* face1;// Faces of the model, which includes the vertex, normal and also the texture from the given object file.
vec3* mem;//
face* fmem;//
int v_Num=0, n_Num=0, t_Num=0, f_Num=0;// Vertex, Normal, Texture and Faces counter

void Obj_Load(char *fname)// Object loader
{
    char no_of_line[1500];
    const char* chr;
    float tmp[4];
    FILE *file;
    file = fopen(fname,"r");
    if(!file)
    {
      cerr << "Error: can't open file " << fname << endl;//if no files print the error message
      exit(1);
    }
    else{

    vertex =(vec3*)malloc(sizeof(vec3)); //using malloc to allocate initial memory for a vertex
    normal=(vec3*)malloc(sizeof(vec3)); //using malloc to allocate initial memory for normal
    texture=(vec3*)malloc(sizeof(vec3)); //using malloc to allocate initial memory for texture
    face1 =(face*)malloc(sizeof(face)); //using malloc to allocate initial memory for faces indices
    mem=(vec3*)malloc(sizeof(vec3));
    fmem =(face*)malloc(sizeof(face));

    while( fgets(no_of_line, sizeof(no_of_line), file) != NULL ){
        int i =0;
        chr=NULL;
        chr = strtok (no_of_line," ");

        //To read Vertices
        if(strcmp(chr,"v")==0)
        {

            for(int i=0;i<4;i++){
                tmp[i]=atof(chr);
                chr = strtok (NULL, " \n");
            }

            vertex[v_Num].xPos= tmp[1]/model_factor;//Vertex x coordinates
            vertex[v_Num].yPos= tmp[2]/model_factor;//Vertex y coordinates
            vertex[v_Num].zPos= tmp[3]/model_factor;//Vertex z coordinates
            normal[v_Num].xPos= 0.0;//Normal x coordinates
            normal[v_Num].yPos= 0.0;//Normal y coordinates
            normal[v_Num].zPos= 0.0;//Normal z coordinates
            v_Num++;

            mem =(vec3*)realloc(vertex,(v_Num+1)*sizeof(vec3)); //reallocate memory for vertex

            if(mem!=NULL){
                 vertex=mem;
            }
            mem =(vec3*)realloc(normal,(v_Num+1)*sizeof(vec3)); //reallocate memory for normal

            if(mem!=NULL){
                 normal=mem;
            }
        }
        else if(strcmp(chr,"vn")==0)//To read normals
        {

            for(int i=0;i<4;i++)
            {
                tmp[i]=atof(chr);
                chr = strtok (NULL, " \n");
            }

            normal[n_Num].xPos= tmp[1];//Normal x coordinates
            normal[n_Num].yPos= tmp[2];//Normal y coordinates
            normal[n_Num].zPos= tmp[3];//Normal z coordinates
            n_Num++;

            mem =(vec3*)realloc(normal,(n_Num+1)*sizeof(vec3)); //To reallocate memory for normal

            if(mem!=NULL){
                normal=mem;
            }
        }
        else if(strcmp(chr,"vt")==0) //To read textures
        {
            for(int i=0;i<4;i++){
                tmp[i]=atof(chr);
                chr = strtok (NULL, " \n");
            }

                texture[t_Num].xPos= tmp[1];//Texture x coordinates
                texture[t_Num].yPos= tmp[2];//Texture y coordinates
                texture[t_Num].zPos= tmp[3];//Texture z coordinates
                t_Num++;

                 mem =(vec3*)realloc(texture,(t_Num+1)*sizeof(vec3)); //To reallocate memory for texture

               if(mem!=NULL){
                 texture=mem;
               }
            }
             else if(strcmp(chr,"f")==0) //To read faces - v index/ vt index / vn index
             {

            int ind[12];
              for(int i=0;i<11;i++)
            {
                ind[i]=atoi(chr);
                chr = strtok (NULL, " /");
            }


            face1[f_Num].vert1 = ind[1] - 1;//Face Vertex x coordinate
            face1[f_Num].vert2 = ind[4] - 1;//Face Vertex y coordinate
            face1[f_Num].vert3 = ind[7] - 1;//Face Vertex z coordinate

            face1[f_Num].tex1 = ind[2] - 1;//Face Texture x coordinate
            face1[f_Num].tex2 = ind[5] - 1;//Face Texture x coordinate
            face1[f_Num].tex3 = ind[8] - 1;//Face Texture x coordinate

            face1[f_Num].normal1 = ind[3] - 1;//Face Normal x coordinate
            face1[f_Num].normal2 = ind[6] - 1;//Face Normal x coordinate
            face1[f_Num].normal3 = ind[9] - 1;//Face Normal x coordinate

            f_Num++;

              fmem =(face*)realloc(face1,(f_Num+1)*sizeof(face)); //To reallocate memory for faces array

               if(fmem!=NULL){
                 face1=fmem;
               }

	}
         }
   }fclose(file);
 cout<<"vcount : "<<v_Num<<" ncount : "<<n_Num<<" count : "<<t_Num<<" fcount : "<<f_Num<<endl;
}

void Cal_Normal()//Calculating normal function
{
     for(int i=0;i<f_Num;i++){

    vec3 vertex1 = vertex[face1[i].vert1];//Vertex 1
    vec3 vertex2 = vertex[face1[i].vert2];//Vertex 2
    vec3 vertex3 = vertex[face1[i].vert3];//Vertex 3

    float Vx = vertex2.xPos - vertex1.xPos;//Vertex 2 x position
    float Vy = vertex2.yPos - vertex1.yPos;//Vertex 2 y position
    float Vz = vertex2.zPos - vertex1.zPos;//Vertex 2 z position

    float Ux = vertex1.xPos - vertex3.xPos;//Vertex 1 x position
    float Uy = vertex1.yPos - vertex3.yPos;//Vertex 1 y position
    float Uz = vertex1.zPos - vertex3.zPos;//Vertex 1 z position

    //Cross product calculation
    float Nx = (Uy * Vz) - (Uz * Vy);
    float Ny = (Uz * Vx) - (Ux * Vz);
    float Nz = (Ux * Vy) - (Uy * Vx);

    float length = sqrt( Nx * Nx + Ny * Ny + Nz * Nz);

    float *normalNew = new float[3];

      normalNew[0] = Nx/ length; //normalNew X value
      normalNew[1] = Ny/length; // normalNew Y Value
      normalNew[2] = Nz/ length; // normalNew Z Value


        normal[face1[i].vert1].xPos = (normal[face1[i].vert1].xPos + normalNew[0])/2;//Normal x-axis position of Vertex Face
        normal[face1[i].vert1].yPos = (normal[face1[i].vert1].yPos + normalNew[1])/2;//Normal y-axis position of Vertex Face
        normal[face1[i].vert1].zPos = (normal[face1[i].vert1].zPos + normalNew[2])/2;//Normal z-axis position of Vertex Face

        normal[face1[i].vert2].xPos = (normal[face1[i].vert2].xPos + normalNew[0])/2;//Normal x-axis position of Vertex Face
        normal[face1[i].vert2].yPos = (normal[face1[i].vert2].yPos + normalNew[1])/2;//Normal y-axis position of Vertex Face
        normal[face1[i].vert2].zPos = (normal[face1[i].vert2].zPos + normalNew[2])/2;//Normal z-axis position of Vertex Face

        normal[face1[i].vert3].xPos = (normal[face1[i].vert3].xPos + normalNew[0])/2;//Normal x-axis position of Vertex Face
        normal[face1[i].vert3].yPos = (normal[face1[i].vert3].yPos + normalNew[1])/2;//Normal y-axis position of Vertex Face
        normal[face1[i].vert3].zPos = (normal[face1[i].vert3].zPos + normalNew[2])/2;//Normal z-axis position of Vertex Face

    }
}

static void resize(int width, int height)
{
     double Ratio;

     Wdth = (float)width;
     Hight = (float)height;

     Ratio= (double)width /(double)height;

    glViewport(0,0,(GLsizei) width,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	gluPerspective (45.0f,Ratio,0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

 }

 static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0, 0, 10, 0.0, 0.0, 0.0, 0.0, 1.0, 100.0);

     if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME


    glTranslated(0.0,0.0,zoom);
    glRotated(Rotate_X_axis,1,0,0);
    glRotated(Rotate_Y_axis,0,1,0);

    glBindTexture(GL_TEXTURE_2D, tex);
    glPushMatrix();
       for(int i=0;i<f_Num;i++)
		{

        glBegin(GL_TRIANGLES);//To draw Triangles

        //Vertex 1
        glNormal3f(normal[face1[i].normal1].xPos,normal[face1[i].normal1].yPos,normal[face1[i].normal1].zPos);
        glVertex3f(vertex[face1[i].vert1].xPos,vertex[face1[i].vert1].yPos,vertex[face1[i].vert1].zPos);
        //Vertex 2
        glNormal3f(normal[face1[i].normal2].xPos,normal[face1[i].normal2].yPos,normal[face1[i].normal2].zPos);
        glVertex3f(vertex[face1[i].vert2].xPos,vertex[face1[i].vert2].yPos,vertex[face1[i].vert2].zPos);
        //Vertex 3
        glNormal3f(normal[face1[i].normal3].xPos,normal[face1[i].normal3].yPos,normal[face1[i].normal3].zPos);
        glVertex3f(vertex[face1[i].vert3].xPos,vertex[face1[i].vert3].yPos,vertex[face1[i].vert3].zPos);

            glEnd();

		}
    glPopMatrix();
    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q'://Quit screen
            exit(0);
            break;

        case 'w'://To show wireframe
            WireFrame =!WireFrame;
            break;

        case 'e':
            zoom+=10;//Zooming in
            break;
        case 'r':
            zoom-=10;//Zooming out
            break;
    }
}

void Specialkeys(int key, int x, int y)
{
     switch(key)
    {
    case GLUT_KEY_END:
	  zoom += (float) 1.0f;
		break;
	case GLUT_KEY_HOME:
	  zoom -= (float) 1.0f;
		break;
    case GLUT_KEY_UP://Rotation of X coordinate +ve side
	 	Rotate_X_axis = ((int)Rotate_X_axis +2)%360;
		break;
	case GLUT_KEY_DOWN:
		Rotate_X_axis = ((int)Rotate_X_axis -2)%360;//Rotation of X coordinate -ve side
		  break;
	case GLUT_KEY_LEFT:
    	Rotate_Y_axis =((int)Rotate_Y_axis +2)%360;//Rotation of Y coordinate +ve side
		break;
	case GLUT_KEY_RIGHT:
		Rotate_Y_axis = ((int)Rotate_Y_axis -2)%360;//Rotation of Y coordinate -ve side
		 break;
   }
  glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

void loadTexture(char* fileName)//Load Texture
{

    glGenTextures(1,&tex);
    //glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D, tex);
    int Cwidth, Cheight;
    unsigned char* image;

    image = SOIL_load_image(fileName,&Cwidth,&Cheight, 0,SOIL_LOAD_RGBA);

    if(!image)cout<<"fail to find image "<<fileName<<endl;

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,Cwidth, Cheight,0,GL_RGBA,GL_UNSIGNED_BYTE,image);
    SOIL_free_image_data(image);

    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
}

static void init(void)
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // assign a color you like

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

    //loadTexture("images/h.png");

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
}


/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(1920,1080);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("CSCI 272 Project 4");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    Obj_Load("bunny.obj");//Load Object
    Cal_Normal();//Calculating Normal
    glutIdleFunc(idle);

    glutMainLoop();

    return EXIT_SUCCESS;
}
