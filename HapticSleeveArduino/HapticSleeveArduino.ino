// NewSweep by Daniel Wilbern: dwilbern@nmu.edu

#include <Servo.h>

#define SETPOS 'p'
#define GETPOS 'g'
#define CALIBRATE 'c'
#define ECHO 'e'

Servo myservo;
int servoPin = 9;
int feedbackPin = A0;

int minDegrees = 10;
int maxDegrees = 170;
int minFeedback, maxFeedback;

int targetPos = 0;
int degreesTolerance = 20;

void setup() {

  myservo.attach(servoPin);
  Serial.begin(115200); // 115200 baud
}

void loop() {

  handleSerialCommunication();
}

void handleSerialCommunication() {

  unsigned char incomingByte;

  if(Serial.available() > 0) {

    incomingByte = Serial.read();

    switch(incomingByte) {

    case SETPOS:
      //targetPos = Serial.parseInt();
      myservo.write(Serial.parseInt());
      break;

    case GETPOS:
      Serial.println(getServoPos());
      break;

    case CALIBRATE:
      calibrateServoFeedback();
      Serial.println("done");
      break;

    case ECHO:
    delay(10);
      Serial.read(); // eat the space
      while(Serial.available() > 0)
        Serial.write(Serial.read());
      break;
    }
  }
  
}

void calibrateServoFeedback() {
  myservo.write(minDegrees);
  delay(2000); // give the servo more than enough time to get there and settle
  minFeedback = analogRead(feedbackPin);

  myservo.write(maxDegrees);
  delay(2000); // give the servo more than enough time to get there and settle
  maxFeedback = analogRead(feedbackPin);
}

int getServoPos() {

  return map(analogRead(feedbackPin), minFeedback, maxFeedback, minDegrees, maxDegrees);
}









