using namespace std;

class Tank{
	public:
	double location[2];
	double barrelAngle;
	double barrelHeight;
	double reloadSpeedMilli;
	
	Tank();
	void draw();
	void move(int direction);
	
	private:
	void drawTurret();
	void drawBody();
	void drawWheels();
	void drawPartOfCircle(int numTriangles,int degrees, double scale);
	int speed;
};

Tank::Tank(){
	location[0] = 500;
	location[1] = 0;
	barrelHeight=15;
	reloadSpeedMilli = 1000;
	barrelAngle = 0;
	speed = 7;
}

void Tank::draw(){
	drawWheels();
	drawBody();
	drawTurret();
}

void Tank::drawTurret(){
	glPushMatrix();
	glTranslatef(5,barrelHeight,0);//move turret up
	glPushMatrix();
	glColor3f(.2,.7,.2);
	//cannon
	glRotatef(barrelAngle,0,0,1);
	glBegin(GL_QUADS);
		glVertex2f(0,7.5);
		glVertex2f(30,7.5);
		glVertex2f(30,0);
		glVertex2f(0,0);
	glEnd();
	glPopMatrix();
	
	//turret
	glPushMatrix();
	glScalef(12,12,1);
	drawPartOfCircle(10,180,1);
	glScalef(1.0/12.0,1.0/12.0,1);
	
	glPopMatrix();
	glPopMatrix();
}

void Tank::drawBody(){
	glPushMatrix();
	glColor3f(.2,.7,.2);
	glTranslatef(0,5,0);
	glBegin(GL_QUADS);
		glVertex2f(-20,12.5);
		glVertex2f(20,12.5);
		glVertex2f(25,0);
		glVertex2f(-25,0);
		
	glEnd();
	glPopMatrix();
}

void Tank::drawWheels(){
	glPushMatrix();
	glColor3f(0,0,0);
	glTranslatef(-20,5,0);
	drawPartOfCircle(10,360,4);
	glTranslatef(8,0,0);
	drawPartOfCircle(10,360,4);
	glTranslatef(8,0,0);
	drawPartOfCircle(10,360,4);
	glTranslatef(8,0,0);
	drawPartOfCircle(10,360,4);
	glTranslatef(8,0,0);
	drawPartOfCircle(10,360,4);
	glTranslatef(8,0,0);
	drawPartOfCircle(10,360,4);
	
	glScalef(10,10,0);
	glPopMatrix();
}

void Tank::move(int direction){//moves the tank in only the X direction, the direction input just changes the sign of motion
	if(location[0]+(direction*speed) > 1000 || location[0]+(direction*speed) < 0)
		return;
	location[0]+=direction*speed;
}

void Tank::drawPartOfCircle(int numTriangles,int degrees, double scale){//see main file
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
