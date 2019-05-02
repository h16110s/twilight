#include "PLED.h"


void PowerLed::changeStatus(LED_STATUS status){
    switch(status){
        case LED_INIT:
            // LED MODE CHANGE (Initialize Status) =============
            digitalWrite(this->green,HIGH);
            digitalWrite(this->red,HIGH);
            break;
        case LED_ERROR:
            // LED MODE CHANGE (Error Status) ==================
            digitalWrite(this->green, LOW);
            digitalWrite(this->red,HIGH);
            break;
        case LED_GREEN:
            // LED MODE CHANGE (Error Status) =============
            digitalWrite(this->green,HIGH);
            digitalWrite(this->red,LOW);
            break;
        case LED_OFF:
            digitalWrite(this->green,LOW);
            digitalWrite(this->red,LOW);
            break;
        case GREEN:
            digitalWrite(this->green,!digitalRead(this->green));
            break;
        case RED:
            digitalWrite(this->red,!digitalRead(this->red));
            break;
        default:
            break;
    }
}

static void PowerLed::changeStatus(LED_STATUS status,int red, int green){
    switch(status){
        case LED_INIT:
            // LED MODE CHANGE (Initialize Status) =============
            digitalWrite(green,HIGH);
            digitalWrite(red,HIGH);
            break;
        case LED_ERROR:
            // LED MODE CHANGE (Error Status) =============
            digitalWrite(green,LOW);
            digitalWrite(red,HIGH);
            break;
        case LED_GREEN:
            // LED MODE CHANGE (Error Status) =============
            digitalWrite(green,HIGH);
            digitalWrite(red,LOW);
            break;
        default:
            break;
    }
}

static String PowerLed::ledStatusToString(LED_STATUS ls){
    switch (ls){
        case LED_INIT:
            return "Init";
        case LED_ERROR:
            return "Read";
        case LED_GREEN:
            return "Green";
        default:
            break;
    }
}