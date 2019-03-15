
#include "enum.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 9
#define LED_NUM 9
#define LED_BOOK 6
#define LED_DOME 9
#define RGB 3
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(LED_NUM, PIN, NEO_GRB + NEO_KHZ800);

int targetLedColor[RGB] = {0};
int currentLedColor[RGB] = {0};

void motorON(){
    digitalWrite(motorL, HIGH); 
    digitalWrite(motorR, HIGH);
}

void motorOFF(){
    digitalWrite(motorR, LOW);
    digitalWrite(motorL, LOW);
}

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

    // NeoPixcel setup =================================
    #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
    #endif
    pixels.begin();
    // =================================================


    // LED MODE CHANGE (Green Status)===================
    digitalWrite(ledG,HIGH);
    digitalWrite(ledR,LOW);
    // =================================================
}

void changeMotorState(int timer){
    static unsigned long startTime;
    if(timer == 0){
        motorOFF();
    }
    else if(digitalRead(motorL)==LOW && digitalRead(motorR)==LOW ){
        startTime =  millis();
        motorON();
    }
    else if(digitalRead(motorL)==HIGH && digitalRead(motorR)==HIGH ){
        if(  millis() - startTime >= timer){
            motorOFF();
        }
    }
}

void playMusic(int num, int vol){
    if(digitalRead(soundBusy) == LOW) return;
    Serial.print(num);
    Serial.println( ".mp3 Play");
    myDFPlayer.volume(vol);
    myDFPlayer.loop(num);
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



void printData(byte *recvData){
    Serial.print("recvData: ");
    for (int i = 0;i < Mirf.payload;i++) {
        Serial.print(recvData[i]);
        Serial.print(" ");
    }
    Serial.println();
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

void dataStop(){
    Serial.println("Data Stop");
    motorOFF();
    myDFPlayer.stop();
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