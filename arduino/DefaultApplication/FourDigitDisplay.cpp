// Asser's Arduino and Raspberry Pi case
// FourDigitDisplay.cpp

#include "Arduino.h"
#include "AAaPC.h"
#include "FourDigitDisplay.h"

FourDigitDisplay::FourDigitDisplay() {
  m_phase = 0;
}

void FourDigitDisplay::set_value(int pos, byte value) {
  m_values[pos] = value;
}

void FourDigitDisplay::advance() {
  vpSet(DISPLAY_POSITIONS[m_phase], false);
  m_phase = (m_phase+1) % 4;

  byte value = m_values[m_phase];
  for (int seg=0; seg<8; seg++) {
    vpSet(DISPLAY_SEGMENTS[seg], bitRead(value, seg));
  }

  vpSetF(DISPLAY_POSITIONS[m_phase], true);
}
