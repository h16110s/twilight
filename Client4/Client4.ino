#include "BookClient.h"
#include <DFPlayer_Mini_Mp3.h>



PowerLed pled(ledR,ledG);
int address;

void setup() {
    Serial.begin (9600);
    myDFSerial.begin (9600);
    pinMode(soundBusy,INPUT);
    pinMode(sw, INPUT);
    pinMode(dip1, INPUT);
    pinMode(dip2, INPUT);
    pinMode(dip3, INPUT);
    // pinMode(, INPUT);
    pinMode(motorR,OUTPUT);
    pinMode(motorL,OUTPUT);
    pinMode(ledG, OUTPUT);
    pinMode(ledR, OUTPUT);
    pinMode(fan, OUTPUT);
    // LED MODE CHANGE (Initialize Status) =============
    pled.changeStatus(LED_INIT);
    // =================================================
    address = getAddress();

    //DFPlayer Initialize ==============================
    // set softwareSerial for DFPlayer-mini mp3 module 
    if(!myDFPlayer.begin(myDFSerial)){
        ERROR("DFPlayerが接続されていません．");
    }
    // myDFPlayer.begin(myDFSerial);
    Serial.println(F("DFPlayer 接続済み"));
    // =================================================
    mp3_set_serial (myDFSerial);
    mp3_set_volume (15);
    while(digitalRead(sw) == HIGH){
        Serial.println("Closed");
        delay(100);
    }


    // Network Initialize ==============================
    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();
    Mirf.setRADDR((byte *)"clie1");
    Mirf.payload = BUF_SIZE;
    Mirf.config();
    Serial.println(address);
    // =================================================

    // NeoPixcel setup =================================
    #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
    #endif
    pixels.begin();
    // =================================================

    // LED MODE CHANGE (Green Status)===================
    pled.changeStatus(LED_GREEN);
    // =================================================
    digitalWrite(sw,LOW);
}

void loop() {
    // Serial.println("Start");
    unsigned long waitTime;
    unsigned long startTime;
    static bool close = true;
    byte recvData[Mirf.payload] = {0};
    if (Mirf.dataReady() ) {
        // Data Recive
        Mirf.getData(recvData);
        // Same SCENE data
        // if(sceneNum == recvData[SCENE]){
        //     return;
        // }
        // RFID none
        if( recvData[TARGET] == 0){
            if(recvData[SCENE] == 2){        
                if( close == true){
                    if(digitalRead(soundBusy) == HIGH) mp3_play(0);
                    close = false;
                }
            }
            else if(recvData[SCENE] == 12){
                // printData(recvData);
                if(close == false){
                    if(digitalRead(soundBusy) == HIGH) mp3_play(0);
                    close = true;
                }
            }
            else {
                dataStop();
            }
        }
        else if(recvData[TARGET] == address ){
            if( millis() - startTime > waitTime){
                startTime = millis();
                printData(recvData);
                // playMusic(recvData[SOUND_NUM],30);
                if(digitalRead(soundBusy) == HIGH) mp3_play(recvData[SOUND_NUM]);
                changeMotorState(recvData[MOTOR_TIME]*10);
                changeFanState(recvData[FAN]);
                changeLedColor(recvData[SCENE]);
                updateLedColor();
                // waitTime = (rand() % 35) *100 + 2000;
                unsigned long endTime = millis();
                delay((rand() % 35) *100 + 3000 - (endTime - startTime));
            }
        }
    }
    
}
