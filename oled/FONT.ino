const uint8_t OLED_FONT[] PROGMEM = {
  0x7F, 0x41, 0x7F, // 0  0
  0x00, 0x00, 0x7F, // 1  1
  0x79, 0x49, 0x4F, // 2  2
  0x41, 0x49, 0x7F, // 3  3
  0x0F, 0x08, 0x7E, // 4  4
  0x4F, 0x49, 0x79, // 5  5
  0x7F, 0x49, 0x79, // 6  6
  0x03, 0x01, 0x7F, // 7  7
  0x7F, 0x49, 0x7F, // 8  8
  0x4F, 0x49, 0x7F, // 9  9
  0x00, 0x60, 0x00, // .  10
  0x1F, 0x78, 0x1F, // V  11
  0x00, 0x00, 0x00, // -  12
  
};



void OLED_printB(uint8_t *buffer) {
  start();
  Tx(ADDR);
  Tx(0x40);
  for (uint8_t i = 0; i < 8; i++) OLED_printD(buffer[i]); // print buffer
  stop();                          // stop transmission
  
}



uint8_t OLED_stretch(uint8_t b) {
  b  = ((b & 2) << 3) | (b & 1);          // split 2 LSB into the nibbles
  b |= b << 1;                            // double the bits
  b |= b << 2;                            // double them again = 4 times
  return b;                               // return the value
}


void OLED_printD(uint8_t ch) {
  uint8_t i, j, k, b;                     // loop variables
  uint8_t sb[4];                          // stretched character bytes
  ch += ch << 1;                          // calculate position of character in font array
  for (i = 8; i; i--) Tx(0x00);    // print spacing between characters
  for (i = 3; i; i--) {                   // font has 3 bytes per character
    b = OLED_FONT[ch++]; // read character byte
    for (j = 0; j < 4; j++, b >>= 2) sb[j] = OLED_stretch(b); // stretch 4 times
    j = 4; if (i == 2) j = 6;             // calculate x-stretch value
    while (j--) {                      // write several times (x-direction)
      for (k = 0; k < 4; k++) Tx(sb[k]); // the 4 stretched bytes (y-direction)
    }
  }
}
