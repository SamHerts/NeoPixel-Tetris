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

const uint8_t alphabet[26][10] = {
  {0x1E, 0x21, 0x21, 0x21, 0x21, 0x3F, 0x21, 0x21, 0x21, 0x21}, // A
  {0x3E, 0x21, 0x21, 0x21, 0x3E, 0x21, 0x21, 0x21, 0x21, 0x3E}, // B
  {0x1F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x1F}, // C
  {0x3C, 0x22, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x22, 0x3C}, // D
  {0x3F, 0x20, 0x20, 0x20, 0x3E, 0x20, 0x20, 0x20, 0x20, 0x3F}, // E
  {0x3F, 0x20, 0x20, 0x20, 0x3C, 0x20, 0x20, 0x20, 0x20, 0x20}, // F
  {0x1F, 0x20, 0x20, 0x20, 0x20, 0x27, 0x21, 0x21, 0x21, 0x1F}, // G
  {0x21, 0x21, 0x21, 0x21, 0x3F, 0x3F, 0x21, 0x21, 0x21, 0x21}, // H
  {0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E}, // I
  {0x0F, 0x02, 0x02, 0x02, 0x02, 0x02, 0x22, 0x22, 0x22, 0x1C}, // J
  {0x22, 0x24, 0x28, 0x30, 0x30, 0x30, 0x28, 0x24, 0x22, 0x21}, // K
  {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x3F}, // L
  {0x21, 0x33, 0x33, 0x2D, 0x2D, 0x21, 0x21, 0x21, 0x21, 0x21}, // M
  {0x21, 0x31, 0x31, 0x29, 0x29, 0x25, 0x25, 0x23, 0x23, 0x21}, // N
  {0x1E, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x1E}, // O
  {0x3E, 0x21, 0x21, 0x21, 0x21, 0x3E, 0x20, 0x20, 0x20, 0x20}, // P
  {0x1E, 0x21, 0x21, 0x21, 0x21, 0x21, 0x25, 0x23, 0x21, 0x1F}, // Q
  {0x3E, 0x21, 0x21, 0x21, 0x21, 0x3E, 0x28, 0x24, 0x22, 0x21}, // R
  {0x1F, 0x20, 0x20, 0x20, 0x1E, 0x01, 0x01, 0x01, 0x01, 0x3E}, // S
  {0x3F, 0x3F, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C}, // T
  {0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x1E}, // U
  {0x21, 0x21, 0x21, 0x21, 0x21, 0x12, 0x12, 0x0C, 0x0C, 0x0C}, // V
  {0x21, 0x21, 0x21, 0x21, 0x21, 0x2D, 0x2D, 0x33, 0x33, 0x21}, // W
  {0x21, 0x21, 0x12, 0x0C, 0x0C, 0x0C, 0x0C, 0x12, 0x21, 0x21}, // X
  {0x21, 0x21, 0x12, 0x12, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C}, // Y
  {0x3F, 0x3F, 0x02, 0x04, 0x04, 0x08, 0x08, 0x10, 0x3F, 0x3F}  // Z
};
// Shape Definitions (4x4 grids)
// 1 = block, 0 = empty
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
uint8_t board[WIDTH][HEIGHT]; // Stores locked blocks (0 = empty, 1-7 = color index + 1)
int currentPieceType = -1;
int currentRotation = 0;
int px = 0;
int py = -4;

void setup() {
  matrix.begin();
  matrix.setBrightness(8);
  randomSeed(analogRead(A1)); // Generate random seed from unconnected pin
  resetBoard();
  matrix.fillScreen(0); // Clear buffer

  // Intro Sequence: T-E-T-R-I-S
  int intro[] = {19, 4, 19, 17, 8, 18}; 
  scrollMessage(intro, 6);
}

void loop() {
  matrix.fillScreen(0); // Clear buffer

  // Run the Tetris Physics
  updateTetris();

  // Draw the result
  drawScreen();
  matrix.show();
  delay(200); // Speed of the fall
}

// --- CORE FUNCTIONS ---

