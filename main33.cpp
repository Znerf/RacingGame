#include <windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>



GLint Xsize=1000;
GLint Ysize=800;
float i,theta;
GLint nml=0,day=1;
GLint movf=0;
GLint movr=0;


GLfloat xt=0.0,yt=0.0,zt=0.0,xw=0.0;   // x,y,z translation factor
GLfloat tx=295,ty=62;
GLfloat xs=1.0,ys=1.0,zs=1.0;

GLfloat xangle=0.0,yangle=0.0,zangle=0.0,angle=0.0;   /* axis angles */

GLfloat r=0,g=0,b=1;
GLint light=1;
int count=1,flg=1;
int start=0;
int flag1=0,aflag=1;            //to switch car driving mode
int flag2=0,wheelflag=1;   //to switch fog effect

GLUquadricObj *t;

//static void SpecialKeyFunc( int Key, int x, int y );

float camxl=4,camyl=0,camzl=2;
float camx=1,camy=1,camz=1;
float camxL=0,camyL=1,camzL=0;

float colR=1,colG=1,colB=1;

/* Simple  transformation routine */
GLvoid Transform(GLfloat Width, GLfloat Height)
{
  glViewport(0, 0, Width, Height);              /* Set the viewport */
  glMatrixMode(GL_PROJECTION);                  /* Select the projection matrix */
  glLoadIdentity();				/* Reset The Projection Matrix */
  gluPerspective(45.0,Width/Height,0.1,100.0);  /* Calculate The Aspect Ratio Of The Window */
  glMatrixMode(GL_MODELVIEW);                   /* Switch back to the modelview matrix */
}


/* A general OpenGL initialization function.  Sets all of the initial parameters. */
GLvoid InitGL(GLfloat Width, GLfloat Height)
{

  glClearColor(1.0, 1.0, 1.0, 1.0);
  glLineWidth(2.0);              /* Add line width,   ditto */
  Transform( Width, Height ); /* Perform the transformation */
  //newly added
  t=gluNewQuadric();
        gluQuadricDrawStyle(t, GLU_FILL);

glEnable(GL_LIGHTING);

glEnable(GL_LIGHT0);

// Create light components
//GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
//GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
//GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
//GLfloat position[] = { 1.5f, 1.0f, 4.0f, 1.0f };


GLfloat ambientLight[] = { 0.4f, 0.4f, 0.4f, 1.0f };
GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat position[] = { -1.0f, -1.0f, -4.0f, 1.0f };

// Assign created components to GL_LIGHT0
glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
glLightfv(GL_LIGHT0, GL_POSITION, position);

}

/* The function called when our window is resized  */


void init()
{
    glClearColor(0,0,0,0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0,900.0,0.0,600.0,50.0,-50.0);
	glutPostRedisplay(); 		// request redisplay
}



void display(int x, int y, char *string, int font)
{
    int len,i;
	//glColor3f(0.8,0.52,1.0);
	glRasterPos2f(x, y);
    len = (int) strlen(string);
    for (i = 0; i < len; i++) {
    if(font==1)
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,string[i]);
	if(font==2)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,string[i]);
	if(font==3)
	        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,string[i]);
	if(font==4)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,string[i]);
	}

}

void display1(void)                       //THE FIRST WINDOW ON EXECUTING THE PROGRAM
{

	glClearColor(0.995,0.76,0.617,1.0);
	display(330,480,"Welcome to Main Menu",1);
	display(390,470-50,"Drive Car",2);
	display(385,470-100,"Car Model",2);
	display(380,470-150,"See Wheels",2);
	display(410,470-200,"Exit",2);

	glutPostRedisplay();
	glutSwapBuffers();

}

