#ifndef __CLIENT__
#define __CLIENT__

#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
#include <Adafruit_NeoPixel.h>
#include "PLED.h"
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Define ==================================
#define TARGET 0        // array data tag
#define SCENE 1
#define SOUND_NUM 2
#define SOUND_VOL 3
#define MOTOR_TIME 4
#define START_DELAY 5
#define BUF_SIZE 8      // reciev data size
#define PIN 9
#define LED_NUM 9
#define LED_BOOK 6
#define LED_DOME 9
#define RGB 3
// =========================================

// Enum ====================================
enum READ_STATUS{
    RFID_None,
    RFID_ERROR,
    RFID_GET,
};
// =========================================

// PIN ==================================
const int soundRX = 2;
const int soundTX = 3;
const int soundBusy = 4;
const int motorR = 5;
const int motorL = 6;
const int sw = 10;
const int ledG = 14; 
const int ledR = 15; 
const int dip1 = 16; 
const int dip2 = 17; 
const int dip3 = 18; 
const int dip4 = 19; 
// ======================================

// Functions ============================
void ERROR(String message);
void playMusic(int num, int vol);
void printData(byte *recvData);
int getAddress();
void dataPlay();
void dataStop();
void motorON();
void motorOFF();
void changeMotorState(int timer);
String ledStatusToString(LED_STATUS ls);
String readStatusToString(READ_STATUS rs);
void changeLedStatus(LED_STATUS ls);
void updateLedColor();
void changeLedColor(int sceneNum);
void setLedColor(int R, int G, int B);
// ======================================


// SerialPort and Grobal Value =======================
static SoftwareSerial myDFSerial(soundRX, soundTX); // RX, TX
static DFRobotDFPlayerMini myDFPlayer;
extern Adafruit_NeoPixel pixels;
extern int targetLedColor[RGB];
extern int currentLedColor[RGB];
// ===================================================

#endif  //__CLIENT__