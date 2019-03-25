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

int lumiPin = A0;
const static uint8_t RADIO_ID = 3;             // Our radio's id.
const static uint8_t DESTINATION_RADIO_ID = 2; // Id of the radio we will transmit to.
const static uint8_t PIN_RADIO_CE = 6;
const static uint8_t PIN_RADIO_CSN = 10;

struct RadioPacket
{
    int parents = 0;
};

struct toSend.
{
    int SwitchOff = 0;
};

NRFLite _radio;
RadioPacket _radioData;
toSend _sender;

void setup()
{
    Serial.begin(9600);


    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);

          if (!_radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN))
    {
        Serial.println("Cannot communicate with radio");
    }

}

void loop()
{
  
    int lumiVal = analogRead(lumiPin);
    lumiVal = 50;
    
    Serial.println(lumiVal);
    
    if(lumiVal >= 200){
      
       //Light on
      _radioData.parents = 1;
      _radio.send(DESTINATION_RADIO_ID, &_radioData, sizeof(_radioData)); // Note how '&' must be placed in front of the variable name.
       Serial.println(_radioData.parents);
      
}else{
  
    while (_radio.hasData())
    {
        _radio.readData(&_sender);
        String msg = "";
        msg += _sender.SwitchOff;
        Serial.println(msg);
        if(_sender.SwitchOff == 1){
        digitalWrite(4, LOW);
        }
    }
    
  _radioData.parents = 0;
  
   }
}
