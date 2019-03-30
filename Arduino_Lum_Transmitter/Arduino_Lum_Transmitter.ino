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

int pinRelay = 8;

int lumiPin = A0;

const static uint8_t RADIO_ID = 3;             // Our radio's id.
const static uint8_t DESTINATION_RADIO_ID = 2; // Id of the radio we will transmit to.
const static uint8_t PIN_RADIO_CE = 6;
const static uint8_t PIN_RADIO_CSN = 10;

int failedCount = 0;

struct RadioPacket
{
  int parents = 0;
};

struct toSend
{
  int SwitchOff;
};

NRFLite _radio;
RadioPacket _radioData;
toSend _sender;

void setup()
{

  pinMode(pinRelay, OUTPUT);
  digitalWrite(pinRelay, HIGH);

  Serial.begin(9600);

  if (!_radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN))
  {
    Serial.println("Cannot communicate with radio");
  }

}

void loop()
{
  Serial.println(failedCount);
  delay(1000);

  int lumiVal = analogRead(lumiPin);
  lumiVal = 50;

  //Serial.println(lumiVal);


  if (lumiVal >= 200) {

    //Light on
    _radioData.parents = 1;
    if (_radio.send(DESTINATION_RADIO_ID, &_radioData, sizeof(_radioData))) // Note how '&' must be placed in front of the variable name.
    {
      Serial.println(_radioData.parents);
      failedCount = 0;
    } else
    {
      Serial.println("Failed");
      failedCount ++;
    }


  } else {
    _radioData.parents = 0;

    if (_radio.send(DESTINATION_RADIO_ID, &_radioData, sizeof(_radioData))) // Note how '&' must be placed in front of the variable name.
    {
      Serial.println(_radioData.parents);
      failedCount = 0;
    } else
    {
      Serial.println("Failed");
      failedCount ++;
    }
  }

  if (failedCount >= 10) {

    Serial.println("Inside");

    if (_radio.hasData()) {
      Serial.println("Receive");
      _radio.readData(&_sender);
      String msg = "";
      msg += _sender.SwitchOff;
      Serial.println(msg);

      if (_sender.SwitchOff == 1) {

        digitalWrite(pinRelay, LOW);

        Serial.println("Action");


      }
    }
  }
}
