#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h> // Include for string manipulation

// Game field dimensions
#define WIDTH 40     // Width of the game field
#define HEIGHT 20    // Height of the game field
#define INITIAL_SIZE 3
#define MAX_BALL_VALUE 15

// Ball structure to store properties of falling balls
typedef struct {
    int x, y;   // Coordinates of the ball on the grid
    int value;  // Numerical value of the ball (ranging from 1 to 15)
} Ball;

// Player's properties
int playerX = WIDTH / 2;        // Starting X position of the player (centered)
int playerValue = INITIAL_SIZE; // Player starts with an initial value of 3

// Function prototypes
void initializeGame();
void drawGame(Ball balls[], int ballCount);
void updateBalls(Ball balls[], int *ballCount);
void movePlayer();
int checkCollision(Ball balls[], int *ballCount);
void addBall(Ball balls[], int *ballCount);
int kbhit();
char getch();

int main() {
    srand(time(NULL)); // Initialize random seed for ball spawning

    Ball balls[100];    // Array to hold falling balls
    int ballCount = 0;  // Current number of active balls

    initializeGame(); // Display welcome message and instructions

    // Main game loop
    while (1) {
        system("clear"); // Clear screen (use "cls" if on Windows)

        drawGame(balls, ballCount); // Render the game grid with player and falling balls

        updateBalls(balls, &ballCount); // Move falling balls and handle off-screen removals

        // Check for player collisions with falling balls
        if (checkCollision(balls, &ballCount) == 1) {
            printf("Game Over! You collided with a bigger number.\n");
            break;
        } else {
            if (checkCollision(balls, &ballCount) == 2) {
                printf("Congratulations! You win!\n");
                break;
            }
        }

        // Handle player movement if a key was pressed
        if (kbhit()) {
            movePlayer();
        }

        usleep(100000);  // Pause briefly to control game speed (in microseconds)
    }

    return 0;
}

// Display welcome message and instructions
void initializeGame() {
    printf("Welcome to the Number Ball Game!\n");
    printf("Use 'a' to move left, 'd' to move right. Eat smaller numbers to grow!\n");
    printf("Press any key to start...\n");
    getchar();
}

// Draw the game field, player, and balls
void drawGame(Ball balls[], int ballCount) {
    char field[HEIGHT][WIDTH * 3]; // Use a wider grid for multi-character representation
    char display[5];               // Buffer for displaying "O" followed by the value

    // Initialize game field with empty spaces
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH * 3; j++) {
            field[i][j] = ' ';
        }
    }

    // Render falling balls based on their numerical values
    for (int i = 0; i < ballCount; i++) {
        if (balls[i].y >= 0 && balls[i].y < HEIGHT && balls[i].x >= 0 && balls[i].x < WIDTH) {
            // Display the ball as "O" followed by its value (e.g., "O3", "O12")
            snprintf(display, sizeof(display), "O%d", balls[i].value);

            // Ensure the representation fits in the game grid
            for (int k = 0; k < strlen(display); k++) {
                if ((balls[i].x * 3 + k) < WIDTH * 3) {
                    field[balls[i].y][balls[i].x * 3 + k] = display[k];
                }
            }
        }
    }

    // Render the player's number at the bottom
    snprintf(display, sizeof(display), "O%d", playerValue);
    if (playerX < WIDTH) {
        for (int k = 0; k < strlen(display); k++) {
            if ((playerX * 3 + k) < WIDTH * 3) {
                field[HEIGHT - 1][playerX * 3 + k] = display[k];
            }
        }
    }

    // Print the updated game grid
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH * 3; j++) {
            printf("%c", field[i][j]);
        }
        printf("\n");
    }

    // Display the player's current value
    printf("Player's Value: O%d\n", playerValue);
}

// Update the position of falling balls
void updateBalls(Ball balls[], int *ballCount) {
    // Move each ball down by one step
    for (int i = 0; i < *ballCount; i++) {
        balls[i].y += 1;
    }

    // Remove balls that move out of the bottom of the screen
    for (int i = 0; i < *ballCount; i++) {
        if (balls[i].y >= HEIGHT) {
            // Shift all remaining balls up in the array
            for (int j = i; j < *ballCount - 1; j++) {
                balls[j] = balls[j + 1];
            }
            (*ballCount)--; // Decrease the count of active balls
        }
    }

    // Randomly spawn a new falling ball
    if (rand() % 10 < 2) {  // 20% chance to spawn a new ball each loop
        addBall(balls, ballCount);
    }
}

// Handle player's left and right movement based on keyboard input
void movePlayer() {
    char key = getch();
    if (key == 'a' && playerX > 0) {
        playerX--;
    } else if (key == 'd' && playerX < WIDTH - 1) {
        playerX++;
    }
}

// Check for collisions between the player and falling balls
int checkCollision(Ball balls[], int *ballCount) {
    for (int i = 0; i < *ballCount; i++) {
        // Check if the ball is within the player's position
        if (balls[i].y == HEIGHT - 1 && balls[i].x == playerX) {
            if (balls[i].value < playerValue) {
                // Consume the ball and increase player's value
                playerValue ++;
                if (playerValue > MAX_BALL_VALUE) {
                    return 2;
                }
                // Remove the consumed ball from the array
                for (int j = i; j < *ballCount - 1; j++) {
                    balls[j] = balls[j + 1];
                }
                (*ballCount)--; // Decrease the ball count
                return 0; // No collision, continue game
            } else {
                // Collision with a larger number - Game Over
                return 1;
            }
        }
    }
    return 0; // No collision detected
}

// Spawn a new ball at the top of the screen
void addBall(Ball balls[], int *ballCount) {
    if (*ballCount < 100) {
        balls[*ballCount].x = rand() % WIDTH;       // Random horizontal position
        balls[*ballCount].y = 0;                     // Start at the top
        balls[*ballCount].value = 1 + rand() % 15;  // Value between 1 and 15
        (*ballCount)++;                             // Increase the active ball count
    }
}

// Non-blocking keyboard input detection
int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt); // Get current terminal attributes
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Apply new settings
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK); // Set non-blocking mode

    ch = getchar(); // Check if a key was pressed

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore terminal settings
    fcntl(STDIN_FILENO, F_SETFL, oldf);      // Restore blocking mode

    if (ch != EOF) {
        ungetc(ch, stdin); // Put character back in input stream
        return 1;          // Key was pressed
    }

    return 0; // No key pressed
}

// Get a single character without requiring Enter key
char getch() {
    struct termios oldt, newt;
    char ch;

    tcgetattr(STDIN_FILENO, &oldt); // Get current terminal attributes
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Apply new settings
    ch = getchar(); // Read a single character
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore terminal settings

    return ch; // Return the character read
}
