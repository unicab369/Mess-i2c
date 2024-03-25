#include "Adafruit_VL53L0X.h"

class Mod_VL53LOX {
    Adafruit_VL53L0X lox = Adafruit_VL53L0X();
    VL53L0X_RangingMeasurementData_t measure;
    bool configured = false;

    public:
        //! Addr 0x29
        void setup(TwoWire *wire = &Wire) {
            if (!lox.begin(0x29, wire)) { 
                Serial.println("[Mod_VL53LOX]: Error");
                return; 
            }
            configured = true;
        }

        void run() {
            if (!configured) { return; }

            lox.rangingTest(&measure, false);    // pass in 'true' to get debug data printout!

            if (measure.RangeStatus != 4) {  // phase failures have incorrect data
                Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
            } else {
                Serial.println(" out of range ");
            }
        }
};