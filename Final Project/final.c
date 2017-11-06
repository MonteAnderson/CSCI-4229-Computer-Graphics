/*
 *  Monte Anderson
 *  Final Project - Penalty Kicks
 * 
 * Key bindings
 *   1	     Penalty Position 1
 *   2 	     Penalty Position 2
 *   3 	     Penalty Position 3
 *   4 	     Penalty Position 4
 * 
 *   =          Change Team (up list)
 *   -          Change Team (down list)
 * 
 *   q          Increase Kick Power (X axis)
 *   e          Decrease Kick Power (X axis)
 *   w          Increase Kick Angle (Y axis, upwards)
 *   s          Decrease Kick Angle (Y axis, downwards)
 *   a          Kick Angle Left (Z axis)
 *   d          Kick Angle Right (Z axis)
 * 
 *   r          Reset ball position and velocities to default. 
 *   t          Toggle axes

 *   SPACEBAR   Kick The Ball (Using current X,Y, and Z Velocities)
 *   arrows     Change view angle
 *   PgDn/PgUp  Zoom in and out
 *   0          Reset view angle
 * 
 *   ESC        Exit
 */

#include "CSCIx229.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
typedef struct {float x,y,z;} Point;
//  Global variables

unsigned int sky[3];

int p = 0;
int soccerball=0;
int soccergoal=0;
int trophy=0;
int stadium=0;
int waterbottle=0;
int suza6=0;
int kicking = 0;
int falling = 0;
int rotatingView = 1;
int fov=55; 
int position = 0;
float color1= 1;
float color2= 1;
float color3 = 1;

int          mode=0;    // Display mode
int          obj=15;    // Display objects (bitmap)
int          move=1;    // Light movement
int          axes=0;    // Display axes
int          th=-130;    // Azimuth of view angle
int          ph=+30;    // Elevation of view angle
int          tex2d[4];  // Textures (names)
int 	     teamNum = 0;
int          team[7];  // Textures (names)
int          dt=50;     // Timer period (ms)
double       asp=1;     // Aspect ratio
double       dim=75;     // Size of world
int          zh=0;      // Light azimuth
float        Ylight=250;  // Elevation of light
float        Lpos[4];   // Light position
unsigned int framebuf=0;// Frame buffer id
double       Svec[4];   // Texture planes S
double       Tvec[4];   // Texture planes T
double       Rvec[4];   // Texture planes R
double       Qvec[4];   // Texture planes Q
int          Width;     // Window width
int          Height;    // Window height
int          shadowdim; // Size of shadow map textures
int          shader;    // Shader

float xVal = 0;
float yVal = 0;
float zVal = 0;

float xVel = 0;
float yVel = 1;
float zVel = 1;

float xRot = 0;
float yRot = 1;

char* text[]={"Shadows","Shadow Map"};

#define MAXN 64    // Maximum number of slices (n) and points in a polygon

static void Sky(double D)
{
   glColor3f(1,1,1);
   glEnable(GL_TEXTURE_2D);

   //  Sides
   glBindTexture(GL_TEXTURE_2D,sky[p]);
   glBegin(GL_QUADS);
   glTexCoord2f(0.00,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);
   glTexCoord2f(0.00,1); glVertex3f(-D,+D,-D);

   glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);
   glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);

   glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
   glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);

   glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(1.00,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(1.00,1); glVertex3f(-D,+D,-D);
   glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
   glEnd();

   //  Top and bottom
   glBindTexture(GL_TEXTURE_2D,sky[p]);
   glBegin(GL_QUADS);
   glTexCoord2f(0.0,0); glVertex3f(+D,+D,-D);
   glTexCoord2f(0.5,0); glVertex3f(+D,+D,+D);
   glTexCoord2f(0.5,1); glVertex3f(-D,+D,+D);
   glTexCoord2f(0.0,1); glVertex3f(-D,+D,-D);

   glTexCoord2f(1.0,1); glVertex3f(-D,-D,+D);
   glTexCoord2f(0.5,1); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.5,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(1.0,0); glVertex3f(-D,-D,-D);
   glEnd();

   glDisable(GL_TEXTURE_2D);
}

