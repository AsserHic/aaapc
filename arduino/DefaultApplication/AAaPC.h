// Asser's Arduino and Raspberry Pi case
// AAaPC.h

#ifndef HEADER_AAAPC
  #define HEADER_AAAPC

#include "Arduino.h"

#define ARR_LENGTH(x)  (sizeof(x) / sizeof((x)[0]))

// Arduino pins:
// INPUTS
const int LIGHT_SENSORS   = A5; // Photo resistors
const int HUMAN_DETECTOR  = A4; // IR move detector
const int JOYSTICK_BUTTON = A3; // Press
const int JOYSTICK_X      = A1; // Horizontal position
const int JOYSTICK_Y      = A2; // Vertical position
const int THERMOMETER     = A0; // LM35 temperature sensor
const int US_DIST_ECHO    =  8; // Ultrasonic HC-SR04 distance sensor

// OUTPUTS
const int US_DIST_TRIG    =  2; // Ultrasonic HC-SR04 distance sensor
const int LED_RGB_RED     = 10; // PWM brightness
const int LED_RGB_GREEN   =  9; // PWM brightness
const int LED_RGB_BLUE    =  6; // PWM brightness
const int PASSIVE_BUZZLER = 11; // PWM sound pitch

const int SR_CLOCK  =  4; // Shift register: clock
const int SR_LATCH  =  7; // Shift register: latch
const int SR_SERIAL = 12; // Shift register: serial output

// Virtual pins:
const int VP_LED_WHITE     = 0;
const int VP_LED_YELLOW    = 8;
const int VP_LED_BLUE      = 16;
const int VP_LIGHT_SENSOR1 = 1;
const int VP_LIGHT_SENSOR2 = 2;
// SMA420564 7-segment 4-digit display
const int VP_DISPLAY_POS_1 = 20;
const int VP_DISPLAY_POS_2 = 21;
const int VP_DISPLAY_POS_3 = 22;
const int VP_DISPLAY_POS_4 = 23;
const int VP_DISPLAY_SEG_A = 11; //  Display segments:
const int VP_DISPLAY_SEG_B = 19; //    AAAA
const int VP_DISPLAY_SEG_C = 17; //  FF    BB
const int VP_DISPLAY_SEG_D = 14; //  FF    BB
const int VP_DISPLAY_SEG_E = 13; //    GGGG
const int VP_DISPLAY_SEG_F = 12; //  EE    CC
const int VP_DISPLAY_SEG_G = 18; //  EE    CC
const int VP_DISPLAY_SEG_P = 15; //    DDDD    PP
const int DISPLAY_POSITIONS[] = {
  VP_DISPLAY_POS_1, VP_DISPLAY_POS_2, VP_DISPLAY_POS_3, VP_DISPLAY_POS_4
};
const int DISPLAY_SEGMENTS[] = {
  VP_DISPLAY_SEG_A,
  VP_DISPLAY_SEG_B,
  VP_DISPLAY_SEG_C,
  VP_DISPLAY_SEG_D,
  VP_DISPLAY_SEG_E,
  VP_DISPLAY_SEG_F,
  VP_DISPLAY_SEG_G,
  VP_DISPLAY_SEG_P
};

/*
 This routine is executed once for the beginning and must be definied in each application locally.
 */
void custom_setup();

/*
 Assign given state to the virtual pin.
 */
void vpSet(int pin, boolean value);

/*
 Assign given state to the virtual pin and flush.
 */
void vpSetF(int pin, boolean value);

/*
 Update current values to all virtual pins.
 */
void vpFlush();

/*
 Assign a new color to the RGB led.
 */
void setRGBLed (byte red, byte green, byte blue);

/*
 Read value of the light sensor.
 */
int readLightSensor(int pin);

/*
 Read distance (cm) from the ultrasonic sensor.
 */
int readDistance();

/*
 Send an error message to the user.
 */
void writeError(char* message);

/*
 Returns the current horizontal position of the joystick between -100 and 100.
 */
int getJoystickX();

/*
 Returns the current vertical position of the joystick between -100 and 100.
 */
int getJoystickY();

/*
 Tells if the joystick button is down.
 */
boolean isJoystickPressed();

/*
 Read temperature as Celcius degrees.
 */
float readTemperature();

/*
 Returns the requested value from the comma separated list.
 */
String getValue(String data, int index);

#endif
