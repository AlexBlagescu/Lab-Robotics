const int pinA = 12;
const int pinB = 8;
const int pinC = 5;
const int pinD = 3;
const int pinE = 2;
const int pinF = 11;
const int pinG = 6;
const int pinDP = A2;
const int pinD1 = 7;
const int pinD2 = 9;
const int pinD3 = 10;
const int pinD4 = 13;

const int pinSW = 4;
const int pinX = A0;
const int pinY = A1;

const int segSize = 8;
const int noOfDisplays = 4;
const int noOfDigits = 10;

const int thresholdMin = 400;
const int thresholdMax = 700;

int switchValue;
int xValue = 0;
int yValue = 0;
int state = 0;
int lastSwitchValue = 0;
int currentDigit = 0;
bool joyMoved = false;
int storeNumber[noOfDisplays] = { 0, 0, 0, 0};

int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

int digits[noOfDisplays] = {
  pinD1, pinD2, pinD3, pinD4
};
byte digitMatrix[noOfDigits][segSize - 1] = {
  
  {1, 1, 1, 1, 1, 1, 0}, 
  {0, 1, 1, 0, 0, 0, 0}, 
  {1, 1, 0, 1, 1, 0, 1}, 
  {1, 1, 1, 1, 0, 0, 1}, 
  {0, 1, 1, 0, 0, 1, 1}, 
  {1, 0, 1, 1, 0, 1, 1}, 
  {1, 0, 1, 1, 1, 1, 1}, 
  {1, 1, 1, 0, 0, 0, 0}, 
  {1, 1, 1, 1, 1, 1, 1}, 
  {1, 1, 1, 1, 0, 1, 1}
};

void displayNumber(byte digit, byte decimalPoint) {
  for (int i = 0; i < segSize - 1; i++) {
    digitalWrite(segments[i], digitMatrix[digit][i]);
  }
  // write the decimalPoint to DP pin
  digitalWrite(segments[segSize - 1], decimalPoint);
}

void showDigit(int num) {
  for (int i = 0; i < noOfDisplays; i++) {
    digitalWrite(digits[i], HIGH);
  }
  digitalWrite(digits[num], LOW);
}
void setup() {
  pinMode(pinSW, INPUT_PULLUP);
  for (int i = 0; i < segSize ; i++)
  {
    pinMode(segments[i], OUTPUT);
  }
  for (int i = 0; i < noOfDisplays; i++)
  {
    pinMode(digits[i], OUTPUT);
  }
  Serial.begin(9600);
}
void loop() {

  switchValue = !digitalRead(pinSW);
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);

  if (switchValue == 1 && lastSwitchValue == 0) {
    state = !state;
    joyMoved = 0;
  }
  if (state == 1 && xValue < thresholdMin && joyMoved == false) {
    currentDigit += 1;
    currentDigit = currentDigit % 4;
    joyMoved = true;
  }

  if (state == 1 && xValue > thresholdMax && joyMoved == false) {
    currentDigit -= 1;
    if (currentDigit == -1) {
      currentDigit = 3;
    }
    joyMoved = true;
  }
  if (xValue >= thresholdMin && xValue <= thresholdMax && state == 1) {
    joyMoved = false;
  }
  if (state == 1) {
    showDigit(currentDigit);
    digitalWrite(pinDP, HIGH);
  }

  if (state == 0 && yValue > thresholdMax && joyMoved == false) {
    storeNumber[currentDigit] = (storeNumber[currentDigit] + 1) % 10;
    joyMoved = true;
  }
  if (state == 0 && yValue < thresholdMin && joyMoved == false) {
    storeNumber[currentDigit] -= 1;
    if (storeNumber[currentDigit] == -1) {
      storeNumber[currentDigit] = 9;
    }
    joyMoved = true;
  }
  if (yValue >= thresholdMin && yValue <= thresholdMax && state == 0) {
    joyMoved = false;
  }
  for (int i = 0; i < noOfDisplays; i++) {
    showDigit(i);
    if (i != currentDigit)
      displayNumber(storeNumber[i], LOW);
    else
      displayNumber(storeNumber[i], HIGH);
    delay(5);
  }
  lastSwitchValue = switchValue;
}
