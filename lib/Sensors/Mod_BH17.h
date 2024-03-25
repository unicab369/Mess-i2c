byte BH17_POWER_DOWN[1] = { 0x00 };
byte BH17_POWER_ON[1]   = { 0x01 };
byte BH17_RESET[1]      = { 0x07 };

byte BH17_CONT_HI1[1]   = { 0x10 };   // 1 lux resolution 120ms
byte BH17_CONT_HI2[1]   = { 0x11 };   // .5 lux resolution 120ms
byte BH17_CONT_LOW[1]   = { 0x13 };   // 4 lux resolution 16ms
byte BH17_ONCE_HI1[1]   = { 0x20 };   // 1 lux resolution 120ms
byte BH17_ONCE_HI2[1]   = { 0x21 };   // .5 lux resolution 120ms
byte BH17_ONCE_LOW[1]   = { 0x23 };   // 4 lux resolution 16ms

byte BH17_BEGIN[2]      = { 0x01, 0x23 };
#define BH17_DEFAULT_MTREG 69

class Mod2_BH17: public SensorBase, public Interface_Lux {
    public:
        //! addr 0x23
        Mod2_BH17(): SensorBase(0x23), Interface_Lux() {}

        uint16_t setup(TwoWire *wire) override {
            uint16_t value = _setup(wire, BH17_CONT_LOW, 1);
            return value;
        }

        bool getReading() override {
            uint16_t value;
            readUint16(0, 0, value);

            if (value>10000000) { value = 0; }
            setLux(value/1.2);
            return true;
        }
};

#define APDS9930_ENABLE 0x00
#define AUTO_INCREMENT  0xA0
#define DEFAULT_ATIME 0xED
/* ALS coefficients */

#define DF      52
#define GA      0.49
#define ALS_B   1.862
#define ALS_C   0.746
#define ALS_D   1.291

byte APDS9930_START[2] = { APDS9930_ENABLE | AUTO_INCREMENT, 0x03 };

class Mod_APDS9930: public SensorBase, public Interface_Lux {

    public:
        //! addr
        Mod_APDS9930(): SensorBase(0x39), Interface_Lux() {}

        uint16_t setup(TwoWire *wire) override {
            // Enable Register (0x00)
            // Reserved SAI PIEN AIEN WEN PEN AEN(1) PON(1) - set bit AEN and PON to HIGH
            uint16_t value = _setup(wire, APDS9930_START, 2);

            uint8_t reg0;
            readByte(0x00 | 0xA0, reg0);
            Serial.print("\nEnable_Reg = "); printBinary(reg0);

            // APDS9930_ATIME
            uint8_t writeVal[2] = { 0x01 | 0xA0, 0xFF };
            writeBuffer(writeVal, 2);

            uint8_t reg1;
            readByte(0x01 | 0xA0, reg1);
            Serial.printf("\nReg1 = %02X", reg1);

            // APDS9930_WTIME
            uint8_t writeVal2[2] = { 0x03 | 0xA0, 0xFF };
            writeBuffer(writeVal2, 2);

            uint8_t reg2;
            readByte(0x03 | 0xA0, reg2);
            Serial.printf("\nReg2 = %02X", reg2);

            // DEFAULT_PPULSE
            uint8_t writeVal3[2] = { 0x0E | 0xA0, 0x08 };
            writeBuffer(writeVal3, 2);

            uint8_t reg3;
            readByte(0x0E | 0xA0, reg3);
            Serial.printf("\nReg3 = %02X", reg3);

            return value;
        }

        bool getReading() override {
            uint8_t request1[2] = { 0x14 | AUTO_INCREMENT, 
                                    0x15 | AUTO_INCREMENT };
            uint8_t request2[2] = { 0x16 | AUTO_INCREMENT, 
                                    0x17 | AUTO_INCREMENT };

            uint16_t ch0, ch1;
            readUint16(request1, 2, ch0, true);
            readUint16(request2, 2, ch1, true);

            uint8_t x[4]={1,8,16,120};
            float ALSIT = 2.73 * (256 - DEFAULT_ATIME);
            float iac  = max(ch0 - ALS_B * ch1, ALS_C * ch0 - ALS_D * ch1);
            if (iac < 0) iac = 0;
            float lpc  = GA * DF / (ALSIT * x[0]);

            setLux(iac * lpc);
            return true;
        }
};

class Mod_Ch32v003: public SensorBase {
    public:
        //! addr 0xfa
        Mod_Ch32v003(): SensorBase(0x78) {}

        uint16_t setup(TwoWire *wire) override {
            uint16_t value = _setup(wire, BH17_POWER_ON, 1);
            return value;
        }

        uint8_t value = 0;

        bool requestReadings() override {
            thisWire->requestFrom(0x78, 10);
            uint8_t buf[10] = { 0 };

            Serial.println("***READ=");
            for (int i=0; i<10; i++) {
                buf[i] = thisWire->read();
                Serial.printf("%u, ", buf[i]);
            }
            Serial.println();

            thisWire->beginTransmission(0x78);
            thisWire->write(0x00);          // register location?
            thisWire->write(0x05);          // register new value?
            int err = thisWire->endTransmission();
            Serial.printf("\n*** SENSOR ERROR = %u", err);

            return false;
        }
};