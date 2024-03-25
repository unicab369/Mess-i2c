#include <Adafruit_MPU6050.h>

class Mod_MPU6050 {
    Adafruit_MPU6050 mpu;
    sensors_event_t a, g, temp;
    bool configured = false;

    // addr 0x68
    public:
        void setup(TwoWire *wire = &Wire) {
            if (!mpu.begin()) {
                Serial.println("[MPU6050] init failed!");
                return;
            }   

            Serial.println("[MPU6050] Started");
            configured = true;
        }

        void run() {
            // if(!mpu.getMotionInterruptStatus()) { return; }
            mpu.getEvent(&a, &g, &temp);

            Serial.print("[MPU6050] Accelerometer (m/s^2) "); 
            Serial.print("X: "); Serial.print(a.acceleration.x, 1); 
            Serial.print(", Y: "); Serial.print(a.acceleration.y, 1); 
            Serial.print(", Z: "); Serial.println(a.acceleration.z, 1);

            Serial.print("[MPU6050] Gyroscope (rps) ");
            Serial.print("X: "); Serial.print(g.gyro.x, 1);
            Serial.print(", Y: "); Serial.print(g.gyro.y, 1);
            Serial.print(", Z: "); Serial.println (g.gyro.z, 1);
        }
};