void timer(int v) {
  if (kicking) {

    yVel = yVel - 0.05;

    xVal += xVel;
    yVal += yVel;
    zVal += zVel;

    if ((yVal >= 14.25) && (xVal <=-20)) {
      falling = 1;
      kicking = 0;
    }
    glutPostRedisplay();
  }	 
/*	GOAL!
	glVertex3f(-18,-2,10);
	 glVertex3f(-18,12.5,10);
	 glVertex3f(18,12.5,10);
	 glVertex3f(18,-2,10);*/
	//BOUNDARIES FOR PHYSICS



  if (falling){
	yVal -=yVel;
   	xVal -= xVel;
	kicking = 0;
	glutPostRedisplay();
	}

  if ((yVal >= 12.5 && yVal <= 15 && zVal >28)){
	xVel = -xVel;
	zVel = -zVel;}

  if ((zVal < 31) && ((xVal >= 16.5) && (xVal <= 19)) && (zVal >= 28.5)){
	yVal -=1;
	xVel = -xVel;
	zVel = -zVel;
	}

  if ((zVal < 31) && ((xVal >= -19) && (xVal <= -16.5)) && (zVal >= 28)){
	xVel = -xVel;
	zVel = -zVel;
	}

  if ((zVal <= 43) && ((xVal >= 16.5) && (xVal <= 19)) && (zVal >= 31) && yVal <= 15){
	xRot = 0;
	yRot = 0;
	falling = 0;
	kicking = 0;
	yVal -=1;
	}

  if ((zVal < 43)&& (zVal >= 40) && (yVal <=13)){
	xRot = 0;
	yRot = 0;
	falling = 0;
	kicking = 0;
	yVal -=1;
	}

  if ((zVal < 43) && (xVal <= -17) && (zVal >= 30)){
	xRot = 0;
	yRot = 0;
	falling = 0;
	kicking = 0;
	yVal -=1;
	}

  if (yVal < 0){
	xRot = 0;
	yRot = 0;
	xVel = 0;
	yVal = 0;
	falling = 0;
	kicking = 0;
	glutPostRedisplay();

	}

  glutTimerFunc(1000/100, timer, v);
}


static void drawSoccerball(float x,float y,float z , float th,float ph , float S)
{
   //  Transform
   glPushMatrix();
   glTranslated(x,y+0.5,z);
   glRotated(ph,1,0,0);
   glRotated(th,0,1,0);
   glScalef(0.85,0.85,0.85);
   glColor3f(1.0,1.0,1.0);
   glCallList(soccerball);
   //  Restore
   glPopMatrix();
}

static void drawsoccergoal(float x,float y,float z , float th,float ph , float S)
{
   //  Transform
   glPushMatrix();
   glTranslated(x,y+0.5,z);
   glRotated(ph,1,0,0);
   glRotated(th,0,1,0);
   //glScalef(1.25,1.25,1.25);
      glColor3f(1.0,1.0,1.0);
   glCallList(soccergoal);

   //  Restore
   glPopMatrix();
}

static void drawtrophy(float x,float y,float z , float th,float ph , float S)
{
   //  Transform
   glPushMatrix();
   glTranslated(x,y+0.5,z);
   glRotated(ph,1,0,0);
   glRotated(th,0,1,0);
   glScalef(0.5,0.5,0.5);
   glColor3f(1.0,1.0,1.0);
   glCallList(trophy);
   //  Restore
   glPopMatrix();
}

static void drawstadium(float x,float y,float z , float th,float ph , float S)
{
   //  Transform
   glPushMatrix();
   glTranslated(x,y+0.5,z);
   glRotated(ph,1,0,0);
   glRotated(th,0,1,0);
   glScalef(1.5,1.5,1.5);
   glColor3f(0.5,0.5,0.5);
   glCallList(stadium);
   //  Restore
   glPopMatrix();
}


static void drawwaterbottle(float x,float y,float z , float th,float ph , float S)
{
   //  Transform
   glPushMatrix();
   glTranslated(x,y+0.5,z);
   glRotated(ph,1,0,0);
   glRotated(th,0,1,0);
   glScalef(0.3,0.4,0.3);
   glColor3f(1,1,1);
   glCallList(waterbottle);
   glPopMatrix();
}

