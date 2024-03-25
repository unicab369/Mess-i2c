class I2CInterface {
    TwoWire *wire;
    uint8_t address;

    public:
        void begin(TwoWire *_wire, uint8_t _addr) {
            wire = _wire;
            address = _addr;
        }
        
        bool checkDevice() {
            // uint32_t time = millis();
            wire->beginTransmission(address);
            uint8_t error = wire->endTransmission();
            // uint32_t timeDif = millis() - time;
            return error == 0;
        }

};

#include "Mod_RTC.h"
#include "SensorBase.h"
#include "Mod_SHT.h"
#include "Mod_BH17.h"
#include "Mod_INA219.h"
#include "Mod_VL53L0X.h"
#include "Mod_MPU6050.h"

class Mng_Sensor {
    Mod2_SHT3 sht;
    Mod2_BH17 bh17;
    Mod_INA219 ina219;
    SensorBase *sensors[2];

    Mod_VL53LOX lox;
    // Mod_MPU6050 mpu;
    bool isLoaded = false;
    char output[32];

    public:
        void setup(TwoWire *wire) {
            sensors[0] = &sht;
            sensors[1] = &bh17;
            for (int i=0; i<2; i++) sensors[i]->setup(wire);
            isLoaded = true;
        }

        void getTempHumLux(float *temp, float *hum, float *lux) {
            *temp = sht.getTemp();
            *hum = sht.getHum();
            *lux = bh17.getLux();
        }

        char* getTempHumLux(bool markup = true) {
            if (!isLoaded) return "<null>";
            sprintf(output, markup ? "T%.2f H%.2f L%.2f" : "%.2f %.2f %.2f", 
                    sht.getTemp(), sht.getHum(), bh17.getLux());
            return output;
        }

        void requestReadings() {
            if (!isLoaded) return;
            for (int i=0; i<2; i++) sensors[i]->requestReadings();
        }

        void collectReadings() {
            if (!isLoaded) return;
            for (int i=0; i<2; i++) sensors[i]->getReading();
        }
        
        // bool checkStatus() {
        //     // lox.run();
        //     // mpu.run();
        // }
};
