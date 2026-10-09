
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define SERVOMIN  100 
#define SERVOMAX  440

int servonum = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Channel 0 Servo test");

  pwm.begin();
  pwm.setPWMFreq(60);  
  delay(10);
}

void loop() {

  for (int pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
    pwm.setPWM(servonum, 0, pulselen);
  }
  delay(500);
  
  for (int pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
    pwm.setPWM(servonum, 0, pulselen);
  }
  delay(1000);
}
