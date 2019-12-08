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
bool isPaused = false;
int numberOfBalls = 5;
int ballsWithoutLifes = 0;
float pontuation = 0;

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

	void setPosition(MyVector position) {
		this->position = position;
	}

	MyVector getSpeed() {
		return speed;
	}

	void setSpeed(MyVector speed) {
		this->speed = speed;
	}

	float getRadius() {
		return radius;
	}

	void setRadius(float radius) {
		this->radius = radius;
	}

	int getLifes() {
		return lifes;
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

		if (position.x <= radius) {
			position.x = radius;
			speed.x = -speed.x;
			lifes--;
		}
		else {
			if (position.x >= windowWidth - radius) {
				position.x = windowWidth - radius;
				speed.x = -speed.x;
				lifes--;
			}
		}

		if (position.y <= radius) {
			position.y = radius;
			speed.y = -speed.y;
			lifes--;
		}
		else {
			if (position.y >= windowHeight - radius) {
				position.y = windowHeight - radius;
				speed.y = -speed.y;
				lifes--;
			}
		}

		if (lifes == 2) {
			color[0] = 100 / 100.0;//R
			color[1] = 100 / 100.0;//G
			color[2] = 0 / 100.0;//B
		}

		if (lifes == 1) {
			color[0] = 100 / 100.0;//R
			color[1] = 0 / 100.0;//G
			color[2] = 0 / 100.0;//B
		}

		if (lifes == 0) {
			ballsWithoutLifes += 1;
		}

	}

	void checkClickCoordinades(int xMouse, int yMouse) {

		yMouse = windowHeight - yMouse;

		float xResult;
		float yResult;

		if (xMouse > speed.x) {
			xResult = xMouse - position.x;
		}
		else {
			xResult = position.x - xMouse;
		}

		if (xResult <= radius) {

			if (yMouse > position.y) {
				yResult = yMouse - position.y;
			}
			else {
				yResult = position.y - yMouse;
			}

			if (yResult <= radius) {
				changeBallSpeed();
			}
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
		this->speed = MyVector(random(-5, 5), random(-5, 5));
	}

};

Ball* balls = new Ball[numberOfBalls];

void pauseGame() {
	isPaused = !isPaused;
	if (isPaused) {
		for (int i = 0; i < numberOfBalls; i++) {
			balls[i].stopBallMovement();
		}
	}
	else {
		for (int i = 0; i < numberOfBalls; i++) {
			balls[i].resumeBallMovement();
		}
	}
}

void mouse(int button, int state, int x, int y) {
	if (!isPaused && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		for (int i = 0; i < numberOfBalls; i++) {
			balls[i].checkClickCoordinades(x, y);
		}
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

	for (int i = 0; i < numberOfBalls; i++) {
		balls[i].drawBall();
	}

	glutSwapBuffers();

}

void refresh(int value) {

	pontuation += 0.01f;
	for (int i = 0; i < numberOfBalls; i++) {

		if (balls[i].getLifes() <= 0) {
			balls[i].setRadius(0);
		}
		else {
			balls[i].moveBall();

			if (numberOfBalls > 1) {
				for (int j = i + 1; j < numberOfBalls; j++) {

					int minusXPosition = balls[i].getPosition().x - balls[j].getPosition().x;
					int minusYPosition = balls[i].getPosition().y - balls[j].getPosition().y;
					int distance = int(sqrt(pow(minusXPosition, 2) + pow(minusYPosition, 2)));

					if (distance < (balls[i].getRadius() + balls[j].getRadius())) {

						MyVector tempSpeed = balls[j].getSpeed();

						balls[j].setSpeed(balls[i].getSpeed());
						balls[i].setSpeed(tempSpeed);

					}

				}
			}
		}

	}

	if (ballsWithoutLifes == numberOfBalls) {
		printf("Game Over! You have made: (%f) points :)\n", (float) pontuation);
		exit(0);
	}

	glutPostRedisplay();
	glutTimerFunc(refreshMillis, refresh, 0);
}

void init() {

	for (int i = 0; i < numberOfBalls; i++) {
		Ball b = Ball();
		balls[i] = b;
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