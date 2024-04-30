#include <Arduino.h>
#line 1 "C:\\Projetos\\github\\SD\\Projeto 1\\Projeto 1.ino"
class DHT22_Sensor
{
    public:
        void getData();
        float getTemperature();
        float getHumidity();
    private:
        byte RH_high, RH_low, temp_high, temp_low, checksum;
        unsigned int hum, temp;
        byte read_DHT22_byte();
};
//----------------------------------------------------------------------

//========================================================================
// Public Methods
//========================================================================
void DHT22_Sensor::getData()
{
  /*start signal*/
  DDRB |= 0b01000000;             /*set data pin for o/p*/
  PORTB &= 0b10111111;           /*first send low pulse*/
  delayMicroseconds(1000);           /*for 1ms*/
  PORTB |= 0b01000000;          /*then send high pulse*/
  delayMicroseconds(40);             /*for 40us*/
  //-------------------------------------------------------------------
  /*response signal*/
  DDRB &= 0b10111111;              /*set data pin for i/p*/
  while((PINB & 0b01000000));          /*wait for DHT22 low pulse*/
  while(!(PINB & 0b01000000));         /*wait for DHT22 high pulse*/
  while((PINB & 0b01000000));          /*wait for DHT22 low pulse*/
  //-------------------------------------------------------------------
  /*read sensor data*/
  RH_high = read_DHT22_byte();       /*read high byte humidity*/
  RH_low = read_DHT22_byte();        /*read low byte humidity*/
  temp_high = read_DHT22_byte();     /*read high byte temp*/
  temp_low = read_DHT22_byte();      /*read low byte temp*/
  checksum = read_DHT22_byte();      /*read checksum*/
  //------------------------------------------------------------------
  hum  = (RH_high << 8) | RH_low;    /*get 16-bit value of humidity*/
  temp = (temp_high << 8) | temp_low;/*get 16-bit value of temp*/
}
//========================================================================
float DHT22_Sensor::getTemperature()
{
  if (temp > 800) {
    return -(((int)temp) - (1 << 15))/10.0f;
  }
  return(temp/10.0f);
}
//========================================================================
float DHT22_Sensor::getHumidity()
{
  return(float(hum)/10.0);
}
//========================================================================
// Private Method
//========================================================================
byte DHT22_Sensor::read_DHT22_byte()
{
  byte dataByte;
  for(byte i=0; i<8; i++)
  {
    while(!(PINB & 0b01000000));       /*detect data bit (high pulse)*/
    delayMicroseconds(50);
    //--------------------------------------------------------------------
    if((PINB & 0b01000000)) dataByte = (dataByte<<1)|(0x01);
    else dataByte = (dataByte<<1);   /*store 1 or 0 in dataByte*/
    //--------------------------------------------------------------------
    while((PINB & 0b01000000));        /*wait for DHT22 low pulse*/
  }
  return dataByte;
}

DHT22_Sensor sensor;

bool high_freq = false;
#line 87 "C:\\Projetos\\github\\SD\\Projeto 1\\Projeto 1.ino"
void alert_on();
#line 96 "C:\\Projetos\\github\\SD\\Projeto 1\\Projeto 1.ino"
void alert_off();
#line 102 "C:\\Projetos\\github\\SD\\Projeto 1\\Projeto 1.ino"
int main();
#line 77 "C:\\Projetos\\github\\SD\\Projeto 1\\Projeto 1.ino"
ISR (TIMER1_COMPA_vect) {
  if (high_freq) {
    TCCR0B = 0b00000100;
  } else {
    TCCR0B = 0b00000011;
  }
  high_freq = !high_freq;
}

bool alert_is_on = false;
void alert_on() {
  high_freq = true;
  TCCR0B = 0b00000011; // start with high frequency
  TCNT1 = 0; // reset counter
  TCCR0A  = 0b11000011; // turn on PWM
  TIMSK1 |= 0b00000010; // enable timer 1 interrupt
  alert_is_on = true;
}

void alert_off() {
  TCCR0A  = 0b00000011; // turn off PWM
  TIMSK1 &= 0b11111101; // disable timer 1 interrupt
  alert_is_on = false;
}

int main() {
  // TIFR0    --
  // TCNT0    --
  // OCR0A/B  --
  // TCCR0A/B --
  // TIMSK    --

  // inout
  DDRB  |= 0b10000000; // pwm 13 out

  // timer 0
  TCCR0A = 0b00000011; // inv with reg A
  TCCR0B = 0b00000011; // no prescaling
                       // fast pwm, TOP 0xFF
  OCR0A = 255; // initial P = 0%
  // P =(256 - OCR0A - 1)/256

  // timer 1
  TCCR1A = 0b00000000;
  TCCR1B = 0b00001101;

  OCR1A = 15624;

  TIMSK1 &= 0b11111101; // disable timer 1 interrupt
  sei();

  Serial.begin(9600);

  while(true) {
    // Get temperature and humidity readings
    sensor.getData();
    float temperature = sensor.getTemperature();
    float humidity = sensor.getHumidity();

    // Print the values to the Serial Monitor
    Serial.println(TCNT1);

    if (temperature > 40 && !alert_is_on) {
      alert_on();
    } else if (temperature <= 40 && alert_is_on) {
      alert_off();
    }
  }
}


