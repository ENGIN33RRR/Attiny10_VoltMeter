
uint8_t AD;
uint16_t VOLT;

const uint8_t Init[24] = {
  0xAE,         // Display OFF
  0xA8, 0x1F,   // set multiplex (HEIGHT-1): 0x1F for 128x32, 0x3F for 128x64
  0x22, 0x00, 0x03, // Page min to max
  0x20, 0x01,   // Memory addressing mode 0x00 Horizontal 0x01 Vertical
  0xDA, 0x02,   // Set COM Pins hardware configuration to sequential
  0x8D, 0x14,   // Charge pump enabled
  0xD3, 0x00,   // Display offset to 0
  0x81, 0xFF,   // Set contrast
  0xD9, 0xF1,   // Set pre-charge period
  0xDB, 0x40,   // Set vcom detect
  0x21, 0x00, 0x7F, // Column min to max
  0xAF,  // Display on

};

#define PI2C_SDA    PB0
#define PI2C_SCL    PB1

#define OUT_REG PORTB


#define SDA_ON (OUT_REG |= (1<< PI2C_SDA))
#define SDA_OFF (OUT_REG &= ~(1<< PI2C_SDA))
#define SCL_ON (OUT_REG |= (1<< PI2C_SCL))
#define SCL_OFF (OUT_REG &= ~(1<< PI2C_SCL))

#define SDA_READ (PINB & (1<<PI2C_SDA))



#define ADDR 0b01111000 //OLED Address plus write bit



inline void dly() { //пустая команда
  __asm__("NOP");
};




void setup () {
  ADMUX = 2 << MUX0;              // ADC1 (PB1)
  ADCSRA = 1 << ADEN | 3 << ADPS0; // Enable ADC, 125kHz clock

  DDRB = 3;
  for (uint8_t i = 0; i < 100; i++) dly();
  
  start();
  Tx(ADDR);
  Tx(0x00);
  for (uint8_t i = 0; i < 24; i++)
  {
    Tx(Init[i]);
  }

  stop();

}





void loop(void) {
  uint8_t buffer[8] = {0, 0, 10, 0, 0, 11, 12, 12}; //знаковый буффер на 8 ячеек.
  ADCSRA = ADCSRA | 1 << ADSC;    // Start
  while (ADCSRA & 1 << ADSC);     // Wait while conversion in progress
  AD = ADCL;                    //Читаем АЦП

  VOLT = (AD *47)/10;  //Преобразуем значение АЦП в вольты

 
    buffer[0] = VOLT / 1000;  //первый знак
    buffer[1] = (VOLT % 1000) / 100; //второй знак
    buffer[3] = (VOLT % 100) / 10; //третий знак
    buffer[4] = VOLT % 10;  //четвертый знак

  

  OLED_printB(buffer);  //Выводим буффер


}
