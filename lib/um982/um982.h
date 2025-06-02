#ifndef UM982_h
#define UM982_h
#include"Arduino.h"

class um982 {
    public:
        um982(Stream* gpsPort);
        void poll();

    private:
        Stream* gpsPort;
};
#endif