static void drawTeamLogo()
{
   	glPushMatrix();
	   glBindTexture(GL_TEXTURE_2D,team[teamNum]);

		//Front
	   glBegin(GL_QUADS);
	   glNormal3f( 0, 0, -1);
	   glTexCoord2f(0,-0); glVertex3f(-36,-2, 46.25);
	   glTexCoord2f(-3,-0); glVertex3f(+36,-2, 46.25);
	   glTexCoord2f(-3,1); glVertex3f(+36,+2, 46.25);
	   glTexCoord2f(0,1); glVertex3f(-36,+2, 46.25);
	   glEnd();

	   glBegin(GL_QUADS);
	   glNormal3f( 0, 0, -1);
	   glTexCoord2f(0,-0); glVertex3f(-36,-2, -96.25);
	   glTexCoord2f(3,-0); glVertex3f(+36,-2, -96.25);
	   glTexCoord2f(3,1); glVertex3f(+36,+2, -96.25);
	   glTexCoord2f(0,1); glVertex3f(-36,+2, -96.25);
	   glEnd();

		//Sides
	   glBegin(GL_QUADS);
	   glNormal3f( -1, 0, 0);
	   glTexCoord2f(0,-0); glVertex3f(+36,-2, -96.25);
	   glTexCoord2f(5,-0); glVertex3f(+36,-2, +46.25);
	   glTexCoord2f(5,1); glVertex3f(+36,+2, +46.25);
	   glTexCoord2f(0,1); glVertex3f(+36,+2, -96.25);
	   glEnd();

	   glBegin(GL_QUADS);
	   glNormal3f( -1, 0, 0);
	   glTexCoord2f(0,-0); glVertex3f(-36,-2, -96.25);
	   glTexCoord2f(-5,-0); glVertex3f(-36,-2, +46.25);
	   glTexCoord2f(-5,1); glVertex3f(-36,+2, +46.25);
	   glTexCoord2f(0,1); glVertex3f(-36,+2, -96.25);
	   glEnd();

   glPopMatrix();
}

static void drawFlags()
{
   	glPushMatrix();
	   glBindTexture(GL_TEXTURE_2D,tex2d[3]);
		//Front
	   glBegin(GL_QUADS);
	   glNormal3f( 0, 0, -1);
	   glTexCoord2f(0,0);  glVertex3f(-7,+15, 68 + Sin(zh));
	   glTexCoord2f(-1,0); glVertex3f(+7,+15, 68 + Cos(zh));
	   glTexCoord2f(-1,1); glVertex3f(+7,+30, 68 + Sin(zh) * 0.15);
	   glTexCoord2f(0,1);  glVertex3f(-7,+30, 68 + Cos(zh) * 0.15);
	   glEnd();

		//Behind 
	   glBegin(GL_QUADS);
	   glNormal3f( 0, 0, 1);
	   glTexCoord2f(0,0); glVertex3f(-7,+15, -119 + Sin(zh));
	   glTexCoord2f(1,0); glVertex3f(+7,+15, -119 + Cos(zh));
	   glTexCoord2f(1,1); glVertex3f(+7,+30, -119 + Sin(zh) * 0.15);
	   glTexCoord2f(0,1); glVertex3f(-7,+30, -119 + Cos(zh) * 0.15);
	   glEnd();

		//Sides (Left)
	   glBegin(GL_QUADS);
	   glNormal3f( -1, 0, 0);
	   glTexCoord2f(0,0);  glVertex3f(+59 + Sin(zh),+15, +16);
	   glTexCoord2f(-1,0); glVertex3f(+59 + Cos(zh),+15, 2);
	   glTexCoord2f(-1,1); glVertex3f(+59 + Sin(zh) * 0.15,+30, 2);
	   glTexCoord2f(0,1);  glVertex3f(+59 + Cos(zh) * 0.15,+30, +16);
	   glEnd();

	   glBegin(GL_QUADS);
	   glNormal3f( -1, 0, 0);
	   glTexCoord2f(0,0);  glVertex3f(+59 - Sin(zh),+15, -53);
	   glTexCoord2f(-1,0); glVertex3f(+59 - Cos(zh),+15, -67);
	   glTexCoord2f(-1,1); glVertex3f(+59 - Sin(zh) * 0.15,+30, -67);
	   glTexCoord2f(0,1);  glVertex3f(+59 - Cos(zh) * 0.15,+30, -53);
	   glEnd();

		//Sides (Right)
	   glBegin(GL_QUADS);
	   glNormal3f( 0, 0, -1);
	   glTexCoord2f(0,0); glVertex3f(-59 + Sin(zh),+15, +16);
	   glTexCoord2f(1,0); glVertex3f(-59 + Cos(zh),+15, 2);
	   glTexCoord2f(1,1); glVertex3f(-59 + Sin(zh) * 0.15,+30, 2);
	   glTexCoord2f(0,1); glVertex3f(-59 + Cos(zh) * 0.15,+30, 16);
	   glEnd();

	   glBegin(GL_QUADS);
	   glNormal3f( 0, 0, -1);
	   glTexCoord2f(0,0); glVertex3f(-59 - Sin(zh),+15, -53);
	   glTexCoord2f(1,0); glVertex3f(-59 - Cos(zh),+15, -67);
	   glTexCoord2f(1,1); glVertex3f(-59 - Sin(zh) * 0.15,+30, -67);
	   glTexCoord2f(0,1); glVertex3f(-59 - Cos(zh) * 0.15,+30, -53);
	   glEnd();

   glPopMatrix();
}

