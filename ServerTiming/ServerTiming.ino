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
#define POT1P D7
#define POT1M D6
#define POT2P D5
#define POT2M D2

#define POT_IN A0

#define SWM D1
#define SW1P D7
#define SW2P D6
#define SW3P D5

void readPotSw(uint16_t* vals){
  //First read pot 1
  pinMode(SWM, INPUT);
  pinMode(POT2M, INPUT);
  pinMode(POT2P, INPUT);
  pinMode(POT1M, OUTPUT);
  pinMode(POT1P, OUTPUT);
  digitalWrite(POT1M, LOW);
  digitalWrite(POT1P, HIGH);
  delayMicroseconds(10);
  vals[0] = analogRead(A0);

  //Read pot 2
  pinMode(SWM, INPUT);
  pinMode(POT2M, OUTPUT);
  pinMode(POT2P, OUTPUT);
  pinMode(POT1M, INPUT);
  pinMode(POT1P, INPUT);
  digitalWrite(POT2M, LOW);
  digitalWrite(POT2P, HIGH);
  delayMicroseconds(10);
  vals[1] = analogRead(A0);
}


void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_OUT, OUTPUT);
  Serial.begin(115200);
}

// the loop function runs over and over again forever
void loop() {
  uint16_t vals[2];
  readPotSw(vals);
  Serial.print(vals[0]);
  Serial.print(",");
  Serial.println(vals[1]);
  
  digitalWrite(LED_OUT, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(LED_OUT, LOW);    // turn the LED off by making the voltage LOW
  delay(500);                       // wait for a second
}
