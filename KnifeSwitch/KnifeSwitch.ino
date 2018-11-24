#include "Pimote.h"
Pimote socket;
byte buttonState = 0;
volatile byte triggered = 0;

void setup()
{
  pinMode(2, INPUT_PULLUP);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), switchChange, CHANGE);

  socket.setESD0(A0);    // set Encoded Signal D0
  socket.setESD1(A1);    // set Encoder Signal D1
  socket.setESD2(A2);    // set Encoder Signal D2
  socket.setESD3(A3);    // set Encoder Signal D3
  socket.setMODSEL(A4);  // set MODSEL mode select signal (OOK/FSK)
  socket.setCE(A5); // set CE modular enable (Output ON/OFF)

  socket.begin();
}

void switchChange() {
  triggered = 1;
}

void loop()
{
  if (triggered == 1) {
    delay(5);
    buttonState = digitalRead(2);
    arc();
    if (buttonState == 1) {
        socket.on(1);
    } else {
           socket.off(1);
    }
    triggered = 0;
  }
}

void arc() {
  for (int i = 0; i < 100; i++) {
    int o = random(2); // Random number from 0 to 1
    digitalWrite(4,o);
    float t = ((float)i*(float)i)/1000 + 120;
    tone(3, t ,4);    
    delay(5);
  }
  digitalWrite(4,buttonState);
}