GLvoid DrawGLScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	/* Clear The Screen And The Depth Buffer */

    if(start==0)
    {
        init();
        display1();
    }
    else
    {
        if(count==1)
            InitGL(Xsize,Ysize);
        if(aflag==1)/* Initialize our window. */
            glClearColor(0.14,0.13,.8,1);       //day ma color change garne sky ko
        else
        glClearColor(0.5,0.1,0.1,0);    //night color change garne sky ko
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(-1.0,0.0,-3.5);
        glRotatef(xangle,1.0,0.0,0.0);
        glRotatef(yangle,0.0,1.0,0.0);
        glRotatef(zangle,0.0,0.0,1.0);
        glTranslatef(xt,yt,zt);
        glScalef(xs,ys,zs);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

        if(flag2==1)
        {
            GLfloat fogcolour[4]={.9,.9,.9,1.0};          //fog color

            glFogfv(GL_FOG_COLOR,fogcolour);              /* Define the fog colour */
            glFogf(GL_FOG_DENSITY,0.1);                   /* How dense */
            glFogi(GL_FOG_MODE,GL_EXP);                   /* exponential decay */
            glFogf(GL_FOG_START,3.0);                   /* Where wwe start fogging */
            glFogf(GL_FOG_END,100.0);                       /* end */
            glHint(GL_FOG_HINT, GL_FASTEST);              /* compute per vertex */
            glEnable(GL_FOG);/* ENABLE */
        }
        if(flag2==0)
        {
            glDisable(GL_FOG);
        }

        if(!aflag){
            glBegin(GL_POINTS);
            glColor3f(1,1,1);
            int ccount=0;
            float x=10,y=10;
            while(ccount<20)
            {
                glVertex2f(x,y);
                x+=10;
                y+=10;
                if(y>Ysize) y-=10;
                if(x>Xsize) x-=10;
                    ccount++;
            }
            glEnd();
        }
        gluLookAt(	camx, camy, camz,
			camxl, camyl,  camzl,
			camxL, camyL,  camzL);

        glColor3f(1.0,.75,0.0);

        glPushMatrix();
            glTranslated(0.2,0.3,0.3);
            glutSolidSphere(.035,50,50);
        glPopMatrix();
        glPushMatrix();
            glTranslated(0.2,0.3,0.5);
            glutSolidSphere(.035,50,50);
        glPopMatrix();
        glEnable(GL_NORMALIZE);
        glShadeModel(GL_SMOOTH);



        glBegin(GL_QUADS);                /* OBJECT MODULE*/

        /* top of cube*/
        //************************FRONT BODY****************************************
        glColor3f(.2f,0.5f,.9f);
        glNormal3f(-1.0f,0.0f,1.0f);
        glVertex3f( 0.2, 0.4,0.6);
        glNormal3f(1.0f,0.0f,1.0f);
        glVertex3f(0.6, 0.5,0.6);
        glNormal3f(1.0f,0.0f,1.0f);
        glVertex3f(0.6, 0.5,0.2);
        glNormal3f(-1.0f,0.0f,1.0f);
        glVertex3f( 0.2,0.4,0.2);

        /* bottom of cube*/

        glVertex3f( 0.2,0.2,0.6);
        glVertex3f(0.6,0.2,0.6);
        glVertex3f(0.6,0.2,0.2);
        glVertex3f( 0.2,0.2,0.2);

        /* front of cube*/
        glNormal3f(-1.0f,0.0f,1.0f);
        glVertex3f( 0.2,0.2,0.6);
        glNormal3f(1.0f,0.0f,1.0f);
        glVertex3f(0.2, 0.4,0.6);
        glNormal3f(1.0f,0.0f,1.0f);
        glVertex3f(0.2,0.4,0.2);
        glNormal3f(-1.0f,0.0f,1.0f);
        glVertex3f( 0.2,0.2,0.2);

        /* back of cube.*/
/*
        glColor3f(.5,.5,.5);
        glVertex3f(0.6,0.2,0.6);
        glVertex3f(0.6,0.5,0.6);
        glVertex3f(0.6,0.5,0.2);
        glVertex3f( 0.6,0.2,0.2);
        glColor3f(r,g,b);
  */
        /* left of cube*/
        glNormal3f(-1.0f, 0.0f, -1.0f);
        glVertex3f(0.2,0.2,0.6);
        glNormal3f(-1.0f, 0.0f, 1.0f);
        glVertex3f(0.6,0.2,0.6);
        glNormal3f(-1.0f, 0.0f, 1.0f);
        glVertex3f(0.6,0.5,0.6);
        glNormal3f(-1.0f, 0.0f, -1.0f);
        glVertex3f(0.2,0.4,0.6);

        /* Right of cube */
        glNormal3f(1.0f, 0.0f, -1.0f);
        glVertex3f(0.2,0.2,0.2);
        glNormal3f(1.0f, 0.0f, -1.0f);
        glVertex3f( 0.6,0.2,0.2);
        glNormal3f(1.0f, 0.0f, 1.0f);
        glVertex3f( 0.6,0.5,0.2);
        glNormal3f(1.0f, 0.0f, 1.0f);
        glVertex3f( 0.2,0.4,0.2);

        //****************************************************************************

        glNormal3f(-1.0f, 0.0f, 1.0f);
        glVertex3f(0.7,0.65,0.6);
        glNormal3f(1.0f, 0.0f, -1.0f);
        glVertex3f(0.7,0.65,0.2);
        glNormal3f(1.0f, 0.0f, -1.0f);
        glVertex3f(1.7,0.65,0.2);        //top cover
        glNormal3f(-1.0f, 0.0f, -1.0f);
        glVertex3f(1.7,0.65,0.6);
        //***************************back guard******************************

        //glColor3f(r,g,b);
                    /* Set The Color To Blue*/
        glNormal3f(-1.0f, 0.0f, 1.0f);
        glVertex3f( 1.8, 0.5,0.6);
        glNormal3f(1.0f, 0.0f, 1.0f);
        glVertex3f(1.8, 0.5,0.2);
        glNormal3f(1.0f, 0.0f, 1.0f);
        glVertex3f(2.1, 0.4, 0.2);
        glNormal3f(-1.0f, 0.0f, 1.0f);
        glVertex3f(2.1,0.4,0.6);

        /* bottom of cube*/
        glNormal3f(1.0f, 0.0f, -1.0f);
        glVertex3f( 2.1,0.2,0.6);
        glNormal3f(1.0f, 0.0f, 1.0f);
        glVertex3f(2.1,0.2,0.2);
        glNormal3f(1.0f, 0.0f, 1.0f);
        glVertex3f(1.8,0.2,0.6);
        glNormal3f(1.0f, 0.0f, -1.0f);
        glVertex3f( 1.8,0.2,0.6);

        /* back of cube.*/
        glNormal3f(-1.0f, 0.0f, -1.0f);
        glVertex3f(2.1,0.4,0.6);
        glNormal3f(-1.0f, 0.0f, -1.0f);
        glVertex3f(2.1,0.4,0.2);
        glNormal3f(1.0f, 0.0f, -1.0f);
        glVertex3f(2.1,0.2,0.2);
        glNormal3f(1.0f, 0.0f, -1.0f);
        glVertex3f(2.1,0.2,0.6);

        /* left of cube*/
        glNormal3f(-1.0f, 0.0f, -1.0f);
        glVertex3f(1.8,0.2,0.2);
        glNormal3f(-1.0f, 0.0f, 1.0f);
        glVertex3f(1.8,0.5,0.2);
        glNormal3f(-1.0f, 0.0f, 1.0f);
        glVertex3f(2.1,0.4,0.2);
        glNormal3f(-1.0f, 0.0f, -1.0f);
        glVertex3f(2.1,0.2,0.2);

        /* Right of cube */
        glNormal3f(1.0f, 0.0f, -1.0f);
        glVertex3f(1.8,0.2,0.6);
        glNormal3f(1.0f, 0.0f, -1.0f);
        glVertex3f(1.8,0.5,0.6);
        glNormal3f(1.0f, 0.0f, 1.0f);
        glVertex3f(2.1,0.4,0.6);
        glNormal3f(1.0f, 0.0f, 1.0f);
        glVertex3f(2.1,0.2,0.6);

        //******************MIDDLE BODY************************************
        glNormal3f(-1.0f, 0.0f, 1.0f);
        glVertex3f( 0.6, 0.5,0.6);
        glNormal3f(1.0f, 0.0f, 1.0f);
        glVertex3f(0.6, 0.2,0.6);
        glNormal3f(1.0f, 0.0f, 1.0f);
        glVertex3f(1.8, 0.2, 0.6);
        glNormal3f(-1.0f, 0.0f, 1.0f);
        glVertex3f(1.8,0.5,0.6);
        /* bottom of cube*/
        glVertex3f( 0.6,0.2,0.6);
        glVertex3f(0.6,0.2,0.2);
        glVertex3f(1.8,0.2,0.2);
        glVertex3f( 1.8,0.2,0.6);

        /* back of cube.*/
        glNormal3f(-1.0f, 0.0f, -1.0f);
        glVertex3f(0.6,0.5,0.2);
        glNormal3f(-1.0f, 0.0f, -1.0f);
        glVertex3f(0.6,0.2,0.2);
        glNormal3f(1.0f, 0.0f, -1.0f);
        glVertex3f(1.8,0.2,0.2);
        glNormal3f(1.0f, 0.0f, -1.0f);
        glVertex3f(1.8,0.5,0.2);


        //*********************ENTER WINDOW**********************************
        glColor3f(0.35,0.42,0.53);
        glNormal3f(1.0f, 0.0f, 1.0f);
        glVertex3f( 0.77, 0.63,0.2);
        glNormal3f(-1.0f, 0.0f, 1.0f);
        glVertex3f(0.75, 0.5,0.2);        //quad front window
        glNormal3f(-1.0f, 0.0f, 1.0f);
        glVertex3f(1.2, 0.5, 0.2);
        glNormal3f(1.0f, 0.0f, 1.0f);
        glVertex3f( 1.22,0.63,0.2);

        glNormal3f(-1.0f, 0.0f, -1.0f);
        glVertex3f(1.27,0.63,.2);
        glNormal3f(-1.0f, 0.0f, -1.0f);
        glVertex3f(1.25,0.5,0.2);        //quad back window
        glNormal3f(1.0f, 0.0f, -1.0f);
        glVertex3f(1.65,0.5,0.2);
        glNormal3f(1.0f, 0.0f, -1.0f);
        glVertex3f(1.67,0.63,0.2);

        glColor3f(.42,.55,b);

        glNormal3f(1.0f, 0.0f, -1.0f);
        glVertex3f(0.7,0.65,0.2);
        glNormal3f(1.0f, 0.0f, -1.0f);
        glVertex3f(0.7,0.5,.2);       //first separation
        glNormal3f(1.0f, 0.0f, 1.0f);
        glVertex3f(0.75,0.5,0.2);
        glNormal3f(1.0f, 0.0f, 1.0f);
        glVertex3f(0.77,0.65,0.2);

        glNormal3f(1.0f, 0.0f, -1.0f);
        glVertex3f(1.2,0.65,0.2);
        glNormal3f(-1.0f, 0.0f, -1.0f);
        glVertex3f(1.2,0.5,.2);       //second separation
        glNormal3f(-1.0f, 0.0f, 1.0f);
        glVertex3f(1.25,0.5,0.2);
        glNormal3f(1.0f, 0.0f, 1.0f);
        glVertex3f(1.27,0.65,0.2);

        glNormal3f(1.0f, 0.0f, -1.0f);
        glVertex3f(1.65,0.65,0.2);
        glNormal3f(-1.0f, 0.0f, -1.0f);
        glVertex3f(1.65,0.5,.2);     //3d separation
        glNormal3f(-1.0f, 0.0f, 1.0f);
        glVertex3f(1.7,0.5,0.2);
        glNormal3f(1.0f, 0.0f, 1.0f);
        glVertex3f(1.7,0.65,0.2);

        glNormal3f(1.0f, 0.0f, -1.0f);
        glVertex3f( 0.75, 0.65,0.2);
        glNormal3f(1.0f, 0.0f, -1.0f);
        glVertex3f(0.75, 0.63,0.2);        //line strip
        glNormal3f(1.0f, 0.0f, 1.0f);
        glVertex3f(1.7, 0.63, 0.2);
        glNormal3f(1.0f, 0.0f, 1.0f);
        glVertex3f( 1.7,0.65,0.2);

        glNormal3f(1.0f, 0.0f, -1.0f);
        glVertex3f( 0.75, 0.65,0.6);
        glNormal3f(1.0f, 0.0f, -1.0f);
        glVertex3f(0.75, 0.63,0.6);        //line strip
        glNormal3f(1.0f, 0.0f, 1.0f);
        glVertex3f(1.7, 0.63, 0.6);
        glNormal3f(1.0f, 0.0f, 1.0f);
        glVertex3f( 1.7,0.65,0.6);

        //left shading
        glColor3f(0.35,0.43,0.53);
        glNormal3f(-1.0f, 0.0f, -1.0f);
        glVertex3f( 0.77, 0.63,0.6);
        glNormal3f(-1.0f, 0.0f, 1.0f);
        glVertex3f(0.75, 0.5,0.6);        //quad front window
        glNormal3f(-1.0f, 0.0f, 1.0f);
        glVertex3f(1.2, 0.5, 0.6);
        glNormal3f(-1.0f, 0.0f, -1.0f);
        glVertex3f( 1.22,0.63,0.6);

        glNormal3f(-1.0f, 0.0f, -1.0f);
        glVertex3f(1.27,0.63,.6);
        glNormal3f(-1.0f, 0.0f, 1.0f);
        glVertex3f(1.25,0.5,0.6);        //quad back window
        glNormal3f(-1.0f, 0.0f, 1.0f);
        glVertex3f(1.65,0.5,0.6);
        glNormal3f(-1.0f, 0.0f, -1.0f);
        glVertex3f(1.67,0.63,0.6);

        glColor3f(.42f,.55f,b);
        glNormal3f(-1.0f, 0.0f, -1.0f);
        glVertex3f(0.7,0.65,0.6);
        glNormal3f(-1.0f, 0.0f, 1.0f);
        glVertex3f(0.7,0.5,.6);       //first separation
        glNormal3f(-1.0f, 0.0f, 1.0f);
        glVertex3f(0.75,0.5,0.6);
        glNormal3f(-1.0f, 0.0f, -1.0f);
        glVertex3f(0.77,0.65,0.6);

        glNormal3f(-1.0f, 0.0f, -1.0f);
        glVertex3f(1.2,0.65,0.6);
        glNormal3f(-1.0f, 0.0f, 1.0f);
        glVertex3f(1.2,0.5,.6);       //second separation
        glNormal3f(-1.0f, 0.0f, 1.0f);
        glVertex3f(1.25,0.5,0.6);
        glNormal3f(-1.0f, 0.0f, -1.0f);
        glVertex3f(1.27,0.65,0.6);

        glNormal3f(-1.0f, 0.0f, -1.0f);
        glVertex3f(1.65,0.65,0.6);
        glNormal3f(-1.0f, 0.0f, 1.0f);
        glVertex3f(1.65,0.5,.6);
        glNormal3f(-1.0f, 0.0f, 1.0f);
        glVertex3f(1.7,0.5,0.6);          //third seperation
        glNormal3f(-1.0f, 0.0f, -1.0f);
        glVertex3f(1.7,0.65,0.6);
        glEnd();


        //**************************************************************
        glBegin(GL_QUADS);

        /* top of cube*/

        glColor3f(0.35,0.42,0.53);

        glVertex3f( 0.6, 0.5,0.6);
        glVertex3f(0.6, 0.5,0.2);        //quad front window
        glVertex3f(0.7, 0.65, 0.2);
        glVertex3f( 0.7,0.65,0.6);

        glVertex3f(1.7,0.65,.6);
        glVertex3f(1.7,0.65,0.2);        //quad back window
        glVertex3f(1.8,0.5,0.2);
        glVertex3f(1.8,0.5,0.6);

            glEnd();


        if(wheelflag)
        {
            glPushMatrix();
            glTranslatef(xw,0,0);
            glColor3f(0.5,.2,0.3);
            glBegin(GL_QUADS);
            for(i=0;i<200;i+=0.2)
            {
                glVertex3f(-100+i,0,1);
                glVertex3f(-99.9+i,0,1);
                glVertex3f(-99.9+i,0.2,1);
                glVertex3f(-100+i,0.2,1);
                i+=0.5;
            }
            for(i=0;i<200;i+=0.2)
            {
                glVertex3f(-100+i,0,0);
                glVertex3f(-99.9+i,0,0);
                glVertex3f(-99.9+i,0.2,0);
                glVertex3f(-100+i,0.2,0);
                i+=0.5;
            }
            glEnd();
            glPopMatrix();
        }
        //*************************************************************************************************
        glBegin(GL_TRIANGLES);                /* start drawing the cube.*/

        /* top of cube*/
        glColor3f(0.3,0.3,0.3);
        glVertex3f( 0.6, 0.5,0.6);
        glVertex3f( 0.7,0.65,0.6);       //tri front window
        glVertex3f(0.7,0.5,0.6);

        glVertex3f( 0.6, 0.5,0.2);
        glVertex3f( 0.7,0.65,0.2);       //tri front window
        glVertex3f(0.7,0.5,0.2);

        glVertex3f( 1.7, 0.65,0.2);
        glVertex3f( 1.8,0.5,0.2);       //tri back window
        glVertex3f( 1.7,0.5,0.2);

        glVertex3f( 1.7, 0.65,0.6);
        glVertex3f( 1.8,0.5,0.6);       //tri back window
        glVertex3f(1.7,0.5,0.6);

        glEnd();
        //************IGNITION SYSTEM**********************************
        glPushMatrix();

        glColor3f(0.2,0.2,0.2);
        glTranslatef(1.63,0.19,0.3);
        glRotatef(90.0,0,1,0);
        gluCylinder(t,0.02,0.03,.48,10,10);
        glTranslatef(0,0,0.47);
        gluCylinder(t,0.03,0.015,.08,10,10);
        glTranslatef(0,0,0.08);
        gluCylinder(t,0.015,0.015,.02,10,10);
        glPopMatrix();

        //// 2nd ignition......


        glPushMatrix();

        glColor3f(0.2,0.2,0.2);
        glTranslatef(1.63,0.19,0.25);
        glRotatef(90.0,0,1,0);
        gluCylinder(t,0.02,0.03,.48,10,10);
        glTranslatef(0,0,0.47);
        gluCylinder(t,0.03,0.015,.08,10,10);
        glTranslatef(0,0,0.08);
        gluCylinder(t,0.015,0.015,.02,10,10);
        glPopMatrix();

        //////////  headlight..................
        glPushMatrix();

        glTranslatef(0.185,0.3,0.5);
        glRotatef(90,0,1,0);
        glColor3f(1,0,0);
        glutSolidSphere(0.034,20,20);
        glColor3f(0,0,0);
        glutSolidTorus(0.012,0.038,10,25);


        glTranslatef(0.2,0,0);
        glColor3f(1,0,0);
        glutSolidSphere(0.034,20,20);
        glColor3f(0,0,0);
        glutSolidTorus(0.012,0.038,10,25);



        glPopMatrix();




        //********************WHEEL*********************************************

        glColor3f(0.7,0.7,0.7);
        glPushMatrix();


        glBegin(GL_LINE_STRIP);
        for(theta=0;theta<360;theta=theta+20)
        {
            glVertex3f(0.6,0.2,0.62);
            glVertex3f(0.6+(0.08*(cos(((theta+angle)*3.14)/180))),0.2+(0.08*(sin(((theta+angle)*3.14)/180))),0.62);
        }
        glEnd();


        glBegin(GL_LINE_STRIP);
        for(theta=0;theta<360;theta=theta+20)
        {
            glVertex3f(0.6,0.2,0.18);
            glVertex3f(0.6+(0.08*(cos(((theta+angle)*3.14)/180))),0.2+(0.08*(sin(((theta+angle)*3.14)/180))),0.18);
        }
        glEnd();

        glBegin(GL_LINE_STRIP);
        for(theta=0;theta<360;theta=theta+20)
        {
            glVertex3f(1.7,0.2,0.18);
            glVertex3f(1.7+(0.08*(cos(((theta+angle)*3.14)/180))),0.2+(0.08*(sin(((theta+angle)*3.14)/180))),0.18);
        }
        glEnd();

        glBegin(GL_LINE_STRIP);
        for(theta=0;theta<360;theta=theta+20)
        {
            glVertex3f(1.7,0.2,0.62);
            glVertex3f(1.7+(0.08*(cos(((theta+angle)*3.14)/180))),0.2+(0.08*(sin(((theta+angle)*3.14)/180))),0.62);
        }
        glEnd();
        glTranslatef(0.6,0.2,0.6);
        glColor3f(0,0,0);
        glutSolidTorus(0.025,0.07,10,25);

        glTranslatef(0,0,-0.4);
        glutSolidTorus(0.025,0.07,10,25);

        glTranslatef(1.1,0,0);
        glutSolidTorus(0.025,0.07,10,25);

        glTranslatef(0,0,0.4);
        glutSolidTorus(0.025,0.07,10,25);
//******************** OBJECTS *******************************


        glTranslatef(-movf,0,movr);


         //*****************************road and surrounding development***********************************
        if(flag1)
         {

        glPushMatrix();

        glPushMatrix();

        glTranslatef(0,-0.09,0);

        glColor3f(0.1,0.1,0.1);
        glTranslatef(-10,0,-0.9);
        glBegin(GL_POLYGON);
        glVertex3f(0,0,0);
        glVertex3f(0,0,0.8);
        glVertex3f(110,0,0.8);/////// road 1
        glVertex3f(110,0,0);
        glEnd();



        glTranslatef(0,0,0.8);
        glBegin(GL_POLYGON);
        glColor3f(0.8,0.6,0.3);
        glVertex3f(0,0,0);    ////////median
        glVertex3f(0,0,0.2);
        glVertex3f(110,0,0.2);
        glVertex3f(110,0,0);
        glEnd();

         glTranslatef(0,0,0.2);
        glBegin(GL_POLYGON);
        glColor3f(0.1,0.1,0.1);
        glVertex3f(0,0,0);
        glVertex3f(0,0,0.8);                     // road 2
        glVertex3f(110,0,0.8);
        glVertex3f(110,0,0);
        glEnd();


        glTranslatef(110,0,0.8);
        glBegin(GL_POLYGON);
        glColor3f(0.1,0.1,0.1);
        glVertex3f(0,0,0);
        glVertex3f(2,0,0);//////////////ghumti
        glVertex3f(1.7,0,-1.4);
        glVertex3f(0,0,-1.8);
        glEnd();




        //////// 2222222


        glTranslatef(2,0,0);
        glRotatef(90,0,-1,0);

        glBegin(GL_POLYGON);
        glColor3f(0.1,0.1,0.1);
        glVertex3f(0,0,0);
        glVertex3f(0,0,0.8);
        glVertex3f(40,0,0.8);/////// road 1
        glVertex3f(40,0,0);
        glEnd();



        glTranslatef(0,0,0.8);
        glBegin(GL_POLYGON);
        glColor3f(0.8,0.6,0.3);
        glVertex3f(0,0,0);    ////////median
        glVertex3f(0,0,0.2);
        glVertex3f(40,0,0.2);
        glVertex3f(40,0,0);
        glEnd();

         glTranslatef(0,0,0.2);
        glBegin(GL_POLYGON);
        glColor3f(0.1,0.1,0.1);
        glVertex3f(0,0,0);
        glVertex3f(0,0,0.8);                     // road 2
        glVertex3f(40,0,0.8);
        glVertex3f(40,0,0);
        glEnd();


        glTranslatef(40,0,0.8);
        glRotatef(0,0,-1,0);
        glBegin(GL_POLYGON);
        glColor3f(0.1,0.1,0.1);
        glVertex3f(0,0,0);
        glVertex3f(2,0,0);//////////////ghumt2
        glVertex3f(1.7,0,-1.4);
        glVertex3f(0,0,-1.8);
        glEnd();



        //////3rd



        glTranslatef(2,0,0);
        glRotatef(90,0,-1,0);

        glBegin(GL_POLYGON);
        glColor3f(0.1,0.1,0.1);
        glVertex3f(0,0,0);
        glVertex3f(0,0,0.8);
        glVertex3f(110,0,0.8);/////// road 1
        glVertex3f(110,0,0);
        glEnd();



        glTranslatef(0,0,0.8);
        glBegin(GL_POLYGON);
        glColor3f(0.8,0.6,0.3);
        glVertex3f(0,0,0);    ////////median
        glVertex3f(0,0,0.2);
        glVertex3f(110,0,0.2);
        glVertex3f(110,0,0);
        glEnd();

         glTranslatef(0,0,0.2);
        glBegin(GL_POLYGON);
       glColor3f(0.1,0.1,0.1);
        glVertex3f(0,0,0);
        glVertex3f(0,0,0.8);                     // road 2
        glVertex3f(110,0,0.8);
        glVertex3f(110,0,0);
        glEnd();


        glTranslatef(110,0,0.8);
        glRotatef(0,0,-1,0);
        glBegin(GL_POLYGON);
        glColor3f(0.1,0.1,0.1);
        glVertex3f(0,0,0);
        glVertex3f(2,0,0);//////////////ghumt2
        glVertex3f(1.7,0,-1.4);
        glVertex3f(0,0,-1.8);
        glEnd();



/////////////  4th


        glTranslatef(2,0,0);
        glRotatef(90,0,-1,0);

        glBegin(GL_POLYGON);
        glColor3f(0.1,0.1,0.1);
        glVertex3f(0,0,0);
        glVertex3f(0,0,0.8);
        glVertex3f(40,0,0.8);/////// road 1
        glVertex3f(40,0,0);
        glEnd();



        glTranslatef(0,0,0.8);
        glBegin(GL_POLYGON);
        glColor3f(0.8,0.6,0.3);
        glVertex3f(0,0,0);    ////////median
        glVertex3f(0,0,0.2);
        glVertex3f(40,0,0.2);
        glVertex3f(40,0,0);
        glEnd();

         glTranslatef(0,0,0.2);
        glBegin(GL_POLYGON);
        glColor3f(0.1,0.1,0.1);
        glVertex3f(0,0,0);
        glVertex3f(0,0,0.8);                     // road 2
        glVertex3f(40,0,0.8);
        glVertex3f(40,0,0);
        glEnd();


        glTranslatef(40,0,0.8);
        glRotatef(0,0,-1,0);
        glBegin(GL_POLYGON);
        glColor3f(0.1,0.1,0.1);
        glVertex3f(0,0,0);
        glVertex3f(2,0,0);//////////////ghumt2
        glVertex3f(1.7,0,-1.4);
        glVertex3f(0,0,-1.8);
        glEnd();

        glPopMatrix();


        ////////// green plane..............


        glTranslatef(0,-0.1,0);
        glColor3f(0.2,0.8,0.2);
        glBegin(GL_QUADS);
        glVertex3f(500,0,500);
        glVertex3f(500,0,-500);//////////////ghumt2
        glVertex3f(-500,0,-500);
        glVertex3f(-500,0,500);
        glEnd();

        glPopMatrix();

///////////////...   dhunga haru...................///////////////
        glPushMatrix();
        glColor3f(0.6,0.6,0.6);
        glTranslatef(9,0.1,10);
        glScalef(1.5,1.3,2);
        glutSolidSphere(1,3,3);


        glTranslatef(20,1,7);
        glScalef(2.5,1,1);
        glutSolidDodecahedron();

        glColor3f(0.8,0.8,0.8);
        glTranslatef(-2,-1,-8);
        glScalef(1.5,1.3,2);
        glutSolidSphere(1,3,3);

        glTranslatef(20,0,-10);
        glutSolidDodecahedron();

        glTranslatef(20,0,13);
        glutSolidDodecahedron();


        glPopMatrix();


        ///////// dhungas on left line        ....

        glPushMatrix();


        glTranslatef(10,0,-15);

        glPushMatrix();
        glColor3f(0.6,0.6,0.6);
        glTranslatef(9,0.1,10);
        glScalef(1.5,1.3,2);
        glutSolidSphere(1,3,3);


        glTranslatef(20,1,-7);
        glScalef(2.5,1,1);
        glutSolidDodecahedron();

        glColor3f(0.8,0.8,0.8);
        glTranslatef(10,-1,0);
        glScalef(1.5,1.3,2);
        glutSolidSphere(1,3,3);


        glPopMatrix();

        glPopMatrix();



        /////************  street light ********************///////////

        glPushMatrix();

        glTranslatef(0,0,0.7);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();


        //////// 2
        glPushMatrix();

        glTranslatef(0,0,-0.7);
        glRotatef(180,0,1,0);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();



        /////// making copy of street light


        glPushMatrix();
        glTranslatef(20,0,0);

        glPushMatrix();

        glTranslatef(0,0,0.5);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();



        glPushMatrix();

        glTranslatef(0,0,-0.7);
        glRotatef(180,0,1,0);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();

        glPopMatrix();

        //////////   end of copy

        /////// making copy of street light


        glPushMatrix();
        glTranslatef(40,0,0);

        glPushMatrix();

        glTranslatef(0,0,0.5);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();



        glPushMatrix();

        glTranslatef(0,0,-0.7);
        glRotatef(180,0,1,0);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();

        glPopMatrix();

        //////////   end of copy



         /////// making copy of street light


        glPushMatrix();
        glTranslatef(60,0,0);

        glPushMatrix();

        glTranslatef(0,0,0.5);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();



        glPushMatrix();

        glTranslatef(0,0,-0.7);
        glRotatef(180,0,1,0);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();

        glPopMatrix();

        //////////   end of copy




         /////// making copy of street light


        glPushMatrix();
        glTranslatef(80,0,0);

        glPushMatrix();

        glTranslatef(0,0,0.5);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();



        glPushMatrix();

        glTranslatef(0,0,-0.7);
        glRotatef(180,0,1,0);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();

        glPopMatrix();

        //////////   end of copy



        /////// making copy of street light


        glPushMatrix();

        glTranslatef(101,0,5);
        glRotatef(90,0,-1,0);
        glPushMatrix();

        glTranslatef(0,0,0.5);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();



        glPushMatrix();

        glTranslatef(0,0,-0.7);
        glRotatef(180,0,1,0);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();

        glPopMatrix();

        //////////   end of copy


         /////// making copy of street light


        glPushMatrix();

        glTranslatef(101,0,15);
        glRotatef(90,0,-1,0);
        glPushMatrix();

        glTranslatef(0,0,0.5);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();



        glPushMatrix();

        glTranslatef(0,0,-0.7);
        glRotatef(180,0,1,0);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();

        glPopMatrix();

        //////////   end of copy


          /////// making copy of street light


        glPushMatrix();

        glTranslatef(101,0,25);
        glRotatef(90,0,-1,0);
        glPushMatrix();

        glTranslatef(0,0,0.5);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();



        glPushMatrix();

        glTranslatef(0,0,-0.7);
        glRotatef(180,0,1,0);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();

        glPopMatrix();

        //////////   end of copy




        /////// making copy of street light


        glPushMatrix();

        glTranslatef(101,0,35);
        glRotatef(90,0,-1,0);
        glPushMatrix();

        glTranslatef(0,0,0.5);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();



        glPushMatrix();

        glTranslatef(0,0,-0.7);
        glRotatef(180,0,1,0);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();

        glPopMatrix();

        //////////   end of copy



  /////// making copy of street light


        glPushMatrix();

        glTranslatef(90,0,42.1);
        glRotatef(0,0,-1,0);
        glPushMatrix();

        glTranslatef(0,0,0.5);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();



        glPushMatrix();

        glTranslatef(0,0,-0.7);
        glRotatef(180,0,1,0);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();

        glPopMatrix();

        //////////   end of copy

        /////// making copy of street light


        glPushMatrix();

        glTranslatef(70,0,42.1);
        glRotatef(0,0,-1,0);
        glPushMatrix();

        glTranslatef(0,0,0.5);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();



        glPushMatrix();

        glTranslatef(0,0,-0.7);
        glRotatef(180,0,1,0);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();

        glPopMatrix();

        //////////   end of copy



        /////// making copy of street light


        glPushMatrix();

        glTranslatef(50,0,42.1);
        glRotatef(0,0,-1,0);
        glPushMatrix();

        glTranslatef(0,0,0.5);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();



        glPushMatrix();

        glTranslatef(0,0,-0.7);
        glRotatef(180,0,1,0);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();

        glPopMatrix();

        //////////   end of copy






        /////// making copy of street light


        glPushMatrix();

        glTranslatef(30,0,42.1);
        glRotatef(0,0,-1,0);
        glPushMatrix();

        glTranslatef(0,0,0.5);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();



        glPushMatrix();

        glTranslatef(0,0,-0.7);
        glRotatef(180,0,1,0);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();

        glPopMatrix();

        //////////   end of copy

        /////// making copy of street light


        glPushMatrix();

        glTranslatef(10,0,42.1);
        glRotatef(0,0,-1,0);
        glPushMatrix();

        glTranslatef(0,0,0.5);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();



        glPushMatrix();

        glTranslatef(0,0,-0.7);
        glRotatef(180,0,1,0);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();

        glPopMatrix();

        //////////   end of copy



         /////// making copy of street light


        glPushMatrix();

        glTranslatef(-5,0,42.1);
        glRotatef(0,0,-1,0);
        glPushMatrix();

        glTranslatef(0,0,0.5);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();



        glPushMatrix();

        glTranslatef(0,0,-0.7);
        glRotatef(180,0,1,0);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();

        glPopMatrix();

        //////////   end of copy


         /////// making copy of street light


        glPushMatrix();

        glTranslatef(-11,0,35);
        glRotatef(90,0,-1,0);
        glPushMatrix();

        glTranslatef(0,0,0.5);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();



        glPushMatrix();

        glTranslatef(0,0,-0.7);
        glRotatef(180,0,1,0);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();

        glPopMatrix();

        //////////   end of copy



         /////// making copy of street light


        glPushMatrix();

        glTranslatef(-11,0,25);
        glRotatef(90,0,-1,0);
        glPushMatrix();

        glTranslatef(0,0,0.5);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();



        glPushMatrix();

        glTranslatef(0,0,-0.7);
        glRotatef(180,0,1,0);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();

        glPopMatrix();

        //////////   end of copy



         /////// making copy of street light


        glPushMatrix();

        glTranslatef(-11,0,15);
        glRotatef(90,0,-1,0);
        glPushMatrix();

        glTranslatef(0,0,0.5);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();



        glPushMatrix();

        glTranslatef(0,0,-0.7);
        glRotatef(180,0,1,0);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();

        glPopMatrix();

        //////////   end of copy




         /////// making copy of street light


        glPushMatrix();

        glTranslatef(-11,0,5);
        glRotatef(90,0,-1,0);
        glPushMatrix();

        glTranslatef(0,0,0.5);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();



        glPushMatrix();

        glTranslatef(0,0,-0.7);
        glRotatef(180,0,1,0);
        glScalef(0.5,0.5,0.5);

        glRotatef(-90,1,0,0);
        glColor3f(0.3,0.3,0.3);
        glTranslatef(0,-1,-0.1);
        glutSolidTorus(0.2,0.3,8,10);
        glColor3f(0.35,0.35,0.35);
        gluCylinder(t,0.2,0.1,3,10,10);
        glTranslatef(0,0,3);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,-0.02,0.44);
        glRotatef(-55,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.47);
        glRotatef(-20,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0,0.4);
        glRotatef(-10,1,0,0);
        gluCylinder(t,0.1,0.1,0.5,10,10);
        glTranslatef(0,0.2,0.4);
        glColor3f(1,1,1);
        glutSolidSphere(0.35,20,20);
        glPopMatrix();

        glPopMatrix();

        //////////   end of copy







        //////////////////   TREES      /////////////////

        glPushMatrix();




        glTranslatef(10,-0.5,0);
        glPushMatrix();
        glTranslatef(0,2.8,2);
        glColor3f(0.1,0.5,0.1);
        glScalef(2.5,1.3,1);
        glutSolidSphere(1,5,5);
        glScalef(1,0.65,1);
        glColor3f(0.3,0.1,0.1);
        glTranslatef(0,-3,0);
        glRotatef(90.0,-1,0,0);
        gluCylinder(t,0.3,0.1,3,3,10);
        glPopMatrix();

       //  copy of tree
        glTranslatef(20,0,0);
        glPushMatrix();
        glTranslatef(0,2.8,2);
        glColor3f(0.1,0.5,0.1);
        glScalef(2.5,1.3,1);
        glutSolidSphere(1,5,5);
        glScalef(1,0.65,1);
        glColor3f(0.3,0.1,0.1);
        glTranslatef(0,-3,0);
        glRotatef(90.0,-1,0,0);
        gluCylinder(t,0.3,0.1,3,3,10);
        glPopMatrix();

        //  copy of tree
        glTranslatef(20,0,0);
        glPushMatrix();
        glTranslatef(0,2.8,2);
        glColor3f(0.1,0.5,0.1);
        glScalef(2.5,1.3,1);
        glutSolidSphere(1,5,5);
        glScalef(1,0.65,1);
        glColor3f(0.3,0.1,0.1);
        glTranslatef(0,-3,0);
        glRotatef(90.0,-1,0,0);
        gluCylinder(t,0.3,0.1,3,3,10);
        glPopMatrix();

        //  copy of tree
        glTranslatef(20,0,0);
        glPushMatrix();
        glTranslatef(0,2.8,2);
        glColor3f(0.1,0.5,0.1);
        glScalef(2.5,1.3,1);
        glutSolidSphere(1,5,5);
        glScalef(1,0.65,1);
        glColor3f(0.3,0.1,0.1);
        glTranslatef(0,-3,0);
        glRotatef(90.0,-1,0,0);
        gluCylinder(t,0.3,0.1,3,3,10);
        glPopMatrix();

        //  copy of tree
        glTranslatef(20,0,0);
        glPushMatrix();
        glTranslatef(0,2.8,2);
        glColor3f(0.1,0.5,0.1);
        glScalef(2.5,1.3,1);
        glutSolidSphere(1,5,5);
        glScalef(1,0.65,1);
        glColor3f(0.3,0.1,0.1);
        glTranslatef(0,-3,0);
        glRotatef(90.0,-1,0,0);
        gluCylinder(t,0.3,0.1,3,3,10);
        glPopMatrix();


        glPopMatrix();

        ////////  left column.............

        glPushMatrix();

        glTranslatef(5,-0.5,-8);


         glTranslatef(20,0,0);
        glPushMatrix();
        glTranslatef(0,2.8,2);
        glColor3f(0.1,0.5,0.1);
        glScalef(2.5,1.3,1);
        glutSolidSphere(1,5,5);
        glScalef(1,0.65,1);
        glColor3f(0.3,0.1,0.1);
        glTranslatef(0,-3,0);
        glRotatef(90.0,-1,0,0);
        gluCylinder(t,0.3,0.1,3,3,10);
        glPopMatrix();

       //  copy of tree
        glTranslatef(20,0,0);
        glPushMatrix();
        glTranslatef(0,2.8,2);
        glColor3f(0.1,0.5,0.1);
        glScalef(2.5,1.3,1);
        glutSolidSphere(1,5,5);
        glScalef(1,0.65,1);
        glColor3f(0.3,0.1,0.1);
        glTranslatef(0,-3,0);
        glRotatef(90.0,-1,0,0);
        gluCylinder(t,0.3,0.1,3,3,10);
        glPopMatrix();

        //  copy of tree
        glTranslatef(20,0,0);
        glPushMatrix();
        glTranslatef(0,2.8,2);
        glColor3f(0.1,0.5,0.1);
        glScalef(2.5,1.3,1);
        glutSolidSphere(1,5,5);
        glScalef(1,0.65,1);
        glColor3f(0.3,0.1,0.1);
        glTranslatef(0,-3,0);
        glRotatef(90.0,-1,0,0);
        gluCylinder(t,0.3,0.1,3,3,10);
        glPopMatrix();

        //  copy of tree
        glTranslatef(20,0,0);
        glPushMatrix();
        glTranslatef(0,2.8,2);
        glColor3f(0.1,0.5,0.1);
        glScalef(2.5,1.3,1);
        glutSolidSphere(1,5,5);
        glScalef(1,0.65,1);
        glColor3f(0.3,0.1,0.1);
        glTranslatef(0,-3,0);
        glRotatef(90.0,-1,0,0);
        gluCylinder(t,0.3,0.1,3,3,10);
        glPopMatrix();

        //  copy of tree
        glTranslatef(20,0,0);
        glPushMatrix();
        glTranslatef(0,2.8,2);
        glColor3f(0.1,0.5,0.1);
        glScalef(2.5,1.3,1);
        glutSolidSphere(1,5,5);
        glScalef(1,0.65,1);
        glColor3f(0.3,0.1,0.1);
        glTranslatef(0,-3,0);
        glRotatef(90.0,-1,0,0);
        gluCylinder(t,0.3,0.1,3,3,10);
        glPopMatrix();




        glPopMatrix();



        ///// tree for right side..............

         glPushMatrix();

        glTranslatef(5,-0.5,28);


         glTranslatef(20,0,0);
        glPushMatrix();
        glTranslatef(0,2.8,2);
        glColor3f(0.1,0.5,0.1);
        glScalef(2.5,1.3,1);
        glutSolidSphere(1,5,5);
        glScalef(1,0.65,1);
        glColor3f(0.3,0.1,0.1);
        glTranslatef(0,-3,0);
        glRotatef(90.0,-1,0,0);
        gluCylinder(t,0.3,0.1,3,3,10);
        glPopMatrix();

       //  copy of tree
        glTranslatef(20,0,0);
        glPushMatrix();
        glTranslatef(0,2.8,2);
        glColor3f(0.1,0.5,0.1);
        glScalef(2.5,1.3,1);
        glutSolidSphere(1,5,5);
        glScalef(1,0.65,1);
        glColor3f(0.3,0.1,0.1);
        glTranslatef(0,-3,0);
        glRotatef(90.0,-1,0,0);
        gluCylinder(t,0.3,0.1,3,3,10);
        glPopMatrix();

        //  copy of tree
        glTranslatef(20,0,0);
        glPushMatrix();
        glTranslatef(0,2.8,2);
        glColor3f(0.1,0.5,0.1);
        glScalef(2.5,1.3,1);
        glutSolidSphere(1,5,5);
        glScalef(1,0.65,1);
        glColor3f(0.3,0.1,0.1);
        glTranslatef(0,-3,0);
        glRotatef(90.0,-1,0,0);
        gluCylinder(t,0.3,0.1,3,3,10);
        glPopMatrix();

        //  copy of tree
        glTranslatef(20,0,0);
        glPushMatrix();
        glTranslatef(0,2.8,2);
        glColor3f(0.1,0.5,0.1);
        glScalef(2.5,1.3,1);
        glutSolidSphere(1,5,5);
        glScalef(1,0.65,1);
        glColor3f(0.3,0.1,0.1);
        glTranslatef(0,-3,0);
        glRotatef(90.0,-1,0,0);
        gluCylinder(t,0.3,0.1,3,3,10);
        glPopMatrix();

        //  copy of tree
        glTranslatef(20,0,0);
        glPushMatrix();
        glTranslatef(0,2.8,2);
        glColor3f(0.1,0.5,0.1);
        glScalef(2.5,1.3,1);
        glutSolidSphere(1,5,5);
        glScalef(1,0.65,1);
        glColor3f(0.3,0.1,0.1);
        glTranslatef(0,-3,0);
        glRotatef(90.0,-1,0,0);
        gluCylinder(t,0.3,0.1,3,3,10);
        glPopMatrix();




        glPopMatrix();



        ////right side extra trees

         glPushMatrix();

        glTranslatef(-7,-0.5,37);


         glTranslatef(20,0,0);
        glPushMatrix();
        glTranslatef(0,2.8,2);
        glColor3f(0.1,0.5,0.1);
        glScalef(2.5,1.3,1);
        glutSolidSphere(1,5,5);
        glScalef(1,0.65,1);
        glColor3f(0.3,0.1,0.1);
        glTranslatef(0,-3,0);
        glRotatef(90.0,-1,0,0);
        gluCylinder(t,0.3,0.1,3,3,10);
        glPopMatrix();

       //  copy of tree
        glTranslatef(20,0,0);
        glPushMatrix();
        glTranslatef(0,2.8,2);
        glColor3f(0.1,0.5,0.1);
        glScalef(2.5,1.3,1);
        glutSolidSphere(1,5,5);
        glScalef(1,0.65,1);
        glColor3f(0.3,0.1,0.1);
        glTranslatef(0,-3,0);
        glRotatef(90.0,-1,0,0);
        gluCylinder(t,0.3,0.1,3,3,10);
        glPopMatrix();

        //  copy of tree
        glTranslatef(20,0,0);
        glPushMatrix();
        glTranslatef(0,2.8,2);
        glColor3f(0.1,0.5,0.1);
        glScalef(2.5,1.3,1);
        glutSolidSphere(1,5,5);
        glScalef(1,0.65,1);
        glColor3f(0.3,0.1,0.1);
        glTranslatef(0,-3,0);
        glRotatef(90.0,-1,0,0);
        gluCylinder(t,0.3,0.1,3,3,10);
        glPopMatrix();

        //  copy of tree
        glTranslatef(20,0,0);
        glPushMatrix();
        glTranslatef(0,2.8,2);
        glColor3f(0.1,0.5,0.1);
        glScalef(2.5,1.3,1);
        glutSolidSphere(1,5,5);
        glScalef(1,0.65,1);
        glColor3f(0.3,0.1,0.1);
        glTranslatef(0,-3,0);
        glRotatef(90.0,-1,0,0);
        gluCylinder(t,0.3,0.1,3,3,10);
        glPopMatrix();

        //  copy of tree
        glTranslatef(20,0,0);
        glPushMatrix();
        glTranslatef(0,2.8,2);
        glColor3f(0.1,0.5,0.1);
        glScalef(2.5,1.3,1);
        glutSolidSphere(1,5,5);
        glScalef(1,0.65,1);
        glColor3f(0.3,0.1,0.1);
        glTranslatef(0,-3,0);
        glRotatef(90.0,-1,0,0);
        gluCylinder(t,0.3,0.1,3,3,10);
        glPopMatrix();




        glPopMatrix();



        ///////////// Traingular tree ..... 1st.....

        glTranslatef(0,0,8);

        GLfloat tsl=6,tst=6;
        glPushMatrix();

        glRotatef(90,-1,0,0);
        glColor3f(0.1,0.5,0.1);
        glScalef(2,2,2);

        glTranslatef(2,-3,1.5);
        glutSolidCone(0.3,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.4,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.5,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.6,0.5,tsl,tst);
        glTranslatef(0,0,-0.8);
        glColor3f(0.3,0.2,0.1);
        gluCylinder(t,0.18,0.1,0.9,10,10);

        glPopMatrix();

        ////

        //............t tree copy .....
        glTranslatef(10,0,0);

        glPushMatrix();

        glRotatef(90,-1,0,0);
        glColor3f(0.1,0.5,0.1);
        glScalef(2,2,2);

        glTranslatef(2,-3,1.5);
        glutSolidCone(0.3,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.4,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.5,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.6,0.5,tsl,tst);
        glTranslatef(0,0,-0.8);
        glColor3f(0.3,0.2,0.1);
        gluCylinder(t,0.18,0.1,0.9,10,10);

        glPopMatrix();
       ///////////// end of copy........


        //............t tree copy .....
        glTranslatef(10,0,0);

        glPushMatrix();

        glRotatef(90,-1,0,0);
        glColor3f(0.1,0.5,0.1);
        glScalef(2,2,2);

        glTranslatef(2,-3,1.5);
        glutSolidCone(0.3,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.4,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.5,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.6,0.5,tsl,tst);
        glTranslatef(0,0,-0.8);
        glColor3f(0.3,0.2,0.1);
        gluCylinder(t,0.18,0.1,0.9,10,10);

        glPopMatrix();
       ///////////// end of copy........


        //............t tree copy .....
        glTranslatef(10,0,0);

        glPushMatrix();

        glRotatef(90,-1,0,0);
        glColor3f(0.1,0.5,0.1);
        glScalef(2,2,2);

        glTranslatef(2,-3,1.5);
        glutSolidCone(0.3,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.4,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.5,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.6,0.5,tsl,tst);
        glTranslatef(0,0,-0.8);
        glColor3f(0.3,0.2,0.1);
        gluCylinder(t,0.18,0.1,0.9,10,10);

        glPopMatrix();
       ///////////// end of copy........





        ///////////// Traingular tree ..... 2nd.....

        glTranslatef(-10,0,-10);
        glPushMatrix();

        glRotatef(90,-1,0,0);
        glColor3f(0.1,0.5,0.1);
        glScalef(2,2,2);

        glTranslatef(2,-3,1.5);
        glutSolidCone(0.3,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.4,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.5,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.6,0.5,tsl,tst);
        glTranslatef(0,0,-0.8);
        glColor3f(0.3,0.2,0.1);
        gluCylinder(t,0.18,0.1,0.9,10,10);

        glPopMatrix();

        ////

        //............t tree copy .....
        glTranslatef(10,0,0);

        glPushMatrix();

        glRotatef(90,-1,0,0);
        glColor3f(0.1,0.5,0.1);
        glScalef(2,2,2);

        glTranslatef(2,-3,1.5);
        glutSolidCone(0.3,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.4,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.5,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.6,0.5,tsl,tst);
        glTranslatef(0,0,-0.8);
        glColor3f(0.3,0.2,0.1);
        gluCylinder(t,0.18,0.1,0.9,10,10);

        glPopMatrix();
       ///////////// end of copy........


        //............t tree copy .....
        glTranslatef(10,0,0);

        glPushMatrix();

        glRotatef(90,-1,0,0);
        glColor3f(0.1,0.5,0.1);
        glScalef(2,2,2);

        glTranslatef(2,-3,1.5);
        glutSolidCone(0.3,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.4,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.5,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.6,0.5,tsl,tst);
        glTranslatef(0,0,-0.8);
        glColor3f(0.3,0.2,0.1);
        gluCylinder(t,0.18,0.1,0.9,10,10);

        glPopMatrix();
       ///////////// end of copy........


        //............t tree copy .....
        glTranslatef(10,0,0);

        glPushMatrix();

        glRotatef(90,-1,0,0);
        glColor3f(0.1,0.5,0.1);
        glScalef(2,2,2);

        glTranslatef(2,-3,1.5);
        glutSolidCone(0.3,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.4,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.5,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.6,0.5,tsl,tst);
        glTranslatef(0,0,-0.8);
        glColor3f(0.3,0.2,0.1);
        gluCylinder(t,0.18,0.1,0.9,10,10);

        glPopMatrix();
       ///////////// end of copy........





       /////////// t tree left side.......

        glTranslatef(-40,0,-8);

       ///////////// Traingular tree ..... 1st.....





        glPushMatrix();

        glRotatef(90,-1,0,0);
        glColor3f(0.1,0.5,0.1);
        glScalef(2,2,2);

        glTranslatef(2,-3,1.5);
        glutSolidCone(0.3,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.4,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.5,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.6,0.5,tsl,tst);
        glTranslatef(0,0,-0.8);
        glColor3f(0.3,0.2,0.1);
        gluCylinder(t,0.18,0.1,0.9,10,10);

        glPopMatrix();

        ////

        //............t tree copy .....
        glTranslatef(10,0,0);

        glPushMatrix();

        glRotatef(90,-1,0,0);
        glColor3f(0.1,0.5,0.1);
        glScalef(2,2,2);

        glTranslatef(2,-3,1.5);
        glutSolidCone(0.3,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.4,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.5,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.6,0.5,tsl,tst);
        glTranslatef(0,0,-0.8);
        glColor3f(0.3,0.2,0.1);
        gluCylinder(t,0.18,0.1,0.9,10,10);

        glPopMatrix();
       ///////////// end of copy........


        //............t tree copy .....
        glTranslatef(10,0,0);

        glPushMatrix();

        glRotatef(90,-1,0,0);
        glColor3f(0.1,0.5,0.1);
        glScalef(2,2,2);

        glTranslatef(2,-3,1.5);
        glutSolidCone(0.3,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.4,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.5,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.6,0.5,tsl,tst);
        glTranslatef(0,0,-0.8);
        glColor3f(0.3,0.2,0.1);
        gluCylinder(t,0.18,0.1,0.9,10,10);

        glPopMatrix();
       ///////////// end of copy........


        //............t tree copy .....
        glTranslatef(10,0,0);

        glPushMatrix();

        glRotatef(90,-1,0,0);
        glColor3f(0.1,0.5,0.1);
        glScalef(2,2,2);

        glTranslatef(2,-3,1.5);
        glutSolidCone(0.3,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.4,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.5,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.6,0.5,tsl,tst);
        glTranslatef(0,0,-0.8);
        glColor3f(0.3,0.2,0.1);
        gluCylinder(t,0.18,0.1,0.9,10,10);

        glPopMatrix();
       ///////////// end of copy........





        ///////////// Traingular tree ..... 2nd.....

        glTranslatef(-10,0,-7);
        glPushMatrix();

        glRotatef(90,-1,0,0);
        glColor3f(0.1,0.5,0.1);
        glScalef(2,2,2);

        glTranslatef(2,-3,1.5);
        glutSolidCone(0.3,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.4,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.5,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.6,0.5,tsl,tst);
        glTranslatef(0,0,-0.8);
        glColor3f(0.3,0.2,0.1);
        gluCylinder(t,0.18,0.1,0.9,10,10);

        glPopMatrix();

        ////

        //............t tree copy .....
        glTranslatef(10,0,0);

        glPushMatrix();

        glRotatef(90,-1,0,0);
        glColor3f(0.1,0.5,0.1);
        glScalef(2,2,2);

        glTranslatef(2,-3,1.5);
        glutSolidCone(0.3,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.4,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.5,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.6,0.5,tsl,tst);
        glTranslatef(0,0,-0.8);
        glColor3f(0.3,0.2,0.1);
        gluCylinder(t,0.18,0.1,0.9,10,10);

        glPopMatrix();
       ///////////// end of copy........


        //............t tree copy .....
        glTranslatef(10,0,0);

        glPushMatrix();

        glRotatef(90,-1,0,0);
        glColor3f(0.1,0.5,0.1);
        glScalef(2,2,2);

        glTranslatef(2,-3,1.5);
        glutSolidCone(0.3,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.4,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.5,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.6,0.5,tsl,tst);
        glTranslatef(0,0,-0.8);
        glColor3f(0.3,0.2,0.1);
        gluCylinder(t,0.18,0.1,0.9,10,10);

        glPopMatrix();
       ///////////// end of copy........


        //............t tree copy .....
        glTranslatef(10,0,0);

        glPushMatrix();

        glRotatef(90,-1,0,0);
        glColor3f(0.1,0.5,0.1);
        glScalef(2,2,2);

        glTranslatef(2,-3,1.5);
        glutSolidCone(0.3,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.4,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.5,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.6,0.5,tsl,tst);
        glTranslatef(0,0,-0.8);
        glColor3f(0.3,0.2,0.1);
        gluCylinder(t,0.18,0.1,0.9,10,10);

        glPopMatrix();
       ///////////// end of copy........








////////// T TREE 2nd side




        glPushMatrix();
        glTranslatef(-50,0,70);


        glPushMatrix();

        glRotatef(90,-1,0,0);
        glColor3f(0.1,0.5,0.1);
        glScalef(2,2,2);

        glTranslatef(2,-3,1.5);
        glutSolidCone(0.3,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.4,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.5,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.6,0.5,tsl,tst);
        glTranslatef(0,0,-0.8);
        glColor3f(0.3,0.2,0.1);
        gluCylinder(t,0.18,0.1,0.9,10,10);

        glPopMatrix();

        ////

        //............t tree copy .....
        glTranslatef(10,0,0);

        glPushMatrix();

        glRotatef(90,-1,0,0);
        glColor3f(0.1,0.5,0.1);
        glScalef(2,2,2);

        glTranslatef(2,-3,1.5);
        glutSolidCone(0.3,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.4,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.5,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.6,0.5,tsl,tst);
        glTranslatef(0,0,-0.8);
        glColor3f(0.3,0.2,0.1);
        gluCylinder(t,0.18,0.1,0.9,10,10);

        glPopMatrix();
       ///////////// end of copy........


        //............t tree copy .....
        glTranslatef(10,0,0);

        glPushMatrix();

        glRotatef(90,-1,0,0);
        glColor3f(0.1,0.5,0.1);
        glScalef(2,2,2);

        glTranslatef(2,-3,1.5);
        glutSolidCone(0.3,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.4,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.5,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.6,0.5,tsl,tst);
        glTranslatef(0,0,-0.8);
        glColor3f(0.3,0.2,0.1);
        gluCylinder(t,0.18,0.1,0.9,10,10);

        glPopMatrix();
       ///////////// end of copy........


        //............t tree copy .....
        glTranslatef(10,0,0);

        glPushMatrix();

        glRotatef(90,-1,0,0);
        glColor3f(0.1,0.5,0.1);
        glScalef(2,2,2);

        glTranslatef(2,-3,1.5);
        glutSolidCone(0.3,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.4,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.5,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.6,0.5,tsl,tst);
        glTranslatef(0,0,-0.8);
        glColor3f(0.3,0.2,0.1);
        gluCylinder(t,0.18,0.1,0.9,10,10);

        glPopMatrix();
       ///////////// end of copy........





        ///////////// Traingular tree ..... 2nd.....

        glTranslatef(-10,0,-10);
        glPushMatrix();

        glRotatef(90,-1,0,0);
        glColor3f(0.1,0.5,0.1);
        glScalef(2,2,2);

        glTranslatef(2,-3,1.5);
        glutSolidCone(0.3,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.4,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.5,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.6,0.5,tsl,tst);
        glTranslatef(0,0,-0.8);
        glColor3f(0.3,0.2,0.1);
        gluCylinder(t,0.18,0.1,0.9,10,10);

        glPopMatrix();

        ////

        //............t tree copy .....
        glTranslatef(10,0,0);

        glPushMatrix();

        glRotatef(90,-1,0,0);
        glColor3f(0.1,0.5,0.1);
        glScalef(2,2,2);

        glTranslatef(2,-3,1.5);
        glutSolidCone(0.3,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.4,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.5,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.6,0.5,tsl,tst);
        glTranslatef(0,0,-0.8);
        glColor3f(0.3,0.2,0.1);
        gluCylinder(t,0.18,0.1,0.9,10,10);

        glPopMatrix();
       ///////////// end of copy........


        //............t tree copy .....
        glTranslatef(10,0,0);

        glPushMatrix();

        glRotatef(90,-1,0,0);
        glColor3f(0.1,0.5,0.1);
        glScalef(2,2,2);

        glTranslatef(2,-3,1.5);
        glutSolidCone(0.3,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.4,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.5,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.6,0.5,tsl,tst);
        glTranslatef(0,0,-0.8);
        glColor3f(0.3,0.2,0.1);
        gluCylinder(t,0.18,0.1,0.9,10,10);

        glPopMatrix();
       ///////////// end of copy........


        //............t tree copy .....
        glTranslatef(10,0,0);

        glPushMatrix();

        glRotatef(90,-1,0,0);
        glColor3f(0.1,0.5,0.1);
        glScalef(2,2,2);

        glTranslatef(2,-3,1.5);
        glutSolidCone(0.3,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.4,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.5,0.5,tsl,tst);
        glTranslatef(0,0,-0.3);
        glutSolidCone(0.6,0.5,tsl,tst);
        glTranslatef(0,0,-0.8);
        glColor3f(0.3,0.2,0.1);
        gluCylinder(t,0.18,0.1,0.9,10,10);

        glPopMatrix();
       ///////////// end of copy........


glPopMatrix();







//***************PYRAMIDS************************
        glTranslatef(10,-0.5,40);
        glRotatef(90,-1,0,0);
        glColor3f(0.90,0.72,0.52);


        glutSolidCone(10,10,4,10);


        glTranslatef(10,10,0);
        glutSolidCone(10,10,4,10);
        }
        glPopMatrix();
        //*************************************************************
        glPopMatrix();

        glEnable(GL_DEPTH_TEST);
        glutPostRedisplay();
        glutSwapBuffers();
    }
}


