#ifndef __PLEDH__
#define __PLEDH__
#include "Arduino.h"

enum LED_STATUS{
    LED_INIT,
    LED_ERROR,
    LED_GREEN
};

class PowerLed{
private:
    int red;
    int green;
public:
    PowerLed(int red, int green){
        this->red = red;
        this->green = green;
    }
    static void changeStatus(LED_STATUS status,int red, int green);
    static String ledStatusToString(LED_STATUS ls);
    void changeStatus(LED_STATUS status);
};

#endif