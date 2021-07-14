
//includes areas for keyboard control, mouse control, resizing the window
//and draws a spinning rectangle
#include <windows.h>		// Header File For Windows
#include "Image_Loading/nvImage.h"
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
//#include <iostream>
//using namespace std;
#include <math.h>
#include "OBB.h"
#include "mmsystem.h"
OBB playerCar, aiCar, box2, box3, box4, box5, box6, box7, box8, box9, box10, box11, box12, box13, turn1, turn2, turn3, turn4, finish, checkpoint1, checkpoint2, checkpoint3, slant1, slant2, slant3, slant4;

GLuint oCarTexture = 0;
GLuint road01 = 0;
GLuint rCarTexture = 0;
GLuint road02 = 0;
GLuint road03 = 0;
GLuint road04 = 0;
GLuint road05 = 0;
GLuint road08 = 0;
GLuint road09 = 0;
GLuint road10 = 0;
GLuint road11 = 0;
GLuint road12 = 0;
GLuint road13 = 0;
GLuint road14 = 0;
GLuint road20 = 0;
GLuint road21 = 0;
GLuint road22 = 0;
GLuint road23 = 0;
GLuint road26 = 0;
GLuint road27 = 0;
GLuint road28 = 0;
GLuint road29 = 0;
GLuint road30 = 0;
GLuint road31 = 0;
GLuint road32 = 0;
GLuint road39 = 0;
GLuint road40 = 0;
GLuint road41 = 0;
GLuint road44 = 0;
GLuint road45 = 0;
GLuint road46 = 0;
GLuint road47 = 0;
GLuint road48 = 0;
GLuint road49 = 0;
GLuint road50 = 0;
GLuint road56 = 0;
GLuint road62 = 0;
GLuint road63 = 0;
GLuint road64 = 0;
GLuint road65 = 0;
GLuint road66 = 0;
GLuint road67 = 0;
GLuint road68 = 0;
GLuint road87 = 0;
GLuint road89 = 0;
GLuint road100 = 0;
GLuint road101 = 0;
GLuint road102 = 0;
GLuint road103 = 0;
GLuint road104 = 0;
GLuint road105 = 0;
GLuint road106 = 0;
GLuint road107 = 0;
GLuint road108 = 0;
GLuint crowd1 = 0;
GLuint crowd2 = 0;
GLuint controls = 0;
GLuint nightmode = 0;
GLuint lap1 = 0;
GLuint lap2 = 0;
GLuint lose = 0;
GLuint win = 0;

GLuint loadPNG(char* name) {
	// Texture loading object
	nv::Image img;

	GLuint myTextureID;

	// Return true on success
	if (img.loadImageFromFile(name))
	{
		glGenTextures(1, &myTextureID);
		glBindTexture(GL_TEXTURE_2D, myTextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, img.getInternalFormat(), img.getWidth(), img.getHeight(), 0, img.getFormat(), img.getType(), img.getLevel(0));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
	}

	else
		MessageBox(NULL, "Failed to load texture", "End of the world", MB_OK | MB_ICONINFORMATION);

	return myTextureID;
}

const float PI = 3.1415926535897932384626433832795f;

int	mouse_x=0, mouse_y=0;
bool LeftPressed = false;
int screenWidth=800, screenHeight=800;
bool keys[256];
float spin=0;				

//variables for moving object
float Xtri = 150;
//float Xtri = 0;
float Ytri = -650;			//x and y coords of the user controlled object
//float Ytri = 0;

float Xai = 150;

float Yai = -700;			//x and y coords of the ai controlled object

const float maxspeed = 0.15;
float speed = 0.0;
const float revmaxspeed = 0.08;
float revspeed = 0.0;
float aispeed = 0.0;
float steer = 90.0;			//direction user is facing
float aisteer = 90.0;		//direction ai is facing
int animation = 0;
bool start = false;
bool nToggle = false;
bool nTogglec = false;
bool chk1 = false;
bool chk2 = false;
bool chk3 = false;
int playerlap = 0;
bool winner = false;

float point1x = 150;
float point1y = -800;

//float fowardx;
//float forwardy;
float aiforwardx = 0.001;
float aiforwardy = 0.0;

float vecx1;
float vecy1;
float vecx2;
float vecy2;

//draw circle function
void drawCircle(float x, float y, float radius); //prototype the draw function

//OPENGL FUNCTION PROTOTYPES
void display();				//draws everything to the screen
void reshape(int width, int height);//called when the window is resized
void init();				//called in winmain when the program starts.
void processKeys();			//called in winmain to process keyboard controls
void createTrack(float x, float y, GLuint texture, float rotation);
bool playerCollision(OBB object);
void originalv(OBB name, float blX, float blY, float tlX, float tlY, float trX, float trY, float brX, float brY);
bool circlecollisioninner(float x, float y, float radius, float cX, float cY);
bool circlecollisionouter(OBB object, float x, float y, float radius, float cX, float cY);
float aidotproduct(float vectorx, float vectory);


