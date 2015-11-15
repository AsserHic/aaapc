/*
 This is the default application for Asser's Arduino and Raspberry Pi case.

 by: Marko Laakso
 */

#define ARR_LENGTH(x)  (sizeof(x) / sizeof((x)[0]))

// Arduino pins:
const int BUILT_IN_LED = 13;

const int SR_CLOCK  = 10; // Shift register: clock
const int SR_LATCH  =  9; // Shift register: latch
const int SR_SERIAL =  8; // Shift register: serial output

byte shiftRegister[] = { B00000000 };

// Virtual pins:
const int VP_LED1 = 1;
const int VP_LED2 = 2;

/*
 This routine is executed once for the beginning.
 */
void setup() {

  pinMode(BUILT_IN_LED, OUTPUT);
  pinMode(SR_CLOCK,     OUTPUT);
  pinMode(SR_LATCH,     OUTPUT);
  pinMode(SR_SERIAL,    OUTPUT);

  // Activate serial port with the given boud rate
  Serial.begin(9600);
  
}

/*
 This function is called in an infinite loop forever.
 */
void loop() {
  vpSet(VP_LED1, HIGH);
  vpSetF(VP_LED2, LOW);
  delay(1000);
  vpSet(VP_LED1, LOW);
  vpSetF(VP_LED2, HIGH);
  delay(1000);
}

/*
 Assign given state to the virtual pin.
 */
inline void vpSet(int pin, boolean value) {
  int  reg = pin / 8;
  int  pos = pin % 8;
  byte regValue;

  if (value) {
     regValue = (shiftRegister[reg] |= 1 << pos);
  } else {
     regValue = (shiftRegister[reg] &= ~(1 << pos));
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

void vpFlush() {
  digitalWrite(SR_LATCH, LOW);
  for (int reg = ARR_LENGTH(shiftRegister)-1; reg >= 0; reg++) {
    shiftOut(SR_SERIAL, SR_CLOCK, MSBFIRST, shiftRegister[reg]);
  }
  digitalWrite(SR_LATCH, HIGH);
}
