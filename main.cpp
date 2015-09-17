/*
 *  CSCI 441, Computer Graphics, Fall 2015
 *
 *  Project: lab01
 *  File: main.cpp
 *
 *	Author: Jeffrey Paone - Fall 2015
 *
 *  Description:
 *      Contains the code for a simple 2D OpenGL / GLUT example.  
 *  Animation, keyboard, and mouse interaction will be added to 
 *  this very basic example.
 *
 */

#ifdef __APPLE__			// if compiling on Mac OS
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else					// else compiling on Linux OS
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <time.h>
#include "Tank.cpp"
#include "Missile.cpp"

// global variables to keep track of window width and height.
// set to initial values for convenience, but we need variables
// for later on in case the window gets resized.
int windowWidth = 1000, windowHeight = 800;

// global variable to keep track of the window id OH GOOD LORD THIS HORRIBLE CODING
int windowId;
Tank myTank;
int currentBullet = 0;
int MAX_BULLETS = 10;
//gross static declaration cuz easy
vector<Missile> missiles;
double bulletLocations[10][2] = {{-5,-5},{-5,-5},{-5,-5},{-5,-5},{-5,-5},{-5,-5},{-5,-5},{-5,-5},{-5,-5},{-5,-5}};
double bulletSpeeds[10][2] = {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};
double bulletSpeedMAG = 7;
double GRAVITY_CONSTANT = .2;
double SPAWN_CHANCE = .2;
double BULLET_SIZE = 5;
bool GAMEOVER = false;
int numDestroyed = 0;
double timeOfLastMissileSpawn = 0;
int interval = 1000;
double sunAngle = 0;
double rate=.1;
int shotsFired=0;
//
//  void changeSize(int w, int h)
//
//      We will register this function as GLUT's reshape callback.
//   When the window is resized, the OS will tell GLUT and GLUT will tell
//   us by calling this function - GLUT will give us the new window width
//   and height as 'w' and 'h,' respectively - so save those to our global vars.
//
void changeSize( int w, int h ) {
    //save the new window width / height that GLUT informs us about
    windowWidth = w;
    windowHeight = h;

    // update the projection matrix based on the window size
    // the GL_PROJECTION matrix governs properties of the view;
    // i.e. what gets seen - use an Orthographic projection that ranges
    // from [0, windowWidth] in X and [0, windowHeight] in Y. (0,0) is the lower left.
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0, windowWidth, 0, windowHeight );
    
    // update the viewport as well - tell OpenGL we want to render the whole window
    glViewport( 0, 0, windowWidth, windowHeight );

    // just as good practice, switch back to GL_MODELVIEW mode; that's the matrix
    // that we want to be changing with glTranslatef(), glScalef(), etc., so
    // just in case we forget to explicitly set it later...
    glMatrixMode( GL_MODELVIEW );
}
//draws part of a circle, degrees determines how many degrees of a circle it will cover, scale just enlarges the circle
void drawPartOfCircle(int numTriangles,int degrees, double scale){
	glPushMatrix();
	glScalef(scale,scale,1);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0,0);
	float rad = (2.0*3.14159/(360/degrees))/((float) numTriangles);//angle to increase by each time
	for(int i = 0;i<=numTriangles;i++){
		float x = cos(rad*((float)i));
		float y = sin(rad*((float)i));
		glVertex2f(x,y);
	}
	glEnd();
	glPopMatrix();
}
//fires a bullet from the tank in the direction it's turret is facing
void fireBullet(){
	currentBullet++;
	if(currentBullet>=MAX_BULLETS)
		currentBullet=0;
	bulletLocations[currentBullet][0] = myTank.location[0];
	bulletLocations[currentBullet][1] = myTank.location[1]+myTank.barrelHeight;
	bulletSpeeds[currentBullet][0] = bulletSpeedMAG*cos(myTank.barrelAngle/57.2957795);
	bulletSpeeds[currentBullet][1] = bulletSpeedMAG*sin(myTank.barrelAngle/57.2957795);
	shotsFired++;
}
//draws/animates the background
void drawBackground(){
	glPushMatrix();
	glColor3f(.5*sin(sunAngle/57.2957795)+.05,sin(sunAngle/57.2957795)+.1,sin(sunAngle/57.2957795)+.1);
	glBegin(GL_QUADS);
		glVertex2f(0,windowHeight);
		glVertex2f(windowWidth,windowHeight);
		glVertex2f(windowWidth,0);
		glVertex2f(0,0);
	glEnd();
	glTranslatef(windowWidth/2,0,0);
	glRotatef(sunAngle,0,0,1);
	glTranslatef(windowWidth/2,0,0);
	glColor3f(1,1,.1);
	drawPartOfCircle(50,360,100);
	sunAngle+=rate;
	glPopMatrix();
}
//redraws bullets and missiles at their locations
void drawObjects(){
	glColor3f(1,.3,0);
	for(int i = 0; i<MAX_BULLETS;i++){
		glPushMatrix();
		glTranslatef(bulletLocations[i][0],bulletLocations[i][1],0);
		drawPartOfCircle(10,360,BULLET_SIZE);
		glPopMatrix();
	}
	for(unsigned int i = 0; i < missiles.size();i++){
		glPushMatrix();
			missiles.at(i).draw();
		glPopMatrix();
	}
}
//called each turn, checks if it's time for more missiles then spawns them if needed and slightly decreases the interval
void spawnMissiles(){
	if(clock() - timeOfLastMissileSpawn >= interval){
		Missile temp;
		missiles.push_back(temp);
		timeOfLastMissileSpawn = clock();
		interval = interval-10;
	}
}
//normal keyboard handler
void keyboardHandler(unsigned char key, int mooseX, int mooseY){
	if(key==27)exit(0);
}

