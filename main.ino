#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define PIN A0
#define WIDTH 6
#define HEIGHT 10

// Setup the matrix
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(WIDTH, HEIGHT, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

// Tetris Colors (Standard: Cyan, Blue, Orange, Yellow, Green, Purple, Red)
const uint16_t tetrisColors[] = {
  matrix.Color(0, 255, 255),   // I - Cyan
  matrix.Color(0, 0, 255),     // J - Blue
  matrix.Color(255, 165, 0),   // L - Orange
  matrix.Color(255, 255, 0),   // O - Yellow
  matrix.Color(0, 255, 0),     // S - Green
  matrix.Color(128, 0, 128),   // T - Purple
  matrix.Color(255, 0, 0)      // Z - Red
};

// Shape Definitions (4x4 grid with rotation)
const uint16_t tetrominoes[7][4] = {
  // I (Cyan)
  {0x0F00, 0x2222, 0x00F0, 0x4444},
  // J (Blue)
  {0x8E00, 0x6440, 0x0E20, 0x44C0},
  // L (Orange)
  {0x2E00, 0x4460, 0x0E80, 0xC440},
  // O (Yellow) - Rotations are all the same
  {0x6600, 0x6600, 0x6600, 0x6600},
  // S (Green)
  {0x6C00, 0x4620, 0x06C0, 0x8C40},
  // T (Purple)
  {0x4E00, 0x4640, 0x0E40, 0x4C40},
  // Z (Red)
  {0xC600, 0x2640, 0x0C60, 0x4C80}
};

// Game State Variables
int board[WIDTH][HEIGHT];
int currentPieceType = -1;
int currentRotation = 0;
int px = 0;
int py = -4;

void setup() {
  matrix.begin();
  matrix.setBrightness(10);
  randomSeed(analogRead(A1)); // Generate random seed from unconnected pin
  resetBoard();
}

void loop() {
  matrix.fillScreen(0); 

  updateTetris();

  drawBoard();
  drawCurrentPiece();

  matrix.show();
  delay(150);
}

void updateTetris() {
  // 1. If no piece exists, spawn one
  if (currentPieceType == -1) {
    spawnPiece();
    
    // Immediate Game Over check: if the new piece collides immediately, reset board
    if (checkCollision(px, py, currentRotation)) {
      delay(1000);
      resetBoard();
    }
    return;
  }

  if (!checkCollision(px, py + 1, currentRotation)) {
    py++; // Move down
  } else {
    // 3. Hit something! Lock it in place.
    lockPiece();
    clearLines();
    currentPieceType = -1;
  }
}

bool checkCollision(int newX, int newY, int rot) {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (getPixel(currentPieceType, rot, x, y)) {
        int boardX = newX + x;
        int boardY = newY + y;

        if (boardX < 0 || boardX >= WIDTH || boardY >= HEIGHT) return true;
        if (boardY >= 0 && board[boardX][boardY] != 0) return true;
      }
    }
  }
  return false;
}

void lockPiece() {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (getPixel(currentPieceType, currentRotation, x, y)) {
        int boardX = px + x;
        int boardY = py + y;
        
        // Only lock if it's actually on the screen
        if (boardX >= 0 && boardX < WIDTH && boardY >= 0 && boardY < HEIGHT) {
          board[boardX][boardY] = currentPieceType + 1; // Store color index + 1
        }
      }
    }
  }
}

void spawnPiece() {
  currentPieceType = random(0, 7);
  
  // Best move variables
  int bestScore = -30000;
  int bestX = 0;
  int bestRot = 0;
  int r = 0;

  // Rotations loop
  for (int r = 0; r < 4; r++) {
    
    // X position (from -2 to WIDTH)
    for (int x = -2; x < WIDTH; x++) {
      int y = -2; // Start high
      
      // If we can't even spawn here, skip it
      if (checkCollision(x, y, r)) continue;

      // Drop it until it hits something
      while (!checkCollision(x, y + 1, r)) {
        y++;
      }

      int score = evaluateBoard(x, y, r);

      if (score > bestScore) {
        bestScore = score;
        bestX = x;
        bestRot = r;
      }
    }
  }

  // Apply the best move
  px = bestX;
  py = -2; // Start at top
  currentRotation = bestRot;
}

