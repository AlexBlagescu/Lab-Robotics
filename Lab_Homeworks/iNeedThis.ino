#include "LedControl.h"
LedControl lc = LedControl(12, 11, 10, 1);
const int pinSW = 9; // digital pin connected to switch output
const int pinX = A0; // A0 - analog pin connected to X output
const int pinY = A1; // A1 - analog pin connected to Y output
const int maxNumberBullets = 10;
const int shotDelay = 400;
const int monsterDelay = 400;
const int nrLevel = 4;
const int monstersPerLevel = 10;
int lives = 3;
int gameState = 1;
int currentLevel = 0;
int countMonsters = 0;
int timeMonsterCreate = 0;
int lastSwitchValue = 0;
int xValue = 0;
int yValue = 0;
int minThreshold = 400;
int maxThreshold = 600;
int switchValue = 0;
bool joyMoved = false;
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
  {0, 7}, {1, 7}, {2, 7}, {1, 6}
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
    monsters[currentLevel][countMonsters][0].y = 1;
    monsters[currentLevel][countMonsters][0].x = randNumber - 1;
    monsters[currentLevel][countMonsters][0].t = millis();
    monsters[currentLevel][countMonsters][1].y = 1;
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
    if (monsters[currentLevel][j][0].y == 7 && monsters[currentLevel][j][1].y == 7) {
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
          (monsters[currentLevel][j][0].y == 7 && monsters[currentLevel][j][1].y == 7)) {
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
void setup() {
  pinMode(pinSW, INPUT_PULLUP);
  lc.shutdown(0, false);
  lc.setIntensity(0, 2);
  lc.clearDisplay(0);
  Serial.begin(9600);
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
  while (gameState) {
    Serial.println(lives);
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
      gameState = !gameState;
      break;
    }
    else if (checkGameOver() == false && countMonsters == 10) {
      happyFace();
      gameState = !gameState;
      break;
    }
    lastSwitchValue = switchValue;
  }
}
