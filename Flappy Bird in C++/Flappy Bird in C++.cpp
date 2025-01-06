#include "raylib.h"
#include <vector>
#include <cstdlib>
#include <ctime>

// Constants
const int screenWidth = 800;
const int screenHeight = 600;
const int birdRadius = 20;
const float gravity = 600.0f;
const float flapStrength = -300.0f;
const int pipeWidth = 80;
const int pipeGap = 200;
const float pipeSpeed = 200.0f;

struct Pipe {
    float x;
    float height;
    bool scored;
};

int main() {
    // Initialization
    InitWindow(screenWidth, screenHeight, "Flappy Bird");
    SetTargetFPS(60);

    // Variables
    Vector2 bird = { screenWidth / 4.0f, screenHeight / 2.0f };
    float birdVelocity = 0.0f;
    std::vector<Pipe> pipes;
    float spawnTimer = 0.0f;
    int score = 0;
    bool gameOver = false;

    // Seed the random number generator
    std::srand(std::time(nullptr));

    // Main Game Loop
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        // Restart the game if game over
        if (gameOver && IsKeyPressed(KEY_R)) {
            bird = { screenWidth / 4.0f, screenHeight / 2.0f };
            birdVelocity = 0.0f;
            pipes.clear();
            score = 0;
            gameOver = false;
        }

        // Update
        if (!gameOver) {
            // Bird movement
            if (IsKeyPressed(KEY_SPACE)) {
                birdVelocity = flapStrength;
            }
            birdVelocity += gravity * deltaTime;
            bird.y += birdVelocity * deltaTime;

            // Spawn pipes
            spawnTimer += deltaTime;
            if (spawnTimer > 2.0f) {
                float pipeHeight = 100 + std::rand() % (screenHeight - pipeGap - 200);
                pipes.push_back({ (float)screenWidth, (float)pipeHeight, false });
                spawnTimer = 0.0f;
            }

            // Update pipes
            for (auto& pipe : pipes) {
                pipe.x -= pipeSpeed * deltaTime;

                // Check for scoring
                if (!pipe.scored && pipe.x + pipeWidth < bird.x) {
                    score++;
                    pipe.scored = true;
                }
            }

            // Remove pipes that have gone off-screen
            if (!pipes.empty() && pipes.front().x + pipeWidth < 0) {
                pipes.erase(pipes.begin());
            }

            // Collision detection
            for (const auto& pipe : pipes) {
                if (bird.x + birdRadius > pipe.x && bird.x - birdRadius < pipe.x + pipeWidth) {
                    if (bird.y - birdRadius < pipe.height || bird.y + birdRadius > pipe.height + pipeGap) {
                        gameOver = true;
                    }
                }
            }

            // Check if bird hits the ground or ceiling
            if (bird.y + birdRadius > screenHeight || bird.y - birdRadius < 0) {
                gameOver = true;
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(SKYBLUE);

        // Draw the ground
        DrawRectangle(0, screenHeight - 50, screenWidth, 50, DARKGREEN);
        DrawRectangle(0, screenHeight - 60, screenWidth, 10, GREEN);

        // Draw the bird
        DrawCircleV(bird, birdRadius, YELLOW);
        DrawCircleLines(bird.x, bird.y, birdRadius, ORANGE);

        // Draw pipes
        for (const auto& pipe : pipes) {
            DrawRectangle(pipe.x, 0, pipeWidth, pipe.height, DARKGRAY);
            DrawRectangle(pipe.x, pipe.height + pipeGap, pipeWidth, screenHeight - pipe.height - pipeGap, DARKGRAY);
            DrawRectangleLines(pipe.x, 0, pipeWidth, pipe.height, BLACK);
            DrawRectangleLines(pipe.x, pipe.height + pipeGap, pipeWidth, screenHeight - pipe.height - pipeGap, BLACK);
        }

        // Draw score
        DrawText(TextFormat("Score: %d", score), 10, 10, 20, WHITE);

        // Game Over screen
        if (gameOver) {
            DrawText("GAME OVER", screenWidth / 2 - 100, screenHeight / 2 - 30, 40, RED);
            DrawText("Press R to Restart", screenWidth / 2 - 120, screenHeight / 2 + 20, 20, WHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
