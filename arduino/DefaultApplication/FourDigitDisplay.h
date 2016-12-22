// Asser's Arduino and Raspberry Pi case
// FourDigitDisplay.h

#ifndef HEADER_FOURDIGITDISPLAY
  #define HEADER_FOURDIGITDISPLAY

const byte DISPLAY_VALUE_1 = B01100000;
const byte DISPLAY_VALUE_2 = B11011010;
const byte DISPLAY_VALUE_3 = B11110010;
const byte DISPLAY_VALUE_4 = B01100110;

class FourDigitDisplay {

  private:
    enum { digits = 4 };
    byte  m_values[digits] = { B00000000, B00000000, B00000000, B00000000 };
    int   m_phase;

  public:
    FourDigitDisplay();
    
    void set_value(int pos, byte value);

    void advance();

};

#endif
