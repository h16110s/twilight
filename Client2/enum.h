#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
#include <MsTimer2.h>
#include <TimerOne.h>

#define TARGET 0
#define SCENE 1
#define SOUND_NUM 2
#define SOUND_VOL 3
#define MOTOR_TIME 4
#define START_DELAY 5
#define BUF_SIZE 8



// Functions ============================
void ERROR(String message);
void playMusic(int num);
bool isBusy();
void printData(byte *recvData);
int getAddress();
void dataPlay();
byte* cpyData(byte *dst, byte *src);
// ======================================


// PIN ==================================
const int soundRX = 2;
const int soundTX = 3;
const int soundBusy = 4;
const int motorR = 5;
const int motorL = 6;
const int sw = 11; //
const int ledG = 14; 
const int ledR = 15; 
const int dip1 = 16; 
const int dip2 = 17; 
const int dip3 = 18; 
const int dip4 = 19; 
// ======================================

// SerialPort and other ==============================
SoftwareSerial myDFSerial(soundRX, soundTX); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
// ===================================================







enum READ_STATUS{
    RFID_None,
    RFID_ERROR,
    RFID_GET,
};

String readStatusToString(READ_STATUS rs){
    switch (rs)
    {
        case RFID_ERROR:
            return "Error";
        case RFID_GET:
            return "Read";
        case RFID_None:
            return "None";
        default:
            break;
    }
}

enum LED_STATUS{
    LED_INIT,
    LED_ERROR,
    LED_GREEN
};

String ledStatusToString(LED_STATUS ls){
    switch (ls)
    {
        case LED_INIT:
            return "Init";
        case LED_ERROR:
            return "Read";
        case LED_GREEN:
            return "Green";
        default:
            break;
    }
}

// class MediaData{
// private:
//     String UID;
//     byte* sendData;

// public:
//     MediaData(byte *data, int bufsize){
//         for(int i = 0; i < bufsize; i++){
//             sendData[i] = data[i];
//         }
//     }
//     String getUID() {return UID};
// };