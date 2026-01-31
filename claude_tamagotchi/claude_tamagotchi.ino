/*
 * Claude Code Notifier for M5Stack Core2
 *
 * A cute Claude mascot that notifies you when Claude Code needs attention.
 * Connects to Claude Code via hooks to receive notifications.
 *
 * Hardware: M5Stack Core2
 * Libraries required: M5Unified, M5GFX
 */

#include <M5Unified.h>

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
#define COLOR_RED        0xF800  // Alert red
#define COLOR_STATUS_BG  0x2104  // Dark gray for status bar

// Claude character bounds for wandering
#define CLAUDE_MIN_X 60
#define CLAUDE_MAX_X 260
#define CLAUDE_MIN_Y 80
#define CLAUDE_MAX_Y 180
#define CLAUDE_SCALE 4

// Animation timing
#define IDLE_ANIM_INTERVAL 80      // Slower for idle
#define ATTENTION_ANIM_INTERVAL 40 // Faster for attention
#define WANDER_INTERVAL 2000       // How often to pick new wander target
#define CHIRP_INTERVAL 3000        // How often to chirp when needing attention

// Device states
enum DeviceState {
  STATE_IDLE,
  STATE_NEEDS_ATTENTION
};

DeviceState currentState = STATE_IDLE;

// Claude position (floating point for smooth movement)
float claudeX = 160;
float claudeY = 130;
float targetX = 160;
float targetY = 130;
float moveSpeed = 0.3;  // Slow wandering speed

// Animation state
float breatheOffset = 0;
float bounceOffset = 0;
unsigned long lastAnimUpdate = 0;
unsigned long lastWanderUpdate = 0;
unsigned long lastChirpTime = 0;
int animFrame = 0;

// Sound state
bool soundEnabled = true;

// Matrix rain effect
#define NUM_DROPS 25
int dropX[NUM_DROPS];
int dropY[NUM_DROPS];
int dropSpeed[NUM_DROPS];
char dropChar[NUM_DROPS];

// Function declarations
void drawClaude(int x, int y, int scale, bool excited);
void drawStatusBar();
void drawBottomUI();
void updateMatrixRain();
void initMatrixRain();
void handleTouch();
void updateIdleAnimation();
void updateAttentionAnimation();
void playChirp();
void pickNewWanderTarget();
void clearClaudeArea(int x, int y);

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);

  // Initialize speaker
  M5.Speaker.setVolume(128);

  // Setup display
  M5.Display.fillScreen(COLOR_BLACK);
  M5.Display.setTextColor(COLOR_WHITE);

  // Initialize matrix rain
  initMatrixRain();

  // Draw initial UI
  drawStatusBar();
  drawBottomUI();
  drawClaude((int)claudeX, (int)claudeY, CLAUDE_SCALE, false);

  // Play startup chirp
  if (soundEnabled) {
    playChirp();
  }

  Serial.begin(115200);
  Serial.println("Claude Code Notifier Started!");
  Serial.println("Send 'ATTENTION' via Serial to trigger notification");
  Serial.println("Send 'IDLE' to return to idle state");
}

