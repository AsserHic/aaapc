/*
 This is the default application for Asser's Arduino and Raspberry Pi case.

 by: Marko Laakso
 */

#define ARR_LENGTH(x)  (sizeof(x) / sizeof((x)[0]))

// Arduino pins:
// INPUTS
const int LIGHT_SENSOR = A5; // Photo resistor

// OUTPUTS
const int LED_RGB_RED     = 10; // PWM brightness
const int LED_RGB_GREEN   =  9; // PWM brightness
const int LED_RGB_BLUE    =  6; // PWM brightness
const int PASSIVE_BUZZLER = 11; // PWM sound pitch

const int SR_CLOCK  =  4; // Shift register: clock
const int SR_LATCH  =  7; // Shift register: latch
const int SR_SERIAL = 12; // Shift register: serial output

// 3 times 74HC595 shift register
byte shiftRegister[] = { B00000000,
                         B00000000,
                         B00000000 };

// Virtual pins:
const int VP_LED_WHITE     = 0;
const int VP_LIGHT_SENSOR1 = 1;
const int VP_LIGHT_SENSOR2 = 2;
const int VP_LIGHT_SENSOR3 = 3;

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

  pinMode(LIGHT_SENSOR, INPUT);

  // Activate serial port with the given boud rate
  Serial.begin(9600);

  // Reset all virtual pins
  vpFlush();
}

/*
 This function is called in an infinite loop forever.
 */
void loop() {
  setRGBLed(200,   0,   0); delay(500);
  setRGBLed(0,   200,   0); delay(500);
  setRGBLed(0,     0, 200); delay(500);
  setRGBLed(0,     0,   0);

  vpSetF(VP_LED_WHITE, true);
  delay(500);
  Serial.print(readLightSensor(VP_LIGHT_SENSOR1));
  Serial.print(F(", "));
  Serial.print(readLightSensor(VP_LIGHT_SENSOR2));
  Serial.print(F(", "));
  Serial.println(readLightSensor(VP_LIGHT_SENSOR3));
  vpSetF(VP_LED_WHITE, false);
}

/*
 Assign given state to the virtual pin.
 */
inline void vpSet(int pin, boolean value) {
  int  reg = pin / 8;
  int  pos = pin % 8;

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
  digitalWrite(SR_CLOCK, LOW);
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
 Send an error message to the user.
 */
void writeError(char* message) {
  Serial.println(message);
  tone(PASSIVE_BUZZLER, 400, 500);
}
