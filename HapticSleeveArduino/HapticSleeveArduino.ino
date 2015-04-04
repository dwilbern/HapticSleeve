// NewSweep by Daniel Wilbern: dwilbern@nmu.edu

#include <Servo.h>

#define SETPOS 'p'
#define GETPOS 'g'
#define SETSPEED 's'

Servo myservo;
int servoPin = 9;
int feedbackPin = A0;

int minDegrees = 10;
int maxDegrees = 170;
int minFeedback, maxFeedback;

int targetPos = 0;
int rotationSpeed = 6;
int degreesTolerance = 10;

void setup() {

  myservo.attach(servoPin);
  Serial.begin(9600); // 9600 baud
  calibrateServoFeedback();
  myservo.write(0);
}

void loop() {
  
  handleSerialCommunication();

  if(abs(getServoPos() - targetPos) <= degreesTolerance)
    return;
  else if (getServoPos() < targetPos)
    myservo.write(getServoPos() + rotationSpeed);
  else
    myservo.write(getServoPos() - rotationSpeed);
}

void handleSerialCommunication() {

  unsigned char incomingByte;

  if(Serial.available() > 0) {

    incomingByte = Serial.read();

    switch(incomingByte) {

    case SETPOS:
      targetPos = Serial.parseInt();
      break;

    case GETPOS:
      Serial.println(getServoPos());
      break;

    case SETSPEED:
      rotationSpeed = Serial.parseInt();
      if(rotationSpeed == 1) rotationSpeed = 6;
      else if(rotationSpeed == 2) rotationSpeed = 8;
      else if(rotationSpeed == 3) rotationSpeed = 10;
      else rotationSpeed = 0;
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







