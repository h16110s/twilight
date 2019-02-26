#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>
// Functions ============================

// ======================================

// PIN ==================================
int soundRX = 5;
int soundTX = 6;
int soundBusy = 7;
int motorR = 8;
int motoeL = 9;
int sw = 11;
int ledG = 19;
int ledR = 20;
int dip1 = 21;
int dip2 = 22;
int dip3 = 23;
int dip4 = 24;
// ======================================

// SoftwareSerial ====================================
SoftwareSerial DFSerial(soundRX, soundTX); // RX, TX
// ===================================================

void setup() {
  Serial.begin (9600);
  DFSerial.begin (9600);

  pinMode(soundBusy,INPUT);
  pinMode(sw, INPUT);
  pinMode(dip1, INPUT);
  pinMode(dip2, INPUT);
  pinMode(dip3, INPUT);
  pinMode(dip4, INPUT);
  pinMode(motorR,OUTPUT);
  pinMode(motoeL,OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledR, OUTPUT);

  //set softwareSerial for DFPlayer-mini mp3 module 
  mp3_set_serial (DFSerial);  
  
}

void loop() {

}