static void Light(int light, float color1, float color2, float color3)
{
   //  Set light position
   Lpos[0] = 1;
   Lpos[1] = Ylight;
   Lpos[2] = 1;
   Lpos[3] = 1;


      float Med[]  = {0.25,0.25,0.25,1.0};
      float High[] = {color1,color2,color3,1.0};


      //  Enable lighting with normalization
      glEnable(GL_LIGHTING);
      glEnable(GL_NORMALIZE);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      glLightfv(GL_LIGHT0,GL_POSITION,Lpos);
      glLightfv(GL_LIGHT0,GL_AMBIENT,Med);
      glLightfv(GL_LIGHT0,GL_DIFFUSE,High);

}

/*
 *  Draw scene
 *    light (true enables lighting)
 */
void Scene(int light)
{
   //  Set light position and properties
   Light(light, color1, color2, color3);
 
   //  Enable textures if lit
   if (light)
   {
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D,tex2d[3]);
   }

   //  Draw objects         x    y   z          th,ph    dims  

   drawSoccerball(xVal,yVal - 1.5,zVal - 20 ,     xRot * zh, yRot * zh , 0.25   );
   drawsoccergoal(0,-3,10 ,     90, 0  , 0.25   );
   drawstadium(0,-3,10.0 ,     0, 0  , 0.25   );
   drawstadium(0,-3,-60.0 ,     180, 0  , 0.25   );

   drawtrophy(-24,-3,12 ,     -25, 0  , 0.25   ); //Trophy
   drawwaterbottle(20,-3,8.0 ,     0*zh, 0  , 0.25   ); //Water
   drawFlags();
   drawTeamLogo();

   //  Disable textures
   if (light) glDisable(GL_TEXTURE_2D);

   //  The floor, ceiling and walls don't cast a shadow, so bail here
   if (!light) return;

   //  Enable textures for floor, ceiling and walls
   glEnable(GL_TEXTURE_2D);

   //  Water texture for floor and ceiling
   glBindTexture(GL_TEXTURE_2D,tex2d[0]);
   glColor3f(1.0,1.0,1.0);
   
   //for (k=-1;k<=box;k+=5)
      //Wall(0,0,0, 0,90*k , 8,8,box?6:2 , 4);

   //  Disable textures
   glDisable(GL_TEXTURE_2D);
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{

   if (rotatingView)
   	th -=1; //ROTATES CAMERA
   int id;
   const double len=20.0;

   //  Eye position
   float Ex = -2*dim*Sin(th)*Cos(ph);
   float Ey = +2*dim        *Sin(ph);
   float Ez = +2*dim*Cos(th)*Cos(ph);

   //  Erase the window and the depth buffers
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   glDisable(GL_LIGHTING);

   /*if (mode)
   {
      //  Half width for shadow map display
      Project(60,asp,dim);
      glViewport(0,0,Width,Height);
   }*/

   
      //  Full width
   Project(fov,asp,dim);
   glViewport(0,0,Width,Height);
   
   if (position == 0) 
	gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,200,0);

   else if (position == 1){
	fov = 60;
	gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,200,0);
	}

   else if (position == 2){
	fov = 40;
	glTranslatef(0,0,-70);
	gluLookAt(Ex,Ey,Ez , 0,0,25 , 0,200,0);
	}

   else if (position == 3){
	fov = 40;
	glTranslatef(25,0,-50);
	gluLookAt(Ex,Ey,Ez , -35,0,0 , 0,200,0);
	}

   else if (position == 4){
	fov = 40;
	glTranslatef(-25,0,-50);
	gluLookAt(Ex,Ey,Ez , 35,0,0 , 0,200,0);
	}


   //else if (position != 0)
      //gluLookAt(Ex,Ey,Ez , xVal,yVal,zVal , 0,10,0); Follow ball path

   Sky(9*dim);

   //  Enable shader program
   glUseProgram(shader);
   id = glGetUniformLocation(shader,"tex");
   if (id>=0) glUniform1i(id,0);
   id = glGetUniformLocation(shader,"depth");
   if (id>=0) glUniform1i(id,1);

   // Set up the eye plane for projecting the shadow map on the scene
   glActiveTexture(GL_TEXTURE1);
   glTexGendv(GL_S,GL_EYE_PLANE,Svec);
   glTexGendv(GL_T,GL_EYE_PLANE,Tvec);
   glTexGendv(GL_R,GL_EYE_PLANE,Rvec);
   glTexGendv(GL_Q,GL_EYE_PLANE,Qvec);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE,GL_COMPARE_R_TO_TEXTURE);
   glActiveTexture(GL_TEXTURE0);


   // Draw objects in the scene (including walls)
   Scene(1);