void loop() {
  M5.update();

  unsigned long currentTime = millis();

  // Check for serial commands (for testing, later will be WiFi/hooks)
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    if (cmd == "ATTENTION") {
      currentState = STATE_NEEDS_ATTENTION;
      lastChirpTime = 0;  // Trigger immediate chirp
      drawStatusBar();
      Serial.println("State: NEEDS_ATTENTION");
    } else if (cmd == "IDLE") {
      currentState = STATE_IDLE;
      drawStatusBar();
      Serial.println("State: IDLE");
    }
  }

  // Determine animation interval based on state
  int animInterval = (currentState == STATE_IDLE) ? IDLE_ANIM_INTERVAL : ATTENTION_ANIM_INTERVAL;

  // Update animation
  if (currentTime - lastAnimUpdate >= animInterval) {
    lastAnimUpdate = currentTime;
    animFrame++;

    // Clear previous Claude position
    clearClaudeArea((int)claudeX, (int)claudeY);

    // Update based on state
    if (currentState == STATE_IDLE) {
      updateIdleAnimation();
    } else {
      updateAttentionAnimation();
    }

    // Update matrix rain
    updateMatrixRain();

    // Draw Claude at current position
    bool excited = (currentState == STATE_NEEDS_ATTENTION);
    drawClaude((int)claudeX, (int)claudeY + (int)bounceOffset, CLAUDE_SCALE, excited);
  }

  // Handle chirping when attention needed
  if (currentState == STATE_NEEDS_ATTENTION && soundEnabled) {
    if (currentTime - lastChirpTime >= CHIRP_INTERVAL) {
      lastChirpTime = currentTime;
      playChirp();
    }
  }

  // Pick new wander target periodically when idle
  if (currentState == STATE_IDLE) {
    if (currentTime - lastWanderUpdate >= WANDER_INTERVAL) {
      lastWanderUpdate = currentTime;
      pickNewWanderTarget();
    }
  }

  // Handle touch input
  handleTouch();

  delay(10);
}

void updateIdleAnimation() {
  // Gentle breathing animation
  breatheOffset = sin(millis() / 800.0) * 2;
  bounceOffset = breatheOffset;

  // Slowly move toward target
  float dx = targetX - claudeX;
  float dy = targetY - claudeY;
  float dist = sqrt(dx * dx + dy * dy);

  if (dist > 1) {
    claudeX += (dx / dist) * moveSpeed;
    claudeY += (dy / dist) * moveSpeed;
  }
}

void updateAttentionAnimation() {
  // Excited bouncing animation
  bounceOffset = sin(millis() / 150.0) * 6;

  // Slight horizontal wiggle
  float wiggle = sin(millis() / 200.0) * 3;
  claudeX = 160 + wiggle;  // Stay centered but wiggle
  claudeY = 130;
}

void pickNewWanderTarget() {
  targetX = random(CLAUDE_MIN_X, CLAUDE_MAX_X);
  targetY = random(CLAUDE_MIN_Y, CLAUDE_MAX_Y);
}

void playChirp() {
  // Cute chirp chirp sound
  M5.Speaker.tone(1800, 80);
  delay(100);
  M5.Speaker.tone(2200, 80);
  delay(100);
  M5.Speaker.tone(1800, 60);
  delay(80);
  M5.Speaker.stop();
}

void clearClaudeArea(int x, int y) {
  // Clear area around Claude for redraw
  M5.Display.fillRect(x - 50, y - 45, 100, 95, COLOR_BLACK);
}

void initMatrixRain() {
  for (int i = 0; i < NUM_DROPS; i++) {
    dropX[i] = random(0, SCREEN_WIDTH);
    dropY[i] = random(25, SCREEN_HEIGHT - 30);
    dropSpeed[i] = random(2, 5);
    dropChar[i] = random(33, 126);
  }
}

void updateMatrixRain() {
  for (int i = 0; i < NUM_DROPS; i++) {
    // Erase old position
    M5.Display.fillRect(dropX[i], dropY[i], 8, 10, COLOR_BLACK);

    // Update position
    dropY[i] += dropSpeed[i];
    if (dropY[i] > SCREEN_HEIGHT - 35) {
      dropY[i] = 25;
      dropX[i] = random(0, SCREEN_WIDTH);
      dropChar[i] = random(33, 126);
    }

    // Only draw if not in Claude's area
    int cdx = abs(dropX[i] - (int)claudeX);
    int cdy = abs(dropY[i] - (int)claudeY);
    if (cdx > 55 || cdy > 50) {
      uint16_t color = (random(10) > 7) ? COLOR_GREEN : COLOR_DARK_GREEN;
      M5.Display.setTextSize(1);
      M5.Display.setTextColor(color, COLOR_BLACK);
      M5.Display.setCursor(dropX[i], dropY[i]);
      M5.Display.print((char)dropChar[i]);

      // Occasionally change character
      if (random(20) == 0) {
        dropChar[i] = random(33, 126);
      }
    }
  }
}

