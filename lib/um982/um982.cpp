#include "Arduino.h"
#include "um982.h"

um982::um982(Stream *gpsPort) : gpsPort(gpsPort) {}

    void um982::poll()
{
    if (gpsPort->available())
    {
        char inChar = gpsPort->read();
        switch (inChar)
        {
        case '$':
            if (msgBufLen < MSG_BUF_SIZE - 1)
            {
                msgBuf[msgBufLen++] = inChar;
            }
            gotDollar = true;
            break;
        case '\r':
            if (msgBufLen < MSG_BUF_SIZE - 1)
            {
                msgBuf[msgBufLen++] = inChar;
            }
            gotCR = true;
            gotDollar = false;
            break;
        case '\n':
            if (msgBufLen < MSG_BUF_SIZE - 1)
            {
                msgBuf[msgBufLen++] = inChar;
            }
            gotLF = true;
            gotDollar = false;
            break;
        default:
            if (gotDollar && msgBufLen < MSG_BUF_SIZE - 1)
            {
                msgBuf[msgBufLen++] = inChar;
            }
            break;
        }
    }

    if (gotCR && gotLF)
    {
        msgBuf[msgBufLen] = '\0'; // Null-terminate buffer
        char *argv[25] = {nullptr};
        int argc = 0;
        char *bufptr = msgBuf;
        argv[argc] = strsep(&bufptr, "*;,");
        while (argv[argc] != nullptr && argc < 24)
        {
            argc++;
            argv[argc] = strsep(&bufptr, "*;,");
        }

        if (argc > 13 && argv[0] && strstr(argv[0], "GGA"))
        {
            strncpy(GGA.fixTime, argv[1] ? argv[1] : "", sizeof(GGA.fixTime) - 1);
            strncpy(GGA.latitude, argv[2] ? argv[2] : "", sizeof(GGA.latitude) - 1);
            strncpy(GGA.latNS, argv[3] ? argv[3] : "", sizeof(GGA.latNS) - 1);
            strncpy(GGA.longitude, argv[4] ? argv[4] : "", sizeof(GGA.longitude) - 1);
            strncpy(GGA.lonEW, argv[5] ? argv[5] : "", sizeof(GGA.lonEW) - 1);
            strncpy(GGA.fixQuality, argv[6] ? argv[6] : "", sizeof(GGA.fixQuality) - 1);
            strncpy(GGA.numSats, argv[7] ? argv[7] : "", sizeof(GGA.numSats) - 1);
            strncpy(GGA.HDOP, argv[8] ? argv[8] : "", sizeof(GGA.HDOP) - 1);
            strncpy(GGA.altitude, argv[9] ? argv[9] : "", sizeof(GGA.altitude) - 1);
            strncpy(GGA.ageDGPS, argv[13] ? argv[13] : "", sizeof(GGA.ageDGPS) - 1);
            if (ggaCallback)
                ggaCallback(GGA);
        }
        else if (argc > 5 && argv[0] && strstr(argv[0], "VTG"))
        {
            strncpy(VTG.heading, argv[1] ? argv[1] : "", sizeof(VTG.heading) - 1);
            strncpy(VTG.speedKnots, argv[5] ? argv[5] : "", sizeof(VTG.speedKnots) - 1);
            VTG.speed = argv[5] ? atof(argv[5]) * 1852 / 3600 : 0.0f; // m/s
            if (vtgCallback)
                vtgCallback(VTG);
        }
        else if (argc > 5 && argv[0] && strstr(argv[0], "HPR"))
        {
            strncpy(HPR.heading, argv[2] ? argv[2] : "", sizeof(HPR.heading) - 1);
            strncpy(HPR.roll, argv[3] ? argv[3] : "", sizeof(HPR.roll) - 1);
            HPR.solQuality = argv[5] ? atoi(argv[5]) : 0;
            if (hprCallback)
                hprCallback(HPR);
        }

        // Reset parser state
        gotCR = gotLF = gotDollar = false;
        memset(msgBuf, 0, sizeof(msgBuf));
        msgBufLen = 0;
    }
}