/*************    START OF OPENGL FUNCTIONS   ****************/
void display()									
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	sndPlaySound("MuteCity.wav", SND_NOSTOP| SND_NOWAIT| SND_ASYNC | SND_LOOP);

	glLoadIdentity();
	//spinning red square
	/*glPushMatrix();
	
		glRotatef(spin, 0.0, 0.0, 1.0);
		glColor3f(1.0,0.0,0.0);
		glBegin(GL_POLYGON);
			glVertex2f(-4,-4);
			glVertex2f(-4,4);
			glVertex2f(4,4);
			glVertex2f(4,-4);
		glEnd();*/

		//draws a point within the push/pop 
		//so the rotation is applied to the point.
		/*glColor3f(1.0,1.0,0.0);
		glPointSize(20.0);
		glBegin(GL_POINTS);
			glVertex2f(0,0);
		glEnd();
		glPointSize(1.0);*/

	//glPopMatrix();

	//draw axes
	/*glLineWidth(3.0);
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_LINES);
		glVertex2f(-20,0);
		glVertex2f(20,0);
		glVertex2f(0,20);
		glVertex2f(0,-20);
	glEnd();
	glLineWidth(1.0);*/
	

	//draw track
	createTrack(100, 100, road10, 0);
	createTrack(150, 100, road11, 0);
	createTrack(150, 50, road29, 0);
	createTrack(100, 50, road28, 0);
	createTrack(100, 0, road101, 0);
	createTrack(150, 0, road102, 0);
	createTrack(100, -50, road01, 0);
	createTrack(150, -50, road01, 0);
	createTrack(100, -100, road01, 0);
	createTrack(150, -100, road01, 0);
	createTrack(100, -150, road104, 0);
	createTrack(150, -150, road105, 0);
	createTrack(100, -200, road48, 0);
	createTrack(150, -200, road49, 0);
	createTrack(200, -200, road50, 0);
	createTrack(100, -250, road66, 0);
	createTrack(150, -250, road67, 0);
	createTrack(200, -250, road68, 0);
	createTrack(150, -300, road48, 0);
	createTrack(200, -300, road49, 0);
	createTrack(250, -300, road50, 0);
	createTrack(150, -350, road66, 0);
	createTrack(200, -350, road67, 0);
	createTrack(250, -350, road68, 0);
	createTrack(200, -400, road21, 0);
	createTrack(250, -400, road23, 0);
	createTrack(200, -450, road21, 0);
	createTrack(250, -450, road23, 0);
	createTrack(200, -500, road21, 0);
	createTrack(250, -500, road23, 0);
	createTrack(200, -550, road44, 0);
	createTrack(250, -550, road45, 0);
	createTrack(200, -600, road62, 0);
	createTrack(250, -600, road63, 0);
	//
	createTrack(300, -550, road04, 0);
	createTrack(300, -600, road40, 0);

	createTrack(350, -550, road10, 0);
	createTrack(400, -550, road11, 0);
	createTrack(350, -600, road28, 0);
	createTrack(400, -600, road29, 0);
	createTrack(350, -650, road46, 0);
	createTrack(400, -650, road47, 0);
	createTrack(350, -700, road64, 0);
	createTrack(400, -700, road65, 0);
	//
	createTrack(300, -650, road04, 0);
	createTrack(300, -700, road40, 0);
	createTrack(250, -650, road04, 0);
	createTrack(250, -700, road40, 0);
	createTrack(200, -650, road04, 0);
	createTrack(200, -700, road40, 0);
	createTrack(150, -650, road106, 0);
	createTrack(150, -700, road107, 0);
	createTrack(100, -650, road87, 0);
	createTrack(100, -700, road89, 0);
	createTrack(50, -650, road04, 0);
	createTrack(50, -700, road40, 0);
	createTrack(0, -650, road04, 0);
	createTrack(0, -700, road40, 0);
	//
	createTrack(-100, -700, road62, 0);
	createTrack(-50, -700, road63, 0);
	createTrack(-100, -650, road44, 0);
	createTrack(-50, -650, road45, 0);
//
	createTrack(-100, -600, road21, 0);
	createTrack(-50, -600, road23, 0);