//Grass
   glBindTexture(GL_TEXTURE_2D,tex2d[1]);

   glBegin(GL_QUADS);
   glNormal3f( 1, 1, 0);
   glTexCoord2f(0,0); glVertex3f(-50,-2,0);
   glTexCoord2f(3,0); glVertex3f(-50,-2,25);
   glTexCoord2f(3,10); glVertex3f(50,-2,25);
   glTexCoord2f(0,10); glVertex3f(50,-2,0);

   glTexCoord2f(0,0); glVertex3f(-50,-2,-25);
   glTexCoord2f(3,0); glVertex3f(-50,-2,-50);
   glTexCoord2f(3,10); glVertex3f(50,-2,-50);
   glTexCoord2f(0,10); glVertex3f(50,-2,-25);

   glTexCoord2f(0,0); glVertex3f(-50,-2,-75);
   glTexCoord2f(3,0); glVertex3f(-50,-2,-100);
   glTexCoord2f(3,10); glVertex3f(50,-2,-100);
   glTexCoord2f(0,10); glVertex3f(50,-2,-75);

   glEnd();



   glBindTexture(GL_TEXTURE_2D,tex2d[2]);
   glBegin(GL_QUADS);
   glNormal3f( 1, 1, 0);
   glTexCoord2f(0,0); glVertex3f(-50,-2,25);
   glTexCoord2f(3,0); glVertex3f(-50,-2,50);
   glTexCoord2f(3,10); glVertex3f(50,-2,50);
   glTexCoord2f(0,10); glVertex3f(50,-2,25);

   glTexCoord2f(0,0); glVertex3f(-50,-2,0);
   glTexCoord2f(3,0); glVertex3f(-50,-2,-25);
   glTexCoord2f(3,10); glVertex3f(50,-2,-25);
   glTexCoord2f(0,10); glVertex3f(50,-2,0);

   glTexCoord2f(0,0); glVertex3f(-50,-2,-50);
   glTexCoord2f(3,0); glVertex3f(-50,-2,-75);
   glTexCoord2f(3,10); glVertex3f(50,-2,-75);
   glTexCoord2f(0,10); glVertex3f(50,-2,-50);
   glEnd();



   //  Disable shader program
   glUseProgram(0);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);


   //  Draw axes (white)
   glColor3f(1,1,1);
   if (axes)
   {
	glLineWidth(1);
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }

   //  Display parameters
   glColor3f(1,0,0);
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f XVel=%.2f YVel=%.2f ZVel=%.2f ",
     th,ph,dim,xVel,yVel, zVel);

   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}


/*
 *  Build Shadow Map
 */
