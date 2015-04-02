// NewSweep by Daniel Wilbern: dwilbern@nmu.edu

#include <Servo.h>

#define SERVOPIN 9

#define SETPOWER 'p'

Servo myservo;

void setup() {

	Serial.begin(9600); // 9600 baud
	myservo.attach(SERVOPIN);
}

void loop() {

  int power;
  unsigned char incomingByte;
  
	if(Serial.available() > 0) {
	
		incomingByte = Serial.read();

		switch(incomingByte) {
		
			case SETPOWER:
				power = Serial.parseInt();
				myservo.write(power);
				break;
		}
	}
}

