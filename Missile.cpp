#include <stdlib.h>
#include <time.h>
#include <iostream>
using namespace std;

class Missile{
		public:
			double location[2];
			double speed[2];
			double health;
			bool destroyed;
			Missile();
			void draw();
			
		private:
			int MAX_SPEED;
			int windowHeight;//temp
			int windowWidth;//temp
			int buffer;
};

Missile::Missile(){
	destroyed = false;//hackey way of doing it, but I have a paper to write :(
	buffer = 20;
	MAX_SPEED = 2;
	windowHeight = 800;
	windowWidth = 1000;
	location[0] = rand()%(windowWidth-buffer) + buffer/2;
	location[1] = windowHeight+10;
	speed[0] = 0;
	speed[1] = -(rand()%MAX_SPEED)-1;
}

void Missile::draw(){
	if(destroyed)
		return;
	glPushMatrix();
	glTranslatef(location[0],location[1],0);
	glColor3f(.5,.5,.5);
	glBegin(GL_QUADS);
		glVertex2f(-4,12);
		glVertex2f(4,12);
		glVertex2f(4,-12);
		glVertex2f(-4,-12);
	glEnd();
	
	glBegin(GL_TRIANGLES);
		glVertex2f(4,-12);
		glVertex2f(0,-20);
		glVertex2f(-4,-12);
	glEnd();
	glPopMatrix();
}