/*
   Rotating DC Motor with Pololu 36V HV Motor Driver

   Created by Furkan Cetin on 27/11/2020
    - Renaming with variables
    - Baudrate changed to 9600
    - New Functions are added
   Modified on 28/11/2020
    - Serial interface is implemented
    - New Functions are added   
*/
#define pinReset 4
#define pinDir 9
#define pinPwmL 5
#define pinPwmR 6
#define pinLed 13

#define pinCurrentSense A3

#define BYTE_SET_SPEED_TARGET_ABS 'u'
#define BYTE_SET_SPEED_TARGET_REL 'j'
#define BYTE_SET_PWM_MAX          'n'
#define BYTE_SET_SPEED_ACC        'b'

#define ACC_MAX 2.0

int PWM_MAX = 100;

float speedAcc = 0.005;
float speedNow = 0;
float speedTarget = 0;

void setup() {
  Serial.begin(9600);

  pinMode(pinReset, OUTPUT);
  digitalWrite(pinReset, 1);

  pinMode(pinDir, OUTPUT);
  pinMode(pinPwmL, OUTPUT);
  pinMode(pinPwmR, OUTPUT);
  pinMode(pinLed, OUTPUT);

  pinMode(pinCurrentSense, INPUT);

  digitalWrite(pinPwmL, 1);
}

void loop() {
  //CALCULATION LAYER
  speedNow += (speedTarget - speedNow) * speedAcc;


  //ACTUATION LAYER
  analogWrite(pinPwmR, abs(speedNow));

  if (speedNow > 0) {
    speedNow = min(speedNow, PWM_MAX);
    digitalWrite(pinDir, 1);
  }
  else {
    speedNow = max(speedNow, -PWM_MAX);
    digitalWrite(pinDir, 0);
  }

  //COMMUNICATION LAYER
  Serial.write(255);
  Serial.write(int(speedNow));

  while (Serial.available() >= 3) {
    if (Serial.read() == 255) {
      char msgType = Serial.read();
      int  msgData = Serial.read();

      if (msgType == BYTE_SET_SPEED_TARGET_ABS) {
        setSpeedTargetAbs(msgData - 127);
      }
      else if (msgType == BYTE_SET_SPEED_TARGET_REL) {
        setSpeedTargetRel(msgData - 127);
      }
      else if (msgType == BYTE_SET_PWM_MAX) {
        setPwmMax(msgData);
      }
      else if (msgType == BYTE_SET_SPEED_ACC) {
        setSpeedAcc(msgData);
      }
    }
  }
}

void setSpeedTargetAbs(int _speed) {
  speedTarget = map(_speed, -127, 127, -PWM_MAX, PWM_MAX);
}

void setSpeedTargetRel(int _speed) {
  speedTarget += _speed;
  speedTarget = min(PWM_MAX, max(speedTarget, -PWM_MAX));
}

void setPwmMax(int _pwm) {
  _pwm = min(255, max(_pwm, 0));
  PWM_MAX = _pwm;
}

void setSpeedAcc(int _acc) {
  speedAcc = (ACC_MAX / 255.0) * (_acc+0.02);
}