void drawStatusBar() {
  // Top status bar background
  M5.Display.fillRect(0, 0, SCREEN_WIDTH, 22, COLOR_STATUS_BG);

  // Sound indicator
  M5.Display.setTextSize(1);
  M5.Display.setTextColor(soundEnabled ? COLOR_GREEN : COLOR_RED, COLOR_STATUS_BG);
  M5.Display.setCursor(5, 6);
  M5.Display.print(soundEnabled ? "SND:ON" : "SND:OFF");

  // Status badge in center
  int badgeWidth = 100;
  int badgeX = (SCREEN_WIDTH - badgeWidth) / 2;
  uint16_t badgeColor = (currentState == STATE_IDLE) ? COLOR_GREEN : COLOR_ORANGE;
  const char* stateText = (currentState == STATE_IDLE) ? "IDLE" : "ATTENTION!";

  M5.Display.fillRoundRect(badgeX, 3, badgeWidth, 16, 4, badgeColor);
  M5.Display.setTextColor(COLOR_BLACK, badgeColor);
  M5.Display.setCursor(badgeX + 15, 6);
  M5.Display.print(stateText);

  // Version/title
  M5.Display.setTextColor(COLOR_DARK_GREEN, COLOR_STATUS_BG);
  M5.Display.setCursor(SCREEN_WIDTH - 55, 6);
  M5.Display.print("Claude");
}

void drawBottomUI() {
  // Bottom area background
  M5.Display.fillRect(0, SCREEN_HEIGHT - 28, SCREEN_WIDTH, 28, COLOR_STATUS_BG);

  // Button labels
  M5.Display.setTextSize(1);

  // Button 1 - Sound toggle
  M5.Display.setTextColor(COLOR_WHITE, COLOR_STATUS_BG);
  M5.Display.setCursor(35, SCREEN_HEIGHT - 18);
  M5.Display.print("SOUND");

  // Button 2 - Test attention (for now)
  M5.Display.setCursor(135, SCREEN_HEIGHT - 18);
  M5.Display.print("ALERT");

  // Button 3 - Dismiss/Idle
  M5.Display.setCursor(235, SCREEN_HEIGHT - 18);
  M5.Display.print("DISMISS");
}