//
	createTrack(-100, -550, road30, 0);
	createTrack(-50, -550, road31, 0);
	createTrack(-0, -550, road32, 0);
	createTrack(-100, -500, road12, 0);
	createTrack(-50, -500, road13, 0);
	createTrack(-0, -500, road14, 0);

	createTrack(-50, -450, road30, 0);
	createTrack(-0, -450, road31, 0);
	createTrack(50, -450, road32, 0);

	
	createTrack(-50, -400, road12, 0);
	createTrack(-0, -400, road13, 0);
	createTrack(50, -400, road14, 0);


	createTrack(0, -350, road21, 0);
	createTrack(0, -300, road21, 0);
	createTrack(0, -250, road21, 0);
	createTrack(50, -350, road23, 0);
	createTrack(50, -300, road23, 0);
	createTrack(50, -250, road23, 0);
	createTrack(0, -200, road21, 0);
	createTrack(0, -150, road21, 0);
	createTrack(0, -100, road21, 0);
	createTrack(50, -200, road23, 0);
	createTrack(50, -150, road23, 0);
	createTrack(50, -100, road23, 0);
	createTrack(0, -50, road21, 0);
	createTrack(0, -0, road21, 0);
	createTrack(50, -50, road23, 0);
	createTrack(50, -0, road23, 0);

	createTrack(0, 50, road26, 0);
	createTrack(50, 50, road27, 0);
	createTrack(0, 100, road08, 0);
	createTrack(50, 100, road09, 0);
	

	//draw temp lines
	/*glPushMatrix();
	//glRotatef(315, 0.0, 0.0, 1.0);
	glBegin(GL_POLYGON);
	glColor4f(1.0, 0.0, 0.0, 0.5);
	glVertex2f(17, -575);
	glVertex2f(17, -375);
	glVertex2f(25, -375);
	glVertex2f(25, -575);
	glEnd();
	glPopMatrix();*/

	float matrixwall1[16];
	glPushMatrix();
		glTranslatef(21, -475, 1);
		glRotatef(333.5, 0.0f, 0.0f, 1.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, matrixwall1);
		glBegin(GL_POLYGON);
			glColor4f(1.0, 0.0, 0.0, 0.0);
			glVertex2f(-4, 115);
			glVertex2f(4, 115);
			glVertex2f(4, -115);
			glVertex2f(-4, -115);
		glEnd();
	glPopMatrix();

	float matrixwall2[16];
	glPushMatrix();
	glTranslatef(-71, -475, 1);
	glRotatef(333.5, 0.0f, 0.0f, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixwall2);
	glBegin(GL_POLYGON);
	glColor4f(1.0, 0.0, 0.0, 0.0);
	glVertex2f(-4, 115);
	glVertex2f(4, 115);
	glVertex2f(4, -115);
	glVertex2f(-4, -115);
	glEnd();
	glPopMatrix();

	float matrixwall3[16];
	glPushMatrix();
	glTranslatef(129, -275, 1);
	glRotatef(26.5, 0.0f, 0.0f, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixwall3);
	glBegin(GL_POLYGON);
	glColor4f(1.0, 0.0, 0.0, 0.0);
	glVertex2f(-4, 115);
	glVertex2f(4, 115);
	glVertex2f(4, -115);
	glVertex2f(-4, -115);
	glEnd();
	glPopMatrix();

	float matrixwall4[16];
	glPushMatrix();
	glTranslatef(221, -275, 1);
	glRotatef(26.5, 0.0f, 0.0f, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixwall4);
	glBegin(GL_POLYGON);
	glColor4f(1.0, 0.0, 0.0, 0.0);
	glVertex2f(-4, 115);
	glVertex2f(4, 115);
	glVertex2f(4, -115);
	glVertex2f(-4, -115);
	glEnd();
	glPopMatrix();

	//bluechkpoint
	/*glPushMatrix();
	glBegin(GL_POLYGON);
	glColor4f(0.0, 0.0, 1.0, 0.5);
	glVertex2f(-25, -371);
	glVertex2f(-25, -375);
	glVertex2f(75, -375);
	glVertex2f(75, -371);
	glEnd();
	glPopMatrix();*/

	//glPushMatrix();
	//glBegin(GL_POLYGON);
	//glColor3f(0.0, 0.0, 0.3);
	////glGetFloatv(GL_MODELVIEW_MATRIX, matrixtestbox);
	//glVertex2f(0, 20);
	//glVertex2f(10, 20);
	//glVertex2f(10, 0);
	//glVertex2f(0, 0);
	//glEnd();
	//glPopMatrix();

	//tempcircle

	//glPushMatrix();
	//glColor3f(1.0, 0.0, 0.0);
	//drawCircle(-25, -625, 93.0);
	//glPopMatrix();

	//glPushMatrix();
	//glColor3f(1.0, 0.0, 0.0);
	//drawCircle(-25, -625, 8.0);
	//glPopMatrix();

	////double points
	//glPushMatrix();
	//glColor3f(1.0, 0.0, 0.0);
	//drawCircle(125, -167, 8.0);
	//glPopMatrix();

	//glPushMatrix();
	//glColor3f(1.0, 0.0, 0.0);
	//drawCircle(125, 17, 8.0);
	//glPopMatrix();

	//glPushMatrix();
	//glColor3f(1.0, 0.0, 0.0);
	//drawCircle(75, 25, 8.0);
	//glPopMatrix();

	//glPushMatrix();
	//glColor3f(1.0, 0.0, 0.0);
	//drawCircle(75, 25, 93.0);
	//glPopMatrix();

	//glPushMatrix();
	//glColor3f(1.0, 0.0, 0.0);
	//drawCircle(275, -525, 8.0);
	//glPopMatrix();

	//glPushMatrix();
	//glColor3f(1.0, 0.0, 0.0);
	//drawCircle(275, -525, 93.0);
	//glPopMatrix();

	//glPushMatrix();
	//glColor3f(1.0, 0.0, 0.0);
	//drawCircle(325, -625, 8.0);
	//glPopMatrix();

	//glPushMatrix();
	//glColor3f(1.0, 0.0, 0.0);
	//drawCircle(325, -625, 93.0);
	//glPopMatrix();

	//draw car2
	float matrixaicar[16];
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, rCarTexture);
	glTranslatef(Xai, Yai, 0.0);
	glRotatef(aisteer, 0.0, 0.0, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixaicar);
	glBegin(GL_POLYGON);
	glColor3f(1.0, 1.0, 1.0);
	glTexCoord2f(0.0, 0.0); glVertex2f(-3, -6);
	glTexCoord2f(0.0, 1.0); glVertex2f(-3, 6);
	glTexCoord2f(1.0, 1.0);	glVertex2f(3, 6);
	glTexCoord2f(1.0, 0.0); glVertex2f(3, -6);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	
	//carcricle
	//glPushMatrix();
	//glColor3f(1.0, 0.0, 0.0);
	//drawCircle(Xtri, Ytri, 6.0);
	//glPopMatrix();
	
	//draw car
	float matrixplayercar[16];
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, oCarTexture);
	glTranslatef(Xtri, Ytri, 0.0);
	glRotatef(steer, 0.0, 0.0, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrixplayercar);
		glBegin(GL_POLYGON);
		glColor3f(1.0,1.0,1.0);
		glTexCoord2f(0.0, 0.0); glVertex2f(-3, -6);
		glTexCoord2f(0.0, 1.0); glVertex2f(-3, 6);
		glTexCoord2f(1.0, 1.0);	glVertex2f(3, 6);
		glTexCoord2f(1.0, 0.0); glVertex2f(3, -6);
		glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//win
	if (winner == true) {
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D, win);
		glTranslatef(Xtri-100, Ytri-100, 1);
		glBegin(GL_POLYGON);
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord2f(0.0, 0.0); glVertex2f(0, 0);
		glTexCoord2f(0.0, 1.0); glVertex2f(0, 50);
		glTexCoord2f(1.0, 1.0);	glVertex2f(150, 50);
		glTexCoord2f(1.0, 0.0); glVertex2f(150, 0);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	//controls
	if (start == false) {
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D, controls);
		glBegin(GL_POLYGON);
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord2f(0.0, 0.0); glVertex2f(100, -630);
		glTexCoord2f(0.0, 1.0); glVertex2f(250, -630);
		glTexCoord2f(1.0, 1.0);	glVertex2f(250, -720);
		glTexCoord2f(1.0, 0.0); glVertex2f(100, -720);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}

	//night mode
	if (nToggle == true) {
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D, nightmode);
		glTranslatef(Xtri, Ytri, 0.0);
		glRotatef(steer, 0.0, 0.0, 1.0);
		glBegin(GL_POLYGON);
		//glColor4f(1.0, 0.0, 0.0, 0.5);
		glTexCoord2f(0.0, 0.0); glVertex2f(-150, -150);
		glTexCoord2f(0.0, 1.0); glVertex2f(-150, 150);
		glTexCoord2f(1.0, 1.0); glVertex2f(150, 150);
		glTexCoord2f(1.0, 0.0); glVertex2f(150, -150);
		glEnd();
		glPopMatrix();
	}
	

	//animated crowd
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	animation = animation + 1;
	if (animation < 300) {
		glBindTexture(GL_TEXTURE_2D, crowd1);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, crowd2);
		if (animation > 600) {
			animation = 0;
		 }
	}
	glBegin(GL_POLYGON);
	//glColor4f(1.0, 0.0, 0.0, 0.3);
	glTexCoord2f(0.0, 0.0); glVertex2f( 50, -570);
	glTexCoord2f(0.0, 1.0); glVertex2f( 150, -570);
	glTexCoord2f(1.0, 1.0); glVertex2f( 150, -620);
	glTexCoord2f(1.0, 0.0); glVertex2f( 50, -620);
	glEnd();
	glPopMatrix();

	//lap
	glPushMatrix();
	glTranslatef(Xtri-80, Ytri+80, 1);
	if (playerlap == 0) {
		glBindTexture(GL_TEXTURE_2D, lap1);
	}
	else if (playerlap > 0) {
		glBindTexture(GL_TEXTURE_2D, lap2);
		

	}
	if (playerlap > 1) {
		winner = true;
	}
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex2f(0, 0);
	glTexCoord2f(0.0, 1.0); glVertex2f(0, 10);
	glTexCoord2f(1.0, 1.0); glVertex2f(40, 10);
	glTexCoord2f(1.0, 0.0); glVertex2f(40, 0);
	glEnd();
	glPopMatrix();

	//tyre tracks
	//trackeditor
	

	//ai
	if (start) {
		float tmpx = aiforwardx;
		float tmpy = aiforwardy;
		
		
		aiforwardx = -sin(aisteer*PI / 180) * aispeed;
		aiforwardy = cos(aisteer*PI / 180) * aispeed;

		vecx1 = aiforwardx - tmpx;
		vecy1 = aiforwardy = tmpy;

		if (aispeed < 0.15f)
		{
			aispeed += 0.00001f;
		}
		else { aispeed = 0.15f; }

		//aisteer = aidotproduct(vecx1, vecy1);

		Yai += aiforwardy;
		Xai += aiforwardx;

		/*if (circlecollisioninner(Xai, Yai, 6.0, Xtri, Ytri)) {
			aispeed = -aispeed/2;
		}*/
		if (playerCollision(aiCar)) {
			aispeed = -aispeed / 2;
		}

	}

		/*glColor3f(1.0,1.0,0.0);
		glPointSize(20.0);
		glBegin(GL_POINTS);
			glVertex2f(0,0);
		glEnd();
		glPointSize(1.0);*/
	

	
	playerCar.transformPoints(matrixplayercar);
	aiCar.transformPoints(matrixaicar);
	slant1.transformPoints(matrixwall1);
	slant2.transformPoints(matrixwall2);
	slant3.transformPoints(matrixwall3);
	slant4.transformPoints(matrixwall4);
	box2.transformPointsStat();
	box3.transformPointsStat();
	box4.transformPointsStat();
	box5.transformPointsStat();
	box6.transformPointsStat();
	box7.transformPointsStat();
	box8.transformPointsStat();
	box9.transformPointsStat();
	box10.transformPointsStat();
	box11.transformPointsStat();
	box12.transformPointsStat();
	turn1.transformPointsStat();
	turn2.transformPointsStat();
	turn3.transformPointsStat();
	turn4.transformPointsStat();
	finish.transformPointsStat();
	checkpoint1.transformPointsStat();
	checkpoint2.transformPointsStat();
	checkpoint3.transformPointsStat();
	/*box13.transformPointsStat();*/

	playerCollision(box2);
	playerCollision(box3);
	playerCollision(box4);
	playerCollision(box5);
	playerCollision(box6);
	playerCollision(box7);
	playerCollision(box8);
	playerCollision(box9);
	playerCollision(box10);
	playerCollision(box11);
	playerCollision(box12);
	playerCollision(slant1);
	playerCollision(slant2);
	playerCollision(slant3);
	playerCollision(slant4);

	//if (playerCollision(box2)) {
	//	glColor3f(1.0, 0.0, 1.0);
	//}
	//else {
	//	glColor3f(1.0, 1.0, 1.0);
	//}

	circlecollisioninner(Xai, Yai, 6.0, Xtri, Ytri);

	circlecollisioninner(-25, -625, 8.0, Xtri, Ytri);
	circlecollisioninner(125, -167, 8.0, Xtri, Ytri);
	circlecollisioninner(125, 17, 8.0, Xtri, Ytri);
	circlecollisioninner(75, 25, 8.0, Xtri, Ytri);
	circlecollisioninner(275, -525, 8.0, Xtri, Ytri);
	circlecollisioninner(325, -625, 8.0, Xtri, Ytri);

	circlecollisionouter(turn1, -25, -625, 93.0, Xtri, Ytri);
	circlecollisionouter(turn2, 75, 25, 93.0, Xtri, Ytri);
	circlecollisionouter(turn3, 275, -525, 93.0, Xtri, Ytri);
	circlecollisionouter(turn4, 325, -625, 93.0, Xtri, Ytri);

	//playerCar.drawOBB();
	//aiCar.drawOBB();
	/*box2.drawOBB();
	box3.drawOBB();
	box4.drawOBB();
	box5.drawOBB();
	box6.drawOBB();
	box7.drawOBB();
	box8.drawOBB();
	box9.drawOBB();
	box10.drawOBB();
	box11.drawOBB();
	box12.drawOBB();
	slant1.drawOBB();
	slant2.drawOBB();
	slant3.drawOBB();
	slant4.drawOBB();*/

	
	/*box13.drawOBB();*/

	//finish line detection
	//finish.drawOBB();
	//checkpoint1.drawOBB();
	//checkpoint2.drawOBB();
	//checkpoint3.drawOBB();

	if (playerCar.SAT2D(checkpoint1)) {
		chk1 = true;
	}
	if (playerCar.SAT2D(checkpoint2)) {
		if (chk1) {
			chk2 = true;
		}
	}
	if (playerCar.SAT2D(checkpoint3)) {
		if (chk2) {
			chk3 = true;
		}
	}
	if (playerCar.SAT2D(finish)) {
		if (chk3) {
			chk1 = false;
			chk2 = false;
			chk3 = false;
			playerlap++;
		}
	}
	if (playerlap == 2) {
		glColor3f(1.0, 0.0, 1.0);
	}

	glFlush();

	if (steer > 360) {
		steer = steer - 360;
	}
	
}

