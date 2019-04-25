#include "BookClient.h"

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
    pinMode(motorL,OUTPUT);
    pinMode(ledG, OUTPUT);
    pinMode(ledR, OUTPUT);

    // LED MODE CHANGE (Initialize Status) =============
    PowerLed pled = PowerLed(ledR, ledG);
    pled.changeStatus(LED_INIT);
    // =================================================

    //DFPlayer Initialize ==============================
    // set softwareSerial for DFPlayer-mini mp3 module 
    if(!myDFPlayer.begin(myDFSerial)){
        ERROR("DFPlayerが接続されていません．");
    }
    // myDFPlayer.begin(myDFSerial);
    Serial.println(F("DFPlayer 接続済み"));
    // =================================================

    // Network Initialize ==============================
    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();
    Mirf.setRADDR((byte *)"clie1");
    Mirf.payload = BUF_SIZE;
    Mirf.config();
    Serial.print("ネットワーク準備：このクライアントは");
    Serial.print(getAddress());
    Serial.println("番");
    Serial.println("接続テスト開始・・・");
    while(true){
        byte sendData[Mirf.payload] = {0};
        if(Mirf.dataReady()){
            Mirf.getData(sendData);
            if(sendData[0] == getAddress()){
                Serial.println("親機を確認");
                break;
            }
        }
        delay(100);
    }
    Serial.println("ネットワーク準備完了");
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
}

void loop() {
    byte recvData[Mirf.payload] = {0};
    if (Mirf.dataReady()) {
        // Data Recive
        Mirf.getData(recvData);
        // Same SCENE data
        // if(sceneNum == recvData[SCENE]){
        //     return;
        // }
        // RFID none
         if( recvData[TARGET] == 0){
            dataStop();
            return;
        }
        // not Target data
        else if(recvData[TARGET] != getAddress()) {
            return;
        }
        printData(recvData);
        changeMotorState(recvData[MOTOR_TIME]*10);
        playMusic(recvData[SOUND_NUM],30);
        changeLedColor(recvData[SCENE]);
        updateLedColor();
    }
}
