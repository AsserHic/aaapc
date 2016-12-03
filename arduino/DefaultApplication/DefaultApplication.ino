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

/*
 This function is called in an infinite loop forever.
 */
void loop() {
  //tone(PASSIVE_BUZZLER, 600, 50);

  digitalWrite(LED_BUILTIN, digitalRead(HUMAN_DETECTOR));

  setRGBLed(200,   0,   0); delay(500);
  setRGBLed(0,   200,   0); delay(500);
  setRGBLed(0,     0, 200); delay(500);
  setRGBLed(0,     0,   0);

  vpSetF(VP_LED_WHITE,  true); delay(500); vpSet(VP_LED_WHITE,  false);
  vpSetF(VP_LED_YELLOW, true); delay(500); vpSet(VP_LED_YELLOW, false);
  vpSetF(VP_LED_BLUE,   true); delay(500); vpSet(VP_LED_BLUE,   false);
  //vpFlush();

  for (int p=0; p<ARR_LENGTH(DISPLAY_POSITIONS); p++) {
    vpSet(DISPLAY_POSITIONS[p], true);
    for (int s=0; s<ARR_LENGTH(DISPLAY_SEGMENTS); s++) {
      vpSetF(DISPLAY_SEGMENTS[s], true); delay(200); vpSet(DISPLAY_SEGMENTS[s], false);
    }
    vpSet(DISPLAY_POSITIONS[p], false);
  }
  vpFlush();

  /*
  Serial.print(readLightSensor(VP_LIGHT_SENSOR1));
  Serial.print(F(", "));
  Serial.println(readLightSensor(VP_LIGHT_SENSOR2));
  */

  //Serial.println(readDistance());
  //Serial.println(readTemperature());
}