float aidotproduct(float vectorx, float vectory) {
	float numerator = (vectorx*point1x) + (vectory*point1y);
	float denominatorai = sqrt((vectorx*vectorx) + (vectory*vectory));
	float denominatorpoint = sqrt((point1x*point1x) + (point1y*point1y));
	float angle = acos((numerator / (denominatorai*denominatorpoint)));
	return angle;
}

bool circlecollisionouter(OBB object, float x, float y, float radius, float cX, float cY) {
	float collision = ((cX - x)*(cX - x)) + ((y - cY)*(y - cY));
	float totaldistance = (radius - 3.0)*(radius - 3.0);

	if (playerCar.SAT2D(object)) {
		if (collision <= totaldistance) {
			
		}
		else {
			if (speed > 0.0) {
				revspeed = speed / 2;
				speed = 0;
			}
			else if (revspeed > 0.0) {
				speed = revspeed / 2;
				revspeed = 0;
			}
			return true;
		}
	}
	else {
		return false;
	}
}

bool circlecollisioninner(float x, float y, float radius, float cX, float cY) {
	float collision = ((cX - x)*(cX - x)) + ((y - cY)*(y - cY));
	float totaldistance = (radius + 3.0)*(radius + 3.0);
	if (collision <= totaldistance) {
		if (speed > 0.0) {
			revspeed = speed / 2;
			speed = 0;
		}
		else if (revspeed > 0.0) {
			speed = revspeed / 2;
			revspeed = 0;
		}
		return true;
	}
	else {
		return false;
	}
}

