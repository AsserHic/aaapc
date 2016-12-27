// Asser's Arduino and Raspberry Pi case
// FourDigitDisplay.h

#ifndef HEADER_FOURDIGITDISPLAY
  #define HEADER_FOURDIGITDISPLAY

const byte DISPLAY_VALUE_VOID = B00000000;
const byte DISPLAY_VALUE_0    = B11111100;
const byte DISPLAY_VALUE_1    = B00000110;
const byte DISPLAY_VALUE_2    = B01011011;
const byte DISPLAY_VALUE_3    = B01001111;
const byte DISPLAY_VALUE_4    = B01100110;
const byte DISPLAY_VALUE_5    = B10110110;
const byte DISPLAY_VALUE_6    = B10111110;
const byte DISPLAY_VALUE_7    = B11100000;
const byte DISPLAY_VALUE_8    = B11111110;
const byte DISPLAY_VALUE_9    = B11110110;

class FourDigitDisplay {

  private:
    enum { digits = 4 };
    byte    m_values[digits] = { DISPLAY_VALUE_VOID,
                                 DISPLAY_VALUE_VOID,
                                 DISPLAY_VALUE_VOID,
                                 DISPLAY_VALUE_VOID };
    int     m_phase;
    boolean m_enabled;

    void disable_if_empty();

  public:
    FourDigitDisplay();

    void set_enabled(boolean value);

    void set_value(int pos, byte value);

    void advance();

};

#endif
