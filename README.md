# Falling Rocks

A simple terminal-based game where you control a player who dodges or collects falling rocks with numerical values. The objective is to avoid collision with rocks having a greater value than the player and to collect rocks with smaller values to grow in size. If the player reaches a certain value, they win the game!

## Gameplay

- **Controls**:
  - Press **'a'** to move left.
  - Press **'d'** to move right.

- **Objective**:
  - Avoid rocks with values equal or greater than the player's value.
  - Collect rocks with values less than the player's value to increase your size.
  - Reach the maximum value to win the game!

- **Game Over**:
  - The game ends if the player collides with a rock of a equal or higher value.

## How It Works

The game continuously updates and displays a grid representing the game field:
- Rocks with values (from 1 to 15) fall from the top of the grid at random positions.
- The player starts at a central position on the bottom row, with an initial size of 3.
- Rocks fall one row per game loop iteration. The game clears the screen each time it updates, giving the effect of falling rocks.
- The player grows by collecting rocks with smaller values until they reach the maximum size of 15.

## Requirements

This game is written in C and requires:
- **GCC Compiler** to compile and run the code.
- **POSIX-compatible terminal** (Linux or macOS) for non-blocking keyboard input (`kbhit()` and `getch()` functions).

## Compilation and Execution

To compile and run the game:

```c
gcc falling_rocks.c -o falling_rocks
./falling_rocks
```

## Code Structure

- initializeGame() - Displays the welcome message and instructions.
- drawGame() - Renders the game grid with player and rocks.
- updateBalls() - Handles the movement and removal of rocks, and spawns new ones.
- movePlayer() - Updates player position based on user input.
- checkCollision() - Checks for collisions between the player and rocks, determining game over or player growth.
- addBall() - Adds a new rock to the game.
- kbhit() and getch() - Handle non-blocking keyboard input.

## License

This project has a free license.
