#include <windows.h>
#include <stdio.h>
#include <windows.h>
#include <stdio.h>
#include <GL/glut.h>
#include <Math.h>
constexpr auto PI = 3.14159265f;

// Variaveis Globais
int refreshMillis = 30;
int windowWidth = 640;
int windowHeight = 480;
int windowPosX = 50;
int windowPosY = 50;

GLfloat ballRadius = 0.1f;
GLfloat ballX = 0.0f;
GLfloat ballY = 0.0f;
GLfloat ballXMax, ballXMin, ballYMax, ballYMin;
GLfloat xSpeed = 0.025f;
GLfloat	ySpeed = 0.025f;

// Area de projecao
GLdouble clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop;

bool fullScreenMode = false;
bool paused = false;
GLfloat xSpeedSaved, ySpeedSaved;


class MyVector {
public:
	float x;
	float y;

	MyVector() {
		x = 0;
		y = 0;
	}

	MyVector(float _x, float _y) {
		x = _x;
		y = _y;
	}

	void sum(MyVector v) {
		x += v.x;
		y += v.y;
	}
};

class Ball {
private:
	MyVector position;
	MyVector speed;
	float radius;
	float color[3];
	int lifes;

	int random(int from, int to) {
		return rand() % (to - from + 1) + from;
	}

public:
	Ball() {
		position = MyVector(200, 200);
		speed = MyVector(random(-5, 5), random(-5, 5));
		radius = 15;
		color[0] = 50 / 100.0;//R
		color[1] = 50 / 100.0;//G
		color[2] = 50 / 100.0;//B
		lifes = 3;
	}

	Ball(int _x, int _y) {
		position = MyVector(_x, _y);
		speed = MyVector(random(-5, 5), random(-5, 5));
		radius = 15;
		color[0] = 50 / 100.0;//R
		color[1] = 50 / 100.0;//G
		color[2] = 50 / 100.0;//B
		lifes = 3;
	}

	void drawBall() {

		float increment = 0.1;
		glColor3fv(color);
		glBegin(GL_POLYGON);
		for (float angle = 0; angle < 2 * PI; angle += increment) {
			float x = position.x + radius * cos(angle);
			float y = position.y + radius * sin(angle);
			glVertex2f(x, y);
		}
		glEnd();
	}

	void checkPosition() { // Completar

		if (position.y <= radius) {
			position.y = radius;
		}

		if (position.y >= windowHeight - radius) {
			position.y = windowHeight - radius;
		}

		if (position.x >= windowWidth - radius) {
			position.x = windowWidth - radius;
		}

		if (position.x <= radius) {
			position.x = radius;
		}

	}

	void moveBall() {
		position.x += speed.x;
		position.y += speed.y;
		checkPosition();
	}
};

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	Ball b[2];
	b[0] = Ball(200, 200);
	b[0].drawBall();
	b[1] = Ball(300, 300);
	b[1].drawBall();

	glutSwapBuffers();

}

void init() {

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(windowPosX, windowPosY);
	glutCreateWindow("Bouncing Balls Game");
	glClearColor(0, 0, 0, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluOrtho2D(0, windowWidth, 0, windowHeight);

}

// Timer para renderizar imagem
void timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(refreshMillis, timer, 0);
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	init();
	glutDisplayFunc(display);
	glutTimerFunc(0, timer, 0);
	glutMainLoop();
	return 0;
}