void updateTetris() {
  // 1. If no piece exists, spawn one
  if (currentPieceType == -1) {
    spawnPiece();
    
    // Immediate Game Over check: if the new piece collides immediately, reset board
    if (checkCollision(px, py, currentRotation)) {
      drawFail();
      resetBoard();
    }
    return;
  }

  // 2. Try to move down
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

  // 1. Loop through all 4 rotations
  for (int r = 0; r < 4; r++) {
    // 2. Loop through all X positions (from -2 to WIDTH)
    for (int x = -2; x < WIDTH; x++) {
      
      // Calculate where the piece would land (Hard Drop)
      int y = 0;
      // If we can't even spawn here, skip it
      if (checkCollision(x, y, r)) continue;

      // Drop it until it hits something
      while (!checkCollision(x, y + 1, r)) {
        y++;
      }

      // 3. Evaluate this final position
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
  py = -1; // Start at top
  currentRotation = bestRot;
}

int evaluateBoard(int pieceX, int pieceY, int rot) {
  // Create a temporary board simulation
  uint8_t tempBoard[WIDTH][HEIGHT];
  memcpy(tempBoard, board, sizeof(board));

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
  return (completeLines * 70) - (aggregateHeight * 5) - (holes * 30) - (bumpiness * 8);
}

bool getPixel(int type, int rot, int x, int y) {
  uint16_t shapeData = tetrominoes[type][rot % 4];
  int bitIndex = 15 - (y * 4 + x);
  return (shapeData >> bitIndex) & 1;
}

// --- DRAWING FUNCTIONS ---
void drawScreen(){
  drawBoard();
  drawCurrentPiece();
}

// Scrolls a sequence of letters continuously
void scrollMessage(int* letters, int count) {
  int letterHeight = 10;
  int gap = 2;
  static int offset = random(0,count);
  uint16_t colors[count];
  for (int i = 0; i < count; i++)
  {
    colors[i] = tetrisColors[(i + offset) % count];
  }
  offset > count ? offset = 0: offset += 1;
  
  int totalChainHeight = count * (letterHeight + gap);

  // Start: Top of first letter is at the bottom of the screen (HEIGHT)
  // End:  Top of first letter is far enough up that the last letter is off screen
  for (int globalY = HEIGHT; globalY > -totalChainHeight; globalY--) {
    matrix.fillScreen(0);

    for (int i = 0; i < count; i++) {
      uint16_t color = tetrisColors[random(0, 7)];
      // global position + its place in the line
      int letterY = globalY + (i * (letterHeight + gap));

      drawLetterAt(letters[i], letterY, colors[i]);
    }
    
    matrix.show();
    delay(60);
  }
}

// Draw a letter at a specific Y coordinate
void drawLetterAt(int letterIndex, int yPos, uint16_t color) {
  if (letterIndex < 0 || letterIndex > 25) return;

  for (int row = 0; row < 10; row++) {
    int screenY = yPos + row;

    if (screenY >= 0 && screenY < HEIGHT) {
      uint8_t rowData = alphabet[letterIndex][row];
      
      for (int x = 0; x < WIDTH; x++) {
        bool pixelOn = (rowData >> (5 - x)) & 1;
        
        if (pixelOn) {
          matrix.drawPixel(x, screenY, color); 
        }
      }
    }
  }
}

void scrollLetter(int letterIndex, uint16_t color) {
  // Clamp index to prevent crashes
  if(letterIndex < 0 || letterIndex > 25) return;

  // Start with offset at HEIGHT (just below screen)
  // End with offset at -10 (just above screen, since letter height is 10)
  for (int offset = HEIGHT; offset >= -10; offset--) {
    matrix.fillScreen(0);

    // Loop through the 10 rows of the letter data
    for (int row = 0; row < 10; row++) {
      int screenY = row + offset;

      // Optimization: Only draw pixels that are currently visible on screen
      if (screenY >= 0 && screenY < HEIGHT) {
        uint8_t rowData = alphabet[letterIndex][row];
        
        for (int x = 0; x < WIDTH; x++) {
          // Check the bit for the specific pixel
          bool pixelOn = (rowData >> (5 - x)) & 1;
          
          if (pixelOn) {
             // Draw pixel with random Tetris color (shimmer effect)
             matrix.drawPixel(x, screenY, color); 
          }
        }
      }
    }
    matrix.show();
    delay(60);
  }
}

void drawBoard() {
  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT; y++) {
      if (board[x][y] > 0) {
        matrix.drawPixel(x, y, tetrisColors[board[x][y] - 1]);
      }
    }
  }
}

void drawFail(){
  for (int i = 0; i < 3; i++){
    matrix.fillScreen(0);
    matrix.drawFastVLine(0,0, HEIGHT, matrix.Color(255, 0, 0));
    matrix.drawFastVLine(WIDTH - 1, 0, HEIGHT, matrix.Color(255, 0, 0));
    matrix.show();
    delay(100);
    drawScreen();
    matrix.drawFastVLine(0,0, HEIGHT, matrix.Color(255, 0, 0));
    matrix.drawFastVLine(WIDTH - 1, 0, HEIGHT, matrix.Color(255, 0, 0));
    matrix.show();
    delay(100);
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
      // Flash effect
      for (int i = 0; i < 2; i++){
        matrix.fillScreen(0);
        drawScreen();
        matrix.drawFastHLine(0, y, WIDTH, matrix.Color(255, 255, 255));
        matrix.show();
        delay(50);
        matrix.drawFastHLine(0, y, WIDTH, matrix.Color(0, 0, 0));
        matrix.show();
        delay(50);
      }
      
      // Move everything down
      for (int ny = y; ny > 0; ny--) {
        for (int nx = 0; nx < WIDTH; nx++) {
          board[nx][ny] = board[nx][ny - 1];
        }
      }
      for (int nx = 0; nx < WIDTH; nx++) {
        board[nx][0] = 0;
      };

      y++; 
      matrix.fillScreen(0);
      drawScreen();
      matrix.show();
    }
  }
}

void resetBoard() {
  memset(board, 0, sizeof(board));
  currentPieceType = -1;
}
