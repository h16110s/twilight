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