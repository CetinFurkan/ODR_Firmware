/*
   Simple Joystick is interfaced for sending data over bluetooth

   Created by Furkan Cetin on 27/11/2020
   Modified on 26/11/2020:
    - Initial structure

*/
int joyNowX = 0;
int joyNowY = 0;

int joyLastX = 0;
int joyLastY = 0;


void setup() {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  Serial.begin(9600);

  // Set maximum PWM value
  Serial.write(255);
  Serial.write('n');
  Serial.write(100);

  // Set maximum rotating angle value
  Serial.write(255);
  Serial.write('m');
  Serial.write(65);
}

void loop() {
  joyNowX = analogRead(A1) / 4.0;
  joyNowY = analogRead(A0) / 4.0;

  //Send if joyNowX data is changed
  if (joyNowX != joyLastX) {
    Serial.write(255);
    Serial.write('a'); // For Rotater
    Serial.write(joyNowX);
    joyLastX = joyNowX;
  }

  //Send if joyNowY data is changed
  if (joyNowY != joyLastY) {
    Serial.write(255);
    Serial.write('u'); //For Trackter
    Serial.write(joyNowY);
    joyLastY = joyNowY;
  }
}