void NormalKey(GLubyte key, GLint x, GLint y)
{
    switch ( key )    {
     case 27:
         if (start==0) exit(0);
         else start=0;
         break;

      case ' ':start=1;
                   break;

      case 'x': xangle += 5.0;
                      break;

       case 'X':xangle -= 5.0;
                       break;

      case 'y':
        yangle += 5.0;
        break;

     case 'Y':
        yangle -= 5.0;
        break;

     case 'z':
        zangle += 5.0;
        break;

     case 'Z':
        zangle -= 5.0;
        break;

      case 'u':                          /* Move up */
        yt += 0.2;
        break;

      case 'U':
        yt -= 0.2;                      /* Move down */
        break;

    case 'i':                          /* Move up */
        xt += 0.2;
        break;

      case 'I':
        xt -= 0.2;                      /* Move down */
        break;

      case 'f':                          /* Move forward */
        movf += 1;
        break;

      case 'F':
        movf -= 1;                      /* Move away */
        break;

	  case 'd':
	      movr+=1;
		  break;

	  case 'g':
	      movr-=1;
		  break;

	  case 'a':ys+=.2;
		  break;

	  case 'A':ys-=0.2;
		  break;

	  case 'q':xs+=.2;
		  break;

	  case 'Q':xs-=0.2;
		  break;

        case 'n':camx++;
                break;

        case 'N':camx--;
                break;

        case 'm':camy++;
                break;

        case 'M':camy--;
                break;

        case 'b':camz++;
                break;

        case 'B':camz--;
                break;

        case 'J':camxl++;
                break;

        case 'j':camxl--;
                break;

        case 'k':camyl++;
                break;

        case 'K':camyl--;
                break;

        case 'H':camzl++;
                break;

        case 'h':camzl--;
                break;

        case '1':camxL++;
                break;

        case '2':camxL--;
                break;

        case '3':camyL++;
                break;

        case '4':camyL--;
                break;

        case '5':camzL++;
                break;

        case '6':camzL--;
                break;

     default:
	break;
    }

}
static void SpecialKeyFunc( int Key, int x, int y )
{
	switch ( Key ) {
	case GLUT_KEY_DOWN:
		if(!wheelflag)
            zt += 0.2;
            angle+=5;
		if(wheelflag)
		{
            angle+=5;
            xw+=0.2;
		}
	    //glutPostRedisplay();
		break;

    case GLUT_KEY_UP:
		if(!wheelflag)
            zt -= 0.2;
            angle-=5;
		if(wheelflag)
		{
            angle+=5;
            xw-=0.2;
		}
        //glutPostRedisplay();
		break;

    case GLUT_KEY_LEFT:
        yangle += 5.0;
        //glutPostRedisplay();
        break;

     case GLUT_KEY_RIGHT:
        yangle -= 5.0;
        //glutPostRedisplay();
        break;
	}
}

