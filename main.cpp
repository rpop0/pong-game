#include <GL/freeglut.h>
#include <iostream>
#include<random>
#include<chrono>


std::default_random_engine eng;
unsigned long int t = std::chrono::high_resolution_clock::now().time_since_epoch().count();
std::mt19937 mt(t);
std::uniform_real_distribution<GLfloat> ballAngleDist(0, 0.01);

struct Ball{
    GLfloat ballSpeed = 0.01;
    GLfloat ballAngleY = 0.01;
    GLfloat ballY = 0, ballX = 0;
};

int leftScore = 0;
int rightScore = 0;

struct Player{
    GLfloat left{};
    GLfloat right{};
    GLfloat bottom{};
    GLfloat top{};
};


Ball gameBall;
Player leftPlayer;
Player rightPlayer;

void keyboardListener(unsigned char key, int x, int y){
    switch(key){
        case 'w':
            if(leftPlayer.top  <= 0.95){
                leftPlayer.top += 0.2;
                leftPlayer.bottom += 0.2;
            }
            break;
        case 's':
            if(leftPlayer.bottom >= -0.95){
                leftPlayer.top -= 0.2;
                leftPlayer.bottom -= 0.2;
            }
            break;
        default:
            break;
    }

}

void arrowKeyListener(int key, int x, int y){
    switch(key){
        case GLUT_KEY_UP:
            if(rightPlayer.top <= 0.95){
                rightPlayer.top += 0.2;
                rightPlayer.bottom += 0.2;
            }
            break;
        case GLUT_KEY_DOWN:
            if(rightPlayer.bottom >= -0.95){
                rightPlayer.bottom -= 0.2;
                rightPlayer.top -= 0.2;
            }
            break;
        default:
            break;
    }
}

void initPlayers(){
    rightPlayer.left = 0.95;
    rightPlayer.right = 0.98;
    rightPlayer.top = 0.2;
    rightPlayer.bottom = -0.2;

    leftPlayer.left = -0.98;
    leftPlayer.right = -0.95;
    leftPlayer.top = 0.2;
    leftPlayer.bottom = -0.2;
}

void drawRightPaddle(){
    // RIGHT PADDLE
    glBegin(GL_QUADS);
    glColor3f(0, 0, 1);
    glVertex2f(rightPlayer.left, rightPlayer.top); // Top left
    glVertex2f(rightPlayer.right, rightPlayer.top); // Top right
    glVertex2f(rightPlayer.right, rightPlayer.bottom); // Bottom right
    glVertex2f(rightPlayer.left, rightPlayer.bottom); // Bottom left
    glEnd();
}

void drawLeftPaddle(){
    // LEFT PADDLE
    glBegin(GL_QUADS);
    glColor3f(1, 0, 0);
    glVertex2f(leftPlayer.left, leftPlayer.top);
    glVertex2f(leftPlayer.right, leftPlayer.top);
    glVertex2f(leftPlayer.right, leftPlayer.bottom);
    glVertex2f(leftPlayer.left, leftPlayer.bottom);
    glEnd();
}

void drawChar(float x, float y, void *font, const char *string)
{
    glColor3f(1, 1, 1);
    glRasterPos2f(x, y);

    glutBitmapString(font, reinterpret_cast<const unsigned char *>(string));
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT);

    drawChar(0.2f, 0.8f, GLUT_BITMAP_TIMES_ROMAN_24, std::to_string(rightScore).c_str());
    drawChar(-0.2f, 0.8f, GLUT_BITMAP_TIMES_ROMAN_24, std::to_string(leftScore).c_str());

    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    glVertex2f(0, 1);
    glVertex2f(0, -1);
    glEnd();

    drawRightPaddle();

    drawLeftPaddle();


    glBegin(GL_QUADS);
    glColor3f(1, 1, 0);
    glVertex2f(-0.03f + gameBall.ballX, 0.03f + gameBall.ballY);
    glVertex2f(0.03f + gameBall.ballX, 0.03f + gameBall.ballY);
    glVertex2f(0.03f + gameBall.ballX, -0.03f + gameBall.ballY);
    glVertex2f(-0.03f + gameBall.ballX, -0.03f + gameBall.ballY);
    glEnd();

    glutSwapBuffers();

}



void leftRightCheck(){
    if(gameBall.ballX >=1){
        leftScore++;
        gameBall.ballX = 0;
        gameBall.ballY = 0;
        gameBall.ballAngleY = 0.01f;
    } else if (gameBall.ballX <= -1){
        rightScore++;
        gameBall.ballX = 0;
        gameBall.ballY = 0;
        gameBall.ballAngleY = 0.01f;
    }
}

void timer(int iUnused){
    gameBall.ballX += gameBall.ballSpeed;
    gameBall.ballY += 0.01f - gameBall.ballAngleY;
    leftRightCheck();
    glutPostRedisplay();


    if(gameBall.ballX >= rightPlayer.left - 0.05 and (gameBall.ballY <= rightPlayer.top and gameBall.ballY >= rightPlayer.bottom)){
        gameBall.ballSpeed = -gameBall.ballSpeed;
        gameBall.ballAngleY = - ballAngleDist(mt);
    }

    if(gameBall.ballX <= leftPlayer.left + 0.07 and (gameBall.ballY <= leftPlayer.top and gameBall.ballY >= leftPlayer.bottom)){
        gameBall.ballSpeed = -gameBall.ballSpeed;
        gameBall.ballAngleY = - ballAngleDist(mt);
    }

    if(gameBall.ballY >= 1){
        gameBall.ballAngleY = 0.02f - gameBall.ballAngleY;
    }

    if(gameBall.ballY <= -1){
        gameBall.ballAngleY = 0.02f - gameBall.ballAngleY;
    }

    glutTimerFunc(18, timer, 0);
}


int main( int argc, char** argv ) {  // Initialize GLUT and


    glutInit(&argc, argv);
    initPlayers();
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(750,500);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Pong");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(arrowKeyListener);
    timer(0);
    glutMainLoop();
    return 0;

}
