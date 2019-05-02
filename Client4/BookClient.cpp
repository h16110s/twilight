#include "BookClient.h"

// Grobal Value Initialize ===========================
// static SoftwareSerial myDFSerial(soundRX, soundTX); // RX, TX
// static DFRobotDFPlayerMini myDFPlayer;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(LED_NUM, PIN, NEO_GRB + NEO_KHZ800);
int targetLedColor[RGB] = {0};
int currentLedColor[RGB] = {0};
// ===================================================

void playMusicSg(int num){
    Serial.print(num);
    Serial.println(".mp3 Play");
    myDFPlayer.volume(15);
    myDFPlayer.play(num);
    delay(100);
    while(digitalRead(soundBusy) == LOW);
    myDFPlayer.volume(0);
}

void printData(byte *recvData){
    Serial.print("recvData: ");
    for (int i = 0;i < Mirf.payload;i++) {
        Serial.print(recvData[i]);
        Serial.print(" ");
    }
    Serial.println();
}

void motorON(){
    digitalWrite(motorL, HIGH); 
    digitalWrite(motorR, HIGH);
}

void fanON(){
    digitalWrite(fan,HIGH);
}

void fanOFF(){
    digitalWrite(fan,LOW);
}

void motorOFF(){
    digitalWrite(motorR, LOW);
    digitalWrite(motorL, LOW);
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
    // if(digitalRead(dip4) == HIGH){
    //     address += 8;
    // }
    return address;
}

void dataStop(){
    Serial.println("Data Stop");
    motorOFF();
    fanOFF();
    // myDFPlayer.stop();
}

void setLedColor(int R, int G, int B) {
  targetLedColor[0] = G;
  targetLedColor[1] = R;
  targetLedColor[2] = B;
}

void changeLedColor(int sceneNum) {
  switch (sceneNum) {
    case 0:
        setLedColor(0, 0, 0);
      break;

    case 1:
        setLedColor(100, 0, 200);
      break;

    case 2:
        setLedColor(200, 200, 0);
      break;

      case 3:
        setLedColor(200, 0, 0);
      break;

    default:
        setLedColor(0, 0, 0);
      break;
  }
}

void updateLedColor() {
  // update color data ====================================
  for (int i = 0; i < RGB; i++) {
    if (targetLedColor[i] > currentLedColor[i])
      currentLedColor[i]+=8;
    else if (targetLedColor[i] < currentLedColor[i])
      currentLedColor[i]-=8;
    else
      ;
  }
  // ======================================================

  // LED output ===========================================
  for (int i = 0; i < LED_BOOK; i++)
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  for (int i = 6; i < LED_DOME; i++)
    pixels.setPixelColor(i, pixels.Color(currentLedColor[0], currentLedColor[1], currentLedColor[2]));
  pixels.show();
  // ======================================================
}

void changeMotorState(int timer){
    motorON();
    delay(timer);
    motorOFF();
}

void changeFanState(int state){
    if(state == 0){
      fanOFF();
    }
    else{
      fanON();
    }
}

void playMusic(int num, int vol){
    if(digitalRead(soundBusy) == LOW) return;
    Serial.print(num);
    Serial.println( ".mp3 Play");
    myDFPlayer.volume(vol);
    myDFPlayer.play(num);
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


String readStatusToString(READ_STATUS rs){
    switch (rs)
    {
        case RFID_ERROR:
            return "Error";
        case RFID_GET:
            return "Read";
        case RFID_None:
            return "None";
        default:
            break;
    }
}


