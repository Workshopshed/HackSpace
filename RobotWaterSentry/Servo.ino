#include <Wire.h>

const int slaveAddress = 8;

const int sensePin = A0;
const int drivePin1 = 3;
const int drivePin2 = 4;
const int speedPin = 5; //Needs to support PWM

short registers[5];
int readCmd;
int blink = 0;

enum readRegisters { Target = 0,
                     Running = 1,
                     Position = 2,
                     Speed = 3,
                     Direction = 4
                   };

enum commands { CmdStop = 1,
                CmdAngle = 2,
                CmdSpeed = 3,
              };

void setup() {
  stop();

  pinMode(drivePin1, OUTPUT);
  pinMode(drivePin2, OUTPUT);
  pinMode(speedPin, OUTPUT);
  pinMode(13, OUTPUT);        //Onboard LED
  pinMode(sensePin, INPUT);

  Wire.begin(slaveAddress);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // write data
  Wire.onRequest(requestEvent); // requests to read data

}

void loop() {
  digitalWrite(13, blink);
  blink = !blink;  
  
  registers[Position] = analogRead(sensePin);

  if (registers[Running]) {
    if (registers[Direction] == 1) {
      if (registers[Position] >= registers[Target]) {
        stop();
      }
    }
    if (registers[Direction] == -1) {
      if (registers[Position] <= registers[Target]) {
        stop();
      }
    }
  }
}

void stop() {
  digitalWrite(drivePin1, LOW);
  digitalWrite(drivePin2, LOW);
  analogWrite(speedPin, 0);
  registers[Running] = false;
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int bytesReceived) {
  char command;
  int value;
  int lowB;
  int highB;

  switch (bytesReceived) {
    case 1:
      //Read request register
      readCmd = Wire.read();
      break;
    case 3:
      command = Wire.read(); // receive byte as a character
      //Pi is low endian
      lowB = Wire.read();
      highB = Wire.read();
      value = (highB << 8) | lowB;

      switch (command) {
        case CmdStop:
          stop();
          break;
        case CmdAngle:
          registers[Target] = value;
          registers[Running] = true;
          if (registers[Target] > registers[Position]) {
            digitalWrite(drivePin1, LOW);
            digitalWrite(drivePin2, HIGH);
            registers[Direction] = 1;
          }
          else {
            digitalWrite(drivePin2, LOW);
            digitalWrite(drivePin1, HIGH);            
            registers[Direction] = -1;
          }
          break;
        case CmdSpeed:
          registers[Speed] = value;
          analogWrite(speedPin, registers[Speed]);
          break;

      }
      break;
    default:
      Serial.println("Invalid command");
      for (int a = 0; a < bytesReceived; a++) {
        Wire.read();  // throw buffer away so we can read again
      }
  }
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
// Don't add serial print to this routine, it will error!!

void requestEvent() {

  short value = registers[readCmd];
  Wire.write((uint8_t *)&value, sizeof(value));
//  Serial.print("ReadRequest=");
//  Serial.print(value);
}
