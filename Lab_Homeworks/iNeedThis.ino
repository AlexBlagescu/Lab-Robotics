#include "LedControl.h"
#include <LiquidCrystal.h>
#include <LiquidMenu.h>
LedControl lc = LedControl(12, 11, 10, 1);
const byte LCD_RS = 13;
const byte LCD_E = 6;
const byte LCD_D4 = 5;
const byte LCD_D5 = 4;
const byte LCD_D6 = 3;
const byte LCD_D7 = 2;
const int pinSW = 9; // digital pin connected to switch output
const int pinX = A0; // A0 - analog pin connected to X output
const int pinY = A1; // A1 - analog pin connected to Y output
const int maxNumberBullets = 10;
const int shotDelay = 200;
const int monsterDelay = 400;
const int nrLevel = 4;
const int monstersPerLevel = 10;
const int screenUpdateDelay = 60;
int currentScreen = 0;
int highScore = 0;
int lives = 3;
int score = 0;
int currentLevel = 0;
int countMonsters = 0;
unsigned long timeMonsterCreate = 0;
unsigned long timeScreenUpdate = 0;
int lastSwitchValue = 0, lastSwitchValueMenu = 0;
int xValue = 0, xValueMenu = 0;
int yValue = 0, yValueMenu = 0;
int minThreshold = 400;
int maxThreshold = 600;
int switchValue = 0, switchValueMenu = 0;
bool joyMoved = false, joyMovedMenu = false, changeScreen = false;

LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

////            WELCOME SCREEN
LiquidLine welcomeLine1(1, 0, "Welcome to ");
LiquidLine welcomeLine2(2, 1, "Line Invaders!");

LiquidScreen welcomeScreen(welcomeLine1, welcomeLine2);

////            SECOND SCREEN
LiquidLine playGameLine(3, 0, "Play Game");
LiquidLine enterLine(2, 1, "Enter -> JS");

LiquidScreen secondaryScreen(playGameLine, enterLine);

////            THIRD SCREEN
LiquidLine highScoreLine(3, 0, "High Score");

LiquidScreen thirdScreen(highScoreLine, enterLine);

//              FOURTH SCREEN

LiquidLine settingsLine(3, 0, "Settings");

LiquidScreen fourthScreen(settingsLine, enterLine);

//              FIFTH SCREEN

LiquidLine highScoreDisplayLine(0, 0, "High Score: ", highScore);
LiquidLine returnLine(0, 1, "Press JS to exit");

LiquidScreen fifthScreen(highScoreDisplayLine, returnLine);

//              SIXTH SCREEN

LiquidLine levelLine(3, 0, "Level: ", currentLevel);

LiquidScreen sixthScreen(levelLine, returnLine);

//              SEVENTH SCREEN

LiquidLine livesLevelLine(0, 0, "Lives: ", lives, " Lvl: ", currentLevel);
LiquidLine scoreLine(4, 1, "Score: ", score);

LiquidScreen seventhScreen(livesLevelLine, scoreLine);

LiquidMenu menu(lcd);

