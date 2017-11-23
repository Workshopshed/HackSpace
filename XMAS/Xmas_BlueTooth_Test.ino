int sequence;

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
  sequence = 0;
}

void loop() { 
  for (int ledpin=0; ledpin <= 4; ledpin++){
      digitalWrite(ledpin + 2, pattern[sequence][ledpin]);
   }
  
  delay(200);
  
  sequence = (sequence + 1) % 4;
}