bool playerCollision(OBB object) {
	if (playerCar.SAT2D(object)) {
		if (speed > 0.0) {
			revspeed = speed / 2;
			speed = 0;
		}
		else if (revspeed > 0.0) {
			speed = revspeed / 2;
			revspeed = 0;
		}
		return true;
	}
	else {

		return false;
	}	
}

//bool collision(float minx, float maxx, float miny, float maxy) {
//	if (minx < Xtri && Xtri < maxx) {
//		if (miny < Ytri && Ytri < maxy) {
//			return true;
//		}
//		else {
//
//		}
//	}
//	else {
//		
//		speed = -speed;
//		revspeed = -revspeed;
//		steer =  270 + steer;
//
//
//	}
//	return false;
//}

void createTrack(float x, float y, GLuint texture, float rotation)
{
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 1.0); glVertex2f(-25, 25);//top left
	glTexCoord2f(1.0, 1.0); glVertex2f(25, 25);//top right
	glTexCoord2f(1.0, 0.0);	glVertex2f(25, -25);// bottom right
	glTexCoord2f(0.0, 0.0); glVertex2f(-25, -25);//bottom left
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void drawCircle(float x, float y, float radius)
{
	glBegin(GL_LINE_LOOP);
	for(float i=0; i<360; i+=5) 
	{
		float xcoord = x + radius * cosf(i*(PI/180.0f));
		float ycoord = y + radius * sinf(i*(PI/180.0f));
		glVertex2f(xcoord, ycoord);
	}
	glEnd();
}


//void originalv(OBB &name, float blX, float blY, float tlX, float tlY, float trX, float trY, float brX, float brY) {
//	name.vertOriginal[0].x = blX;
//	name.vertOriginal[0].y = blY;
//
//	name.vertOriginal[1].x = tlX;
//	name.vertOriginal[1].y = tlY;
//
//	name.vertOriginal[2].x = trX;
//	name.vertOriginal[2].y = trY;
//
//	name.vertOriginal[3].x = brX;
//	name.vertOriginal[3].y = brY;
//}


