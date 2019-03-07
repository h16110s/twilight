enum READ_STATUS{
    RFID_None,
    RFID_ERROR,
    RFID_GET,
};

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

enum LED_STATUS{
    LED_INIT,
    LED_ERROR,
    LED_GREEN
};

String ledStatusToString(LED_STATUS ls){
    switch (ls)
    {
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
