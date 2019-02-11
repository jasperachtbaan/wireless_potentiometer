/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/

// the setup function runs once when you press reset or power the board
#define LED_OUT D4
#define POT_SEL D2

#define POT_IN A0

#define SW1 D7
#define SW2 D6
#define SW3 D5

void readPotSw(uint16_t* vals){
  //First read pot 1
  digitalWrite(POT_SEL, 0);
  vals[0] = analogRead(A0);

  //Read pot 2
  digitalWrite(POT_SEL, 1);
  vals[1] = analogRead(A0);

  //Read switches
  
  vals[2] = digitalRead(SW1);

  vals[3] = digitalRead(SW2);

  vals[4] = digitalRead(SW3);
}


void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_OUT, OUTPUT);
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(SW3, INPUT_PULLUP);
  pinMode(POT_SEL, OUTPUT);
  Serial.begin(115200);
}

// the loop function runs over and over again forever
void loop() {
  uint16_t vals[5];
  readPotSw(vals);
  Serial.print(vals[0]);
  Serial.print(",");
  Serial.print(vals[1]);
  Serial.print(",");
  Serial.print(vals[2]);
  Serial.print(",");
  Serial.print(vals[3]);
  Serial.print(",");
  Serial.println(vals[4]);
  
  digitalWrite(LED_OUT, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(50);                       // wait for a second
  digitalWrite(LED_OUT, HIGH);    // turn the LED off by making the voltage LOW
  delay(50);                       // wait for a second
}
