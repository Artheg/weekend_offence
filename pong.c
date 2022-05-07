#include <float.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

static const int screenWidth = 800;
static const int screenHeight = 600;

static const Vector2 padSize = {100, 10};
static const Vector2 padMargin = {0, 50};

static const float playerSpeed = 0.5;
static Vector2 playerPos = {0, 0};

static Vector2 AIPos = {0, 0};

static int ballRadius = 10;
static Vector2 ballSpeed = {0, 0};
static Vector2 ballPosition = {0, 0};

static bool isGameInProgress = false;

static void UpdateGame(void);

static void ProcessInput(void);
static void ProcessAI(void);
static void ProcessBall(void);
static void CheckBallOutOfBounds(void);

static void Redraw(void);
static void DrawPlayer(void);
static void DrawAI(void);
static void DrawBall(void);

int main() {
  // initialization
  InitWindow(screenWidth, screenHeight, "Weekend offence: Pong");
  playerPos.x = (screenWidth - padSize.x) / 2.0;
  playerPos.y = screenHeight - padSize.y - padMargin.y;

  AIPos.x = (screenWidth - padSize.x) / 2.0;
  AIPos.y = padMargin.y;

  ballPosition.x = (screenWidth - ballRadius) / 2.0;
  ballPosition.y = (screenHeight - ballRadius) / 2.0;
  //

  // GameLoop
  while (!WindowShouldClose()) {
    UpdateGame();
  }
  //

  return 0;
}

void TryStartGame() {
  if (!isGameInProgress) {
    ballSpeed.y = rand() % 2 == 0 ? 0.8 : -0.8;
  }
  isGameInProgress = true;
}

void ResetBall() {
  isGameInProgress = false;
  ballSpeed.x = 0;
  ballSpeed.y = 0;
  ballPosition.x = (screenWidth - ballRadius) / 2.0;
  ballPosition.y = (screenHeight - ballRadius) / 2.0;
}

void UpdateGame() {
  ProcessInput();
  ProcessAI();
  ProcessBall();
  Redraw();
}

void ProcessInput() {
  if (IsKeyDown(KEY_SPACE)) {
    TryStartGame();
  }

  if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_H) || IsKeyDown(KEY_J)) {
    playerPos.x -= playerSpeed;
    if (playerPos.x < 0.0) {
      playerPos.x = 0;
    }
  }

  if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_L) || IsKeyDown(KEY_K)) {
    playerPos.x += playerSpeed;
    if (playerPos.x + padSize.x > screenWidth) {
      playerPos.x = screenWidth - padSize.x;
    }
  }
}

void ProcessAI() { AIPos.x = playerPos.x; }

void ProcessBall() {
  ballPosition.x += ballSpeed.x;
  ballPosition.y += ballSpeed.y;

  const float playerHorBounds = playerPos.x + padSize.x;
  const float AIHorBounds = AIPos.x + padSize.x;
  const float playerVerBounds = playerPos.y;
  const float AIVerBounds = AIPos.y + padSize.y;

  const bool ballHitsPlayer = ((int)ballPosition.x >= (int)playerPos.x && (int)ballPosition.x <= (int)playerHorBounds) &&
                              ((int)ballPosition.y + ballRadius == (int)playerVerBounds);

  const bool ballHitsAI = ((int)ballPosition.x >= (int)AIPos.x && (int)ballPosition.x <= (int)AIHorBounds) &&
                          ((int)ballPosition.y - ballRadius == (int)AIVerBounds);

  if (ballHitsPlayer || ballHitsAI) {
    ballSpeed.y = ballSpeed.y * -1.0;
  }

  CheckBallOutOfBounds();
}

void Redraw() {
  BeginDrawing();
  ClearBackground(BLACK);
  DrawText("Pong", 100, 200, 42, WHITE);

  DrawPlayer();
  DrawAI();
  DrawBall();

  EndDrawing();
}

void CheckBallOutOfBounds() {
  if (ballPosition.x <= ballRadius || ballPosition.x > screenWidth - ballRadius) {
    ResetBall();
  }
  if (ballPosition.y <= 0 + ballRadius || ballPosition.y > screenHeight - ballRadius) {
    ResetBall();
  }
}

void DrawPlayer() {
  DrawRectangle(playerPos.x, playerPos.y, padSize.x, padSize.y, WHITE);
}

void DrawAI() {
  DrawRectangle(AIPos.x, AIPos.y, padSize.x, padSize.y, MAGENTA);
}

void DrawBall() { DrawCircle(ballPosition.x, ballPosition.y, ballRadius, RED); }
