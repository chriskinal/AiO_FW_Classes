#ifndef UM982_h
#define UM982_h
#include"Arduino.h"

class um982 {
    public:
        um982(Stream* gpsPort);
        const char* poll();

        struct GGA_DATA
        {
            char fixTime[12];
            char latitude[15];
            char latNS[3];
            char longitude[15];
            char lonEW[3];
            char fixQuality[2];
            char numSats[4];
            char HDOP[5];
            char altitude[12];
            char ageDGPS[10];
        };
        GGA_DATA GGA;
        struct VTG_DATA
        {
            char heading[12];
            char speedKnots[10];
            float speed;
        };
        VTG_DATA VTG;

        struct HPR_DATA
        {
            char heading[8];
            char roll[8];
            int solQuality;
        };
        HPR_DATA HPR;

    private:
        Stream* gpsPort;
        char msgBuf[384];
        int msgBufLen = 0;
        bool gotDollar = false;
        bool gotCR = false;
        bool gotLF = false;
        int retVal = 0;
};
#endif