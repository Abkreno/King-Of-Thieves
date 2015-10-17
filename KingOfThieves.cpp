#include <glut.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#define maxLevel 3   // change max level from 1 -> 1000
#define numOfGrounds 1000
#define PI 3.1415926535897932384626433832795
struct obsticle {
	int x0, x1, y0, y1;
}obsticles[numOfGrounds];

int max(int a, int b) {
	return a < b ? b : a;
}
int min(int a, int b) {
	return a > b ? b : a;
}
void drawCircule(int, int, int,float,float,float);
void drawRect(int ,int ,int ,int);
int WIDTH = 500, HEIGHT = 500;
float GX[numOfGrounds] = { 0  , 0,  100 ,0 , 170}; // the x coordinate of the ground
float GY[numOfGrounds] = { 0  , 200,  400 ,600 ,800}; // the y coordinate of the ground
float GW[numOfGrounds] = { 600, 320,  320 ,320,320}; // the width coordinate of the ground
float GS[numOfGrounds] = { 3, 10,  10 ,10,10}; // the sickness of the ground
float cameraY = 0,camDir = -1;
int time = 0, score = 0,doorX0 = 0,doorX1=50,doorY0=0,doorY1=80;
bool gameOver = false, col = false;
class Player {
public:
	float positionX, positionY, velocityX, velocityY, gravity, dir, speedUp;
	int currentGround;
	bool onGround;
	Player::Player() {
		positionX = 0; positionY = 0;
		velocityX = 0.05;
		speedUp = 0;
		velocityY = 0;
		gravity = -0.0002;
		dir = 1;
		currentGround = 0;
		onGround = true;
	}

	
	void draw() {
		glPushMatrix();
		glTranslatef(positionX, positionY, 0);
		glScaled(1, 1, 0);
		drawCircule(8, 8, 49,0.2,0.3,0.5); // head
		drawCircule(2, 9+3*dir, 51, 1.0, 1.0, 1.0); // eye
		drawRect(20, 15, 0, 20); //body
		drawRect(20, 3, 1, 0); //left leg
		drawRect(20, 3, 10, 0); // right leg
		glPopMatrix();
	}
	bool aboveGround(int ground) {
		if (positionX <= GW[ground] + GX[ground] && positionX >= GX[ground] && positionY >= GY[ground])return true;
		return false;
	}
	bool belowGround(int ground) {
		if (positionY+51 < GY[ground])
			return true;
		return false;
	}
	void updateGround() {
		
		for (int i =min(currentGround+5, numOfGrounds - 1); i >= 0; i--) {
			if (aboveGround(i)) {
				if (i > currentGround)
					camDir = -1;
				else if(i < currentGround)
					camDir = 1;
				currentGround = i;
				if (currentGround > score) {
					score = currentGround;
				}
				return;
			}
		}
	}
	void updateCelling() {
		for (int i = currentGround; i < min(currentGround + 6, numOfGrounds - 1); i++) {
			if (currentGround<i&&positionX <= GW[i] + GX[i] && positionX >= GX[i] && abs((int)(positionY - GY[i]))<=20) {
				velocityY = -0.05;
				speedUp = 0;
				return;
			}
		}
	}
	void checkDoor() {
		if (positionX >= doorX0&&positionY >= doorY0-10&&positionX <= doorX1&&positionY <= doorY1) {
			gameOver = true;
		}
	}
	void checkObsticles() {
		if (positionX >= obsticles[currentGround].x0&&positionY >= obsticles[currentGround].y0-10
			&&positionX <= obsticles[currentGround].x1&&positionY <= obsticles[currentGround].y1) {
			positionX = 0; positionY = 0;
			velocityX = 0.05;
			speedUp = 0;
			velocityY = 0;
			gravity = -0.0002;
			dir = 1;
			currentGround = 0;
			cameraY = 0;
			score = 0;
			onGround = true;
		}
	}
	void move() {
		velocityY += gravity;
		positionY += velocityY;
		positionX += velocityX*dir + speedUp*dir;
	 
		updateGround();
		updateCelling();
		checkObsticles();
		checkDoor();
		

		if (positionY < GY[currentGround] + GS[currentGround])
		{
			positionY = GY[currentGround] + GS[currentGround];
			velocityY = 0.0;
			speedUp = 0;
			onGround = true;
		}

		if (positionX < 0 || positionX+15 > WIDTH)
			dir *= -1;
	}

	void jump() {
		if (onGround)
		{
			velocityY = 0.2;
			onGround = false;
		}
		else {
			if (positionX + 40 >= WIDTH||positionX -40 <=0) {
				velocityY = 0.3;
				speedUp = 0.03;
			}
		}
	}

};

Player p;