void ShadowMap(void)
{
   double Lmodel[16];  //  Light modelview matrix
   double Lproj[16];   //  Light projection matrix
   double Tproj[16];   //  Texture projection matrix
   double Dim=200.0;     //  Bounding radius of scene
   double Ldist;       //  Distance from light to scene center

   //  Save transforms and modes
   glPushMatrix();
   glPushAttrib(GL_TRANSFORM_BIT|GL_ENABLE_BIT);

   //  No write to color buffer and no smoothing
   glShadeModel(GL_FLAT);
   glColorMask(0,0,0,0);

   // Overcome imprecision
   glEnable(GL_POLYGON_OFFSET_FILL);

   //  Light distance
   Ldist = sqrt(Lpos[0]*Lpos[0] + Lpos[1]*Lpos[1] + Lpos[2]*Lpos[2]);
   if (Ldist<1.1*Dim) Ldist = 1.1*Dim;

   //  Set perspective view from light position
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(114.6*atan(Dim/Ldist),1,Ldist-Dim,Ldist+Dim);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(Lpos[0],Lpos[1],Lpos[2] , 0,0,0 , 0,1,0);

   //  Size viewport to desired dimensions
   glViewport(0,0,shadowdim,shadowdim);

   // Redirect traffic to the frame buffer
   glBindFramebuffer(GL_FRAMEBUFFER,framebuf);

   // Clear the depth buffer
   glClear(GL_DEPTH_BUFFER_BIT);
   // Draw all objects that can cast a shadow
   Scene(0);

   //  Retrieve light projection and modelview matrices
   glGetDoublev(GL_PROJECTION_MATRIX,Lproj);
   glGetDoublev(GL_MODELVIEW_MATRIX,Lmodel);

   // Set up texture matrix for shadow map projection,
   // which will be rolled into the eye linear
   // texture coordinate generation plane equations
   glLoadIdentity();
   glTranslated(0.5,0.5,0.5);
   glScaled(0.5,0.5,0.5);
   glMultMatrixd(Lproj);
   glMultMatrixd(Lmodel);

   // Retrieve result and transpose to get the s, t, r, and q rows for plane equations
   glGetDoublev(GL_MODELVIEW_MATRIX,Tproj);
   Svec[0] = Tproj[0];    Tvec[0] = Tproj[1];    Rvec[0] = Tproj[2];    Qvec[0] = Tproj[3];
   Svec[1] = Tproj[4];    Tvec[1] = Tproj[5];    Rvec[1] = Tproj[6];    Qvec[1] = Tproj[7];
   Svec[2] = Tproj[8];    Tvec[2] = Tproj[9];    Rvec[2] = Tproj[10];   Qvec[2] = Tproj[11];
   Svec[3] = Tproj[12];   Tvec[3] = Tproj[13];   Rvec[3] = Tproj[14];   Qvec[3] = Tproj[15];

   // Restore normal drawing state
   glShadeModel(GL_SMOOTH);
   glColorMask(1,1,1,1);
   glDisable(GL_POLYGON_OFFSET_FILL);
   glPopAttrib();
   glPopMatrix();
   glBindFramebuffer(GL_FRAMEBUFFER,0);

   //  Check if something went wrong
   ErrCheck("ShadowMap");
}

/*
 *
 */
void InitMap()
{
   unsigned int shadowtex; //  Shadow buffer texture id
   int n;

   //  Make sure multi-textures are supported
   glGetIntegerv(GL_MAX_TEXTURE_UNITS,&n);
   if (n<2) Fatal("Multiple textures not supported\n");

   //  Get maximum texture buffer size
   glGetIntegerv(GL_MAX_TEXTURE_SIZE,&shadowdim);
   //  Limit texture size to maximum buffer size
   glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE,&n);
   if (shadowdim>n) shadowdim = n;

   //  Limit texture size to 2048 for performance
   //EDIT THIS FOR BETTER SHADOWS
   if (shadowdim>2048) shadowdim = 2048;


   if (shadowdim<512) Fatal("Shadow map dimensions too small %d\n",shadowdim);

   //  Do Shadow textures in MultiTexture 1
   glActiveTexture(GL_TEXTURE1);

   //  Allocate and bind shadow texture
   glGenTextures(1,&shadowtex);
   glBindTexture(GL_TEXTURE_2D,shadowtex);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, shadowdim, shadowdim, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

   //  Map single depth value to RGBA (this is called intensity)
   glTexParameteri(GL_TEXTURE_2D,GL_DEPTH_TEXTURE_MODE,GL_INTENSITY);

   //  Set texture mapping to clamp and linear interpolation
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

   //  Set automatic texture generation mode to Eye Linear
   glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
   glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
   glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
   glTexGeni(GL_Q,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);

   // Switch back to default textures
   glActiveTexture(GL_TEXTURE0);

   // Attach shadow texture to frame buffer
   glGenFramebuffers(1,&framebuf);
   glBindFramebuffer(GL_FRAMEBUFFER,framebuf);
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowtex, 0);
   //  Don't write or read to visible color buffer
   glDrawBuffer(GL_NONE);
   glReadBuffer(GL_NONE);
   //  Make sure this all worked
   if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) Fatal("Error setting up frame buffer\n");
   glBindFramebuffer(GL_FRAMEBUFFER,0);

   //  Check if something went wrong
   ErrCheck("InitMap");

   //  Create shadow map
   ShadowMap();
}

/*
 *  GLUT calls this routine when nothing else is going on
 */
