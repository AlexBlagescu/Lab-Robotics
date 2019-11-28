#include <LiquidCrystal.h>
#include <EEPROM.h>

const int RS = 12;
const int enable = 11;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;

const int pinSW = 10;
const int pinX = A0;
const int pinY = A1;

const int thresholdMin = 400;
const int thresholdMax = 700;

int switchValue;
int lastSwitchValue = 0;
int xValue = 0;
int yValue = 0;
int iter1 = 0;
int iter2 = 0;
int iter3 = 0;
int iter = 0;
int iterNrPhase = 0;
int nr = 0;
int addr = 0;

int lives = 3;
int score = 3;
int lvl = 1;
int highScore = 0;
unsigned long updatePlayTime = 5000;
unsigned long cursorShowTime = 0;
unsigned long cursorBlinkingTime = 0;

int cursorStateOX[3] = {
  0, 6, 3
};
int cursorStateOY[3] = {
  0, 0, 1
};
int cursorStateSettingsOX[2] = {
  0, 0
};
int cursorStateSettingsOY[2] = {
  0, 1
};
int cursorPosition = 0;
int cursorSettingsPosition = 0;
bool openPhaseState = true;
bool buttonPhase = false;
bool joyMoved = false;
bool lcdContent = true;
bool phaseBlink = false;
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);
String openPhase[3] = {
  "Play", "HighScore", "Settings"
};

