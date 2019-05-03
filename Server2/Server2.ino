#include <SPI.h>
#include <MFRC522.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
#include "enum.h"
#include <PLED.h>
#include <SceneData.h>
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

// NeoPixel =====================================
#define PIN            6
#define NUMPIXELS      65
// ==============================================


// Define =======================================
#define DELAY_TIME 200
#define NONE_COUNT 2
#define ERROR_DELAY 100
#define BUF_SIZE 8
#define DEVICE_NUM 4
// ==============================================

// Functions ============================
void printData(byte *recvData);
String getUID();
// ======================================


// PIN ==========================================
const int sw1 = 2;
const int sw2 = 3;
constexpr uint8_t RST_PIN = 9;          // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 10;         // Configurable, see typical pin layout above
const int ledG = 14;
const int ledR = 15;
const int dip1 = 21;
const int dip2 = 22;
const int dip3 = 23;
const int dip4 = 24;
// ==============================================

// MFRC522 PORT ======================================
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
// ===================================================

// Grobal 
READ_STATUS rfidState = RFID_None;


// LED
PowerLed pled(ledR, ledG);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);



// Scene Instance ==================================
SceneData Giraffe("04 76 A0 62 BB 2B 80");
SceneData Rhino("04 8c e0 72 d5 38 80"); 
SceneData Panda("04 3A A0 62 BB 2B 80");    
SceneData Rabbit("04 93 A0 62 BB 2B 80");
SceneData Fish("04 B1 A0 62 BB 2B 80");
SceneData Ghost("04 75 A0 62 BB 2B 80");
SceneData Tarantula("04 94 A0 62 BB 2B 80");
SceneData Labuka("04 58 A0 62 BB 2B 80");
SceneData Building("04 95 39 72 D5 38 80");
SceneData End("04 A2 E0 72 D5 38 80");
// =================================================

