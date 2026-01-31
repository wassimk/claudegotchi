/*
 * Claude Tamagotchi for M5Stack Core2
 *
 * A cute Claude mascot that displays coding status
 * Inspired by the Claude Code tamagotchi concept
 *
 * Hardware: M5Stack Core2
 * Libraries required: M5Core2
 */

#include <M5Core2.h>

// Screen dimensions
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

// Colors (RGB565 format)
#define COLOR_BLACK      0x0000
#define COLOR_WHITE      0xFFFF
#define COLOR_CLAUDE     0xFDE0  // Yellow/orange for Claude
#define COLOR_CLAUDE_DARK 0xD4A0 // Darker shade for details
#define COLOR_GREEN      0x07E0  // Matrix green
#define COLOR_DARK_GREEN 0x03E0  // Darker green
#define COLOR_ORANGE     0xFD20  // Status orange
#define COLOR_STATUS_BG  0x2104  // Dark gray for status bar

// Claude character position and size
#define CLAUDE_X 160
#define CLAUDE_Y 130
#define CLAUDE_SCALE 4

// Animation state
float breatheOffset = 0;
unsigned long lastAnimUpdate = 0;
const int ANIM_INTERVAL = 50;  // ms between animation frames

// Status states
enum ClaudeState {
  STATE_IDLE,
  STATE_CODING,
  STATE_REASONING,
  STATE_TESTING,
  STATE_SLEEPING
};

ClaudeState currentState = STATE_CODING;
String stateLabels[] = {"IDLE", "CODING", "REASONING", "TESTING", "SLEEPING"};
uint16_t stateColors[] = {COLOR_WHITE, COLOR_GREEN, COLOR_ORANGE, COLOR_GREEN, 0x001F};

// Matrix rain effect
#define NUM_DROPS 20
int dropX[NUM_DROPS];
int dropY[NUM_DROPS];
int dropSpeed[NUM_DROPS];

// Function declarations
void drawClaude(int x, int y, int scale, float bounce);
void drawStatusBar();
void drawBottomStatus();
void updateMatrixRain();
void initMatrixRain();
void handleTouch();

void setup() {
  M5.begin();
  M5.Lcd.fillScreen(COLOR_BLACK);
  M5.Lcd.setTextColor(COLOR_WHITE);
  M5.Lcd.setTextSize(2);

  // Initialize matrix rain
  initMatrixRain();

  // Draw initial screen
  drawStatusBar();
  drawBottomStatus();
  drawClaude(CLAUDE_X, CLAUDE_Y, CLAUDE_SCALE, 0);

  Serial.begin(115200);
  Serial.println("Claude Tamagotchi Started!");
}

void loop() {
  M5.update();

  unsigned long currentTime = millis();

  // Update animation
  if (currentTime - lastAnimUpdate >= ANIM_INTERVAL) {
    lastAnimUpdate = currentTime;

    // Breathing/bouncing animation
    breatheOffset = sin(currentTime / 500.0) * 3;

    // Clear character area and redraw
    M5.Lcd.fillRect(CLAUDE_X - 40, CLAUDE_Y - 50, 80, 100, COLOR_BLACK);

    // Update matrix rain in character area background
    updateMatrixRain();

    // Draw Claude with bounce
    drawClaude(CLAUDE_X, CLAUDE_Y + (int)breatheOffset, CLAUDE_SCALE, breatheOffset);
  }

  // Handle touch input
  handleTouch();

  delay(10);
}

void initMatrixRain() {
  for (int i = 0; i < NUM_DROPS; i++) {
    dropX[i] = random(0, SCREEN_WIDTH);
    dropY[i] = random(0, SCREEN_HEIGHT);
    dropSpeed[i] = random(2, 6);
  }
}

void updateMatrixRain() {
  // Only draw rain on sides of screen (not over Claude)
  for (int i = 0; i < NUM_DROPS; i++) {
    // Erase old position
    M5.Lcd.fillRect(dropX[i], dropY[i], 8, 12, COLOR_BLACK);

    // Update position
    dropY[i] += dropSpeed[i];
    if (dropY[i] > SCREEN_HEIGHT - 30) {
      dropY[i] = 25;
      dropX[i] = random(0, SCREEN_WIDTH);
    }

    // Only draw if not in Claude's area
    if (dropX[i] < CLAUDE_X - 50 || dropX[i] > CLAUDE_X + 50) {
      // Draw character
      char c = random(33, 126);  // Random ASCII character
      uint16_t color = (random(10) > 7) ? COLOR_GREEN : COLOR_DARK_GREEN;
      M5.Lcd.setTextSize(1);
      M5.Lcd.setTextColor(color);
      M5.Lcd.setCursor(dropX[i], dropY[i]);
      M5.Lcd.print(c);
    }
  }
}

void drawStatusBar() {
  // Top status bar background
  M5.Lcd.fillRect(0, 0, SCREEN_WIDTH, 22, COLOR_STATUS_BG);

  // WiFi icon (simple representation)
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(COLOR_WHITE);
  M5.Lcd.setCursor(5, 6);
  M5.Lcd.print("WiFi");

  // Battery indicator
  M5.Lcd.setCursor(SCREEN_WIDTH - 40, 6);
  M5.Lcd.print("100%");

  // Status badge in center
  int badgeWidth = 80;
  int badgeX = (SCREEN_WIDTH - badgeWidth) / 2;
  uint16_t badgeColor = stateColors[currentState];

  M5.Lcd.fillRoundRect(badgeX, 3, badgeWidth, 16, 4, badgeColor);
  M5.Lcd.setTextColor(COLOR_BLACK);
  M5.Lcd.setCursor(badgeX + 10, 6);
  M5.Lcd.print(stateLabels[currentState]);
}

