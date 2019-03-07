#include <SPI.h>
#include <MFRC522.h>
#include "enum.h"

// Define =======================================
#define DELAY_TIME 500
#define NONE_COUNT 2
// ==============================================

// Functions ============================
void changeLedStatus(LED_STATUS);
// ======================================

// PIN ==========================================
const int sw1 = 5;
const int sw2 = 6;
constexpr uint8_t RST_PIN = 9;          // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 10;         // Configurable, see typical pin layout above
const int ledG = 19;
const int ledR = 20;
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
String strUID = "-1";

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


void setup(){
    Serial.begin(9600);
    pinMode(sw1,INPUT);
    pinMode(sw2,INPUT);
    pinMode(dip1, INPUT);
    pinMode(dip2, INPUT);
    pinMode(dip3, INPUT);
    pinMode(dip4, INPUT);
    pinMode(ledG, OUTPUT);
    pinMode(ledR, OUTPUT);

    changeLedStatus(LED_INIT);

    while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
    SPI.begin();      // Init SPI bus
    // mfrc522.PCD_Init();   // Init MFRC522
    mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
    Serial.println(F("Scan PICC to see UID"));
}

void loop(){
    changeLedStatus(LED_GREEN); //Change LED Mode
    delay(DELAY_TIME);
    // Main Process Initialize ======================================
    mfrc522.PCD_Init();
    READ_STATUS corState = RFID_None;    // Real Time RFID scan state
    static int noneCount;
    // ==============================================================

    // None RFIDTag =================================================
    if (! mfrc522.PICC_IsNewCardPresent()) {
        if(noneCount >= NONE_COUNT){
            rfidState = RFID_None;
            strUID = "-1";
        }
        else{
            noneCount++;
        }
        Serial.println("corState: " + readStatusToString(corState));
        Serial.println("rfidState: " + readStatusToString(rfidState) + " " + strUID);
        mfrc522.PICC_HaltA();
        return;
    }
    // ==============================================================

    // Tag Read Error ===============================================
    if(! mfrc522.PICC_ReadCardSerial() ){
        corState = RFID_ERROR;
        return;
    }
    // ==============================================================

    // Main Process =================================================
    corState = RFID_GET;
    rfidState = RFID_GET;
    noneCount = 0;
    String bufUID = getUID();
    strUID = bufUID;
    Serial.println("corState: " + readStatusToString(corState));
    Serial.println("rfidState: " + readStatusToString(rfidState)+ " " + strUID);


    // mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
    mfrc522.PICC_HaltA();
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