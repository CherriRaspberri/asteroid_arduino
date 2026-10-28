#include <Arduino.h>
#include <Chrono.h>
#include <Bounce2.h>
#include <MicroOscSlip.h>

// Variables instances
Chrono chronoPot;
Bounce2::Button bouton = Bounce2::Button();
MicroOscSlip<128> monOsc(&Serial);

void setup()
{
    // Serial start
    Serial.begin(115200);

    // LED setup
    pinMode(3, OUTPUT);
}

void loop()
{
    // Checks for incoming OSC messages
    monOsc.onOscMessageReceived(traiterMessageOsc);

    // Checks if chrono has passed 20 milliseconds
    // If so, read the analog value of pin 3 and send it via OSC
    if (chronoPot.hasPassed(20))
    {
        chronoPot.restart();
        // Value varies from 0 to 1023
        int value = analogRead(3);
        monOsc.sendInt("/photo", value);
    }
}

// Function to handle incoming OSC messages
void traiterMessageOsc(MicroOscMessage &message)
{
    // Check if the message address is "/lumiere" and the type tag is "i" (integer)
    if (message.checkOscAddressAndTypeTags("/lumiere", "i"))
    {
        // Fetches the integer value
        int value = message.nextAsInt();
        // Sets the LED brightness based on the received value
        digitalWrite(3, value);
    }
}