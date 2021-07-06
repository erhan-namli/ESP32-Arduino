///// Erhan Namlı - 06/07/2021



#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

///////// Servo Kütüphaneleri ve Pin tanımlamaları

#include <ESP32Servo.h>
int servoPin = 13;
Servo myservo;
int pos = 0;

//////////   Motor bağlantıları ve Bluetooth ile okunan değerin değişkeni

char receivedChar;// received value will be stored as CHAR in this variable

const int MR1 = 26; //ESP32 pins (MR=Right Motor) (ML=Left Motor) (1=Forward) (2=Backward)
const int MR2 = 27;
const int ML1 = 14;
const int ML2 = 12;


/////////    Ultrasonik mesafe sensörü pin tanımlamaları

int echo = 5;
int trig = 2;

long duration;
float distance;

//////// Algoritma için gerekli değişkenler

float anlikMesafe;
float sagMesafe;
float solMesafe;

void setup() {

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);


  // Servo motor pwm için gerekli tanımlamalar

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);    // standard 50 hz servo
  myservo.attach(servoPin, 500, 2400); // attaches the servo on pin 18 to the servo object
  // using default min/max of 1000us and 2000us
  // different servos may require different min/max settings
  // for an accurate 0 to 180 sweep


  Serial.begin(115200);
  SerialBT.begin("Eranin Arabasi"); //You can change your Bluetooth device name

  pinMode(MR1, OUTPUT);
  pinMode(MR2, OUTPUT);
  pinMode(ML1, OUTPUT);
  pinMode(ML2, OUTPUT);
}


void loop() {


  receivedChar = (char)SerialBT.read();

  if (Serial.available()) {
    SerialBT.write(Serial.read());

  }
  if (SerialBT.available()) {

    Serial.print ("Received:");//print on serial monitor
    Serial.println(receivedChar);//print on serial monitor

    if (receivedChar == 'F')
    {
      Forward();

    }
    if (receivedChar == 'B')
    {

      Backward();
    }
    if (receivedChar == 'L')
    {

      Left();
    }
    if (receivedChar == 'R')
    {

      Right();
    }
    if (receivedChar == 'S')
    {
      Stop();
    }
    delay(20);

    anlikMesafe = mesafeOlc();

    if (anlikMesafe < 30) {

      geriGit();
      delay(350);
      anlikMesafe = mesafeOlc();
      
      solMesafe = solaBak();
      sagMesafe = sagaBak();

      if (solMesafe > sagMesafe) {

        Left();

      } else {

        Right();

      }
    }


  }
  delay(30);
}

// Fonksiyonlar

float solaBak() {

  myservo.write(180);
  return mesafeOlc();
}

float sagaBak() {

  myservo.write(0);
  return mesafeOlc();
}

void geriGit() {

  Backward();
}

float mesafeOlc() {

  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH);

  distance = (duration / 29.1) / 2;

  return distance;
}



void Forward() {
  //RIGHT MOTOR
  digitalWrite(MR1, HIGH); //MOVE FRONT
  digitalWrite(MR2, LOW); //MOVE BACK
  //LEFT MOTOR
  digitalWrite(ML1, LOW); //MOVE BACK
  digitalWrite(ML2, HIGH); //MOVE FRONT
}
void Backward() {
  digitalWrite(MR1, LOW);
  digitalWrite(MR2, HIGH);
  digitalWrite(ML1, HIGH);
  digitalWrite(ML2, LOW);
}
void Left() {
  digitalWrite(MR1, HIGH);
  digitalWrite(MR2, LOW);
  digitalWrite(ML1, HIGH);
  digitalWrite(ML2, LOW);
}
void Right() {
  digitalWrite(MR1, LOW);
  digitalWrite(MR2, HIGH);
  digitalWrite(ML1, LOW);
  digitalWrite(ML2, HIGH);
}
void Stop() {
  digitalWrite(MR1, LOW);
  digitalWrite(MR2, LOW);
  digitalWrite(ML1, LOW);
  digitalWrite(ML2, LOW);
}
