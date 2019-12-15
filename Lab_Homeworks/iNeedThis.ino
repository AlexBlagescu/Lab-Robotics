// Ship direction:
// 0 - N
// 1 - E
// 2 - S
// 3 - V
#include "LedControl.h"
LedControl lc = LedControl(12, 11, 10, 1);
const int pinSW = 9; // digital pin connected to switch output
const int pinX = A0; // A0 - analog pin connected to X output
const int pinY = A1; // A1 - analog pin connected to Y output
int lastSwitchValue = 0;
int xValue = 0;
int yValue = 0;
int shotState = 0;
int minThreshold = 400;
int maxThreshold = 600;
int switchValue = 0;
int nrBullets = 0;
bool joyMoved = false;
struct point {
  int x;
  int y;
};
point shipXY[4] = {
  {0, 7}, {1, 7}, {2, 7}, {1, 6}
};
point shotFristWeapon[5];
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
void setup() {
  pinMode(pinSW, INPUT_PULLUP);
  lc.shutdown(0, false);
  lc.setIntensity(0, 2);
  lc.clearDisplay(0);
  Serial.begin(9600);
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
    shotState = !shotState;
  }
  if (shotState == 1) {
    point firstFirePosition;
    firstFirePosition.x = shipXY[3].x;
    firstFirePosition.y = shipXY[3].y - 1;
    if (nrBullets < 5) {
      shipXY[nrBullets].x = firstFirePosition.x;
      shipXY[nrBullets].y = firstFirePosition.y;
      nrBullets += 1;
      //for (int i = nrBullets; i< 
    }
  }
}
void loop() {
  switchValue = !digitalRead(pinSW);
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
//  moveShip();
//  displayShip();
//  
  shotShip();
  lastSwitchValue = switchValue;
}
