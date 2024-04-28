#include <GL/glut.h>
#include <cstdlib>
#include <iostream>
#include <vector> 
#include <ctime>
#include <string>

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720
#define CELL_SIZE      20
#define NUM_OBSTACLES  10 // Adjust the number of obstacles as needed

enum Direction { UP, DOWN, LEFT, RIGHT };
Direction snakeDirection = RIGHT;

// Snake segments
struct SnakeSegment {
    int x;
    int y;
};

// Vector to store snake body parts
std::vector<SnakeSegment> snakeSegments;

// Food coordinates
int foodX, foodY;

// Vector to store obstacle coordinates
std::vector<std::pair<int, int>> obstacles;

// Game over flag
bool gameOver = false;

bool checkFoodCollision() {
    for (const auto& obstacle : obstacles) {
        if (foodX == obstacle.first && foodY == obstacle.second)
            return true;
    }
    return false;
}

void init() {
    // Clear existing segments and obstacles
    snakeSegments.clear();
    obstacles.clear();

    // Add initial segment
    snakeSegments.push_back({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });

    srand(time(nullptr));
    // Generate random food position
    do {
        foodX = rand() % (SCREEN_WIDTH / CELL_SIZE) * CELL_SIZE;
        foodY = rand() % (SCREEN_HEIGHT / CELL_SIZE) * CELL_SIZE;
    } while (checkFoodCollision());

    // Generate random obstacles
    for (int i = 0; i < NUM_OBSTACLES; ++i) {
        int obstacleX = rand() % (SCREEN_WIDTH / CELL_SIZE) * CELL_SIZE;
        int obstacleY = rand() % (SCREEN_HEIGHT / CELL_SIZE) * CELL_SIZE;

        // Randomly choose whether to add a shape or a single obstacle
        if (rand() % 2 == 0) {
            // Add a random connected shape
            int shapeType = rand() % 3; // Three possible shapes
            switch (shapeType) {
            case 0: // Shape 1
                obstacles.push_back({ obstacleX, obstacleY });
                obstacles.push_back({ obstacleX + CELL_SIZE, obstacleY });
                obstacles.push_back({ obstacleX, obstacleY + CELL_SIZE });
                obstacles.push_back({ obstacleX - CELL_SIZE, obstacleY + CELL_SIZE });
                obstacles.push_back({ obstacleX - CELL_SIZE * 2, obstacleY + CELL_SIZE });
                break;
            case 1: // Shape 2
                obstacles.push_back({ obstacleX, obstacleY });
                obstacles.push_back({ obstacleX + CELL_SIZE, obstacleY });
                obstacles.push_back({ obstacleX + CELL_SIZE * 2, obstacleY });
                obstacles.push_back({ obstacleX + CELL_SIZE * 2, obstacleY + CELL_SIZE });
                obstacles.push_back({ obstacleX + CELL_SIZE, obstacleY + CELL_SIZE });
                obstacles.push_back({ obstacleX, obstacleY + CELL_SIZE });
                break;
            case 2: // Shape 3
                obstacles.push_back({ obstacleX, obstacleY });
                obstacles.push_back({ obstacleX + CELL_SIZE, obstacleY });
                obstacles.push_back({ obstacleX + CELL_SIZE * 2, obstacleY });
                obstacles.push_back({ obstacleX + CELL_SIZE * 3, obstacleY });
                obstacles.push_back({ obstacleX + CELL_SIZE * 3, obstacleY + CELL_SIZE });
                obstacles.push_back({ obstacleX + CELL_SIZE * 2, obstacleY + CELL_SIZE });
                obstacles.push_back({ obstacleX + CELL_SIZE, obstacleY + CELL_SIZE });
                obstacles.push_back({ obstacleX, obstacleY + CELL_SIZE });
                break;
            case 3: // Shape 4
                obstacles.push_back({ obstacleX, obstacleY });
                obstacles.push_back({ obstacleX + CELL_SIZE, obstacleY });
                obstacles.push_back({ obstacleX + CELL_SIZE * 2, obstacleY });
                obstacles.push_back({ obstacleX + CELL_SIZE * 2, obstacleY + CELL_SIZE });
                obstacles.push_back({ obstacleX + CELL_SIZE, obstacleY + CELL_SIZE });
                obstacles.push_back({ obstacleX + CELL_SIZE, obstacleY + CELL_SIZE * 2 });
                break;
            case 4: // Shape 5
                obstacles.push_back({ obstacleX, obstacleY });
                obstacles.push_back({ obstacleX + CELL_SIZE, obstacleY });
                obstacles.push_back({ obstacleX + CELL_SIZE, obstacleY + CELL_SIZE });
                obstacles.push_back({ obstacleX + CELL_SIZE * 2, obstacleY + CELL_SIZE });
                obstacles.push_back({ obstacleX + CELL_SIZE * 2, obstacleY + CELL_SIZE * 2 });
                obstacles.push_back({ obstacleX + CELL_SIZE, obstacleY + CELL_SIZE * 2 });
                obstacles.push_back({ obstacleX, obstacleY + CELL_SIZE * 2 });
                break;

            }
        }
        else {
            // Add a single obstacle
            obstacles.push_back({ obstacleX, obstacleY });
        }
    }
}

