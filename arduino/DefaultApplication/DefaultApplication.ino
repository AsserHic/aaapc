/*
 This is the default application for Asser's Arduino and Raspberry Pi case.

 by: Marko Laakso
 */

#include "AAaPC.h"

/*
 This routine is executed once for the beginning.
 */
void custom_setup() {
  for (int pitch=100; pitch<1000; pitch += 100) {
     tone(PASSIVE_BUZZLER, pitch, 100);
  }
}

const String SWITCH_ON = "1";

/*
 This function is called in an infinite loop forever.
 */
void loop() {
  while (Serial.available() > 0) {
    String msg   = Serial.readStringUntil('\n');
    String op_id = msg.substring(0, 2);
    String args  = msg.substring(2);

    if (op_id == "Lx") {
       int r = getValue(args, 0).toInt();
       int g = getValue(args, 1).toInt();
       int b = getValue(args, 2).toInt();
       setRGBLed(r, g, b);
    } else
    if (op_id == "Lb") {
       vpSetF(VP_LED_BLUE, args == SWITCH_ON);
    } else
    if (op_id == "Lw") {
       vpSetF(VP_LED_WHITE, args == SWITCH_ON);
    } else
    if (op_id == "Ly") {
       vpSetF(VP_LED_YELLOW, args == SWITCH_ON);
    } else
    if (op_id == "RT") {
       Serial.print(op_id);
       Serial.println(readTemperature());
    } else
    if (op_id == "RD") {
       Serial.print(op_id);
       Serial.println(readDistance());
    } else
    if (op_id == "BZ") {
       int pitch    = getValue(args, 0).toInt();
       int duration = getValue(args, 1).toInt();
       tone(PASSIVE_BUZZLER, pitch, duration);
    }
  }

  digitalWrite(LED_BUILTIN, digitalRead(HUMAN_DETECTOR));

  for (int p=0; p<ARR_LENGTH(DISPLAY_POSITIONS); p++) {
    vpSet(DISPLAY_POSITIONS[p], true);
    for (int s=0; s<ARR_LENGTH(DISPLAY_SEGMENTS); s++) {
      vpSetF(DISPLAY_SEGMENTS[s], true); delay(150); vpSet(DISPLAY_SEGMENTS[s], false);
    }
    vpSet(DISPLAY_POSITIONS[p], false);
  }
  vpFlush();

  /*
  Serial.print(getJoystickX());
  Serial.print(F(", "));
  Serial.print(getJoystickY());
  Serial.print(F(", "));
  Serial.println(isJoystickPressed());

  Serial.print(readLightSensor(VP_LIGHT_SENSOR1));
  Serial.print(F(", "));
  Serial.println(readLightSensor(VP_LIGHT_SENSOR2));
  */
}
