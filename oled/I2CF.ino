
/*  i2c start sequence */
void start() {
  SDA_ON;
  dly();
  SCL_ON;
  dly();
  SDA_OFF;
  dly();
  SCL_OFF;
  dly();
}


/*  i2c stop sequence */
void stop() {
  SDA_OFF;
  dly();
  SCL_ON;
  dly();
  SDA_ON;
  dly();
}

/* Transmit 8 bit data to slave */
bool Tx(uint8_t dat) {
  for (uint8_t i = 0; i < 8; i++) {
    (dat & 0x80) ? SDA_ON : SDA_OFF;
    dat <<= 1;
    dly();
    SCL_ON;
    dly();
    SCL_OFF;
    
  }

  SDA_ON;
  SCL_ON;
  dly();
  bool ack = !SDA_READ;    // Acknowledge bit
  SCL_OFF;
  return ack;
}