void myMenu(int id)
{
   if(id==3)//fog
	{
		flag2=1;
		wheelflag=0;
		xangle += 5.0;
		//glutPostRedisplay();
	}
	if (id==5)//toggle
	{
        if(day)
        {

            if(light)
            {
                count++;
                glDisable(GL_LIGHTING);
                glDisable(GL_LIGHT0);
                light=0;
            }
            else
            {
                count--;
                light=1;
                glEnable(GL_LIGHTING);
                glEnable(GL_LIGHT0);
            }
    //        glutPostRedisplay();
        }
        else
        {

          if(nml==0 && flag2==2)
             {
               flag2=0;
               nml=1;
             }
         else
         {
             flag2=2;
             nml=0;

             aflag=0;
             day=0;

             glClearColor(0.1,0.1,0.1,0);
             GLfloat fogcolour[4]={0.0,0.0,0.0,1.0};

             glFogfv(GL_FOG_COLOR,fogcolour);              /* Define the fog colour */
             glFogf(GL_FOG_DENSITY,0.5);                   /* How dense */
             glFogi(GL_FOG_MODE,GL_EXP);                   /* exponential decay */
                     /* end */
             glHint(GL_FOG_HINT, GL_FASTEST);              /* compute per vertex */
             glEnable(GL_FOG);


         }
        }

	}


    if(id==12)//day
	{
        aflag=1;
        day=1;
        glClearColor(1,1,1,1);
        glDisable(GL_FOG);

	}

	if(id==13)//night
	{
        aflag=0;
        day=0;
        flag2=2;
        glClearColor(0.1,0.1,0.1,0);
        GLfloat fogcolour[4]={0.0,0.0,0.0,1.0};

        glFogfv(GL_FOG_COLOR,fogcolour);              /* Define the fog colour */
        glFogf(GL_FOG_DENSITY,0.5);                   /* How dense */
        glFogi(GL_FOG_MODE,GL_EXP);                   /* exponential decay */
                             /* end */
        glHint(GL_FOG_HINT, GL_FASTEST);              /* compute per vertex */
        glEnable(GL_FOG);

        //glutPostRedisplay();
    }
}