void setup(){
    Serial.begin(9600);
    Serial.println("Beginning ...");
    pinMode(sw1,INPUT);
    pinMode(sw2,INPUT);
    pinMode(dip1, INPUT);
    pinMode(dip2, INPUT);
    pinMode(dip3, INPUT);
    pinMode(dip4, INPUT);
    pinMode(ledG, OUTPUT);
    pinMode(ledR, OUTPUT);
    pled.changeStatus(LED_INIT);

    // MFRC522 Initialize ==================================================================================
    while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
    SPI.begin();      // Init SPI bus
    // mfrc522.PCD_Init();   // Init MFRC522
    mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
    // =====================================================================================================
    
    // nRF24L01 Initialize (Mirf Initialize) =======================
    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();
    Mirf.setRADDR((byte *)"serv1");
    Mirf.setTADDR((byte *)"clie1");
    Mirf.payload = BUF_SIZE;
    Mirf.config();
    // =============================================================
    Serial.println(F("Scan PICC to see UID"));
    // SceneData Building("04 3b a0 62 bb 2b 80"); 


    // Full Color LED ==============================================
    #if defined (__AVR_ATtiny85__)
        if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
    #endif
    pixels.begin();

    //--- white ---
    for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(150, 150, 150));
    }
    pixels.show();
    // =============================================================
    
    // Make SceneData ==============================================
    byte giraffData[DEVICE_NUM][Mirf.payload] = {
        {0,3,0,0,0,0,0,0},
        {0,3,0,0,0,0,0,0},
        {0,3,0,0,0,0,0,0},
        {0,3,0,0,0,0,0,0}};

    byte rhinoData[DEVICE_NUM][Mirf.payload] = {
        {1,6,6,0,0,0,0,0},
        {2,6,7,0,0,0,0,0},
        {3,6,8,0,0,0,0,0},
        {4,6,9,0,0,0,0,0}};

    byte pandaData[DEVICE_NUM][Mirf.payload] = {
        {0,9,0,0,0,0,0,0},
        {0,9,0,0,0,0,0,0},
        {0,9,0,0,0,0,0,0},
        {0,9,0,0,0,0,0,0}};

    byte rabbitData[DEVICE_NUM][Mirf.payload] = {
        {0,11,0,0,0,1,0,0},
        {0,11,0,0,0,1,0,0},
        {0,11,0,0,0,1,0,0},
        {0,11,0,0,0,1,0,0}};
    
    byte fishData[DEVICE_NUM][Mirf.payload] = {
        {1,5,2,0,10,0,0,0},
        {2,5,3,0,15,0,0,0},
        {3,5,4,0,10,0,0,0},
        {4,5,5,0,15,0,0,0}};
    
    byte ghostData[DEVICE_NUM][Mirf.payload] = {
        {1,4,1,0,0,0,0,0},
        {2,4,1,0,0,0,0,0},
        {3,4,1,0,0,0,0,0},
        {4,4,1,0,0,0,0,0}};
        
    byte tarantulaData[DEVICE_NUM][Mirf.payload] = {
        {0,7,0,0,0,0,0,0},
        {0,7,0,0,0,0,0,0},
        {0,7,0,0,0,0,0,0},
        {0,7,0,0,0,0,0,0}};

    byte labukaData[DEVICE_NUM][Mirf.payload] = {
        {1,10,5,0,10,0,0,0},
        {2,10,4,0,15,0,0,0},
        {3,10,3,0,10,0,0,0},
        {4,10,2,0,15,0,0,0}};

    byte buildingData[DEVICE_NUM][Mirf.payload] = {
        {1,8,10,0,0,0,0,0},
        {2,8,11,0,100,0,0,0},
        {3,8,12,0,50,0,0,0},
        {4,8,13,0,0,0,0,0}};
    
    byte endingData[DEVICE_NUM][Mirf.payload] = {
        {0,12,0,0,0,0,0,0},
        {0,12,0,0,0,0,0,0},
        {0,12,0,0,0,0,0,0},
        {0,12,0,0,0,0,0,0}};
    
    for(int i = 0; i < DEVICE_NUM; i++){
        Giraffe.setSceneData(i,giraffData[i]); // ok
        Rhino.setSceneData(i,rhinoData[i]); // ok
        Panda.setSceneData(i,pandaData[i]);    //ok
        Rabbit.setSceneData(i,rabbitData[i]);
        Fish.setSceneData(i,fishData[i]);
        Ghost.setSceneData(i,ghostData[i]);
        Tarantula.setSceneData(i,tarantulaData[i]);
        Labuka.setSceneData(i,labukaData[i]);
        Building.setSceneData(i,buildingData[i]);
        End.setSceneData(i,endingData[i]);
    }

    // ==============================================================


    Serial.println("Scene One Start");
    while(true){
        byte sceneOne[Mirf.payload] = {0,1,0,0,0,0,0,0};
        if (!Mirf.isSending()) {
            Mirf.send(sceneOne);
            printData(sceneOne);
            delay(100);
        }
        // Serial.println(digitalRead(sw1));
        if(digitalRead(sw1) == HIGH || digitalRead(sw2) == HIGH){
            break;
        }
    }
    pled.changeStatus(GREEN);
    Serial.println("Scene Two Start");
    delay(2000);
    pled.changeStatus(LED_INIT);
    
    while(true){
        byte sceneTwo[Mirf.payload] = {0,2,0,0,0,0,0,0};
        if (!Mirf.isSending()) {
            Mirf.send(sceneTwo);
            printData(sceneTwo);
            delay(100);
        }
        // Serial.println(digitalRead(sw1));
        if(digitalRead(sw1) == HIGH || digitalRead(sw2) == HIGH){
            break;
        }
    }
}