void showOpenPhase() {
  //  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(openPhase[0]);
  lcd.setCursor(7, 0);
  lcd.print(openPhase[1]);
  lcd.setCursor(4, 1);
  lcd.print(openPhase[2]);
}
void showPlayPhase() {
  lcd.setCursor(0, 0);
  lcd.print("Lives: ");
  lcd.setCursor(7, 0);
  lcd.print(lives);
  lcd.setCursor(10, 0);
  lcd.print("Lvl: ");
  lcd.setCursor(15, 0);
  lcd.print(lvl);
  lcd.setCursor(4, 1);
  lcd.print("Score: ");
  lcd.setCursor(11, 1);
  score = lvl * 3;
  lcd.print(score);
}
int checkCursorPosition() {
  if (xValue > thresholdMax && joyMoved == false) {
    deleteBox(cursorStateOX[cursorPosition], cursorStateOY[cursorPosition]);
    cursorPosition += 1;
    if (cursorPosition >= 2)
      cursorPosition = 2;
    joyMoved = true;
  }
  if (xValue < thresholdMin && joyMoved == false) {
    deleteBox(cursorStateOX[cursorPosition], cursorStateOY[cursorPosition]);
    cursorPosition -= 1;
    if (cursorPosition <= 0)
      cursorPosition = 0;
    joyMoved = true;
  }
  return cursorPosition;
}
void checkJoyReturn() {
  if (xValue >= thresholdMin && xValue <= thresholdMax) {
    joyMoved = false;
  }
}
void drawCursor() {
  lcd.setCursor(cursorStateOX[cursorPosition], cursorStateOY[cursorPosition]);
  lcd.print(">");
}
void drawCursorSettings() {
  lcd.setCursor(cursorStateSettingsOX[cursorSettingsPosition], cursorStateSettingsOY[cursorSettingsPosition]);
  lcd.print(">");
}
void deleteBox(int oxValue, int oyValue) {
  lcd.setCursor(oxValue, oyValue);
  lcd.print(" ");
}
bool checkButtonPressed() {
  if (switchValue == 1 && lastSwitchValue == 0) {
    openPhaseState = !openPhaseState;
    joyMoved = false;
    return true;
  }
  return false;
}
void clearLCD() {
  if (lcdContent == true) {
    lcdContent = false;
    lcd.clear();
  }
}
void showEndGame() {
  lcd.setCursor(0, 0);
  lcd.print("CONGRATULATIONS!");
  lcd.setCursor(0, 1);
  lcd.print("Press JS to exit");
}
void play() {
  unsigned long currentTime = millis();
  if (currentTime - updatePlayTime >= 5000 && nr < 2) {
    updatePlayTime = currentTime;
    lvl += 1;
    score = lvl * 3;
    nr += 1;
  }
  if (nr != 2)
    showPlayPhase();
  else {
    if (iterNrPhase == 0) {
      lcdContent = true;
      clearLCD();
    }
    score -= 3;
    iterNrPhase += 1;
    if (addr == 0) {
      EEPROM.write(addr, score);
      addr = 1;
    }
    else {
      highScore = EEPROM.read(addr - 1);
      if (score > highScore)
        EEPROM.write(addr - 1, score);
    }
    showEndGame();
    if (checkButtonPressed() == true) {
      lives = 3;
      score = 3;
      lvl = 1;
      nr = 0;
    }
    lastSwitchValue = switchValue;
  }
}
void showHighScore() {
  lcd.setCursor(0, 0);
  lcd.print("High Score: ");
  lcd.setCursor(13, 0);
  lcd.print(EEPROM.read(0));
  lcd.setCursor(0, 1);
  lcd.print("Press JS to exit");
}
void HighScorePhase() {
  if (iter == 0) {
    lcdContent = true;
    clearLCD();
  }
  showHighScore();
  checkButtonPressed();
  lastSwitchValue = switchValue;
  iter += 1;
}
void showSettings() {
  lcd.setCursor(1, 0);
  lcd.print("Name: ");
  lcd.setCursor(7, 0);
  lcd.print("Alex");
  lcd.setCursor(1, 1);
  lcd.print("Start level: ");
  lcd.setCursor(14, 1);
  lcd.print(lvl);
}
void drawCursorBlinking() {
  if (iter3 == 0) {
     cursorBlinkingTime = millis();
  }
  cursorShowTime = millis();
  if (cursorShowTime - cursorBlinkingTime >= 1000) {
    cursorBlinkingTime = cursorShowTime;
    phaseBlink = !phaseBlink;
  }
  if (phaseBlink == false) {
    lcd.setCursor(cursorStateSettingsOX[cursorSettingsPosition], cursorStateSettingsOY[cursorSettingsPosition]);
    lcd.print(" ");
  }
  else {
    drawCursorSettings();
  }
}
void Settings() {
  if (iter == 0) {
    lcdContent = true;
    clearLCD();
    cursorSettingsPosition = 0;
    joyMoved = false;
  }
  showSettings();
  if (switchValue == 1 && lastSwitchValue == 0 && cursorSettingsPosition == 1) {
    buttonPhase = !buttonPhase;
    joyMoved = false;
  }
  if (buttonPhase == false) {
    drawCursorSettings();
    if (yValue > thresholdMax && joyMoved == false) {
      deleteBox(cursorStateSettingsOX[cursorSettingsPosition], cursorStateSettingsOY[cursorSettingsPosition]);
      cursorSettingsPosition = 0;
      joyMoved = true;
    }
    if (yValue < thresholdMin && joyMoved == false) {
      deleteBox(cursorStateSettingsOX[cursorSettingsPosition], cursorStateSettingsOY[cursorSettingsPosition]);
      cursorSettingsPosition = 1;
      joyMoved = true;
    }
    if (yValue >= thresholdMin && yValue <= thresholdMax) {
      joyMoved = false;
    }
    if (xValue < thresholdMin) {
      joyMoved = true;
      openPhaseState = !openPhaseState;
      lastSwitchValue = 0;
    }
    iter3 = 0;
  }
  else {
    drawCursorBlinking();
    if (yValue > thresholdMax && joyMoved == false) {
      lvl += 1;
      if (lvl > 9) {
        lvl = 9;
      score = 3 * lvl;
      }
      joyMoved = true;
    }
    if (yValue < thresholdMin && joyMoved == false) {
      lvl -= 1;
      if (lvl < 1) {
        lvl = 1;
      }
      score = 3 * lvl;
      joyMoved = true;
    }
    if (yValue >= thresholdMin && yValue <= thresholdMax) {
      joyMoved = false;
    }
    iter3 += 1;
  }
  iter += 1;
  lastSwitchValue = switchValue;
}
void setup() {
  pinMode(pinSW, INPUT_PULLUP);
  lcd.begin(16, 2);
  Serial.begin(9600);

}
void loop() {
  switchValue = !digitalRead(pinSW);
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  if (openPhaseState == true) {
    if (iter1 == 0) {
      lcdContent = true;
      clearLCD();
    }
    showOpenPhase();
    checkCursorPosition();
    checkJoyReturn();
    drawCursor();
    checkButtonPressed();
    lastSwitchValue = switchValue;
    iter1 += 1;
    iter2 = 0;
    iter = 0;
    iterNrPhase = 0;
  }
  else {
    if (iter2 == 0)  {
      lcdContent = true;
      clearLCD();
    }
    if (cursorPosition == 0) {
      if (iter == 0) {
        updatePlayTime = millis();
      }
      play();
      iter += 1;
    }
    if (cursorPosition == 1) {
      HighScorePhase();
    }
    if (cursorPosition == 2) {
      Settings();
    }
    iter2 += 1;
    iter1 = 0;
  }
}
