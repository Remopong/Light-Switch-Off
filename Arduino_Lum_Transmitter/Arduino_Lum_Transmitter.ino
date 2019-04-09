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
int pinOUT = 8;
int lumiPin = A0;
const static uint8_t RADIO_ID = 3;             // Our radio's id.
const static uint8_t DESTINATION_RADIO_ID = 2; // Id of the radio we will transmit to.
const static uint8_t PIN_RADIO_CE = 6;
const static uint8_t PIN_RADIO_CSN = 10;
int failedCount = 0;
int parents = 0;
struct receive{
  int SwitchOff;
};

NRFLite _radio;
receive _Received;

void setup()
{
  pinMode(pinOUT, OUTPUT);
  digitalWrite(pinOUT, LOW);
  Serial.begin(9600);
  if (!_radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN))
    Serial.println("Cannot communicate with radio");
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
    parents = 1;
    if (_radio.send(DESTINATION_RADIO_ID, &parents, sizeof(parents))) // Note how '&' must be placed in front of the variable name.
    {
      Serial.println(parents);
      failedCount = 0;
    } else {
      Serial.println("Failed");
      failedCount ++;
    }
  } else {
    parents = 0;
    if (_radio.send(DESTINATION_RADIO_ID, &parents, sizeof(parents))) // Note how '&' must be placed in front of the variable name.
    {
      Serial.println(parents);
      failedCount = 0;
    } else
    {
      Serial.println("Failed");
      failedCount ++;
    }
  }
  if (failedCount >= 2) {
    Serial.println("Inside");
    if (_radio.hasData()) {
      Serial.println("Receive");
      _radio.readData(&_Received);
      String msg = "";
      msg += _Received.SwitchOff;
      Serial.println(msg);
      if (_Received.SwitchOff == 1) {
        pinMode(pinOUT, INPUT);
        Serial.println("Action");
      }
    }
  }
}
