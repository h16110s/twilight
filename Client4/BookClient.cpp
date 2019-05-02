#include "BookClient.h"


// Grobal Value Initialize ===========================
// static SoftwareSerial myDFSerial(soundRX, soundTX); // RX, TX
// static DFRobotDFPlayerMini myDFPlayer;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
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


void changeLedColor(int sceneNum, int add) {
  switch (sceneNum) {
    case 0:
      setLedColor(0, 0, 0);
      break;

    case 1:
      setLedColor(0, 0, 0);
      break;

    case 2:
      setLedColor(0, 0, 0);
      break;

    case 3:
      if (add == 1)
        setLedColor(40, 100, 40); //黄緑系
      else if (add == 2)
        setLedColor(40, 100, 0);
      else if (add == 3)
        setLedColor(60, 100, 20);
      else if (add == 4)
        setLedColor(0, 100, 20);
      break;

    case 4:
      if (add == 1)
        setLedColor(75, 25, 100); //紫
      else if (add == 2)
        setLedColor(0, 0, 100); //青
      else if (add == 3)
        setLedColor(0, 100, 0); //緑
      else if (add == 4)
        setLedColor(100, 95, 0); //橙
      break;

    case 5:
      setLedColor(70, 100, 20); //水
      break;

    case 6:
      setLedColor(70, 100, 20); //蛍色
      break;

    case 7:
      setLedColor(100, 0, 0); //赤
      break;

    case 8:
      setLedColor(100, 100, 0); //黄
      break;

    case 9:
      if (add == 1)
        setLedColor(0, 100, 30); //緑系
      else if (add == 2)
        setLedColor(0, 100, 10);
      else if (add == 3)
        setLedColor(0, 100, 40);
      else if (add == 4)
        setLedColor(0, 100, 20);
      break;

    case 10:
      setLedColor(0, 0, 100); //青
      break;

    case 11:
      if (add == 1)
        setLedColor(100, 50, 0); //橙
      else if (add == 2)
        setLedColor(100, 0, 0); //赤
      else if (add == 3)
        setLedColor(100, 50, 0); //橙
      else if (add == 4)
        setLedColor(100, 0, 0); //赤
      break;

    case 12:
      setLedColor(0, 100, 100); //水
      break;

    case 13:
      setLedColor(0, 0, 0);
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
      currentLedColor[i]+=5;
    else if (targetLedColor[i] < currentLedColor[i])
      currentLedColor[i]-=5;
    else
      ;
  }
  // ======================================================

  // LED output ===========================================
  for (int i = 0; i < LED_BOOK; i++)
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  for (int i = 10; i < LED_DOME; i++)
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


struct LED {
  int r;
  int g;
  int b;
  int brightness;
  bool wave;
};
struct LED ledF[15];

//本開いたとき_LED点灯
void book_open() {
  const int max_brightness = 30;
  const int min_brightness = 0;
  bool fadein = true;
  bool fadeout = false;
  int counter = 0;
  randomSeed(analogRead(A6));

  //--- 初期設定 ---
  for (int i = 0; i < 15; i++) {
    ledF[i].r = 0;
    ledF[i].g = 0;
    ledF[i].b = 0;
    ledF[i].brightness = 0;
    ledF[i].wave = false;
  }

  //--- 初期値の設定 ---
  for (int i = 0; i < 10; i++) {
    ledF[i].r = ledF[i].g = ledF[i].b = random(min_brightness, max_brightness);
  }

  //--- キューブ点灯 ---
  for (int i = 10; i < 15; i++) {
    ledF[i].r = 0;
    ledF[i].g = 100;
    ledF[i].b = 100;
    ledF[i].brightness = 100;
    pixels.setPixelColor(i, pixels.Color(
                           ledF[i].r * (ledF[i].brightness / 100.0),
                           ledF[i].g * (ledF[i].brightness / 100.0),
                           ledF[i].b * (ledF[i].brightness / 100.0)
                         ));
  }
  pixels.show();

  //--- 繰り返し処理 ---
  while (counter < 200) {

    //--- 表紙の10個 ---
    for (int i = 0; i < 10; i++) {

      if (counter > 100) {
        fadeout = true;
      }

      if (fadein == true) {
        ledF[i].brightness ++;
        if (ledF[i].brightness > 100) {
          ledF[i].brightness = 100;
          fadein = false;
        }
      }

      if (fadeout == true) {
        ledF[i].brightness --;
        if (ledF[i].brightness < 0) {
          ledF[i].brightness = 0;
          fadeout = false;
        }
      }

      if (ledF[i].wave == true) {
        ledF[i].r ++;
        ledF[i].g ++;
        ledF[i].b ++;
        if (ledF[i].r > max_brightness || ledF[i].g > max_brightness || ledF[i].b > max_brightness) {
          ledF[i].wave = false;
          ledF[i].r = max_brightness;
          ledF[i].g = max_brightness;
          ledF[i].b = max_brightness;
        }
      }
      else if (ledF[i].wave == false) {
        ledF[i].r --;
        ledF[i].g --;
        ledF[i].b --;
        if (ledF[i].r < min_brightness || ledF[i].g < min_brightness || ledF[i].b < min_brightness) {
          ledF[i].wave = true;
          ledF[i].r = min_brightness;
          ledF[i].g = min_brightness;
          ledF[i].b = min_brightness;
        }
      }
      //Serial.println((ledF[i].brightness / 100.0));
      pixels.setPixelColor(i, pixels.Color(
                             ledF[i].r * (ledF[i].brightness / 100.0),
                             ledF[i].g * (ledF[i].brightness / 100.0),
                             ledF[i].b * (ledF[i].brightness / 100.0)
                           ));
    }

    //--- キューブの5個 ---
    for (int i = 10; i < 15; i++) {
      if (ledF[i].wave == false) {
        //ledF[i].r --;
        ledF[i].g --;
        ledF[i].b --;
        if (ledF[i].b < min_brightness) {
          ledF[i].wave = true;
          ledF[i].r = min_brightness;
          ledF[i].g = min_brightness;
          ledF[i].b = min_brightness;
        }
      }

      pixels.setPixelColor(i, pixels.Color(
                             ledF[i].r * (ledF[i].brightness / 100.0),
                             ledF[i].g * (ledF[i].brightness / 100.0),
                             ledF[i].b * (ledF[i].brightness / 100.0)
                           ));
    }

    pixels.show();
    delay(30);
    counter++;
  }
}
