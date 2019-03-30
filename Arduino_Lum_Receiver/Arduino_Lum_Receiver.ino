/*

  Radio    Arduino
  CE    -> 9
  CSN   -> 10 (Hardware SPI SS)
  MOSI  -> 11 (Hardware SPI MOSI)
  MISO  -> 12 (Hardware SPI MISO)
  SCK   -> 13 (Hardware SPI SCK)
  IRQ   -> No connection
  VCC   -> No more than 3.6 volts
  GND   -> GND

*/
#include <SPI.h>
#include <NRFLite.h>
int in = 8;
int buttonState = 0;
const static uint8_t RADIO_ID = 2;       // Our radio's id.  The transmitter will send to this id.
const static uint8_t DESTINATION_RADIO_ID = 3;
const static uint8_t PIN_RADIO_CE = 6;
const static uint8_t PIN_RADIO_CSN = 10;
int parents;

typedef struct sender{
  int SwitchOff = 1;
}              _send;

NRFLite _radio;
_send toSend;

void setup()
{
  Serial.begin(9600);
  pinMode(in, INPUT_PULLUP);
  buttonState = LOW;
  if (!_radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN))
    Serial.println("Cannot communicate with radio");
}

void loop()
{
  buttonState = digitalRead(in);
  if (buttonState == 1) {
    toSend.SwitchOff = 0;
    while (_radio.hasData())
    {
      _radio.readData(&parents);
      Serial.print("Receiving = ");
      String msg = "";
      msg += parents;
      Serial.println(msg);
    }
  } else {
    toSend.SwitchOff = 1;
    Serial.println("Sending");
    _radio.send(DESTINATION_RADIO_ID, &toSend, sizeof(toSend));
  }
}