void keyboardSpecialHandler(int key, int mooseX, int mooseY){
	/*if(key==GLUT_KEY_UP)//just for testing and fun
		myTank.barrelAngle++;
	else if(key==GLUT_KEY_DOWN)
		myTank.barrelAngle--;*/
	if(key==GLUT_KEY_LEFT)//just for testing and fun
		myTank.move(-1);
	else if(key==GLUT_KEY_RIGHT)
		myTank.move(1);
	glutPostRedisplay();
}
//called if a missile touches the bottom of the screen
void endGame(){
	GAMEOVER=true;
	cout << "GAMEOVER\nMISSILES DESTROYED: " << numDestroyed << "\nSHOTS FIRED: " << shotsFired;
	exit(0);
}
//helper function to return distance between the given missile and given bullet
double distanceBetween(int indexB, int indexM){
	return pow(pow(bulletLocations[indexB][0]-missiles[indexM].location[0],2)+pow(bulletLocations[indexB][1]-missiles[indexM].location[1],2),.5);
}
//checks if any bullets have collided with any missiles
void checkCollisions(){
	for(int i = 0;i<MAX_BULLETS;i++){
		for(unsigned int j = 0; j<missiles.size();j++){
			if(!missiles[j].destroyed)
				if(distanceBetween(i,j) < (BULLET_SIZE*2.5)){
					missiles[j].destroyed=true;
					numDestroyed++;
				}
		}
	}
}
//moves all of the bullets and missiles
void updateObjects(){
	for(int i = 0;i<MAX_BULLETS;i++){
		//bulletSpeeds[i][1]-=GRAVITY_CONSTANT;
		bulletLocations[i][0]+=bulletSpeeds[i][0];
		bulletLocations[i][1]+=bulletSpeeds[i][1];
	}
	for(unsigned int i = 0;i<missiles.size();i++){
		if(missiles.at(i).location[0]<-1 || missiles.at(i).location[0] > windowWidth)
			missiles.at(i).speed[0]=-missiles.at(i).speed[0];
		if(missiles.at(i).location[1]<-1 && !missiles.at(i).destroyed)
			endGame();
		missiles.at(i).location[0]+=missiles.at(i).speed[0];
		missiles.at(i).location[1]+=missiles.at(i).speed[1];
	}
}
//set timer function to ensure objects move at a steady rate
void myTimer(int value){
	updateObjects();
	checkCollisions();
	glutPostRedisplay();
	glutTimerFunc(13, myTimer, 1);
}
//active mouse function
void myMoose(int button, int state, int mooseX, int mooseY){
	if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
		fireBullet();
	//else if(button==GLUT_LEFT_BUTTON && state==GLUT_UP)
		
}
//passive mouse function, tracks the turret
void myMotion(int mooseX,int mooseY){//57.2957795
	double direction[2] = { mooseX-myTank.location[0],(mooseY-myTank.location[1]-windowHeight+myTank.barrelHeight)};
	myTank.barrelAngle = 57.2957795*acos(direction[0]/pow(direction[0]*direction[0]+direction[1]*direction[1],.5));
}

void renderScene() {
    // clear whatever was drawn to the screen last time - 
    // set the clear color to black and then signal to clear the RGB buffer.
    glClearColor( 0, 0, 0, 1 );
    glClear( GL_COLOR_BUFFER_BIT );

    // switch to GL_MODELVIEW, for when we start using glTranslatef(), glScalef(), etc..
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //draw background
	drawBackground();
	//draw tank
	glTranslatef(myTank.location[0],myTank.location[1],0);
	myTank.draw();
	glTranslatef(-myTank.location[0],-myTank.location[1],0);
	//drawObjects
	drawObjects();
	spawnMissiles();
	glutSwapBuffers();
    //glFlush();
}


//
//  int main( int argc, char* argv[] )
//
int main( int argc, char* argv[] ) {
    // initialize GLUT... always need this call.
    glutInit( &argc, argv );

    // request a window with just an RGB frame buffer, nothing fancy...
    // place it at (50,50) on the screen and set its size to our constants from earlier
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
    glutInitWindowPosition( 50, 50 );
    glutInitWindowSize( windowWidth, windowHeight );
    
    // this actually creates our window. 
    windowId = glutCreateWindow( "TAAAAAANK" );

    // register our reshape and display callbacks with GLUT - these functions are above
    glutDisplayFunc( renderScene );
    glutReshapeFunc( changeSize );
	
	/********** ADD CALLBACKS ***************/
	glutKeyboardFunc(keyboardHandler);
	glutSpecialFunc(keyboardSpecialHandler);
	glutTimerFunc(50, myTimer, 1);
	//glutTimerFunc(1000, spawnMissiles, 1);
	glutMouseFunc(myMoose);
	glutPassiveMotionFunc(myMotion);
	
	// begin GLUT loop
    glutMainLoop();

    return 0;
}