void reshape(int width, int height)		// Resize the OpenGL window
{
	screenWidth=width; screenHeight = height;           // to ensure the mouse coordinates match 
														// we will use these values to set the coordinate system

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	//gluOrtho2D(-100, 100, -100, 100);           // set the coordinate system for the window
	gluOrtho2D(Xtri-100, Xtri +100, Ytri-100, Ytri+100);
	//gluOrtho2D(-300, 800, -800, 300);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}
void init()
{
	glClearColor(0.5,0.3,0.05,1.0);						//sets the clear colour to yellow
														//glClear(GL_COLOR_BUFFER_BIT) in the display function
														//will clear the buffer to this colour.
	oCarTexture = loadPNG("orangecar.png");
	road01 = loadPNG("road01.png");
	road02 = loadPNG("road02.png");
	road03 = loadPNG("road03.png");
	road04 = loadPNG("road04.png");
	road05 = loadPNG("road05.png");
	road08 = loadPNG("road08.png");
	road09 = loadPNG("road09.png");
	road10 = loadPNG("road10.png");
	road11 = loadPNG("road11.png");
	road12 = loadPNG("road12.png");
	road13 = loadPNG("road13.png");
	road14 = loadPNG("road14.png");
	road20 = loadPNG("road20.png");
	road21 = loadPNG("road21.png");
	road22 = loadPNG("road22.png");
	road23 = loadPNG("road23.png");
	road26 = loadPNG("road26.png");
	road27 = loadPNG("road27.png");
	road28 = loadPNG("road28.png");
	road29 = loadPNG("road29.png");
	road30 = loadPNG("road30.png");
	road31 = loadPNG("road31.png");
	road32 = loadPNG("road32.png");
	road39 = loadPNG("road39.png");
	road40 = loadPNG("road40.png");
	road41 = loadPNG("road41.png");
	road44 = loadPNG("road44.png");
	road45 = loadPNG("road45.png");
	road46 = loadPNG("road46.png");
	road47 = loadPNG("road47.png");
	road48 = loadPNG("road48.png");
	road49 = loadPNG("road49.png");
	road50 = loadPNG("road50.png");
	road56 = loadPNG("road56.png");
	road62 = loadPNG("road62.png");
	road63 = loadPNG("road63.png");
	road64 = loadPNG("road64.png");
	road65 = loadPNG("road65.png");
	road66 = loadPNG("road66.png");
	road67 = loadPNG("road67.png");
	road68 = loadPNG("road68.png");
	road87 = loadPNG("road87.png");
	road89 = loadPNG("road89.png");
	road100 = loadPNG("road100.png");
	road101 = loadPNG("road101.png");
	road102 = loadPNG("road102.png");
	road103 = loadPNG("road103.png");
	road104 = loadPNG("road104.png");
	road105 = loadPNG("road105.png");
	road106 = loadPNG("road106.png");
	road107 = loadPNG("road107.png");
	road108 = loadPNG("road108.png");
	rCarTexture = loadPNG("redcar.png");
	crowd1 = loadPNG("crowd1.png");
	crowd2 = loadPNG("crowd2.png");
	controls = loadPNG("Controls.png");
	nightmode = loadPNG("nightmode.png");
	lap1 = loadPNG("lap1.png");
	lap2 = loadPNG("lap2.png");
	lose = loadPNG("lose.png");
	win = loadPNG("win.png");

	playerCar.vertOriginal[0].x = -3;
	playerCar.vertOriginal[0].y = -6;

	playerCar.vertOriginal[1].x = -3;
	playerCar.vertOriginal[1].y = 6;

	playerCar.vertOriginal[2].x = 3;
	playerCar.vertOriginal[2].y = 6;

	playerCar.vertOriginal[3].x = 3;
	playerCar.vertOriginal[3].y = -6;

	aiCar.vertOriginal[0].x = -3;
	aiCar.vertOriginal[0].y = -6;

	aiCar.vertOriginal[1].x = -3;
	aiCar.vertOriginal[1].y = 6;

	aiCar.vertOriginal[2].x = 3;
	aiCar.vertOriginal[2].y = 6;

	aiCar.vertOriginal[3].x = 3;
	aiCar.vertOriginal[3].y = -6;

	box2.vertOriginal[0].x = -25;
	box2.vertOriginal[0].y = -725;
	box2.vertOriginal[1].x = -25;
	box2.vertOriginal[1].y = -717;
	box2.vertOriginal[2].x = 325;
	box2.vertOriginal[2].y = -717;
	box2.vertOriginal[3].x = 323;
	box2.vertOriginal[3].y = -725;

	box3.vertOriginal[0].x = -25;
	box3.vertOriginal[0].y = -633;
	box3.vertOriginal[1].x = -25;
	box3.vertOriginal[1].y = -617;
	box3.vertOriginal[2].x = 325;
	box3.vertOriginal[2].y = -617;
	box3.vertOriginal[3].x = 323;
	box3.vertOriginal[3].y = -633;
	
	box4.vertOriginal[0].x = -125;
	box4.vertOriginal[0].y = -625;
	box4.vertOriginal[1].x = -125;
	box4.vertOriginal[1].y = -575;
	box4.vertOriginal[2].x = -117;
	box4.vertOriginal[2].y = -575;
	box4.vertOriginal[3].x = -117;
	box4.vertOriginal[3].y = -625;

	box5.vertOriginal[0].x = -33;
	box5.vertOriginal[0].y = -625;
	box5.vertOriginal[1].x = -33;
	box5.vertOriginal[1].y = -575;
	box5.vertOriginal[2].x = -25;
	box5.vertOriginal[2].y = -575;
	box5.vertOriginal[3].x = -25;
	box5.vertOriginal[3].y = -625;

	box6.vertOriginal[0].x = -25;
	box6.vertOriginal[0].y = -375;
	box6.vertOriginal[1].x = -25;
	box6.vertOriginal[1].y = 25;
	box6.vertOriginal[2].x = -17;
	box6.vertOriginal[2].y = 25;
	box6.vertOriginal[3].x = -17;
	box6.vertOriginal[3].y = -375;

	box7.vertOriginal[0].x = 67;
	box7.vertOriginal[0].y = -375;
	box7.vertOriginal[1].x = 67;
	box7.vertOriginal[1].y = 25;
	box7.vertOriginal[2].x = 83;
	box7.vertOriginal[2].y = 25;
	box7.vertOriginal[3].x = 83;
	box7.vertOriginal[3].y = -375;

	box8.vertOriginal[0].x = 167;
	box8.vertOriginal[0].y = -175;
	box8.vertOriginal[1].x = 167;
	box8.vertOriginal[1].y = 25;
	box8.vertOriginal[2].x = 175;
	box8.vertOriginal[2].y = 25;
	box8.vertOriginal[3].x = 175;
	box8.vertOriginal[3].y = -175;

	box9.vertOriginal[0].x = 117;
	box9.vertOriginal[0].y = -167;
	box9.vertOriginal[1].x = 117;
	box9.vertOriginal[1].y = 17;
	box9.vertOriginal[2].x = 133;
	box9.vertOriginal[2].y = 17;
	box9.vertOriginal[3].x = 133;
	box9.vertOriginal[3].y = -167;

	box10.vertOriginal[0].x = 175;
	box10.vertOriginal[0].y = -525;
	box10.vertOriginal[1].x = 175;
	box10.vertOriginal[1].y = -375;
	box10.vertOriginal[2].x = 183;
	box10.vertOriginal[2].y = -375;
	box10.vertOriginal[3].x = 183;
	box10.vertOriginal[3].y = -525;

	box11.vertOriginal[0].x = 267;
	box11.vertOriginal[0].y = -525;
	box11.vertOriginal[1].x = 267;
	box11.vertOriginal[1].y = -375;
	box11.vertOriginal[2].x = 275;
	box11.vertOriginal[2].y = -375;
	box11.vertOriginal[3].x = 275;
	box11.vertOriginal[3].y = -525;

	box12.vertOriginal[0].x = 275;
	box12.vertOriginal[0].y = -533;
	box12.vertOriginal[1].x = 275;
	box12.vertOriginal[1].y = -525;
	box12.vertOriginal[2].x = 325;
	box12.vertOriginal[2].y = -525;
	box12.vertOriginal[3].x = 325;
	box12.vertOriginal[3].y = -533;

	//box13.vertOriginal[0].x = -33;
	//box13.vertOriginal[0].y = -575;

	//box13.vertOriginal[1].x = 67;
	//box13.vertOriginal[1].y = -375;

	//box13.vertOriginal[2].x = 76;
	//box13.vertOriginal[2].y = -378;

	//box13.vertOriginal[3].x = -28;
	//box13.vertOriginal[3].y = -578;

	turn1.vertOriginal[0].x = -125;
	turn1.vertOriginal[0].y = -725;
	turn1.vertOriginal[1].x = -125;
	turn1.vertOriginal[1].y = -625;
	turn1.vertOriginal[2].x = -25;
	turn1.vertOriginal[2].y = -625;
	turn1.vertOriginal[3].x = -25;
	turn1.vertOriginal[3].y = -725;

	turn2.vertOriginal[0].x = -25;
	turn2.vertOriginal[0].y = 25;
	turn2.vertOriginal[1].x = -25;
	turn2.vertOriginal[1].y = 125;
	turn2.vertOriginal[2].x = 175;
	turn2.vertOriginal[2].y = 125;
	turn2.vertOriginal[3].x = 175;
	turn2.vertOriginal[3].y = 25;

	turn3.vertOriginal[0].x = 175;
	turn3.vertOriginal[0].y = -625;
	turn3.vertOriginal[1].x = 175;
	turn3.vertOriginal[1].y = -525;
	turn3.vertOriginal[2].x = 275;
	turn3.vertOriginal[2].y = -525;
	turn3.vertOriginal[3].x = 275;
	turn3.vertOriginal[3].y = -625;

	turn4.vertOriginal[0].x = 325;
	turn4.vertOriginal[0].y = -725;
	turn4.vertOriginal[1].x = 325;
	turn4.vertOriginal[1].y = -525;
	turn4.vertOriginal[2].x = 425;
	turn4.vertOriginal[2].y = -525;
	turn4.vertOriginal[3].x = 425;
	turn4.vertOriginal[3].y = -725;

	finish.vertOriginal[0].x = 98;
	finish.vertOriginal[0].y = -625;
	finish.vertOriginal[1].x = 102;
	finish.vertOriginal[1].y = -625;
	finish.vertOriginal[2].x = 102;
	finish.vertOriginal[2].y = -725;
	finish.vertOriginal[3].x = 98;
	finish.vertOriginal[3].y = -725;

	checkpoint1.vertOriginal[0].x = -25;
	checkpoint1.vertOriginal[0].y = -371;
	checkpoint1.vertOriginal[1].x = -25;
	checkpoint1.vertOriginal[1].y = -375;
	checkpoint1.vertOriginal[2].x = 75;
	checkpoint1.vertOriginal[2].y = -375;
	checkpoint1.vertOriginal[3].x = 75;
	checkpoint1.vertOriginal[3].y = -371;

	checkpoint2.vertOriginal[0].x = 67;
	checkpoint2.vertOriginal[0].y = 21;
	checkpoint2.vertOriginal[1].x = 67;
	checkpoint2.vertOriginal[1].y = 25;
	checkpoint2.vertOriginal[2].x = 167;
	checkpoint2.vertOriginal[2].y = 25;
	checkpoint2.vertOriginal[3].x = 167;
	checkpoint2.vertOriginal[3].y = 21;

	checkpoint3.vertOriginal[0].x = 175;
	checkpoint3.vertOriginal[0].y = -529;
	checkpoint3.vertOriginal[1].x = 175;
	checkpoint3.vertOriginal[1].y = -525;
	checkpoint3.vertOriginal[2].x = 275;
	checkpoint3.vertOriginal[2].y = -525;
	checkpoint3.vertOriginal[3].x = 275;
	checkpoint3.vertOriginal[3].y = -529;

	slant1.vertOriginal[0].x = -4;
	slant1.vertOriginal[0].y = 115;
	slant1.vertOriginal[1].x = 4;
	slant1.vertOriginal[1].y = 115;
	slant1.vertOriginal[2].x = 4;
	slant1.vertOriginal[2].y = -115;
	slant1.vertOriginal[3].x = -4;
	slant1.vertOriginal[3].y = -115;

	slant2.vertOriginal[0].x = -4;
	slant2.vertOriginal[0].y = 115;
	slant2.vertOriginal[1].x = 4;
	slant2.vertOriginal[1].y = 115;
	slant2.vertOriginal[2].x = 4;
	slant2.vertOriginal[2].y = -115;
	slant2.vertOriginal[3].x = -4;
	slant2.vertOriginal[3].y = -115;

	slant3.vertOriginal[0].x = -4;
	slant3.vertOriginal[0].y = 115;
	slant3.vertOriginal[1].x = 4;
	slant3.vertOriginal[1].y = 115;
	slant3.vertOriginal[2].x = 4;
	slant3.vertOriginal[2].y = -115;
	slant3.vertOriginal[3].x = -4;
	slant3.vertOriginal[3].y = -115;

	slant4.vertOriginal[0].x = -4;
	slant4.vertOriginal[0].y = 115;
	slant4.vertOriginal[1].x = 4;
	slant4.vertOriginal[1].y = 115;
	slant4.vertOriginal[2].x = 4;
	slant4.vertOriginal[2].y = -115;
	slant4.vertOriginal[3].x = -4;
	slant4.vertOriginal[3].y = -115;
}

void processKeys()
{
	if (keys[VK_SPACE]) {
		Xtri = 150;
		Ytri = -650;
		if (nTogglec == false) {
			if (nToggle) {
				nToggle = false;
				nTogglec = true;
			}
			else {
				nToggle = true;
				nTogglec = true;
			}
		}
	}
	if (keys[VK_SPACE] == false) {
		nTogglec = false;
	}
	if(keys[VK_LEFT])
	{	
		if (steer > 360) {
			steer = steer - 360;
		}
		if (steer < 0) {
			steer = 360;
		}
		else { steer += 0.2f; }
		//Xtri-=0.01f;
	}
	if(keys[VK_RIGHT])
	{	
		if (steer < 0) {
			steer = 360;
		}
		if (steer > 360) {
			steer = steer - 360;
		}
		else { steer -= 0.2f; }
		//Xtri+=0.01f +speed;
	}
	if(keys[VK_UP])
	{	
		start = true;
		if (revspeed > 0.0) {
			revspeed += -0.00005f;
		}
		else {
			revspeed = 0.0;
			float forwardx = -sin(steer*PI / 180) * speed;
			float forwardy = cos(steer*PI / 180) * speed;

			if (speed < maxspeed)
			{
				speed += 0.00005f;
			}
			else { speed = maxspeed; }


			Ytri += forwardy;
			Xtri += forwardx;
		}
		
	}
	if (keys[VK_UP] == false)
	{
		if (revspeed > 0.0) {

		}
		else {
			if (speed > 0.0) {
				speed += -0.00005f;

			}
			else {
				speed = 0.0;
			}
			float forwardx = -sin(steer*PI / 180) * speed;
			float forwardy = cos(steer*PI / 180) * speed;
			Ytri += forwardy;
			Xtri += forwardx;
			//Ytri += speed;
		}
		
	}
	if(keys[VK_DOWN])
	{
		start = true;

		if (speed > 0.0) {//if car is moving forward
			speed += -0.00005f;//slowdown car faster if moving forward while pressing back
		}
		else {//if car is idle
			speed = 0.0;
			
			if (revspeed < revmaxspeed)//if not at max speed
			{
				revspeed += 0.00005f;//increment reverse speed
			}
			else {
				revspeed = revmaxspeed; //else limit to maxspeed
			}
			float backwardx = sin(steer*PI / 180) * revspeed;
			float backwardy = -cos(steer*PI / 180) * revspeed;
			Ytri += backwardy;
			Xtri += backwardx;
		}

		
	}
	if (keys[VK_DOWN] == false)
	{	
		if (speed > 0.0) {
			
		}
		else {
			if (revspeed > 0.0)
			{
				revspeed += -0.00002f;
			}
			else { revspeed = 0.0; }
			float backwardx = sin(steer*PI / 180) * revspeed;
			float backwardy = -cos(steer*PI / 180) * revspeed;
			Ytri += backwardy;
			Xtri += backwardx;
		}
		
		//Ytri -= revspeed;
	}

}
/**************** END OPENGL FUNCTIONS *************************/

//WIN32 functions
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
void KillGLWindow();									// releases and destroys the window
bool CreateGLWindow(char* title, int width, int height); //creates the window
int WINAPI WinMain(	HINSTANCE, HINSTANCE, LPSTR, int);  // Win32 main function

//win32 global variabless
HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application


/******************* WIN32 FUNCTIONS ***************************/
int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	bool	done=false;								// Bool Variable To Exit Loop

	// Create Our OpenGL Window
	if (!CreateGLWindow("OpenGL Win32 Example",screenWidth,screenHeight))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=true;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			if(keys[VK_ESCAPE])
				done = true;

			processKeys();
			reshape(screenWidth, screenHeight);
			display();					// Draw The Scene
			SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (int)(msg.wParam);						// Exit The Program
}

