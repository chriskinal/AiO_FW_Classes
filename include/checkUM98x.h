void checkUM98x()
{
    uint32_t baudrate = 0;
    int bytesRead = 0;
    for (uint32_t i = 0; i < nrBaudrates; i++)
    {
        baudrate = baudrates[i];
        Serial.print("Checking for UM98x at baudrate: ");
        Serial.println(baudrate);
        SerialGPS1.begin(baudrate);
        // Increase the size of the serial buffer to hold longer UM981 config messages
        SerialGPS1.addMemoryForRead(tmpGPSrxbuffer, tmp_serial_buffer_size);
        SerialGPS1.addMemoryForWrite(tmpGPStxbuffer, tmp_serial_buffer_size);
        delay(100);
        SerialGPS1.write("VERSION\r\n");
        delay(100);
        while (SerialGPS1.available())
        {
            char incoming[tmp_serial_buffer_size];

            bytesRead = SerialGPS1.readBytesUntil('\n', incoming, tmp_serial_buffer_size);
            if (strstr(incoming, "UM981") != NULL) // Got a UM981 version line
            {
                Serial.print("UM981 VERSION: ");
                Serial.println(incoming);
                usingUM981 = true;
                if (baudrate != 460800)
                {
                    Serial.println("UM981 baudrate wrong for AOG. Setting to 460800 bps for AOG");
                    SerialGPS1.write("CONFIG COM1 460800\r\n");
                    delay(100);
                    SerialGPS1.write("CONFIG COM2 460800\r\n");
                    delay(100);
                    SerialGPS1.write("CONFIG COM3 460800\r\n");
                    delay(100);
                    SerialGPS1.begin(baudGPS);
                    SerialGPS1.addMemoryForRead(GPS1rxbuffer, sizeof(GPS1rxbuffer));
                    SerialGPS1.addMemoryForWrite(GPS1txbuffer, sizeof(GPS1txbuffer));
                    delay(100);
                    SerialGPS1.write("SAVECONFIG\r\n");
                }

                break;
            }
            if (usingUM981)
            {
                break;
            }
            if (strstr(incoming, "UM982") != NULL) // Got a UM982 version line
            {
                Serial.print("UM982 VERSION: ");
                Serial.println(incoming);
                usingUM982 = true;
                if (baudrate != 460800)
                {
                    Serial.println("UM982 baudrate wrong for AOG. Setting to 460800 bps for AOG");
                    SerialGPS1.write("CONFIG COM1 460800\r\n");
                    delay(100);
                    SerialGPS1.write("CONFIG COM2 460800\r\n");
                    delay(100);
                    SerialGPS1.write("CONFIG COM3 460800\r\n");
                    delay(100);
                    SerialGPS1.begin(baudGPS);
                    SerialGPS1.addMemoryForRead(GPS1rxbuffer, sizeof(GPS1rxbuffer));
                    SerialGPS1.addMemoryForWrite(GPS1txbuffer, sizeof(GPS1txbuffer));
                    delay(100);
                    SerialGPS1.write("SAVECONFIG\r\n");
                }
                break;
            }
            if (usingUM982)
            {
                break;
            }
        }
        if (usingUM981 || usingUM982 || bytesRead > 2046)
        {
            break;
        }
    }
}
