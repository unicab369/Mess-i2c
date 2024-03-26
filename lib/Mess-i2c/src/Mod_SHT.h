byte SHT_RESET_CMD[2]           = { 0x30, 0xA2 } ;
byte SHT_HIGHREP_HOLD_CMD[2]    = { 0x2C, 0x06 };
byte SHT_MEDREP_HOLD_CMD[2]     = { 0x2C, 0x0D };
byte SHT_LOWREP_HOLD_CMD[2]     = { 0x2C, 0x10 };
byte SHT_HIGHREP_FREE_CMD[2]    = { 0x24, 0x00 };
byte SHT_MEDREP_FREE_CMD[2]     = { 0x24, 0x0B };
byte SHT_LOWREP_FREE_CMD[2]     = { 0x24, 0x16 };

class Mod2_SHT3: public SensorBase, public Interface_TempHum {
    public:
        //! addr 0x44
        Mod2_SHT3(): SensorBase(0x44), Interface_TempHum() {}

        uint16_t setup(TwoWire *wire) override {
            uint16_t value = _setup(wire, SHT_RESET_CMD, 2);
            return value;
        }

        bool requestReadings() override {
            writeBuffer(SHT_HIGHREP_FREE_CMD, 2);
            return true;
        }

        bool getReading() override {
            uint8_t buf[6] = { 0 };
            bool check2 = readBuffer(buf, 6);

            int32_t temp = makeUint16(buf[0], buf[1]);
            temp = ((4375 * temp) >> 14) - 4500;
            setTemp((float)temp / 100.0f);

            uint32_t hum = (uint32_t)makeUint16(buf[3], buf[4]);
            hum = (625 * hum) >> 12;
            setHum((float)hum / 100.0f);
            return true;
        }
};