void drawRect(int h, int w, int x, int y) {
	glPushMatrix();
	glTranslatef(x, y, 0);
	glBegin(GL_QUADS);
	glColor3f(0, 0, 0);
	glVertex2d(0, 0);
	glVertex2d(0, h);
	glVertex2d(w, h);
	glVertex2d(w, 0);
	glEnd();
	glPopMatrix();
}
void drawCircule(int r,int x, int y,float c1,float c2,float c3) {
	glPushMatrix();
	glTranslatef(x, y, 0);
	glColor3f(c1, c2, c3);

	glBegin(GL_POLYGON);
	float radius = r;
	for (double i = 0; i < 2.0 * PI; i += PI / 6.0)
		glVertex3f(cos(i) * radius, sin(i) * radius, 0.0);
	glEnd();
	glPopMatrix();
}
void drawDigit(char num,int x,int y) {
	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glScalef(0.2, 0.2, 0);
	glColor3f(0.2, 0.2, 0.0);
	glutStrokeCharacter(GLUT_STROKE_ROMAN, num);
	glPopMatrix();
}
void drawNumber(int num, int x,int y) {
	int t = 0;
	int T = num;
	while (T > 0)t++, T /= 10;
	T = num;
	int shift = 20 * (t);
	do {
		drawDigit(T % 10+'0', x+shift, y);
		shift -= 20;
		T /= 10;
	} while (T > 0);
}

void updateCamera() {
	if (camDir == -1&&cameraY > camDir*GY[p.currentGround]) {
		cameraY += camDir*0.08;
		if (cameraY < camDir*GY[p.currentGround]) {
			cameraY = camDir*GY[p.currentGround];
		}
	}
	else if (camDir == 1 && cameraY < camDir*GY[p.currentGround]) {
		cameraY += camDir*0.08;
		if (cameraY > camDir*GY[p.currentGround]) {
			cameraY = camDir*GY[p.currentGround];
		}
	}

}
void drawString(char * s,int x,int y) {
	int n = strlen(s);
	int shift = 0;
	for (int i = 0; i < n; i++) {
		drawDigit(s[i], x+shift, y);
		shift += 10;
	}
}
void drawDoor() {
	glPushMatrix();
	glBegin(GL_QUADS);
	glColor3f(0.2, 0.1, 0.6);
	glVertex2d(doorX0, doorY0);
	glVertex2d(doorX0, doorY1);
	glVertex2d(doorX1, doorY1);
	glVertex2d(doorX1, doorY0);
	glEnd();
	glPopMatrix();
}
void drawObsitcle(int i) {
	glPushMatrix();
	glBegin(GL_QUADS);
	glColor3f(0.2, 0.2, 0.3);
	glVertex2d(obsticles[i].x0, obsticles[i].y0);
	glVertex2d(obsticles[i].x0, obsticles[i].y1);
	glVertex2d(obsticles[i].x1, obsticles[i].y1);
	glVertex2d(obsticles[i].x1, obsticles[i].y0);
	glEnd();
	glPopMatrix();
}

void Display() {
	glClear(GL_COLOR_BUFFER_BIT);
	if (!gameOver) {
		

		glPushMatrix();

		updateCamera();

		glTranslated(0, cameraY, 0);

		p.draw();
		int g = p.positionY / 200.0;
		for (int i = max(0, g - 15); i < min(numOfGrounds, g + 15); i++) {
			drawRect(GS[i], GW[i], GX[i], GY[i]);
			drawObsitcle(i);
		}
		drawDoor();
		
		glPopMatrix();

		glPushMatrix();
		drawString("T i m  e : ", 5, 470);
		drawDigit(time / 60 + '0', 105, 470);
		drawDigit(':', 125, 470);
		drawNumber(time % 60, 116, 470);
		drawString("S c o r e : ", 330, 470);
		drawNumber(score, 440, 470);
		glPopMatrix();
	}
	else {
		drawString("Y o u    a  r e    o  u  t !", WIDTH / 2 - 130, HEIGHT / 2);
	}
	glFlush();
}
void Timer(int value) {
	time++;
	glutTimerFunc(1000, Timer, 0);
}
void Anim() {
	p.move();
	glutPostRedisplay();
}
void keyBoard(unsigned char key, int x, int y) {
	if (key == 32) {
		p.jump();
	}
	if (key == 115) {
		p.currentGround = 0;
	}
}
void main(int argc, char** argr) {
	glutInit(&argc, argr);

	glutInitWindowSize(HEIGHT, WIDTH);
	glutInitWindowPosition(150, 150);

	glutCreateWindow("King of Thieves");
	glutDisplayFunc(Display);
	glutIdleFunc(Anim);
	glutKeyboardFunc(keyBoard);
	glutTimerFunc(0, Timer, 0);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	gluOrtho2D(0.0, HEIGHT, 0.0, WIDTH);
	int y = 1000;
	for (int i = 5; i < numOfGrounds; i++) {
		GX[i] = GX[i % 4 + 1];
		GY[i] = y;
		GW[i] = 320;
		GS[i] = 10;
		y += 200;
	}
	for (int i = 1; i < maxLevel; i++) {
		obsticles[i].x0 = GX[i] + 130;
		obsticles[i].x1 = GX[i] + 130+20;
		obsticles[i].y0 = GY[i] + GS[i];
		obsticles[i].y1 = GY[i] + GS[i]+20;
	}
	doorX0 += GX[maxLevel] + 130;
	doorX1 += GX[maxLevel] + 130;
	doorY0 += GY[maxLevel] + GS[maxLevel];
	doorY1 += GY[maxLevel] + GS[maxLevel];
	glutMainLoop();
}
