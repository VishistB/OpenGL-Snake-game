#include <GL/glut.h>
#include <cstdlib>
#include <iostream>
#include <vector> 
#include <ctime>


#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720
#define CELL_SIZE      20

enum Direction { UP, DOWN, LEFT, RIGHT };
Direction snakeDirection = RIGHT;

//snake segments
struct SnakeSegment {
    int x;
    int y;
};

//snake body parts
std::vector<SnakeSegment> snakeSegments;

//food coords
int foodX, foodY;

void init() {

    // Clear existing segmentz
    snakeSegments.clear(); 
    // Add initial seg
    snakeSegments.push_back({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 }); 


    srand(time(nullptr));
    // Generate random food position
    foodX = rand() % (SCREEN_WIDTH / CELL_SIZE) * CELL_SIZE;
    foodY = rand() % (SCREEN_HEIGHT / CELL_SIZE) * CELL_SIZE;
}

void update(int value) {
    // Move the snake based on its direction
    // Get the current head of the snake
    SnakeSegment head = snakeSegments[0]; 
    switch (snakeDirection) {
    case UP:
        head.y -= CELL_SIZE;
        break;
    case DOWN:
        head.y += CELL_SIZE;
        break;
    case LEFT:
        head.x -= CELL_SIZE;
        break;
    case RIGHT:
        head.x += CELL_SIZE;
        break;
    }

    // Snake food consumption check
    if (head.x == foodX && head.y == foodY) {
        foodX = rand() % (SCREEN_WIDTH / CELL_SIZE) * CELL_SIZE;
        foodY = rand() % (SCREEN_HEIGHT / CELL_SIZE) * CELL_SIZE;

        // Increase the length of the snake
        snakeSegments.push_back({ head.x, head.y });
    }

    // Update the positions of the rest of the segments
    for (size_t i = snakeSegments.size() - 1; i > 0; --i) {
        // Move each segment to the position of the segment in front of it
        snakeSegments[i] = snakeSegments[i - 1]; 
    }
    // Update the position of the head
    snakeSegments[0] = head; 

    // Call update function again after a delay
    glutPostRedisplay();
    glutTimerFunc(100, update, 0);
}



void display() {
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw snake segments
    //RGB order for ref
    glColor3f(0.0f, 1.0f, 0.0f);
    for (const auto& segment : snakeSegments) {
        glBegin(GL_QUADS);
        glVertex2i(segment.x, segment.y);
        glVertex2i(segment.x + CELL_SIZE, segment.y);
        glVertex2i(segment.x + CELL_SIZE, segment.y + CELL_SIZE);
        glVertex2i(segment.x, segment.y + CELL_SIZE);
        glEnd();
    }

    // Draw food
    //red fud
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2i(foodX, foodY);
    glVertex2i(foodX + CELL_SIZE, foodY);
    glVertex2i(foodX + CELL_SIZE, foodY + CELL_SIZE);
    glVertex2i(foodX, foodY + CELL_SIZE);
    glEnd();

    glutSwapBuffers();
}



void reshape(int w, int h) {
    glViewport(0, 0, w, h);

    // Set up the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, h, 0, -1, 1);

    // Switch back to modelview matrix
    glMatrixMode(GL_MODELVIEW);
}

void specialKeyboard(int key, int x, int y) {
    // Handle special keys for snake direction
    //std::cout << key << "\n";
    switch (key) {
    case GLUT_KEY_UP:
        if (snakeDirection != DOWN)
            snakeDirection = UP;
        //std::cout << "UP" << "\n";
        break;
    case GLUT_KEY_DOWN:
        if (snakeDirection != UP)
            snakeDirection = DOWN;
        //std::cout << "DOWN" << "\n";
        break;
    case GLUT_KEY_LEFT:
        if (snakeDirection != RIGHT)
            snakeDirection = LEFT;
        //std::cout << "LEFT" << "\n";
        break;
    case GLUT_KEY_RIGHT:
        if (snakeDirection != LEFT)
            snakeDirection = RIGHT;
        //std::cout << "RIGHT" << "\n";
        break;
    }
}

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow("Snake Game");

    // callback functn
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeyboard); // Register special key input function

    // Set bg color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black

    // Init game
    init();

    // Call update function to move snake
    glutTimerFunc(100, update, 0);

    // Enter GLUT event processing loop
    glutMainLoop();

    return 0;
}
