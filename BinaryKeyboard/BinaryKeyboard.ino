#include <Keyboard.h>

int keyCount;
char keyPressed;
char keyCharacter;

void setup()
{ 
  pinMode(5, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  
  Keyboard.begin();
  
  attachInterrupt(digitalPinToInterrupt(5), key0, FALLING);
  attachInterrupt(digitalPinToInterrupt(7), key1, FALLING); 
}

void key1()
{
  keyPressed = '1';
}

void key0()
{
  keyPressed = '0';
}

void loop()
{
  if (keyPressed == '0') {
     	Keyboard.print('0');
    	keyCount += 1;
        keyCharacter = keyCharacter << 1;
  }
  if (keyPressed == '1') {
     	Keyboard.print('1');
        keyCount += 1;
        keyCharacter = keyCharacter << 1;
        keyCharacter = keyCharacter | 1;
  }
  if (keyCount == 8) {
    for(int i=0;i<8;i++) {
      Keyboard.write(KEY_BACKSPACE);
    }
    Keyboard.print(keyCharacter);
    keyCount = 0;
    keyCharacter  = 0;
  }
 
  keyPressed = ' ';
  delay(100);
}
