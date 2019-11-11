const int analogInPinRed = A0, analogInPinGreen = A1, analogInPinBlue = A2;
const int analogOutPinRed = 9, analogOutPinGreen = 10, analogOutPinBlue = 11;

int sensorValueRed = 0, sensorValueGreen = 0, sensorValueBlue = 0;
int outputValueRed = 0, outputValueGreen = 0, outputValueBlue = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  sensorValueRed = analogRead(analogInPinRed);
  sensorValueGreen = analogRead(analogInPinGreen);
  sensorValueBlue = analogRead(analogInPinBlue);

  setColor(sensorValueRed, sensorValueGreen, sensorValueBlue);
}

void setColor (int red, int green, int blue) {
  outputValueRed = map(red, 0, 1023, 0, 255);
  analogWrite(analogOutPinRed, outputValueRed);

  outputValueGreen = map(green, 0, 1023, 0, 255);
  analogWrite(analogOutPinGreen, outputValueGreen);

  outputValueBlue = map(blue, 0, 1023, 0, 255);
  analogWrite(analogOutPinBlue, outputValueBlue);

  // print the results to the Serial Monitor:
  Serial.print("sensorRed = ");
  Serial.print(red);
  Serial.print("\t outputRed = ");
  Serial.println(outputValueRed);

  Serial.print("sensorGreen = ");
  Serial.print(green);
  Serial.print("\t outputGreen = ");
  Serial.println(outputValueGreen);

  Serial.print("sensorBlue = ");
  Serial.print(blue);
  Serial.print("\t outputBlue = ");
  Serial.println(outputValueBlue);
  Serial.println("\n");
}