void drawBottomStatus() {
  // Bottom area
  M5.Lcd.fillRect(0, SCREEN_HEIGHT - 25, SCREEN_WIDTH, 25, COLOR_BLACK);

  // Draw "CODING" text centered at bottom
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(COLOR_WHITE);
  String bottomText = stateLabels[currentState];
  int textWidth = bottomText.length() * 12;
  M5.Lcd.setCursor((SCREEN_WIDTH - textWidth) / 2, SCREEN_HEIGHT - 22);
  M5.Lcd.print(bottomText);

  // Draw button labels
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(COLOR_DARK_GREEN);
  M5.Lcd.setCursor(45, SCREEN_HEIGHT - 8);
  M5.Lcd.print("FEED");
  M5.Lcd.setCursor(145, SCREEN_HEIGHT - 8);
  M5.Lcd.print("PLAY");
  M5.Lcd.setCursor(240, SCREEN_HEIGHT - 8);
  M5.Lcd.print("STAT");
}

void drawClaude(int x, int y, int scale, float bounce) {
  // Claude's pixel art representation
  // Body (main rectangle) - yellow/orange
  int bodyWidth = 10 * scale;
  int bodyHeight = 12 * scale;
  int bodyX = x - bodyWidth / 2;
  int bodyY = y - bodyHeight / 2;

  // Main body
  M5.Lcd.fillRect(bodyX, bodyY, bodyWidth, bodyHeight, COLOR_CLAUDE);

  // Body outline/shading
  M5.Lcd.drawRect(bodyX, bodyY, bodyWidth, bodyHeight, COLOR_CLAUDE_DARK);

  // Eyes (two small black squares)
  int eyeSize = 2 * scale;
  int eyeY = bodyY + 3 * scale;
  int leftEyeX = bodyX + 2 * scale;
  int rightEyeX = bodyX + bodyWidth - 4 * scale;

  M5.Lcd.fillRect(leftEyeX, eyeY, eyeSize, eyeSize, COLOR_BLACK);
  M5.Lcd.fillRect(rightEyeX, eyeY, eyeSize, eyeSize, COLOR_BLACK);

  // Mouth (small line or smile based on state)
  int mouthY = bodyY + 7 * scale;
  int mouthX = bodyX + 3 * scale;
  int mouthWidth = 4 * scale;

  if (currentState == STATE_SLEEPING) {
    // Zzz for sleeping
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(COLOR_WHITE);
    M5.Lcd.setCursor(x + 20, y - 30);
    M5.Lcd.print("Zzz");
  } else {
    // Simple smile
    M5.Lcd.fillRect(mouthX, mouthY, mouthWidth, scale, COLOR_BLACK);
  }

  // Legs (two small rectangles at bottom)
  int legWidth = 2 * scale;
  int legHeight = 3 * scale;
  int legY = bodyY + bodyHeight;
  int leftLegX = bodyX + 2 * scale;
  int rightLegX = bodyX + bodyWidth - 4 * scale;

  // Animate legs slightly
  int legOffset = (int)(abs(bounce) / 2);

  M5.Lcd.fillRect(leftLegX, legY - legOffset, legWidth, legHeight + legOffset, COLOR_CLAUDE);
  M5.Lcd.fillRect(rightLegX, legY - legOffset, legWidth, legHeight + legOffset, COLOR_CLAUDE);

  // Arms (small rectangles on sides)
  int armWidth = 2 * scale;
  int armHeight = 4 * scale;
  int armY = bodyY + 4 * scale;

  M5.Lcd.fillRect(bodyX - armWidth, armY, armWidth, armHeight, COLOR_CLAUDE);
  M5.Lcd.fillRect(bodyX + bodyWidth, armY, armWidth, armHeight, COLOR_CLAUDE);

  // Antenna/ears (small protrusions on top)
  int earWidth = 2 * scale;
  int earHeight = 2 * scale;

  M5.Lcd.fillRect(leftEyeX, bodyY - earHeight, earWidth, earHeight, COLOR_CLAUDE);
  M5.Lcd.fillRect(rightEyeX, bodyY - earHeight, earWidth, earHeight, COLOR_CLAUDE);
}

void handleTouch() {
  if (M5.Touch.ispressed()) {
    TouchPoint_t pos = M5.Touch.getPressPoint();

    // Check which button area was touched
    if (pos.y > SCREEN_HEIGHT - 50) {
      if (pos.x < SCREEN_WIDTH / 3) {
        // Left button - Feed
        currentState = STATE_CODING;
        Serial.println("Feed pressed - Coding");
      } else if (pos.x < 2 * SCREEN_WIDTH / 3) {
        // Middle button - Play
        currentState = STATE_REASONING;
        Serial.println("Play pressed - Reasoning");
      } else {
        // Right button - Status
        currentState = STATE_TESTING;
        Serial.println("Status pressed - Testing");
      }

      // Redraw status elements
      drawStatusBar();
      drawBottomStatus();

      // Debounce
      delay(200);
    }
  }
}