void idle(int k)
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,1440.0);
   //  Update shadow map
   ShadowMap();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
   //  Schedule update
   if (move) glutTimerFunc(dt,idle,0);
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT){
      th += 1;
	rotatingView = 0;
	}
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT){
      th -= 1;
	rotatingView = 0;
	}
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP){
      ph += 1;
	rotatingView = 0;
	}
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN){
      ph -= 1;
	rotatingView = 0;
	}
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN){
      dim += 0.1;
	rotatingView = 0;
	}
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1){
      dim -= 0.1;
	rotatingView = 0;
	}
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Tell GLUT it is necessary to redisplay the scene
   Project(fov, asp, dim);
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{

   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle

   else if (ch == '='){
	teamNum+=1;
	if (teamNum > 6)
		teamNum = 0;
	}

   else if (ch == '-'){
	teamNum-=1;
	if (teamNum <0)
		teamNum = 6;
	}

   else if (ch == '0'){
	position = 1;
	rotatingView = 0;
	ph = 10;
	th = 180;
	dim = 20;    	
	position = 1;
        zVal = 0;
	yVal = 0;
	xVal = 0;
	}

   else if (ch == '7'){
	p = 2;
	color1 = 0.4;
	color2 = 0.4;
	color3 = 0.4;
	}

   else if (ch == '8'){
	p = 0;
	color1 = 1;
	color2 = 1;
	color3 = 1;
	}

   else if (ch == '9'){
	p = 1;
	color1 = 0.6;
	color2 = 0.2;
	color3 = 0.2;
	}

   else if (ch == '1'){
	position = 1;
	rotatingView = 0;
	ph = 10;
	th = 180;
	dim = 20;    	
        zVal = 0;
	yVal = 0;
	xVal = 0;
    	zVel = 1.5;
	yVel = 1;
	xVel = 0;
	}

   else if (ch == '2'){
        position = 2;
	rotatingView = 0;
	ph = 10;
	th = 180;
	dim = 25;    	
        zVal = -50;
	yVal = 0;
	xVal = 0;
    	zVel = 1.5;
	yVel = 1;
	xVel = 0;
	}

   else if (ch == '3'){
        position = 3;
	rotatingView = 0;
	ph = 10;
	th = 180;
	dim = 30;    	
        zVal = -40;
	yVal = 0;
	xVal = 25;
    	zVel = 1.5;
	yVel = 1;
	xVel = 0;
	}

   else if (ch == '4'){
        position = 4;
	rotatingView = 0;
	ph = 10;
	th = 180;
	dim = 30;    	
        zVal = -40;
	yVal = 0;
	xVal = -25;
    	zVel = 1.5;
	yVel = 1;
	xVel = 0;
	}


   else if ((ch == 32) && (!(kicking) || (falling))){
	if (position == 1){
    	zVal = 0;
	yVal = 0;
	xVal = 0;}

	if (position == 2){
    	zVal = -50;
	yVal = 0;
	xVal = 0;}

        kicking = 1;
	}

   else if (ch == 'r'){
	if (position == 1){
    	zVal = 0;
	yVal = 0;
	xVal = 0;
	xRot = 0;
	yRot = 1;
	}

	if (position == 2){
    	zVal = -50;
	yVal = 0;
	xVal = 0;}

	if (position == 3){
    	zVal = -40;
	yVal = 0;
	xVal = 25;}

	if (position == 4){
    	zVal = -40;
	yVal = 0;
	xVal = -25;}


	falling = 0;
        kicking = 0;
    	zVel = 1;
	yVel = 1;
	xVel = 0;
	}


   else if (ch == 'w'){
      yVel += 0.05;
	yRot -= 0.025;
	}

   else if (ch == 's'){
      yVel -= 0.05;
	yRot += 0.025;}

   else if (ch == 'a'){
      xVel += 0.05;
	xRot += 0.025;}
	

   else if (ch == 'd'){
      xVel -= 0.05;
	xRot -= 0.025;}

   else if (ch == 'e')
      zVel -= 0.05;

   else if (ch == 'q')
      zVel += 0.05;

   //  Toggle axes
   else if (ch == 't' || ch == 'T')
      axes = 1-axes;
   //  Update shadow map if light position or objects changed
   if (strchr("<>oO-+[]",ch)) ShadowMap();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Store window dimensions
   Width  = width;
   Height = height;
}

