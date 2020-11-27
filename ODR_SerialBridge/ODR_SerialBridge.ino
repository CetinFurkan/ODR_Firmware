/*
   Simple Serial bridge code for multiple serial comm

   Created by Furkan Cetin on 27/11/2020
    - Initial structure

*/

bool state = false;

void setup() {
  pinMode(13, OUTPUT);

  Serial.begin(9600);   //USB Connection for testing
  Serial1.begin(9600);  //Back Traction Motor
  Serial2.begin(9600);  //Bluetooth Connection
  Serial3.begin(9600);  //Front Rotater Motor
}

void loop() {
  while (Serial.available()) {
    char data = Serial.read();
    Serial1.write(data);
    Serial3.write(data);
  }

  while (Serial2.available()) {
    char data = Serial2.read();
    Serial1.write(data);
    Serial3.write(data);
  }
  while (Serial3.available()) {
    Serial2.write(Serial3.read());
  }

  while (Serial1.available()) {
    Serial2.write(Serial1.read());
  }
}
