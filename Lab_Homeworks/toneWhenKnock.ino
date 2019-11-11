const int knockSensor = A0; 
const int pushButton = 2;
const int buzzerPin = 8;
const int toneTime = 5000;
const int toneFrequency = 1000;

int sensorReading = 0;     
int buttonState = 0;
int timeSensorDetectKnock = -1;
int currentTime = 0;
int buttonValue = 0;

void setup() {
  Serial.begin(9600);       
  pinMode(pushButton, INPUT_PULLUP);
}

void loop() {
  currentTime = millis();
  sensorReading = analogRead(knockSensor);
  buttonState = digitalRead(pushButton);
  buttonValue = !buttonState;
  if (sensorReading > 0 && timeSensorDetectKnock == -1) {
    timeSensorDetectKnock = millis();
    Serial.println("HERE");
  }
  if (currentTime - timeSensorDetectKnock >= toneTime && timeSensorDetectKnock != -1) {
    tone(buzzerPin, toneFrequency);
  }
  if (buttonValue == HIGH && timeSensorDetectKnock != -1) {
    noTone(buzzerPin);
    timeSensorDetectKnock = -1;
  }
}
