enum DisplayMode {
    DISPLAY_2ND,
    DISPLAY_DEFAULT
};

struct DispItem {
    char data[22];
    uint8_t line;

    bool isEmpty() { return strcmp(data, "") == 0; }

    void makeEmpty() {
        strcpy(data, "");
    }

    static DispItem make(const char* str, uint8_t line) {
        DispItem output;
        output.line = line;
        memcpy(output.data, str, sizeof(data));
        return output;
    }
};

#include "AS5600.h"

AS5600 as5600;   //  use default Wire

class Mod_AS5600: private Loggable {
    public:
        Mod_AS5600(): Loggable("AS5600") {}

        void setup(TwoWire *wire = &Wire) {
            // as5600 = AS5600(wire);
            as5600.begin();
            
            xLogStatus("AS56002 = ", as5600.isConnected());
            Serial.print("AS5600 = "); Serial.println(as5600.isConnected());
        }

        void run() {
            xLog("Value = %f", as5600.getAngularSpeed(AS5600_MODE_RPM));
        }
};

class Serv_I2C {
    public:
        Serv_I2C() {}

        Disp_SSD13062 disp;
        DisplayMode dispMode = DISPLAY_DEFAULT;
        Mng_Sensor sensors;
        Mod_RTC rtc;
        PCA96Controller pca96;
        Mod_Ch32v003 ch32v;
        Mod_AS5600 as5600;
        TwoWire *thisWire;

        bool isLoaded = false;
    
        void setup(int scl, int sda, TwoWire *wire = &Wire) {
            bool error = scl == 255 || sda == 255;
            // xLogStatus(__func__, !error);
            if (error) return;

            thisWire = wire;
            wire->begin(sda, scl);
            sensors.setup(wire);
            ch32v.setup(wire);
            as5600.setup(wire);

            rtc.setup(wire);
            pca96.setup(wire);
            disp.setup(wire);
            disp.printline("Hello World!", 0);
            isLoaded = true;
        }

        void scanAll() {
            if (!isLoaded) return;
            byte error, addr;
            unsigned long ref = millis();

            for (addr=2; addr<127; addr++) {
                thisWire->beginTransmission(addr);
                error = thisWire->endTransmission();

                if (error == 0) {
                    // xLogf("Device found. Addr = 0x%02X", addr);
                } else if (error == 4) {
                    // xLogf("Unknown Error. Addr = 0x%02X", addr);
                }
            }

            unsigned long dif = millis() - ref;
            // xLogf("Scan Time = %lu", dif);
        }
        
        void switchDisplayMode() {
            // xLog(__func__);
            dispMode = (dispMode == DISPLAY_DEFAULT) ? DISPLAY_2ND : DISPLAY_DEFAULT;
            disp.clear();
        }

        uint16_t value = 0;

        //! Run
        void run() {
            // pca96.test();

            // pca96.drivePWM(0, value);
            // value+=10;
            // value = value>4000 ? 0 : value;
        }
};