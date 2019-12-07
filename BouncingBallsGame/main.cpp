#include <windows.h>
#include <stdio.h>
#include <windows.h>
#include <stdio.h>
#include <GL/glut.h>
#include <Math.h>
#include <vector>
constexpr auto PI = 3.14159265f;

// Variaveis Globais
int refreshMillis = 30;
int windowWidth = 640;
int windowHeight = 480;
int windowPosX = 50;
int windowPosY = 50;
bool isPaused = false;
int numberOfBalls = 8;

class MyVector {
public:
	float x;
	float y;

	MyVector() {
		this->x = 0;
		this->y = 0;
	}

	MyVector(float x, float y) {
		this->x = x;
		this->y = y;
	}

	void sum(MyVector v) {
		this->x += v.x;
		this->y += v.y;
	}
};

class Ball {
private:
	MyVector position;
	MyVector speed;
	MyVector speedSaved;
	float radius;
	float color[3];
	int lifes;

	int random(int from, int to) {
		return rand() % (to - from + 1) + from;
	}

public:
	Ball() {
		this->position = MyVector(random(1, 640), random(1, 480));
		changeBallSpeed();
		this->radius = 15;
		this->color[0] = 0 / 100.0;//R
		this->color[1] = 100 / 100.0;//G
		this->color[2] = 0 / 100.0;//B
		this->lifes = 3;
	}

	MyVector getPosition() {
		return position;
	}

	MyVector getSpeed() {
		return speed;
	}

	float getRadius() {
		return radius;
	}

	int getLifes() {
		return lifes;
	}

	void setSpeed(MyVector speed) {
		this->speed = speed;
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

	void checkWallCollisions() {

		if (position.y <= radius) {
			position.y = radius;
			speed.y = -speed.y;
			lifes--;
		}

		if (position.x <= radius) {
			position.x = radius;
			speed.x = -speed.x;
			lifes--;
		}

		if (position.y >= windowHeight - radius) {
			position.y = windowHeight - radius;
			speed.y = -speed.y;
			lifes--;
		}

		if (position.x >= windowWidth - radius) {
			position.x = windowWidth - radius;
			speed.x = -speed.x;
			lifes--;
		}

		if (lifes == 2) {
			color[0] = 0 / 100.0;//R
			color[1] = 0 / 100.0;//G
			color[2] = 100 / 100.0;//B
		}

		if (lifes == 0) {
			color[0] = 100 / 100.0;//R
			color[1] = 0 / 100.0;//G
			color[2] = 0 / 100.0;//B
		}
	}

	void moveBall() {
		position.sum(speed);
		checkWallCollisions();
	}

	void stopBallMovement() {
		speedSaved = speed;
		speed = MyVector(0, 0);
	}

	void resumeBallMovement() {
		speed = speedSaved;
	}

	void changeBallSpeed() {
		speed = MyVector(random(-5, 5), random(-5, 5));
	}

};

std::vector<Ball> balls;

void pauseGame() {
	isPaused = !isPaused;
	if (isPaused) {
		for (size_t i = 0; i < balls.size(); i++) {
			balls[i].stopBallMovement();
		}
	}
	else {
		for (size_t i = 0; i < balls.size(); i++) {
			balls[i].resumeBallMovement();
		}
	}
}

void verifyClickCoords(int xMouse, int yMouse) {
	
	yMouse = windowHeight - yMouse;

	for (size_t i = 0; i < balls.size(); i++) {

		float xResult;
		float yResult;

		if (xMouse > balls[i].getPosition().x) {
			xResult = xMouse - balls[i].getPosition().x;
		}
		else {
			xResult = balls[i].getPosition().x - xMouse;
		}

		if (xResult <= (balls[i].getRadius())) {

			if (yMouse > balls[i].getPosition().y) {
				yResult = yMouse - balls[i].getPosition().y;
			}
			else {
				yResult = balls[i].getPosition().y - yMouse;
			}

			if (yResult <= (balls[i].getRadius())) {
				balls[i].changeBallSpeed();
			}
		}
	}
}

void mouse(int button, int state, int x, int y) {
	if (!isPaused && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		verifyClickCoords(x, y);
		printf("Mouse position: (%f,%f)\n", (float)x, (float)y);
	}
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'P': // P: Pausar e retomar o jogo
	case 'p':
		pauseGame();
		break;
	case 'Q':
	case 'q':
		exit(0);
		break;
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	for (size_t i = 0; i < balls.size(); i++) {
		balls[i].drawBall();
	}

	glutSwapBuffers();

}

void refresh(int value) {

	for (size_t i = 0; i < balls.size(); i++) {
		balls[i].moveBall();

		if (numberOfBalls > 1) {
			for (int collide = i + 1; collide < numberOfBalls; collide++) {

				int minusXPosition = balls[i].getPosition().x - balls[collide].getPosition().x;
				int minusYPosition = balls[i].getPosition().y - balls[collide].getPosition().y;
				int distance = int(sqrt(pow(minusXPosition, 2) + pow(minusYPosition, 2)));

				if (distance < (balls[i].getRadius() + balls[collide].getRadius())) {

					MyVector tempSpeed = balls[collide].getSpeed();

					balls[collide].setSpeed(balls[i].getSpeed());
					balls[i].setSpeed(tempSpeed);

				}

			}
		}
	}

	glutPostRedisplay();
	glutTimerFunc(refreshMillis, refresh, 0);
}

void init() {

	for (int i = 0; i < numberOfBalls; i++) {
		Ball b = Ball();
		balls.push_back(b);
	}

}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	init();
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(windowPosX, windowPosY);
	glutCreateWindow("Bouncing Balls Game");
	glClearColor(0, 0, 0, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluOrtho2D(0, windowWidth, 0, windowHeight);
	glutDisplayFunc(display);
	glutTimerFunc(0, refresh, 0);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}