// Asser's Arduino and Raspberry Pi case
// FourDigitDisplay.cpp

#include "Arduino.h"
#include "AAaPC.h"
#include "FourDigitDisplay.h"

FourDigitDisplay::FourDigitDisplay() {
  m_phase   = 0;
  m_enabled = false;
}

void FourDigitDisplay::disable_if_empty() {
  boolean is_empty = true;

  for (int i=0; i<ARR_LENGTH(m_values); i++) {
    if (m_values[i] != DISPLAY_VALUE_VOID) {
       is_empty = false;
       break;
    }
  }
  if (is_empty) m_enabled = false;
}

void FourDigitDisplay::set_enabled(boolean value) {
  m_enabled = value;
  disable_if_empty();
}

void FourDigitDisplay::set_value(int pos, byte value) {
  m_values[pos] = value;
  disable_if_empty();
}

void FourDigitDisplay::advance() {
  if (!m_enabled) return;

  vpSet(DISPLAY_POSITIONS[m_phase], false);
  m_phase = (m_phase+1) % 4;

  byte value = m_values[m_phase];
  if (value == DISPLAY_VALUE_VOID) {
    vpSetF(DISPLAY_POSITIONS[m_phase], false);
  } else {
    for (int seg=0; seg<8; seg++) {
      vpSet(DISPLAY_SEGMENTS[seg], bitRead(value, seg));
    }
    vpSetF(DISPLAY_POSITIONS[m_phase], true);
  }
}
