#ifndef UM982_h
#define UM982_h
#include"Arduino.h"

class um982 {
    public:
        um982(Stream* gpsPort);
        bool poll();

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

    private:
        Stream* gpsPort;
};
#endif