void update(int value) {
    if (!gameOver) {
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

        // Check for collision with boundary
        if (head.x < 0 || head.x >= SCREEN_WIDTH ||
            head.y < 0 || head.y >= SCREEN_HEIGHT) {
            gameOver = true;
        }

        // Check for collision with obstacles
        for (const auto& obstacle : obstacles) {
            if (head.x == obstacle.first && head.y == obstacle.second) {
                gameOver = true;
                break;
            }
        }

        // Check for collision with snake body
        for (size_t i = 1; i < snakeSegments.size(); ++i) {
            if (head.x == snakeSegments[i].x && head.y == snakeSegments[i].y) {
                gameOver = true;
                break;
            }
        }

        // Snake food consumption check
        if (head.x == foodX && head.y == foodY) {
            do {
                foodX = rand() % (SCREEN_WIDTH / CELL_SIZE) * CELL_SIZE;
                foodY = rand() % (SCREEN_HEIGHT / CELL_SIZE) * CELL_SIZE;
            } while (checkFoodCollision());

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
}

void display() {
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    if (gameOver) {
        // Draw game over overlay
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2i(0, 0);
        glVertex2i(SCREEN_WIDTH, 0);
        glVertex2i(SCREEN_WIDTH, SCREEN_HEIGHT);
        glVertex2i(0, SCREEN_HEIGHT);
        glEnd();

        // Display game over text
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2i(SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2);
        std::string gameOverText = "Game Over!";
        for (char c : gameOverText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }

        // Display instructions
        glRasterPos2i(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 20);
        std::string instructionText = "Press ESC to exit, ENTER to restart";
        for (char c : instructionText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
        }
    }
    else {
        // Draw obstacles
        glColor3f(0.5f, 0.5f, 0.5f); // Gray color for obstacles
        for (const auto& obstacle : obstacles) {
            glBegin(GL_QUADS);
            glVertex2i(obstacle.first, obstacle.second);
            glVertex2i(obstacle.first + CELL_SIZE, obstacle.second);
            glVertex2i(obstacle.first + CELL_SIZE, obstacle.second + CELL_SIZE);
            glVertex2i(obstacle.first, obstacle.second + CELL_SIZE);
            glEnd();
        }

        // Draw snake segments
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
        glColor3f(1.0f, 0.0f, 0.0f); // Red color for food
        glBegin(GL_TRIANGLE_FAN);
        //circle midpt
        glVertex2f(foodX + CELL_SIZE / 2.0f, foodY + CELL_SIZE / 2.0f);
        for (int i = 0; i <= 360; ++i) {
            float angle = i * 3.1415926535897932384626433832795 / 180;
            float x = foodX + CELL_SIZE / 2.0f + cos(angle) * CELL_SIZE / 2.0f;
            float y = foodY + CELL_SIZE / 2.0f + sin(angle) * CELL_SIZE / 2.0f;
            glVertex2f(x, y);
        }
        glEnd();

        // Display score
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2i(20, 20);
        std::string scoreText = "Score: " + std::to_string(snakeSegments.size() - 1);
        for (char c : scoreText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
        }
    }

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
    switch (key) {
    case GLUT_KEY_UP:
        if (snakeDirection != DOWN)
            snakeDirection = UP;
        break;
    case GLUT_KEY_DOWN:
        if (snakeDirection != UP)
            snakeDirection = DOWN;
        break;
    case GLUT_KEY_LEFT:
        if (snakeDirection != RIGHT)
            snakeDirection = LEFT;
        break;
    case GLUT_KEY_RIGHT:
        if (snakeDirection != LEFT)
            snakeDirection = RIGHT;
        break;
    }
}

void keyboard(unsigned char key, int x, int y) {
    // Handle keyboard input
    switch (key) {
    case 13: // ENTER key
        if (gameOver) {
            gameOver = false;
            init(); // Restart the game
            update(0); // Start updating again
        }
        break;
    case 27: // ESC key
        exit(0); // Quit the game
        break;
    }
}

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow("Snake Game");

    // Register callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeyboard); // Register special key input function
    glutKeyboardFunc(keyboard); // Register keyboard input function

    // Set background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black

    // Init game
    init();

    // Call update function to move snake
    glutTimerFunc(100, update, 0);

    // Enter GLUT event processing loop
    glutMainLoop();

    return 0;
}
