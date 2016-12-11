/*
 This is the default application for Asser's Arduino and Raspberry Pi case.

 by: Marko Laakso
 */

#include "AAaPC.h"

const int OPER_ERROR         = 1;
const int OPER_RGB_LED       = 10;
const int OPER_BUZZLER       = 15;
const int OPER_LED_BLUE      = 21;
const int OPER_LED_WHITE     = 22;
const int OPER_LED_YELLOW    = 23;
const int OPER_LIGHT_SENSOR1 = 30;
const int OPER_LIGHT_SENSOR2 = 31;
const int OPER_TEMPERATURE   = 33;
const int OPER_DISTANCE      = 35;
const int OPER_JOYSTICK      = 38;

int     joystickX      = 0;
int     joystickY      = 0;
boolean joystickButton = false;

/*
 This routine is executed once for the beginning.
 */
void custom_setup() {
  for (int pitch=100; pitch<1000; pitch += 100) {
     tone(PASSIVE_BUZZLER, pitch, 100);
  }
}

/*
 This function is called in an infinite loop forever.
 */
void loop() {
  while (Serial.available() > 0) {
    int inputChar = Serial.read();
    if (inputChar != '*') {
      continue;
    }

    int op_id = Serial.parseInt();

    switch (op_id) {
      case OPER_RGB_LED:
        setRGBLed(Serial.parseInt(),  // Red
                  Serial.parseInt(),  // Green
                  Serial.parseInt()); // Blue
        break;
      case OPER_LED_BLUE:
        vpSetF(VP_LED_BLUE, 1 == Serial.parseInt());
        break;
      case OPER_LED_WHITE:
        vpSetF(VP_LED_WHITE, 1 == Serial.parseInt());
        break;
      case OPER_LED_YELLOW:
        vpSetF(VP_LED_YELLOW, 1 == Serial.parseInt());
        break;
      case OPER_LIGHT_SENSOR1:
        writeOperation(op_id);
        Serial.println(readLightSensor(VP_LIGHT_SENSOR1));
        break;
      case OPER_LIGHT_SENSOR2:
        writeOperation(op_id);
        Serial.println(readLightSensor(VP_LIGHT_SENSOR2));
        break;
      case OPER_TEMPERATURE:
        writeOperation(op_id);
        Serial.println(readTemperature());
        break;
      case OPER_DISTANCE:
        writeOperation(op_id);
        Serial.println(readDistance());
        break;
      case OPER_BUZZLER:
        tone(PASSIVE_BUZZLER,
             Serial.parseInt(),
             Serial.parseInt());
        break;
      default:
        writeError("Invalid operation");
    }
  }

  updateJoystickStatus(false);

  adjust();
}

void adjust() {
  digitalWrite(LED_BUILTIN, digitalRead(HUMAN_DETECTOR));

  for (int p=0; p<ARR_LENGTH(DISPLAY_POSITIONS); p++) {
    vpSet(DISPLAY_POSITIONS[p], true);
    for (int s=0; s<ARR_LENGTH(DISPLAY_SEGMENTS); s++) {
      vpSetF(DISPLAY_SEGMENTS[s], true); delay(150); vpSet(DISPLAY_SEGMENTS[s], false);
    }
    vpSet(DISPLAY_POSITIONS[p], false);
  }
  vpFlush();
}

boolean updateJoystickStatus(boolean forceSubmit) {
  int     currentX      = getJoystickX();
  int     currentY      = getJoystickX();
  boolean currentButton = isJoystickPressed();
  boolean changed       = (joystickX      != currentX) ||
                          (joystickY      != currentY) ||
                          (joystickButton != currentButton);
  if (changed) {
     joystickX      = currentX;
     joystickY      = currentY;
     joystickButton = currentButton;
  }
  if (changed || forceSubmit) {
     writeOperation(OPER_JOYSTICK);
     Serial.print(currentX);
     Serial.print(",");
     Serial.print(currentY);
     Serial.print(",");
     Serial.println(currentButton ? 1 : 0);
  }
}

/*
 Send an error message to the user.
 */
void writeError(char* message) {
  writeOperation(OPER_ERROR);
  Serial.println(message);
  tone(PASSIVE_BUZZLER, 400, 500);
}

void writeOperation(int op_id) {
  Serial.print("*");
  Serial.print(op_id);
  Serial.print(":");
}