struct point {
  int x;
  int y;
};
struct bullet {
  int x;
  int y;
  unsigned long t;
};
point shipXY[4] = {
  {2, 7}, {3, 7}, {4, 7}, {3, 6}
};
bullet monsters[4][10][2];
bullet shotFirstWeapon[maxNumberBullets] = {
  { -1, -1, -1}, { -1, -1, -1}, { -1, -1, -1}, { -1, -1, -1}, { -1, -1, -1}, { -1, -1, -1}, { -1, -1, -1}, { -1, -1, -1}, { -1, -1, -1}, { -1, -1, -1}
};
bool matrix[8][8] = {
  {1, 0, 0, 0, 0, 0, 0, 1},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 0, 0, 0, 1}
};
void displayShip() {
  for (int i = 0; i < 4; i++)
    lc.setLed(0, shipXY[i].y, shipXY[i].x, true);
}
void removeShip() {
  for (int i = 0; i < 4; i++)
    lc.setLed(0, shipXY[i].y, shipXY[i].x, false);
}
void moveShip() {
  removeShip();
  if (xValue < minThreshold && joyMoved == false) {
    joyMoved = true;
    if (shipXY[0].x - 1 >= 0)
      for (int i = 0; i < 4; i++)
        shipXY[i].x -= 1;
  }
  if (xValue > maxThreshold && joyMoved == false) {
    joyMoved = true;
    if (shipXY[2].x + 1 <= 7)
      for (int i = 0; i < 4; i++)
        shipXY[i].x += 1;
  }
  if (xValue >= minThreshold && xValue <= maxThreshold) {
    joyMoved = false;
  }
}
bool checkButtonPressed() {
  if (switchValue == 1 && lastSwitchValue == 0)
    return true;
  return false;
}
void shotShip () {
  if (checkButtonPressed() == true) {
    bullet firstBullet;
    firstBullet.x = shipXY[3].x;
    firstBullet.y = shipXY[3].y - 1;
    firstBullet.t = millis();
    for (int i = 0; i < maxNumberBullets; i++)
      if (shotFirstWeapon[i].y == -1 && shotFirstWeapon[i].x == -1 && shotFirstWeapon[i].t == -1) {
        shotFirstWeapon[i] = firstBullet;
        break;
      }
  }
}
void showBullets() {
  for (int i = 0; i < maxNumberBullets; i++) {
    if (shotFirstWeapon[i].y != -1 && shotFirstWeapon[i].x != -1 && shotFirstWeapon[i].t != -1) {
      lc.setLed(0, shotFirstWeapon[i].y, shotFirstWeapon[i].x, true);
    }
    if (shotFirstWeapon[i].y == -1) {
      shotFirstWeapon[i].y = -1;
      shotFirstWeapon[i].x = -1;
      shotFirstWeapon[i].t = -1;
    }
  }
}
void updateBullets() {
  for (int i = 0; i < maxNumberBullets; i++)
    if (shotFirstWeapon[i].y != -1 && shotFirstWeapon[i].x != -1 && shotFirstWeapon[i].t != -1) {
      if (millis() - shotFirstWeapon[i].t >= shotDelay) {
        lc.setLed(0, shotFirstWeapon[i].y, shotFirstWeapon[i].x, false);
        shotFirstWeapon[i].y -= 1;
        shotFirstWeapon[i].t = millis();
      }
    }
}
void updateMonsters() {
  for (int j = 0; j < monstersPerLevel; j++) {
    if (monsters[currentLevel][j][0].y != -1 && monsters[currentLevel][j][0].x != -1
        && monsters[currentLevel][j][0].t != -1 && monsters[currentLevel][j][1].y != -1
        && monsters[currentLevel][j][1].x != -1 && monsters[currentLevel][j][1].t != -1) {
      if (millis() - monsters[currentLevel][j][0].t >= monsterDelay) {
        lc.setLed(0, monsters[currentLevel][j][0].y, monsters[currentLevel][j][0].x, false);
        lc.setLed(0, monsters[currentLevel][j][1].y, monsters[currentLevel][j][1].x, false);
        monsters[currentLevel][j][0].y += 1;
        monsters[currentLevel][j][1].y += 1;
        monsters[currentLevel][j][0].t = millis();
        monsters[currentLevel][j][1].t = monsters[currentLevel][j][0].t;
      }
    }
  }
}
void createMonster() {
  if (millis() - timeMonsterCreate >= 1000 && countMonsters < 10) {
    timeMonsterCreate = millis();
    int randNumber = random(1, 8);
    monsters[currentLevel][countMonsters][0].y = 0;
    monsters[currentLevel][countMonsters][0].x = randNumber - 1;
    monsters[currentLevel][countMonsters][0].t = millis();
    monsters[currentLevel][countMonsters][1].y = 0;
    monsters[currentLevel][countMonsters][1].x = randNumber;
    monsters[currentLevel][countMonsters][1].t = monsters[currentLevel][countMonsters][0].t;
    countMonsters += 1;
  }
}
void showMonster() {
  for (int j = 0; j < monstersPerLevel; j++) {
    if (monsters[currentLevel][j][0].y != -1 && monsters[currentLevel][j][0].x != -1
        && monsters[currentLevel][j][0].t != -1 && monsters[currentLevel][j][1].y != -1
        && monsters[currentLevel][j][1].x != -1 && monsters[currentLevel][j][1].t != -1) {
      lc.setLed(0, monsters[currentLevel][j][0].y, monsters[currentLevel][j][0].x, true);
      lc.setLed(0, monsters[currentLevel][j][1].y, monsters[currentLevel][j][1].x, true);
    }
    if (monsters[currentLevel][j][0].y == 8 && monsters[currentLevel][j][1].y == 8) {
      lc.setLed(0, monsters[currentLevel][j][0].y, monsters[currentLevel][j][0].x, false);
      lc.setLed(0, monsters[currentLevel][j][1].y, monsters[currentLevel][j][1].x, false);
      monsters[currentLevel][j][0].y = -1;
      monsters[currentLevel][j][0].x = -1;
      monsters[currentLevel][j][0].t = -1;
      monsters[currentLevel][j][1].y = -1;
      monsters[currentLevel][j][1].x = -1;
      monsters[currentLevel][j][1].t = -1;
    }
  }
}
void checkCollision() {
  for (int i = 0; i < maxNumberBullets; i++) {
    if (shotFirstWeapon[i].y != -1 && shotFirstWeapon[i].x != -1 && shotFirstWeapon[i].t != -1) {
      for (int j = 0; j < monstersPerLevel; j++) {
        if (monsters[currentLevel][j][0].y != -1 && monsters[currentLevel][j][0].x != -1
            && monsters[currentLevel][j][0].t != -1 && monsters[currentLevel][j][1].y != -1
            && monsters[currentLevel][j][1].x != -1 && monsters[currentLevel][j][1].t != -1) {
          if ((shotFirstWeapon[i].y == monsters[currentLevel][j][0].y && shotFirstWeapon[i].x == monsters[currentLevel][j][0].x) ||
              (shotFirstWeapon[i].y == monsters[currentLevel][j][1].y && shotFirstWeapon[i].x == monsters[currentLevel][j][1].x)) {
            score = score +  max(1.2 * monsters[currentLevel][j][0].y, 1.2 * monsters[currentLevel][j][1].y);
            menu.update();
            lc.setLed(0, monsters[currentLevel][j][0].y, monsters[currentLevel][j][0].x, false);
            lc.setLed(0, monsters[currentLevel][j][1].y, monsters[currentLevel][j][1].x, false);
            monsters[currentLevel][j][0].y = monsters[currentLevel][j][0].x = monsters[currentLevel][j][0].t = -1;
            monsters[currentLevel][j][1].y = monsters[currentLevel][j][1].x = monsters[currentLevel][j][1].t = -1;
            shotFirstWeapon[i].y = shotFirstWeapon[i].x = shotFirstWeapon[i].t = -1;
          }
        }
      }
    }
  }
}
void sadFace() {
  lc.clearDisplay(0);
  lc.setLed(0, 1, 1, true);
  lc.setLed(0, 1, 2, true);
  lc.setLed(0, 2, 1, true);
  lc.setLed(0, 2, 2, true);

  lc.setLed(0, 1, 5, true);
  lc.setLed(0, 1, 6, true);
  lc.setLed(0, 2, 5, true);
  lc.setLed(0, 2, 6, true);

  lc.setLed(0, 6, 1, true);
  lc.setLed(0, 5, 2, true);
  lc.setLed(0, 5, 3, true);
  lc.setLed(0, 5, 4, true);
  lc.setLed(0, 5, 5, true);
  lc.setLed(0, 6, 6, true);
}
bool checkGameOver() {
  for (int j = 0; j < monstersPerLevel; j++)
    if (monsters[currentLevel][j][0].y != -1 && monsters[currentLevel][j][0].x != -1
        && monsters[currentLevel][j][0].t != -1 && monsters[currentLevel][j][1].y != -1
        && monsters[currentLevel][j][1].x != -1 && monsters[currentLevel][j][1].t != -1) {

      if ((monsters[currentLevel][j][0].y == shipXY[3].y && monsters[currentLevel][j][0].x == shipXY[3].x) ||
          (monsters[currentLevel][j][1].y == shipXY[3].y && monsters[currentLevel][j][1].x == shipXY[3].x) ||
          (monsters[currentLevel][j][0].y == shipXY[2].y && monsters[currentLevel][j][0].x == shipXY[2].x) ||
          (monsters[currentLevel][j][1].y == shipXY[0].y && monsters[currentLevel][j][1].x == shipXY[0].x) ||
          (monsters[currentLevel][j][0].y == 8 && monsters[currentLevel][j][1].y == 8)) {
        lives -= 1;
        monsters[currentLevel][j][0].y = monsters[currentLevel][j][0].x = monsters[currentLevel][j][0].t = -1;
        monsters[currentLevel][j][1].y = monsters[currentLevel][j][0].x = monsters[currentLevel][j][0].t = -1;
      }
    }
  if (lives == 0)
    return true;
  return false;
}
void happyFace() {
  lc.clearDisplay(0);
  lc.setLed(0, 1, 1, true);
  lc.setLed(0, 1, 2, true);
  lc.setLed(0, 2, 1, true);
  lc.setLed(0, 2, 2, true);

  lc.setLed(0, 1, 5, true);
  lc.setLed(0, 1, 6, true);
  lc.setLed(0, 2, 5, true);
  lc.setLed(0, 2, 6, true);


  lc.setLed(0, 5, 1, true);
  lc.setLed(0, 6, 2, true);
  lc.setLed(0, 6, 3, true);
  lc.setLed(0, 6, 4, true);
  lc.setLed(0, 6, 5, true);
  lc.setLed(0, 5, 6, true);
}
bool checkMonstersAlive() {
  for (int j = 0; j < monstersPerLevel; j++) {
    if (monsters[currentLevel][j][0].y != -1 && monsters[currentLevel][j][0].x != -1
        && monsters[currentLevel][j][0].t != -1 && monsters[currentLevel][j][1].y != -1
        && monsters[currentLevel][j][1].x != -1 && monsters[currentLevel][j][1].t != -1)
      return true;
  }
  return false;
}
void playGame() {
  if (changeScreen == false) {
    changeScreen = true;
    menu.change_screen(7);
  }
  while (1) {
    switchValue = !digitalRead(pinSW);
    xValue = analogRead(pinX);
    yValue = analogRead(pinY);
    moveShip();
    displayShip();
    shotShip();
    showBullets();
    updateBullets();
    createMonster();
    showMonster();
    updateMonsters();
    checkCollision();
    if (checkGameOver() == true) {
      sadFace();
      menu.change_screen(2);
      delay(1000);
      lc.clearDisplay(0);
      return;
    }
    else if (checkGameOver() == false && checkMonstersAlive() == false && countMonsters == 10) {
      happyFace();
      menu.change_screen(2);
      delay(1000);
      lc.clearDisplay(0);
      return;
    }
    if (millis() - timeScreenUpdate >= 100) {
      timeScreenUpdate = millis();
      menu.update();
    }
    lastSwitchValue = switchValue;
  }
}
void highScoreFunction() {
  
}
void settingsFunction() {
  
}
void resetGame() {
  lives = 3;
  score = 0;
  countMonsters = 0;
  timeMonsterCreate = 0;
  lastSwitchValue = 0;
  timeScreenUpdate = 0;
  xValue = 0;
  yValue = 0;
  switchValue = 0;
  joyMoved = false;
  changeScreen = false;
  for (int i = 0; i < maxNumberBullets; i++) {
    shotFirstWeapon[i].y = -1;
    shotFirstWeapon[i].x = -1;
    shotFirstWeapon[i].t = -1;
  }
  for (int j = 0; j < monstersPerLevel; j++) {
    monsters[currentLevel][j][0].y = -1;
    monsters[currentLevel][j][0].x = -1;
    monsters[currentLevel][j][0].t = -1;
    monsters[currentLevel][j][1].y = -1;
    monsters[currentLevel][j][1].x = -1;
    monsters[currentLevel][j][1].t = -1;
  }
  shipXY[0].x = 2;
  shipXY[0].y = 7;
  shipXY[1].x = 3;
  shipXY[1].y = 7;
  shipXY[2].x = 4;
  shipXY[2].y = 7;
  shipXY[3].x = 3;
  shipXY[3].y = 6;
}
void setup() {
  pinMode(pinSW, INPUT_PULLUP);
  lc.shutdown(0, false);
  lc.setIntensity(0, 2);
  lc.clearDisplay(0);
  Serial.begin(9600);

  lcd.begin(16, 2);
  playGameLine.attach_function(0, playGame);
  highScoreLine.attach_function(1, highScoreFunction);
  settingsLine.attach_function(2, settingsFunction);
  menu.add_screen(welcomeScreen);
  menu.add_screen(secondaryScreen);
  menu.add_screen(thirdScreen);
  menu.add_screen(fourthScreen);
  menu.add_screen(fifthScreen);
  menu.add_screen(sixthScreen);
  menu.add_screen(seventhScreen);
  menu.update();

  randomSeed(analogRead(2));
  for (int i = 0; i < nrLevel; i++)
    for (int j = 0; j < monstersPerLevel; j++) {
      monsters[i][j][0].y = -1;
      monsters[i][j][0].x = -1;
      monsters[i][j][0].t = -1;
      monsters[i][j][1].y = -1;
      monsters[i][j][1].x = -1;
      monsters[i][j][1].t = -1;
    }
}
void loop() {
  if (currentScreen == 0) {
    delay(2000);
    menu.next_screen();
    menu.set_focusedLine(0);
    currentScreen += 1;
  }
  switchValueMenu = !digitalRead(pinSW);
  xValueMenu = analogRead(pinX);
  yValueMenu = analogRead(pinY);
  if (yValueMenu < minThreshold && joyMovedMenu == false) {
    joyMovedMenu = true;
    currentScreen -= 1;
    if (currentScreen < 1) {
      currentScreen = 1;
    }
    else {
      menu.previous_screen();
      menu.set_focusedLine(0);
    }
  }
  if (yValueMenu > maxThreshold && joyMovedMenu == false) {
    joyMovedMenu = true;
    currentScreen += 1;
    if (currentScreen > 3) {
      currentScreen = 3;
    }
    else {
      menu.next_screen();
      menu.set_focusedLine(0);
    }
  }
  if (yValueMenu >= minThreshold && yValueMenu <= maxThreshold) {
    joyMovedMenu = false;
  }
  if (switchValueMenu == 1 && lastSwitchValueMenu == 0 && currentScreen == 1) {
    delay(500);
    menu.call_function(0);
    resetGame();
  }
  lastSwitchValueMenu = switchValueMenu;
}
