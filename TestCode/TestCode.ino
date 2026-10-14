#include <UTFT.h>
#include <URTouch.h>

// 1. DISPLAY SETUP
// Common 40-pin driver models: ILI9341_16, ILI9486, ILI9481, SSD1289, or CTE32HR
// If your screen stays white, change 'ILI9341_16' to match the chip on your screen.
UTFT myGLCD(ILI9341_16, 38, 39, 40, 41);

// 2. TOUCH SETUP
URTouch myTouch(6, 5, 4, 3, 2);

// Fonts included with UTFT
extern uint8_t BigFont[];
extern uint8_t SmallFont[];

// Onboard component pin from the shield
const int LED_PIN = 13; // Matches "Led pin13" (D4) on the board

int x, y;
bool ledState = false;

void drawInterface() {
  myGLCD.clrScr();
  
  // Header bar
  myGLCD.setColor(0, 102, 204);
  myGLCD.fillRect(0, 0, 319, 35);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(0, 102, 204);
  myGLCD.setFont(BigFont);
  myGLCD.print("MEGA TOUCH PANEL", CENTER, 10);

  // ON Button (Green)
  myGLCD.setColor(0, 180, 0);
  myGLCD.fillRoundRect(30, 70, 145, 150);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect(30, 70, 145, 150);
  myGLCD.setBackColor(0, 180, 0);
  myGLCD.print("ON", 70, 102);

  // OFF Button (Red)
  myGLCD.setColor(200, 0, 0);
  myGLCD.fillRoundRect(175, 70, 290, 150);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect(175, 70, 290, 150);
  myGLCD.setBackColor(200, 0, 0);
  myGLCD.print("OFF", 210, 102);

  updateStatusText();
}

void updateStatusText() {
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(0, 0, 0);
  if (ledState) {
    myGLCD.setColor(0, 255, 0);
    myGLCD.print("LED PIN 13: ON ", CENTER, 175);
  } else {
    myGLCD.setColor(255, 80, 80);
    myGLCD.print("LED PIN 13: OFF", CENTER, 175);
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Initialize screen in Landscape mode
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.clrScr();

  // Initialize touch controller
  myTouch.InitTouch(LANDSCAPE);
  myTouch.setPrecision(PREC_MEDIUM);

  drawInterface();
}

void loop() {
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();

    // Display raw X/Y coordinates at the bottom for easy debugging
    myGLCD.setFont(SmallFont);
    myGLCD.setColor(200, 200, 200);
    myGLCD.setBackColor(0, 0, 0);
    myGLCD.print("Touch X:     Y:    ", CENTER, 215);
    myGLCD.printNumI(x, 155, 215, 3);
    myGLCD.printNumI(y, 215, 215, 3);

    // Check if ON button (x: 30-145, y: 70-150) is pressed
    if ((x >= 30) && (x <= 145) && (y >= 70) && (y <= 150)) {
      ledState = true;
      digitalWrite(LED_PIN, HIGH);
      updateStatusText();
      delay(150); // Simple debounce
    }

    // Check if OFF button (x: 175-290, y: 70-150) is pressed
    if ((x >= 175) && (x <= 290) && (y >= 70) && (y <= 150)) {
      ledState = false;
      digitalWrite(LED_PIN, LOW);
      updateStatusText();
      delay(150); // Simple debounce
    }
  }
}