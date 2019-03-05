#include <SPI.h>
#include <MFRC522.h>
#include "enum.h"

#define DELAY_TIME 800

constexpr uint8_t RST_PIN = 9;          // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 10;         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance



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
    Serial.begin(9600);   // Initialize serial communications with the PC
    while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
    SPI.begin();      // Init SPI bus
    // mfrc522.PCD_Init();   // Init MFRC522
    mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
    Serial.println(F("Scan PICC to see UID"));
}

void loop(){
    delay(DELAY_TIME);
    // Initialize ===================================================
    mfrc522.PCD_Init();
    READ_STATUS corState = none;    // Real Time RFID scan state
    // ==============================================================

    // None RFIDTag =================================================
    if (! mfrc522.PICC_IsNewCardPresent()) {
        Serial.println("None");
        mfrc522.PICC_HaltA();
        return;
    }
    // ==============================================================

    // Tag Read Error ===============================================
    if(! mfrc522.PICC_ReadCardSerial() ){
        corState = error;
        return;
    }
    // ==============================================================

    // Main Process =================================================
    corState = getRFID;
    String strUID = getUID();
    Serial.println(strUID);

    // mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
    mfrc522.PICC_HaltA();
    // ==============================================================
}