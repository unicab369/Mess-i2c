class Mod_INA219: public SensorBase, public Interface_Current {
   public:
      //! addr 0x40
      Mod_INA219(): SensorBase(0x40), Interface_Current() {}

      uint16_t setup(TwoWire *wire) override {
         uint8_t cmd1[] = { 0x05 };
         uint16_t value = _setup(wire, cmd1, 1);

         //! Reset
         write2Bytes(0x00, 0x8000);

         uint16_t regRead;
         read2Bytes(0x00, regRead);

         //! setADCMode
         uint8_t BIT_MODE_12 = 0b00000011;
         regRead &= ~(0x0F78);
         regRead |= (BIT_MODE_12 << 7) | (BIT_MODE_12 << 3);

         // //! setMeasureMode
         uint8_t CONTINUOUS_MODE = 0b00000111;
         regRead &= ~(0x0007);
         regRead |= CONTINUOUS_MODE;

         //! setPGain
         uint16_t PG_320 = 0x1800;
         regRead &= ~(0x1800);
         regRead |= PG_320;

         //! setBusRange
         uint16_t BRNG_32 = 0x2000;
         regRead &= ~(0x2000);
         regRead |= BRNG_32;
         
         write2Bytes(0x00, regRead);
         Serial.printf("\n***nINA29_CONFIG_REG = 0x%04X", regRead);

         //! set calibrator
         uint16_t calRead;
         read2Bytes(0x05, calRead);
         Serial.printf("\n***nINA29_CAL_REG = 0x%04X", calRead);

         write2Bytes(0x05, 0x1000);
         read2Bytes(0x05, calRead);
         Serial.printf("\n***nINA29_CAL_REG2 = 0x%04X", calRead);
                  
         return 1;
      }

      // bool requestReadings() override {
      //    uint8_t readCmd[] = { 0x02 };
      //    return _requestReadings(readCmd, 2, 2);
      // }

      bool getReading() override {
         uint16_t busVoltage, mA;

         read2Bytes(0x02, busVoltage);
         float busVal = (busVoltage >> 3) * 4;
         setBusVoltage(busVal/1000);

         read2Bytes(0x04, mA);
         float maVal = (int16_t)mA/10.0;
         setCurrent(maVal);
         return true;
      }
};