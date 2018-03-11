#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <math.h>

#define ROWS 40
#define COLUMNS 40

#define PI 3.1415926535897932384626433832795

int direction = 2;
int length = 0;
int snake[(ROWS*COLUMNS)+1][2];
int grid[ROWS][COLUMNS];
int food[2];
bool grow = false;
bool changedir = true;
bool gameover = false;

void squereGrid(int x, int y){
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_LINES);
        glVertex2d(x,y);    glVertex2d(x+1,y);
        glVertex2d(x+1,y);  glVertex2d(x+1,y+1);
        glVertex2d(x+1,y+1);glVertex2d(x,y+1);
        glVertex2d(x,y+1);  glVertex2d(x,y);
    glEnd();
}
void drawGrid(){
    for(int x=0;x<ROWS;x++){
        for(int y=0;y<COLUMNS;y++){
            squereGrid(x,y);
        }
    }
}
void printtext(int x, int y, std::string String){
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, COLUMNS, 0, ROWS, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glPushAttrib(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    glRasterPos2i(x,y);
    for (int i=0; i<String.size(); i++)
    {
        glScaled(0.2, 0.2, 1);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, String[i]);
    }
    glPopAttrib();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void gameOver(){
    if(gameover){
        glColor4d(0, 0, 0, 0.8);
        glBegin(GL_QUADS);
            glVertex2d(0, 0);
            glVertex2d(0, 40);
            glVertex2d(40, 40);
            glVertex2d(40, 0);
        glEnd();
        glColor3d(1, 1, 1);
        printtext(15,20, "GAME OVER");

    }

}

void printGrid(){
    for(int a = 0; a < ROWS; a++){
        for(int b = 0; b < COLUMNS; b++){
            printf("%i", grid[a][b]);

        }
        printf("|%i\n", a);
    }
    printf("\n");
}

void snakeBody(int x, int y){
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_QUADS);
        glVertex2d(x+0.1,y+0.1);
        glVertex2d(x+0.9, y+0.1);
        glVertex2d(x+0.9, y+0.9);
        glVertex2d(x+0.1, y+0.9);
    glEnd();
}

void moveSnake(int dir){
    if(grow){
        for(int pos = length - 1; pos > 0; pos--){
            snake[pos+1][0] = snake[pos][0];
            snake[pos+1][1] = snake[pos][1];
        }
        snake[1][0] = snake[0][0];
        snake[1][1] = snake[0][1];
        length++;
        grow = false;
    }else{
        grid[snake[length-1][0]][snake[length-1][1]] = 0;
        for(int pos = length-1; pos > 0; pos--){
            snake[pos][0] = snake[pos-1][0];
            snake[pos][1] = snake[pos-1][1];
        }
    }
    switch(dir){
            case 1:
                snake[0][0] += 1;
                break;
            case 2:
                snake[0][1] += 1;
                break;
            case 3:
                snake[0][0] -= 1;
                break;
            case 4:
                snake[0][1] -= 1;
    }
    if(grid[(snake[0][0])][(snake[0][1])] == 1){
        gameover = true;
    }else{
        grid[snake[0][0]][snake[0][1]] = 1;
    }
    changedir = true;
    //printGrid();
    if((snake[0][0] < 0 || snake[0][0] > (COLUMNS-1) || snake[0][1] < 0 || snake[0][1] > (ROWS-1)) || gameover){
        gameover = true;
    }else{
        glutTimerFunc(150, moveSnake, direction);
    }
}

void drawSnake(){
    for(int pos = 0; pos < length; pos++){
        snakeBody(snake[pos][0], snake[pos][1]);
    }
}

void drawFood(){
    glColor3f(1.0, 1.0, 0.0);
    double radius = 0.4;
    glBegin(GL_POLYGON);
        for(double i = 0; i < 2 * PI; i += PI / 4)
 					glVertex3f(0.48+food[0]+cos(i) * radius, 0.48+food[1]+ sin(i) * radius, 0.0);
    glEnd();
    /*glBegin(GL_TRIANGLES);
        glVertex2d(food[0], food[1]);
        glVertex2d(food[0]+0.9, food[1]);
        glVertex2d(food[0]+0.45, food[1]+0.9);
    glEnd();*/
}

void spawnFood(){
    int x;
    int y;
    do{
        x = rand() % 40;
        y = rand() % 40;
        //printf("%i %i    %i", x, y,grid[y][x]);
    }while(grid[x][y] == 1);
    food[0] = x;
    food[1] = y;
}
void eatFood(){
    grow = true;
    spawnFood();
}
void checkFood(){
    if(snake[0][0] == food[0] && snake[0][1] == food[1]){
        eatFood();
    }
}

void initSnake(){
    direction = 2;
    snake[0][0] = 20;
    snake[0][1] = 21;
    snake[1][0] = 20;
    snake[1][1] = 20;
    snake[2][0] = 20;
    snake[2][1] = 19;
    grid[snake[0][0]][snake[0][1]] = 1;
    grid[snake[1][0]][snake[1][1]] = 1;
    grid[snake[2][0]][snake[2][1]] = 1;
    length = 3;
    spawnFood();
    for(int a = 0; a < ROWS; a++){
        for(int b = 0; b < COLUMNS; b++){
            grid[a][b] = 0;
        }
    }
}

static void resize(int width, int height){
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, COLUMNS, 0.0, ROWS, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

static void display(void){
    glClear(GL_COLOR_BUFFER_BIT);
    drawGrid();
    drawSnake();
    checkFood();
    drawFood();
    gameOver();
    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y){
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;
    }
    if(changedir){
        changedir = false;
        switch(key){
            case 'w':
            if(direction != 4){
                direction = 2;
            }
            break;

        case 'a':
            if(direction != 1){
                direction = 3;
            }
            break;
        case 's':
            if(direction != 2){
                direction = 4;
            }
            break;
        case 'd':
            if(direction != 3){
                direction = 1;
            }
            break;
        }
    }

    glutPostRedisplay();
}

static void idle(void){
    glutPostRedisplay();
}


int main(int argc, char *argv[]){
    glutInit(&argc, argv);
    glutInitWindowSize(600,600);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

    glutCreateWindow("Snake");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glutTimerFunc(250, moveSnake, direction);

    glClearColor(0,0,0,1);
    glCullFace(GL_BACK);

    srand (time(NULL));
    initSnake();

    glutMainLoop();

    return EXIT_SUCCESS;
}
