// Asser's Arduino and Raspberry Pi case
// FourDigitDisplay.h

#ifndef HEADER_FOURDIGITDISPLAY
  #define HEADER_FOURDIGITDISPLAY

const byte DISPLAY_VALUE_VOID = B00000000;
const byte DISPLAY_VALUE_0    = B00111111;
const byte DISPLAY_VALUE_1    = B00000110;
const byte DISPLAY_VALUE_2    = B01011011;
const byte DISPLAY_VALUE_3    = B01001111;
const byte DISPLAY_VALUE_4    = B01100110;
const byte DISPLAY_VALUE_5    = B01101101;
const byte DISPLAY_VALUE_6    = B01111101;
const byte DISPLAY_VALUE_7    = B00000111;
const byte DISPLAY_VALUE_8    = B01111111;
const byte DISPLAY_VALUE_9    = B01101111;

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

    void clear();

    void advance();

};

#endif
