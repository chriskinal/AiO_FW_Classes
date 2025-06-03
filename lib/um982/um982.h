#ifndef UM982_H
#define UM982_H

#include <Arduino.h>

// Data structures for NMEA sentences
struct GGA_DATA
{
    char fixTime[12] = {0};
    char latitude[15] = {0};
    char latNS[3] = {0};
    char longitude[15] = {0};
    char lonEW[3] = {0};
    char fixQuality[2] = {0};
    char numSats[4] = {0};
    char HDOP[5] = {0};
    char altitude[12] = {0};
    char ageDGPS[10] = {0};
};

struct VTG_DATA
{
    char heading[12] = {0};
    char speedKnots[10] = {0};
    float speed = 0.0f;
};

struct HPR_DATA
{
    char heading[8] = {0};
    char roll[8] = {0};
    int solQuality = 0;
};

class um982
{
public:
    explicit um982(Stream *gpsPort);
    void poll();

    // Callback typedefs
    typedef void (*GGA_Callback)(const GGA_DATA &);
    typedef void (*VTG_Callback)(const VTG_DATA &);
    typedef void (*HPR_Callback)(const HPR_DATA &);

    // Registration methods
    void onGGA(GGA_Callback cb) { ggaCallback = cb; }
    void onVTG(VTG_Callback cb) { vtgCallback = cb; }
    void onHPR(HPR_Callback cb) { hprCallback = cb; }

    GGA_DATA GGA;
    VTG_DATA VTG;
    HPR_DATA HPR;

private:
    Stream *gpsPort = nullptr;
    static constexpr size_t MSG_BUF_SIZE = 384;
    char msgBuf[MSG_BUF_SIZE] = {0};
    int msgBufLen = 0;
    bool gotDollar = false;
    bool gotCR = false;
    bool gotLF = false;
    int retVal = 0;

    GGA_Callback ggaCallback = nullptr;
    VTG_Callback vtgCallback = nullptr;
    HPR_Callback hprCallback = nullptr;
};

#endif // UM982_H