//
//  Read text file
//
static char* ReadText(const char *file)
{
   int   n;
   char* buffer;
   //  Open file
   FILE* f = fopen(file,"rt");
   if (!f) Fatal("Cannot open text file %s\n",file);
   //  Seek to end to determine size, then rewind
   fseek(f,0,SEEK_END);
   n = ftell(f);
   rewind(f);
   //  Allocate memory for the whole file
   buffer = (char*)malloc(n+1);
   if (!buffer) Fatal("Cannot allocate %d bytes for text file %s\n",n+1,file);
   //  Snarf the file
   if (fread(buffer,n,1,f)!=1) Fatal("Cannot read %d bytes for text file %s\n",n,file);
   buffer[n] = 0;
   //  Close and return
   fclose(f);
   return buffer;
}

//
//  Print Shader Log
//
static void PrintShaderLog(int obj,const char* file)
{
   int len=0;
   glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) Fatal("Cannot allocate %d bytes of text for shader log\n",len);
      glGetShaderInfoLog(obj,len,&n,buffer);
      fprintf(stderr,"%s:\n%s\n",file,buffer);
      free(buffer);
   }
   glGetShaderiv(obj,GL_COMPILE_STATUS,&len);
   if (!len) Fatal("Error compiling %s\n",file);
}

//
//  Print Program Log
//
void PrintProgramLog(int obj)
{
   int len=0;
   glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) Fatal("Cannot allocate %d bytes of text for program log\n",len);
      glGetProgramInfoLog(obj,len,&n,buffer);
      fprintf(stderr,"%s\n",buffer);
   }
   glGetProgramiv(obj,GL_LINK_STATUS,&len);
   if (!len) Fatal("Error linking program\n");
}
//
//  Create Shader
//
void CreateShader(int prog,const GLenum type,const char* file)
{
   //  Create the shader
   int shader = glCreateShader(type);
   //  Load source code from file
   char* source = ReadText(file);
   glShaderSource(shader,1,(const char**)&source,NULL);
   free(source);
   //  Compile the shader
   glCompileShader(shader);
   //  Check for errors
   PrintShaderLog(shader,file);
   //  Attach to shader program
   glAttachShader(prog,shader);
}

//
//  Create Shader Program
//
int CreateShaderProg(const char* VertFile,const char* FragFile)
{
   //  Create program
   int prog = glCreateProgram();
   //  Create and compile vertex shader
   if (VertFile) CreateShader(prog,GL_VERTEX_SHADER,VertFile);
   //  Create and compile fragment shader
   if (FragFile) CreateShader(prog,GL_FRAGMENT_SHADER,FragFile);
   //  Link program
   glLinkProgram(prog);
   //  Check for errors
   PrintProgramLog(prog);
   //  Return name
   return prog;
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
	
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("Monte Anderson | Final Project (Penalty Kicks)");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
   if (!GLEW_VERSION_2_0) Fatal("OpenGL 2.0 not supported\n");
#endif


   //  Set callbacks
   glutDisplayFunc(display);
   glutTimerFunc(100, timer, 0);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutTimerFunc(dt,idle,0);
   //  Load textures
   tex2d[0] = LoadTexBMP("white.bmp");
   tex2d[1] = LoadTexBMP("Textures/grass.bmp");
   tex2d[2] = LoadTexBMP("Textures/grass_dark.bmp");
   tex2d[3] = LoadTexBMP("Textures/PL_1.bmp");


   team[0] = LoadTexBMP("Textures/arsenal.bmp");
   team[1] = LoadTexBMP("Textures/Chelsea.bmp");
   team[2] = LoadTexBMP("Textures/ManUnited.bmp");
   team[3] = LoadTexBMP("Textures/LiverPool.bmp");
   team[4] = LoadTexBMP("Textures/HotSpurs.bmp");
   team[5] = LoadTexBMP("Textures/Leicester.bmp");
   team[6] = LoadTexBMP("Textures/WestHam.bmp");

   // Enable Z-buffer
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);
   glPolygonOffset(4,0);
   //  Initialize texture map
   shader = CreateShaderProg("shadow.vert","shadow.frag");
   //  Initialize texture map
   InitMap();
   soccerball = LoadOBJ("yellow_ball_LOWPOLY.obj");
   soccergoal = LoadOBJ("soccergoal.obj");
   trophy = LoadOBJ("trophy.obj");
   stadium = LoadOBJ("stadium.obj");
   waterbottle = LoadOBJ("waterbottle.obj");

   sky[0]     = LoadTexBMP("Textures/test.bmp");
   sky[1]     = LoadTexBMP("Textures/skybox_evening.bmp");
   sky[2]     = LoadTexBMP("Textures/skybox_cloudy.bmp");

   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
