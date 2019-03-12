
#include "enum.h"



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
        ERROR("DFPlayer接続エラー");
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

    // LED MODE CHANGE (Green Status)===================
    digitalWrite(ledG,HIGH);
    digitalWrite(ledR,LOW);
    // =================================================
}

volatile byte dataAddr[BUF_SIZE];
volatile bool clieBusy = false;

void loop() {
    static int sceneNum = 0;
    byte recvData[Mirf.payload] = {0};
    if (Mirf.dataReady()) {
        // Data Recive
        Mirf.getData(recvData);
        printData(recvData);
        cpyData(dataAddr, recvData);
        // Same SCENE data
        if(sceneNum == recvData[SCENE]&& clieBusy){
            return;
        }
        // RFID none
         if( recvData[TARGET] == 0){
            sceneNum = 0;
            dataStop();
            return;
        }
        // not Target data
        else if(recvData[TARGET] != getAddress()) {
            sceneNum = 0;
            return;
        }
        sceneNum = recvData[SCENE];
        // Set to Start Interrupt
        if(!clieBusy) {
            Serial.println(recvData[START_DELAY]);
            clieBusy = true;
            MsTimer2::set(recvData[START_DELAY]*1000, dataPlay);
            MsTimer2::start();
        }
        dataPlay();
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
    Serial.print(num);
    Serial.println(".mp3 Play");
    myDFPlayer.volume(15);
    myDFPlayer.play(num);
    delay(100);
    while(digitalRead(soundBusy) == LOW);
    myDFPlayer.volume(0);
}

void playMusic(int num){
    Serial.print(num);
    Serial.println( ".mp3 Play");
    if(digitalRead(soundBusy) == LOW)
        return;
    myDFPlayer.volume(30);
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

byte* cpyData(byte *dst, byte *src){
    for (int i = 0;i < Mirf.payload;i++) {
        dst[i] = src[i];
    }
    return dst;
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

void dataPlay(){
    MsTimer2::stop();
    // printData(dataAddr);
    Serial.println("Data Play");
    myDFPlayer.volume(dataAddr[SOUND_VOL]);
    playMusic(dataAddr[SOUND_NUM]);
    motorON();
    // Set To Stop Interrupt
    MsTimer2::set(dataAddr[MOTOR_TIME]*1000, dataStop);
    MsTimer2::start();
}

void dataStop(){
    Serial.println("Data Stop");
    clieBusy = false;
    motorOFF();
    myDFPlayer.stop();
}