#include"Arduino.h"
#include "um982.h"

um982::um982(Stream* gpsPort) : gpsPort(gpsPort) {}

const char* um982::poll(){
    retVal = 0;
    if (gpsPort->available())
    {
        char inChar = gpsPort->read();
        switch (inChar)
        {
            case '$':
                msgBuf[msgBufLen] = inChar;
                msgBufLen++;
                gotDollar = true;
                break;
            case '\r':
                msgBuf[msgBufLen] = inChar;
                msgBufLen++;
                gotCR = true;
                gotDollar = false;
                break;
            case '\n':
                msgBuf[msgBufLen] = inChar;
                msgBufLen++;
                gotLF = true;
                gotDollar = false;
                break;
            default:
                if (gotDollar)
                {
                    msgBuf[msgBufLen] = inChar;
                    msgBufLen++;
                }
                break;
        }
    }
    if ( gotCR && gotLF){
        char *argv[25]; // Max 25 arguments
        int argc = 0;
        char *bufptr = msgBuf;
        argv[argc] = strsep(&bufptr, "*;,");
        while ((argv[argc] != NULL) && (argc < 24))
        {
            argc++;
            argv[argc] = strsep(&bufptr, "*;,");
        }
        // for ( int i = 0; i <= argc - 1; i++)
        // {
        //     Serial.println(argv[i]);
        // }
        if (strstr(argv[0], "GGA")) {
        strcpy(GGA.fixTime, argv[1]);
        strcpy(GGA.latitude, argv[2]);
        strcpy(GGA.latNS, argv[3]);
        strcpy(GGA.longitude, argv[4]);
        strcpy(GGA.lonEW, argv[5]);
        strcpy(GGA.fixQuality, argv[6]);
        strcpy(GGA.numSats, argv[7]);
        strcpy(GGA.HDOP, argv[8]);
        strcpy(GGA.altitude, argv[9]);
        strcpy(GGA.ageDGPS, argv[13]);
        retVal = 1;
        }
        if (strstr(argv[0], "VTG"))
        {
            strcpy(VTG.heading, argv[1]);
            strcpy(VTG.speedKnots, argv[5]);
            VTG.speed = atof(argv[5]) * 1852 / 3600; // m/s
            retVal = 2;
        }
        if (strstr(argv[0], "HPR"))
        {
            strcpy(HPR.heading, argv[2]);
            strcpy(HPR.roll, argv[3]);
            HPR.solQuality = atoi(argv[5]);
            retVal = 3;
        }
        // Reset parser
        // Serial.println("Reset Parser");
        gotCR = false;
        gotLF = false;
        gotDollar = false;
        memset(msgBuf, 0, 384);
        msgBufLen = 0;
    }
    switch (retVal)
    {
    case 0:
        return "";
    case 1:
        return "GGA";
    case 2:
        return "VTG";
    case 3:
        return "HPR";
    }
}