#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

// Functions ============================
void ERROR(String message);
void playMusic(int num);
bool isBusy();
void printData(byte *recvData);
const char* getAddress();
// ======================================

// PIN ==================================
const int soundRX = 5;
const int soundTX = 6;
const int soundBusy = 7;
const int motorR = 8;
const int motoeL = 9;
const int sw = 11;
const int ledG = 19;
const int ledR = 20;
const int dip1 = 21;
const int dip2 = 22;
const int dip3 = 23;
const int dip4 = 24;
// ======================================

// SerialPort and other ==============================
SoftwareSerial myDFSerial(soundRX, soundTX); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
// ===================================================

void setup() {
    Serial.begin (9600);
    myDFSerial.begin (9600);

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

    // LED MODE CHANGE (Initialize Status) =============
    digitalWrite(ledG,HIGH);
    digitalWrite(ledR,HIGH);
    // =================================================

    //DFPlayer Initialize ==============================
    //set softwareSerial for DFPlayer-mini mp3 module 
    if(!myDFPlayer.begin(myDFSerial)){
        ERROR("DFPlayer初期化エラー");
    }
    Serial.println(F("DFPlayer 接続済み"));
    // =================================================

    // Network Initialize ==============================
    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();
    const char* address = getAddress();
    Mirf.setRADDR((byte *) address); 
    Mirf.payload = sizeof(unsigned long);
    Mirf.config();
    Serial.println("Listening...");
    // =================================================
}

void loop() {
    // LED MODE CHANGE (Green Status)===================
    digitalWrite(ledG,HIGH);
    digitalWrite(ledR,LOW);
    // =================================================

    byte recvData[Mirf.payload];  // 最大32bit, byteの行列なので要素数は4
    Mirf.getData(recvData);
    // if (recvData[0] == buttonOpen) {  // スイッチを押していない情報であれば
    //   Serial.println("OFF");
    //   digitalWrite(ledPin, LOW);
    // } 
    // else {                          // スイッチを押した情報であれば
    //   Serial.println("ON");
    //   digitalWrite(ledPin, HIGH);
    // }
    delay(100);
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
    Serial.println(num + " mp3 Play");
    myDFPlayer.volume(15);
    myDFPlayer.play(num);
    delay(10);
    while(isBusy);
    myDFPlayer.volume(0);
}

void playMusic(int num){
    Serial.println(num + " mp3 Play");
    myDFPlayer.volume(15);
    myDFPlayer.play(num);
    delay(10);
}

bool isBusy(){
    if(digitalRead(soundBusy) == HIGH){
        return true;
    }
    else{
        return false;
    }
}

void printData(byte *recvData){
    Serial.print("recvData: ");
    for (int i = 0;i < Mirf.payload;i++) {
        Serial.print(recvData[i]);
        Serial.print(" ");
    }
    Serial.println();
}

const char* getAddress(){
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
    return "client" + address;
}