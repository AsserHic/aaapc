/*
 This is the default application for Asser's Arduino and Raspberry Pi case.

 by: Marko Laakso
 */

#define ARR_LENGTH(x)  (sizeof(x) / sizeof((x)[0]))

// Arduino pins:
// INPUTS
const int LIGHT_SENSOR   = A5; // Photo resistor
const int HUMAN_DETECTOR = A4; // IR move detector
const int THERMOMETER    = A0; // LM35 temperature sensor
const int US_DIST_ECHO   =  8; // Ultrasonic HC-SR04 distance sensor

// OUTPUTS
const int US_DIST_TRIG    =  2; // Ultrasonic HC-SR04 distance sensor
const int LED_RGB_RED     = 10; // PWM brightness
const int LED_RGB_GREEN   =  9; // PWM brightness
const int LED_RGB_BLUE    =  6; // PWM brightness
const int PASSIVE_BUZZLER = 11; // PWM sound pitch

const int SR_CLOCK  =  4; // Shift register: clock
const int SR_LATCH  =  7; // Shift register: latch
const int SR_SERIAL = 12; // Shift register: serial output

// 1 times 74HC595 shift register
byte shiftRegister[] = { B00000000, B00000000, B00000000 };

// Virtual pins:
const int VP_LED_WHITE     = 0;
const int VP_LED_YELLOW    = 8;
const int VP_LED_BLUE      = 16;
const int VP_LIGHT_SENSOR1 = 1;
const int VP_LIGHT_SENSOR2 = 2;

const int VP_DISPLAY_POS_1 =  3;
const int VP_DISPLAY_POS_2 =  3;
const int VP_DISPLAY_POS_3 =  3;
const int VP_DISPLAY_POS_4 =  3;
const int VP_DISPLAY_SEG_A =  3; //  Display segments:
const int VP_DISPLAY_SEG_B =  3; //    AAAA
const int VP_DISPLAY_SEG_C =  3; //  FF    BB
const int VP_DISPLAY_SEG_D =  3; //  FF    BB
const int VP_DISPLAY_SEG_E =  3; //    GGGG
const int VP_DISPLAY_SEG_F =  3; //  EE    CC
const int VP_DISPLAY_SEG_G =  3; //  EE    CC
const int VP_DISPLAY_SEG_P =  3; //    DDDD    PP
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
 This routine is executed once for the beginning.
 */
void setup() {
  pinMode(LED_BUILTIN,   OUTPUT);
  pinMode(LED_RGB_RED,   OUTPUT);
  pinMode(LED_RGB_GREEN, OUTPUT);
  pinMode(LED_RGB_BLUE,  OUTPUT);
  pinMode(SR_CLOCK,      OUTPUT);
  pinMode(SR_LATCH,      OUTPUT);
  pinMode(SR_SERIAL,     OUTPUT);
  pinMode(US_DIST_TRIG,  OUTPUT);

  pinMode(US_DIST_ECHO,   INPUT);
  pinMode(LIGHT_SENSOR,   INPUT);
  pinMode(HUMAN_DETECTOR, INPUT);
  pinMode(THERMOMETER,    INPUT);

  // Activate serial port with the given boud rate
  Serial.begin(9600);

  // Reset all virtual pins
  vpFlush();
}

/*
 This function is called in an infinite loop forever.
 */
void loop() {
  //tone(PASSIVE_BUZZLER, 600, 100);

  digitalWrite(LED_BUILTIN, digitalRead(HUMAN_DETECTOR));

  setRGBLed(200,   0,   0); delay(500);
  setRGBLed(0,   200,   0); delay(500);
  setRGBLed(0,     0, 200); delay(500);
  setRGBLed(0,     0,   0);

  vpSetF(VP_LED_WHITE,  true); delay(500); vpSet(VP_LED_WHITE,  false);
  vpSetF(VP_LED_YELLOW, true); delay(500); vpSet(VP_LED_YELLOW, false);
  vpSetF(VP_LED_BLUE,   true); delay(500); vpSet(VP_LED_BLUE,   false);
  vpFlush();

  for (int p=0; p<ARR_LENGTH(DISPLAY_POSITIONS); p++) {
    vpSet(DISPLAY_POSITIONS[p], true);
    for (int s=0; s<ARR_LENGTH(DISPLAY_SEGMENTS); s++) {
      vpSetF(DISPLAY_SEGMENTS[s], true); delay(500); vpSet(DISPLAY_SEGMENTS[s], false);
    }
    vpSet(DISPLAY_POSITIONS[p], false);
  }

  /*
  Serial.print(readLightSensor(VP_LIGHT_SENSOR1));
  Serial.print(F(", "));
  Serial.print(readLightSensor(VP_LIGHT_SENSOR2));
  Serial.print(F(", "));
  Serial.println(readLightSensor(VP_LIGHT_SENSOR3));
  */

  //Serial.println(readDistance());
  //Serial.println(readTemperature());
}

/*
 Assign given state to the virtual pin.
 */
inline void vpSet(int pin, boolean value) {
  int reg = pin / 8;
  int pos = pin % 8;

  if (value) {
     bitSet(shiftRegister[reg], pos);
  } else {
     bitClear(shiftRegister[reg], pos);
  }
}

/*
 Assign given state to the virtual pin and flush.
 */
void vpSetF(int pin, boolean value) {
  vpSet(pin, value);
  vpFlush();
}

/*
 Update current values to all virtual pins.
 */
void vpFlush() {
  digitalWrite(SR_LATCH, LOW);
  for (int reg = ARR_LENGTH(shiftRegister)-1; reg >= 0; reg--) {
    shiftOut(SR_SERIAL, SR_CLOCK, MSBFIRST, shiftRegister[reg]);
  }
  digitalWrite(SR_LATCH, HIGH);
}

/*
 Assign a new color to the RGB led.
 */
void setRGBLed (byte red, byte green, byte blue) {
  analogWrite(LED_RGB_RED,   red);
  analogWrite(LED_RGB_GREEN, green);
  analogWrite(LED_RGB_BLUE,  blue);
}

/*
 Read value of the light sensor.
 */
int readLightSensor(int pin) {
  vpSetF(pin, true);
  delay(15); // Wait for a stable current on the photo resistor.
  int value = analogRead(LIGHT_SENSOR);
  vpSetF(pin, false); // Make sure that we do not leak connections to the shared sink
  return value;
}

/*
 Read temperature as Celcius degrees.
 */
float readTemperature() {
  long  measure = analogRead(THERMOMETER);
  float temperature;

  temperature = measure * 0.48828125;
  return temperature;
}

/*
 Read distance (cm) from the ultrasonic sensor.
 */
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

/*
 Send an error message to the user.
 */
void writeError(char* message) {
  Serial.println(message);
  tone(PASSIVE_BUZZLER, 400, 500);
}