void loop(){
    pled.changeStatus(LED_GREEN); //Change LED Mode

    delay(DELAY_TIME);

    // Main Process Initialize ======================================
    mfrc522.PCD_Init();
    READ_STATUS corState = RFID_None;    // Real Time RFID scan state
    static int noneCount;
    String strUID = "-1";
    byte sendData[DEVICE_NUM][Mirf.payload] = {0};
    for(int i = 0; i < DEVICE_NUM; i++){
        for(int j = 0 ; j < Mirf.payload ; j++){
            sendData[i][j] = 0;
        }
    }
    // ==============================================================

    // None RFIDTag =================================================
    if (! mfrc522.PICC_IsNewCardPresent()) {
        rfidState = RFID_None;
        if(noneCount >= NONE_COUNT){
            strUID = "-1";
        }
        else{
            noneCount++;
        }
    }
    // ==============================================================

    else {
        // Tag Read Error ===============================================
        if(! mfrc522.PICC_ReadCardSerial() ){
            corState = RFID_ERROR;
            pled.changeStatus(LED_ERROR);
            Serial.println("Can't Read RFID tag");
            delay(ERROR_DELAY);
            return;
        }
        // ==============================================================

        // RFID Process =================================================
        corState = RFID_GET;
        rfidState = RFID_GET;
        noneCount = 0;
        String bufUID = getUID();
        strUID = bufUID;
        if(strUID.equalsIgnoreCase(Giraffe.UID)){
            Serial.println("Scene: Giraffe");
            for(int i = 0; i < DEVICE_NUM; i++){ Giraffe.getSceneData(i,sendData[i]); }

        }else if(strUID.equalsIgnoreCase(Rhino.UID)){
             Serial.println("Scene: Rhino");
            for(int i = 0; i < DEVICE_NUM; i++){ Rhino.getSceneData(i,sendData[i]); }

        }else if(strUID.equalsIgnoreCase(Panda.UID)){
             Serial.println("Scene: Panda");
            for(int i = 0; i < DEVICE_NUM; i++){ Panda.getSceneData(i,sendData[i]); }

        }else if(strUID.equalsIgnoreCase(Rabbit.UID)){
             Serial.println("Scene: Rabbit");
            for(int i = 0; i < DEVICE_NUM; i++){ Rabbit.getSceneData(i,sendData[i]); }

        }else if(strUID.equalsIgnoreCase(Fish.UID)){
             Serial.println("Scene: Fish");
            for(int i = 0; i < DEVICE_NUM; i++){ Fish.getSceneData(i,sendData[i]); }

        }else if(strUID.equalsIgnoreCase(Ghost.UID)){
             Serial.println("Scene: Ghost");
            for(int i = 0; i < DEVICE_NUM; i++){ Ghost.getSceneData(i,sendData[i]); }

        }else if(strUID.equalsIgnoreCase(Tarantula.UID)){
             Serial.println("Scene: Tarantula");
            for(int i = 0; i < DEVICE_NUM; i++){ Tarantula.getSceneData(i,sendData[i]); }
 
        }else if(strUID.equalsIgnoreCase(Labuka.UID)){
             Serial.println("Scene: Labuka");
            for(int i = 0; i < DEVICE_NUM; i++){ Labuka.getSceneData(i,sendData[i]); }
        }else if(strUID.equalsIgnoreCase(Building.UID)){
            Serial.println("Scene: Building");
            for(int i = 0; i < DEVICE_NUM; i++){ Building.getSceneData(i,sendData[i]); }
        }else if(strUID.equalsIgnoreCase(End.UID)){
            Serial.println("Scene: Ending");
            for(int i = 0; i < DEVICE_NUM; i++){ End.getSceneData(i,sendData[i]); }
        }
    }
    Serial.println("rfidState: " + readStatusToString(rfidState)+ " " + strUID);
    mfrc522.PICC_HaltA();
    // ==============================================================

    // nRF24L01+ Process ============================================
    if (!Mirf.isSending()) {
        for(int i = 0 ; i < DEVICE_NUM ; i++){
            Mirf.send(sendData[i]);
            printData(sendData[i]);
            delay(20);
        }
    }
    // ==============================================================
}


void printData(byte *recvData){
    Serial.print("sendData: ");
    for (int i = 0;i < Mirf.payload;i++) {
        Serial.print(recvData[i]);
        Serial.print(" ");
    }
    Serial.println();
}

String getUID(){
    // get RFIC UID =====================================================================
    // Serial.println(mfrc522.uid.size);
    String strBuf[mfrc522.uid.size];
    String s = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        strBuf[i] =  String(mfrc522.uid.uidByte[i], HEX);
        if(strBuf[i].length() == 1){
          strBuf[i] = "0" + strBuf[i];
        }
        s += strBuf[i];
        if(i + 1 != mfrc522.uid.size) s += " ";
    }
    return s;
    // ==================================================================================
}

