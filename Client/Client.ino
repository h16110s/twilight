#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

#define TARGET 0
#define SCENE 1
#define SOUND_NUM 2
#define SOUND_VOL 3
#define MOTOR_TIME 4
#define BUF_SIZE 8

// Functions ============================
void ERROR(String message);
void playMusic(int num);
bool isBusy();
void printData(byte *recvData);
int getAddress();
void dataPlay(byte *recvData);
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

void setup() {
    Serial.begin (9600);
    myDFSerial.begin (9600);

    pinMode(soundBusy,INPUT);
    // pinMode(sw, INPUT);
    pinMode(dip1, INPUT);
    pinMode(dip2, INPUT);
    pinMode(dip3, INPUT);
    pinMode(dip4, INPUT);
    pinMode(motorR,OUTPUT);
    pinMode(motorL,OUTPUT);
    pinMode(ledG, OUTPUT);
    pinMode(ledR, OUTPUT);

    // LED MODE CHANGE (Initialize Status) =============
    digitalWrite(ledG,HIGH);
    digitalWrite(ledR,HIGH);
    // =================================================

    //DFPlayer Initialize ==============================
    // set softwareSerial for DFPlayer-mini mp3 module 
    if(!myDFPlayer.begin(myDFSerial)){
        ERROR("DFPlayer初期化エラー");
    }
    // myDFPlayer.begin(myDFSerial);
    Serial.println(F("DFPlayer 接続済み"));
    myDFPlayer.volume(0);
    // =================================================

    // Network Initialize ==============================
    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();
    Mirf.setRADDR((byte *)"clie1");
    Mirf.payload = BUF_SIZE;
    Mirf.config();
    Serial.println("Listening...");
    Serial.println(getAddress());
    // =================================================
}


void loop() {
    // LED MODE CHANGE (Green Status)===================
    digitalWrite(ledG,HIGH);
    digitalWrite(ledR,LOW);
    // =================================================
    static int sceneNum = 0;
    byte recvData[Mirf.payload] = {0}; 
    if (!Mirf.isSending() && Mirf.dataReady()) {
        // Data Recive
        Mirf.getData(recvData);
        printData(recvData);
        if(recvData[0] != getAddress()) {
            sceneNum = 0;
            return;
        }
        else if(sceneNum == recvData[SCENE]){
            return;
        }
        sceneNum = recvData[SCENE];
        dataPlay(recvData);
    }
}

void ERROR(String message){
    // LED MODE CHANGE (Error Status)===================
    digitalWrite(ledG,LOW);
    digitalWrite(ledR,HIGH);
    //==================================================
    while(true){
        Serial.print("Error:");
        Serial.println(message);
        digitalWrite(ledR,HIGH);
        delay(1000);
        digitalWrite(ledR,LOW);
        delay(1000);
    }
}

void playMusicSg(int num){
    Serial.println(num);
    Serial.println(".mp3 Play");
    // myDFPlayer.volume(15);
    myDFPlayer.play(num);
    delay(100);
    while(digitalRead(soundBusy) == LOW);
    myDFPlayer.volume(0);
}

void playMusic(int num){
    Serial.println(num);
    Serial.println( ".mp3 Play");
    if(digitalRead(soundBusy) == LOW)
        return;
    // myDFPlayer.volume(15);
    myDFPlayer.play(num);
    delay(10);
}

void printData(byte *recvData){
    Serial.print("recvData: ");
    for (int i = 0;i < Mirf.payload;i++) {
        Serial.print(recvData[i]);
        Serial.print(" ");
    }
    Serial.println();
}

int getAddress(){
    int address = 0;
    if(digitalRead(dip1) == HIGH){
        address += 1;
    }
    if(digitalRead(dip2) == HIGH){
        address += 2;
    }
    if(digitalRead(dip3) == HIGH){
        address += 4;
    }
    if(digitalRead(dip4) == HIGH){
        address += 8;
    }
    return address;
}

void motorON(){
    digitalWrite(motorL, HIGH); 
    digitalWrite(motorR, HIGH);
}

void motorOFF(){
    digitalWrite(motorR, LOW);
    digitalWrite(motorL, LOW);
}

void dataPlay(byte *recvData){
    myDFPlayer.volume(recvData[SOUND_VOL]);
    playMusic(recvData[SOUND_NUM]);
    motorON();
    delay(recvData[MOTOR_TIME]*1000);
    motorOFF();
}
