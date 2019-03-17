/*

Demonstrates simple RX and TX operation.
Please read the notes in NRFLite.h for a description of all library features.

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
int out = 7;
const static uint8_t RADIO_ID = 2;       // Our radio's id.  The transmitter will send to this id.
const static uint8_t DESTINATION_RADIO_ID = 3;
const static uint8_t PIN_RADIO_CE = 6;
const static uint8_t PIN_RADIO_CSN = 10;

struct RadioPacket // Any packet up to 32 bytes can be sent.
{
    int parents;
};

struct toSend // Any packet up to 32 bytes can be sent.
{
    int SwitchOff = 1;
};

NRFLite _radio;
RadioPacket _radioData;
toSend _sender;

void setup()
{
    Serial.begin(9600);
    pinMode(out, OUTPUT);

    if (!_radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN))
    {
        Serial.println("Cannot communicate with radio");
    }
    
}

void loop()
{
//  delay(5000);
//  _radio.send(DESTINATION_RADIO_ID, &_sender, sizeof(_sender));
//  Serial.println("done");
    while (_radio.hasData())
    {
        _radio.readData(&_radioData);
        String msg = "";
        msg += _radioData.parents;
        Serial.println(msg);
        if(_radioData.parents == 1){
        digitalWrite(out, HIGH);
        }else{
          digitalWrite(out, LOW);
        }
    }
}
