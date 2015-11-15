/*
 This is the default application for Asser's Arduino and Raspberry Pi case.

 by: Marko Laakso
 */

#define ARR_LENGTH(x)  (sizeof(x) / sizeof((x)[0]))

// Arduino pins:
// INPUTS
const int LIGHT_SENSOR = A5; // Photo resistor

// OUTPUTS
const int LED_RGB_RED     =  3; // PWM brightness
const int LED_RGB_GREEN   =  5; // PWM brightness
const int LED_RGB_BLUE    =  6; // PWM brightness
const int PASSIVE_BUZZLER = 11; // PWM sound pitch

const int SR_CLOCK  = 10; // Shift register: clock
const int SR_LATCH  =  9; // Shift register: latch
const int SR_SERIAL =  8; // Shift register: serial output

byte shiftRegister[] = { B00000000 };

// Virtual pins:
const int VP_LED_WHITE     = 0;
const int VP_LED_YELLOW    = 1;
const int VP_LED_RED       = 2;
const int VP_LIGHT_SENSOR1 = 5;
const int VP_LIGHT_SENSOR2 = 6;
const int VP_LIGHT_SENSOR3 = 7;

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
}

/*
 This function is called in an infinite loop forever.
 */
void loop() {
  vpSet(VP_LED_WHITE, HIGH);
  vpSetF(VP_LED_YELLOW, LOW);
  delay(1000);
  vpSet(VP_LED_WHITE, LOW);
  vpSetF(VP_LED_YELLOW, HIGH);
  delay(1000);
}

/*
 Assign given state to the virtual pin.
 */
inline void vpSet(int pin, boolean value) {
  int  reg = pin / 7;
  int  pos = pin % 7;
  byte regValue;

  if (value) {
     regValue = bitSet(shiftRegister[reg], pos);
  } else {
     regValue = bitClear(shiftRegister[reg], pos);
  }
  shiftRegister[reg] = regValue;
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
  for (int reg = ARR_LENGTH(shiftRegister)-1; reg >= 0; reg++) {
    shiftOut(SR_SERIAL, SR_CLOCK, MSBFIRST, shiftRegister[reg]);
    //Serial.print(reg);Serial.print(F(" = "));Serial.println(shiftRegister[reg], BIN);
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
  int o1, o2;

  // Make sure that we read one sensor at the time since they have a shared input.
  switch (pin) {
    case VP_LIGHT_SENSOR1:
         vpSet(VP_LIGHT_SENSOR2, false); vpSet(VP_LIGHT_SENSOR3, false);
         break;
    case VP_LIGHT_SENSOR2:
         vpSet(VP_LIGHT_SENSOR1, false); vpSet(VP_LIGHT_SENSOR3, false);
         break;
    case VP_LIGHT_SENSOR3:
         vpSet(VP_LIGHT_SENSOR1, false); vpSet(VP_LIGHT_SENSOR2, false);
         break;
    default: Serial.println("ERROR: invalid light sensor pin");
  }
  vpSetF(pin, true);
  delay(10); // Wait for a stable current on the photo resistor.

  return analogRead(LIGHT_SENSOR);
}
