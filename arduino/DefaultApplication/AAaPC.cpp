// Asser's Arduino and Raspberry Pi case
// AAaPC.cpp

#include "Arduino.h"
#include "AAaPC.h"

// 3 times 74HC595 shift register
byte shiftRegister[] = { B00000000, B00000000, B00000000 };

/*
  This initialization is called by the Arduino framework.
 */
void setup() {
  pinMode(LED_BUILTIN,     OUTPUT);
  pinMode(LED_RGB_RED,     OUTPUT);
  pinMode(LED_RGB_GREEN,   OUTPUT);
  pinMode(LED_RGB_BLUE,    OUTPUT);
  pinMode(SR_CLOCK,        OUTPUT);
  pinMode(SR_LATCH,        OUTPUT);
  pinMode(SR_SERIAL,       OUTPUT);
  pinMode(US_DIST_TRIG,    OUTPUT);
  pinMode(PASSIVE_BUZZLER, OUTPUT);

  pinMode(US_DIST_ECHO,    INPUT);
  pinMode(LIGHT_SENSORS,   INPUT);
  pinMode(HUMAN_DETECTOR,  INPUT);
  pinMode(THERMOMETER,     INPUT);
  pinMode(JOYSTICK_X,      INPUT);
  pinMode(JOYSTICK_Y,      INPUT);
  pinMode(JOYSTICK_BUTTON, INPUT_PULLUP);

  // Reset all virtual pins
  vpFlush();

  // Activate serial port with the given boud rate
  Serial.begin(9600);

  custom_setup(); // Application specific initialization
}

void vpSet(int pin, boolean value) {
  int reg = pin / 8;
  int pos = pin % 8;

  if (value) {
     bitSet(shiftRegister[reg], pos);
  } else {
     bitClear(shiftRegister[reg], pos);
  }
}

void vpSetF(int pin, boolean value) {
  vpSet(pin, value);
  vpFlush();
}

void vpFlush() {
  digitalWrite(SR_LATCH, LOW);
  for (int reg = ARR_LENGTH(shiftRegister)-1; reg >= 0; reg--) {
    shiftOut(SR_SERIAL, SR_CLOCK, MSBFIRST, shiftRegister[reg]);
  }
  digitalWrite(SR_LATCH, HIGH);
}

void setRGBLed (byte red, byte green, byte blue) {
  red   = constrain(red,   0, 255);
  green = constrain(green, 0, 255);
  blue  = constrain(blue,  0, 255);
  analogWrite(LED_RGB_RED,   red);
  analogWrite(LED_RGB_GREEN, green);
  analogWrite(LED_RGB_BLUE,  blue);
}

int readLightSensor(int pin) {
  vpSetF(pin, true);

  int value = -1;
  for (int test=0; test<100; test++) {
      delay(10); // Wait for a stable current on the photo resistor.
      int new_value = analogRead(LIGHT_SENSORS);
      int diff      = abs(new_value-value);

      value = new_value;
      if (diff < 2) break;
  }
  vpSetF(pin, false); // Make sure that we do not leak connections to the shared sink
  return value;
}

int readDistance() {
  long measure;

  digitalWrite(US_DIST_TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(US_DIST_TRIG, HIGH);
  delayMicroseconds(10);

  digitalWrite(US_DIST_TRIG, LOW);
  measure = pulseIn(US_DIST_ECHO, HIGH);

  measure /= 58.2; // as cm
  if (measure < 0 || measure > 200) {
    measure = 0;
  }
  return (int)measure;
}

float readTemperature() {
  long  measure = analogRead(THERMOMETER);
  float temperature;

  temperature = measure * 0.48828125;
  return temperature;
}

int joystickPosition(int pin, int midpoint) {
  const int TOLERANCE = 1;

  int value = 1023-analogRead(pin);
  if (value >= midpoint-TOLERANCE && value <= midpoint+TOLERANCE) {
     return 0;
  }
  if (value < midpoint) {
      value = map(value, 0, midpoint-TOLERANCE, -100, 0);
  } else {
      value = map(value, midpoint+TOLERANCE, 1023, 0, 100);
  }
  return value;
}

int getJoystickX() { return joystickPosition(JOYSTICK_X, 506); }

int getJoystickY() { return joystickPosition(JOYSTICK_Y, 515); }

boolean isJoystickPressed() {
  return !digitalRead(JOYSTICK_BUTTON);
}

