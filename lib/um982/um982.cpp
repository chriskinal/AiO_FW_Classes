#include"Arduino.h"
#include "um982.h"

char msgBuf[384];
int msgBufLen = 0;
bool gotDollar = false;
bool gotCR = false;
bool gotLF = false;

um982::um982(Stream* gpsPort) : gpsPort(gpsPort) {}

void um982::poll(){
    
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
        for ( int i = 0; i <= argc - 1; i++)
        {
            Serial.println(argv[i]);
        }
        // Reset parser
        Serial.println("Reset Parser");
        gotCR = false;
        gotLF = false;
        gotDollar = false;
        memset(msgBuf, 0, 384);
        msgBufLen = 0;
    }
}