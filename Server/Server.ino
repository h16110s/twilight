#include <SPI.h>
#include <MFRC522.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
#include "enum.h"

// Define =======================================
#define DELAY_TIME 500
#define NONE_COUNT 2
#define ERROR_DELAY 100
#define BUF_SIZE 8
#define DEVICE_NUM 2
#define UID1 "e6 2d 0d 9f"
#define UID2 "60 48 8c 4d"
#define UID3 "04 3b a0 62 bb 2b 80"
#define UID4 "04 59 a0 62 bb 2b 80"
#define UID5 "04 1d a0 62 bb 2b 80"
// ==============================================

// Functions ============================
void changeLedStatus(LED_STATUS);
void printData(byte *recvData);
String getUID();
// ======================================


// PIN ==========================================
const int sw1 = 5;
const int sw2 = 6;
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
String strUID = "-1"; // Send Status

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

    changeLedStatus(LED_INIT);
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
}

void loop(){
    changeLedStatus(LED_GREEN); //Change LED Mode
    delay(DELAY_TIME);
    // Main Process Initialize ======================================
    mfrc522.PCD_Init();
    READ_STATUS corState = RFID_None;    // Real Time RFID scan state
    static int noneCount;
    byte sendData[DEVICE_NUM+1][Mirf.payload] = {0};
    for(int i = 0; i < DEVICE_NUM + 1 ; i++){
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
            changeLedStatus(LED_ERROR);
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
        if(strUID.equalsIgnoreCase(UID1)){
            sendData[1][0] = 1;   // TARGET
            sendData[1][1] = 1;    // Scene num
            sendData[1][2] = 1;   // Sound num
            sendData[1][3] = 1;  // Sound Volume
            sendData[1][4] = 1;  // Motor
            sendData[1][5] = 1;
            sendData[1][8] = 100;
            sendData[2][0] = 2;   // TARGET
            sendData[2][1] = 1;    // Sean num
            sendData[2][2] = 1;   // Sound num
            sendData[2][3] = 1;  // Sound Volume
            sendData[2][4] = 1;  // Motor
            sendData[2][5] = 6;
            sendData[2][8] = 100;
        }else if(strUID.equalsIgnoreCase(UID2)){
            sendData[1][0] = 1;   // TARGET
            sendData[1][1] = 2;    // Scene num
            sendData[1][2] = 2;   // Sound num
            sendData[1][3] = 1;  // Sound Volume
            sendData[1][4] = 3;  // Motor
            sendData[1][5] = 1;
            sendData[1][8] = 100;
            sendData[2][0] = 2;   // TARGET
            sendData[2][1] = 2;    // Sean num
            sendData[2][2] = 2;   // Sound num
            sendData[2][3] = 1;  // Sound Volume
            sendData[2][4] = 3;  // Motor
            sendData[2][5] = 6;
            sendData[2][8] = 100;
        }else if(strUID.equalsIgnoreCase(UID3)){
            sendData[1][0] = 1;   // TARGET
            sendData[1][1] = 3;    // Scene num
            sendData[1][2] = 3;   // Sound num
            sendData[1][3] = 3;  // Sound Volume
            sendData[1][4] = 3;  // Motor
            sendData[1][5] = 1;
            sendData[1][8] = 100;
            sendData[2][0] = 2;   // TARGET
            sendData[2][1] = 3;    // Sean num
            sendData[2][2] = 3;   // Sound num
            sendData[2][3] = 1;  // Sound Volume
            sendData[2][4] = 3;  // Motor
            sendData[2][5] = 6;
            sendData[2][8] = 100;
        }
    }
    // Serial.println("corState: " + readStatusToString(corState));
    Serial.println("rfidState: " + readStatusToString(rfidState)+ " " + strUID);
    // mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
    mfrc522.PICC_HaltA();
    // if(rfidState == RFID_None) return;      
    // ==============================================================

    // nRF24L01+ Process ============================================
    if (!Mirf.isSending()) {
        for(int i = 1 ; i < DEVICE_NUM +1 ; i++){
            Mirf.send(sendData[i]);
            delay(20);
            printData(sendData[i]);
        }
    }
    // ==============================================================
}


void changeLedStatus(LED_STATUS status){
    switch(status){
        case LED_INIT:
            // LED MODE CHANGE (Initialize Status) =============
            digitalWrite(ledG,HIGH);
            digitalWrite(ledR,HIGH);
            break;
        case LED_ERROR:
            // LED MODE CHANGE (Error Status) =============
            digitalWrite(ledG,LOW);
            digitalWrite(ledR,HIGH);
            break;
        case LED_GREEN:
            // LED MODE CHANGE (Error Status) =============
            digitalWrite(ledG,HIGH);
            digitalWrite(ledR,LOW);
            break;
        default:
            break;
    };
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