void colorMenu(int id)
{
    if (id==6)
	{
		r=g=0;
		b=1;
	}
	if(id ==7)
	{
		r=0.8;
		b=g=0;
	}
	if(id==8)
	{
	    g=1;
		r=b=0;
	}
	if (id==9)
	{
		 r=b=g=0;
	}
	if(id==10)
	{
		b=0;
		r=g=1;
	}
	if(id==11)
	{
		b=r=g=.7;
	}

}




void MouseClick(int button, int state, int x, int y){
    if (start==0){
        if(x>400 && x<540 && y>210 && y<250){ //Drive
            start=1;
            flag1=1;
            flag2=0;
            wheelflag=0;
            xangle += 5.0;
        }else if(x>410 && x<540 && y>280 && y<320){ //Drive

            start=1;
            flag1=0;
            wheelflag=0;
        }else if(x>400 && x<540 && y>340 && y<380){ //Drive
            start=1;
            wheelflag=1;

        }else if(x>400 && x<540 && y>410 && y<450){ //Drive
            exit(0);
        }
    }
}


void attachRight(){
    int submenu=glutCreateMenu(colorMenu);
    glutAddMenuEntry("blue", 6);
    glutAddMenuEntry("red", 7);
    glutAddMenuEntry("green",8);
    glutAddMenuEntry("black",9);
    glutAddMenuEntry("yellow",10);
    glutAddMenuEntry("grey",11);

    glutCreateMenu(myMenu);
    glutAddMenuEntry("fog effect",3);
    glutAddMenuEntry("toggle light",5);
    glutAddSubMenu("car colors",submenu);
    glutAddMenuEntry("daymode",12);
    glutAddMenuEntry("Night mode",13);

    glutAttachMenu(GLUT_RIGHT_BUTTON);

}

//};
int main(int argc, char **argv)
{
    glutInit(&argc, argv);               /* Initialize GLUT state. */

    glutInitDisplayMode(GLUT_RGBA |      /* RGB and Alpha */
                        GLUT_DOUBLE|     /* double buffer */
                        GLUT_DEPTH);     /* Z buffer (depth) */

    glutInitWindowSize(Xsize,Ysize);     /* set initial window size. */
    glutInitWindowPosition(0,0);         /* upper left corner of the screen. */

    glutCreateWindow("Car");
    glutDisplayFunc(DrawGLScene);        /* Function to do all our OpenGL drawing. */


    glutKeyboardFunc(NormalKey);         /*Normal key is pressed */
    glutSpecialFunc( SpecialKeyFunc );

    glutMouseFunc(MouseClick);

    InitGL(Xsize,Ysize);


    attachRight();  //right click menu
    glutMainLoop();

    return 0;
}