//WIN32 Processes function - useful for responding to user inputs or other events.
LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}
		break;

		case WM_SIZE:								// Resize The OpenGL Window
		{
			reshape(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
		break;

		case WM_LBUTTONDOWN:
			{
	            mouse_x = LOWORD(lParam);          
				mouse_y = screenHeight - HIWORD(lParam);
				LeftPressed = true;
			}
		break;

		case WM_LBUTTONUP:
			{
	            LeftPressed = false;
			}
		break;

		case WM_MOUSEMOVE:
			{
	            mouse_x = LOWORD(lParam);          
				mouse_y = screenHeight  - HIWORD(lParam);
			}
		break;
		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = true;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}
		break;
		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = false;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}
		break;
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void KillGLWindow()								// Properly Kill The Window
{
	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*/
 
bool CreateGLWindow(char* title, int width, int height)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;											// Return FALSE
	}
	
	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
	dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		24,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		24,											// 24Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	reshape(width, height);					// Set Up Our Perspective GL Screen

	init();
	
	return true;									// Success
}


	/*double resultingPoint[2];
	double point[3] = {-4,-4,0};
	resultingPoint[0] = matrix[0]*point[0] + matrix[4]*point[1] + matrix[8]*point[2] + matrix[12];
	resultingPoint[1] = matrix[1]*point[0] + matrix[5]*point[1] + matrix[9]*point[2] + matrix[13];

	double matrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, matrix);*/
