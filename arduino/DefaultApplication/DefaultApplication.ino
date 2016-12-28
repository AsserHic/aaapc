/*
 This is the default application for Asser's Arduino and Raspberry Pi case.

 by: Marko Laakso
 */

#include "AAaPC.h"
#include "FourDigitDisplay.h"

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
const int OPER_HUMAN_DETECT  = 36;
const int OPER_JOYSTICK      = 38;

const int DISPLAY_SEQUENCE[] = {
  DISPLAY_VALUE_VOID, DISPLAY_VALUE_VOID, DISPLAY_VALUE_VOID, DISPLAY_VALUE_VOID,
  DISPLAY_VALUE_0, DISPLAY_VALUE_1, DISPLAY_VALUE_2, DISPLAY_VALUE_3, DISPLAY_VALUE_4,
  DISPLAY_VALUE_5, DISPLAY_VALUE_6, DISPLAY_VALUE_7, DISPLAY_VALUE_8, DISPLAY_VALUE_9
};

FourDigitDisplay digDisplay;
int              phase          = 0;
boolean          human_present  = false;
int              joystickX      = 0;
int              joystickY      = 0;
boolean          joystickButton = false;

/*
 This routine is executed once for the beginning.
 */
void custom_setup() {
  phase = 0;

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

  advance();
}

void advance() {
  if (phase % 5 == 0) updateHumanDetectorStatus(false);
  if (phase % 4 == 0) updateJoystickStatus(false);

  if (phase % 100 == 0) {
    for (int dp=0; dp < 4; dp++) {
        digDisplay.set_value(dp, DISPLAY_SEQUENCE[(phase/100+dp) % ARR_LENGTH(DISPLAY_SEQUENCE)]);
    }
  }
  digDisplay.advance();

  phase = (phase+1) % 1400;
}

boolean updateHumanDetectorStatus(boolean forceSubmit) {
  boolean is_active = digitalRead(HUMAN_DETECTOR);
  boolean changed   = is_active != human_present;

  if (changed) {
     human_present = is_active;
     digitalWrite(LED_BUILTIN, is_active);
  }
  if (changed || forceSubmit) {
     writeOperation(OPER_HUMAN_DETECT);
     Serial.println(is_active ? 1 : 0);
  }
  return changed;
}

boolean updateJoystickStatus(boolean forceSubmit) {
  const int TOLERANCE     = 1;
  int       currentX      = getJoystickX();
  int       currentY      = getJoystickX();
  boolean   currentButton = isJoystickPressed();
  boolean   changed       = (abs(currentX-joystickY) > TOLERANCE) ||
                            (abs(currentX-joystickY) > TOLERANCE) ||
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
  return changed;
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
