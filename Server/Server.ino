#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN = 9;          // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 10;         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

String getUID(){
    // get RFIC UID =====================================================================
    Serial.println(mfrc522.uid.size);
    String strBuf[mfrc522.uid.size];
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        strBuf[i] =  String(mfrc522.uid.uidByte[i], HEX);
        if(strBuf[i].length() == 1){
          strBuf[i] = "0" + strBuf[i];
        }
    }
    return strBuf[0] + " " + strBuf[1] + " " + strBuf[2] + " " + strBuf[3];
    // ==================================================================================
}


void setup(){
    Serial.begin(9600);   // Initialize serial communications with the PC
    while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
    SPI.begin();      // Init SPI bus
    mfrc522.PCD_Init();   // Init MFRC522
    mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
    Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

bool flag = false;

void loop(){
    // 新しい(同じカードではない)カードを認識した時と無い時
    //  => 同じカードの時 と カードが乗ってないとき
    if (! mfrc522.PICC_IsNewCardPresent()) {
        if(flag){
            Serial.println("同じカード");
        }
        return;
    }


    // 読み込みがエラーだった時
    if(! mfrc522.PICC_ReadCardSerial() ){
        return;
    }

    flag = true;
    String strUID = getUID();
    Serial.println(strUID);

    // Dump debug info about the card; PICC_HaltA() is automatically called
    // mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
    mfrc522.PICC_HaltA();
    // delay(1000);
}