void drawClaude(int x, int y, int scale, bool excited) {
  // Body dimensions
  int bodyWidth = 10 * scale;
  int bodyHeight = 12 * scale;
  int bodyX = x - bodyWidth / 2;
  int bodyY = y - bodyHeight / 2;

  // Main body
  M5.Display.fillRect(bodyX, bodyY, bodyWidth, bodyHeight, COLOR_CLAUDE);
  M5.Display.drawRect(bodyX, bodyY, bodyWidth, bodyHeight, COLOR_CLAUDE_DARK);

  // Eyes
  int eyeSize = 2 * scale;
  int eyeY = bodyY + 3 * scale;
  int leftEyeX = bodyX + 2 * scale;
  int rightEyeX = bodyX + bodyWidth - 4 * scale;

  if (excited) {
    // Wide excited eyes (bigger)
    M5.Display.fillRect(leftEyeX, eyeY, eyeSize + 2, eyeSize + 2, COLOR_BLACK);
    M5.Display.fillRect(rightEyeX - 2, eyeY, eyeSize + 2, eyeSize + 2, COLOR_BLACK);
    // Eye sparkle
    M5.Display.fillRect(leftEyeX + 1, eyeY + 1, 2, 2, COLOR_WHITE);
    M5.Display.fillRect(rightEyeX - 1, eyeY + 1, 2, 2, COLOR_WHITE);
  } else {
    // Normal eyes
    M5.Display.fillRect(leftEyeX, eyeY, eyeSize, eyeSize, COLOR_BLACK);
    M5.Display.fillRect(rightEyeX, eyeY, eyeSize, eyeSize, COLOR_BLACK);
  }

  // Mouth
  int mouthY = bodyY + 7 * scale;
  int mouthX = bodyX + 3 * scale;

  if (excited) {
    // Open mouth (excited)
    M5.Display.fillRect(mouthX, mouthY, 4 * scale, 2 * scale, COLOR_BLACK);
  } else {
    // Simple line mouth
    M5.Display.fillRect(mouthX, mouthY, 4 * scale, scale, COLOR_BLACK);
  }

  // Legs
  int legWidth = 2 * scale;
  int legHeight = 3 * scale;
  int legY = bodyY + bodyHeight;
  int leftLegX = bodyX + 2 * scale;
  int rightLegX = bodyX + bodyWidth - 4 * scale;

  // Leg animation
  int legAnim = excited ? (animFrame % 4) : 0;
  int leftLegOffset = (legAnim == 1 || legAnim == 2) ? 2 : 0;
  int rightLegOffset = (legAnim == 0 || legAnim == 1) ? 2 : 0;

  M5.Display.fillRect(leftLegX, legY + leftLegOffset, legWidth, legHeight, COLOR_CLAUDE);
  M5.Display.fillRect(rightLegX, legY + rightLegOffset, legWidth, legHeight, COLOR_CLAUDE);

  // Arms
  int armWidth = 2 * scale;
  int armHeight = 4 * scale;
  int armY = bodyY + 4 * scale;

  if (excited) {
    // Arms up when excited
    M5.Display.fillRect(bodyX - armWidth, armY - 3 * scale, armWidth, armHeight, COLOR_CLAUDE);
    M5.Display.fillRect(bodyX + bodyWidth, armY - 3 * scale, armWidth, armHeight, COLOR_CLAUDE);
  } else {
    // Normal arms
    M5.Display.fillRect(bodyX - armWidth, armY, armWidth, armHeight, COLOR_CLAUDE);
    M5.Display.fillRect(bodyX + bodyWidth, armY, armWidth, armHeight, COLOR_CLAUDE);
  }

  // Ears/antenna
  int earWidth = 2 * scale;
  int earHeight = 2 * scale;
  M5.Display.fillRect(leftEyeX, bodyY - earHeight, earWidth, earHeight, COLOR_CLAUDE);
  M5.Display.fillRect(rightEyeX, bodyY - earHeight, earWidth, earHeight, COLOR_CLAUDE);

  // Exclamation mark when attention needed
  if (excited && (animFrame % 8 < 5)) {
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(COLOR_ORANGE, COLOR_BLACK);
    M5.Display.setCursor(x + 25, y - 35);
    M5.Display.print("!");
  }
}

void handleTouch() {
  auto touchDetail = M5.Touch.getDetail();

  if (touchDetail.wasPressed()) {
    int tx = touchDetail.x;
    int ty = touchDetail.y;

    // Check if touching bottom button area
    if (ty > SCREEN_HEIGHT - 50) {
      if (tx < SCREEN_WIDTH / 3) {
        // Button 1 - Toggle sound
        soundEnabled = !soundEnabled;
        Serial.print("Sound: ");
        Serial.println(soundEnabled ? "ON" : "OFF");
        drawStatusBar();

        // Play confirmation beep if turning on
        if (soundEnabled) {
          M5.Speaker.tone(1000, 50);
          delay(60);
          M5.Speaker.stop();
        }
      } else if (tx < 2 * SCREEN_WIDTH / 3) {
        // Button 2 - Trigger attention (for testing)
        currentState = STATE_NEEDS_ATTENTION;
        lastChirpTime = 0;
        drawStatusBar();
        Serial.println("State: NEEDS_ATTENTION (manual trigger)");
      } else {
        // Button 3 - Dismiss/return to idle
        currentState = STATE_IDLE;
        drawStatusBar();
        Serial.println("State: IDLE (dismissed)");
      }
    }
  }
}