int evaluateBoard(int pieceX, int pieceY, int rot) {
  // Create a temporary board simulation
  int tempBoard[WIDTH][HEIGHT];
  for(int x=0; x<WIDTH; x++) {
    for(int y=0; y<HEIGHT; y++) {
      tempBoard[x][y] = board[x][y];
    }
  }

  // Add the piece to the temp board
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (getPixel(currentPieceType, rot, x, y)) {
        int bx = pieceX + x;
        int by = pieceY + y;
        if (bx >= 0 && bx < WIDTH && by >= 0 && by < HEIGHT) {
          tempBoard[bx][by] = 1; // Just mark as filled
        }
      }
    }
  }

  // --- HEURISTICS ---
  int aggregateHeight = 0;
  int completeLines = 0;
  int holes = 0;
  int bumpiness = 0;

  // Calculate Heights & Holes
  int columnHeights[WIDTH];
  for(int x=0; x<WIDTH; x++) {
    bool colStarted = false;
    columnHeights[x] = 0;
    for(int y=0; y<HEIGHT; y++) {
      if (tempBoard[x][y] != 0) {
        if (!colStarted) {
          columnHeights[x] = HEIGHT - y; // Height of this column
          colStarted = true;
        }
      } else if (colStarted) {
        holes++; // Empty block BELOW a filled block
      }
    }
    aggregateHeight += columnHeights[x];
  }

  // Calculate Complete Lines
  for (int y = 0; y < HEIGHT; y++) {
    bool full = true;
    for (int x = 0; x < WIDTH; x++) if (tempBoard[x][y] == 0) full = false;
    if (full) completeLines++;
  }

  // Calculate Bumpiness (Jaggedness of surface)
  for (int x = 0; x < WIDTH - 1; x++) {
    bumpiness += abs(columnHeights[x] - columnHeights[x+1]);
  }

  // Final Score Formula (Tunable weights)
  // We want to MAXIMIZE lines, MINIMIZE height, holes, bumpiness
  return (completeLines * 50) - (aggregateHeight * 3) - (holes * 20) - (bumpiness * 2);
}

bool getPixel(int type, int rot, int x, int y) {
  uint16_t shapeData = tetrominoes[type][rot % 4];
  int bitIndex = 15 - (y * 4 + x);
  return (shapeData >> bitIndex) & 1;
}

// --- DRAWING FUNCTIONS ---

void drawBoard() {
  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT; y++) {
      if (board[x][y] > 0) {
        matrix.drawPixel(x, y, tetrisColors[board[x][y] - 1]);
      }
    }
  }
}

void drawCurrentPiece() {
  if (currentPieceType == -1) return;
  
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (getPixel(currentPieceType, currentRotation, x, y)) {
        // Draw only if within screen bounds
        int drawX = px + x;
        int drawY = py + y;
        if (drawX >= 0 && drawX < WIDTH && drawY >= 0 && drawY < HEIGHT) {
          matrix.drawPixel(drawX, drawY, tetrisColors[currentPieceType]);
        }
      }
    }
  }
}

void clearLines() {
  for (int y = HEIGHT - 1; y >= 0; y--) {
    bool full = true;
    for (int x = 0; x < WIDTH; x++) {
      if (board[x][y] == 0) full = false;
    }
    if (full) {
      // Move everything down
      for (int ny = y; ny > 0; ny--) {
        for (int nx = 0; nx < WIDTH; nx++) {
          board[nx][ny] = board[nx][ny - 1];
        }
      }
      for (int nx = 0; nx < WIDTH; nx++) board[nx][0] = 0;
      
      // Flash effect
      matrix.drawFastHLine(0, y, WIDTH, matrix.Color(255, 255, 255));
      matrix.show();
      delay(50);
      
      y++; 
    }
  }
}

void resetBoard() {
  memset(board, 0, sizeof(board));
  currentPieceType = -1;
}
