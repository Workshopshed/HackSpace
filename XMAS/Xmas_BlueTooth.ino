#include <SoftwareSerial.h>

bool running;
int speed;
int sequence;
// software serial RX = digital pin 7, TX = digital pin 8
SoftwareSerial BTserial(7, 8);

//Pattern of LED outputs
unsigned char pattern[4][4] = { {HIGH,LOW,LOW,LOW},
                    		   {LOW,HIGH,LOW,LOW},
							   {LOW,LOW,HIGH,LOW},
							   {LOW,LOW,LOW,HIGH}
                   };

void setup() { 
  // Configure all of the pins that control the LEDs
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  // Initial variable values
  speed = 200;
  running = true;
  sequence = 0;
  // Configure serial port for BlueTooth module
  BTserial.begin(9600);
}

void loop() { 
  if (BTserial.available() > 0) {
      char command = BTserial.read();
      switch (command) {
        case 'S':
          if (speed < 1000) {
              speed += 20;
          }
          running = true;
          break;
        case 'F':
          if (speed > 50) {
              speed -= 20;
          }
          running = true;
          break;
        case 'H':
          running = !running;
          break;
    }
  }

  if (running) {
  //Write the pattern to the LEDs
  for (int ledpin=0; ledpin <= 4; ledpin++){
      digitalWrite(ledpin + 2, pattern[sequence][ledpin]);
   }
   delay(speed);
   sequence = (sequence + 1) % 4;
  }
}




