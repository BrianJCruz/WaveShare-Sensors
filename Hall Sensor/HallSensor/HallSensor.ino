class HallSensor {

  public:
    uint8_t AOUT, DOUT;
    int readings;
	
	// Constructor
    HallSensor(uint8_t AOUT, uint8_t DOUT, int readings= 10) {

      pinMode(AOUT, INPUT);
      pinMode(DOUT, INPUT);
      
      this->AOUT = AOUT;
      this->DOUT = DOUT;

      this->setReadings(readings);

    }

    // Set the amount of readings
    void setReadings(int readings){
      this->readings = readings;
    }

    // Get raw data without the average.
    float getData() {
      return analogRead(AOUT);
    }

    // Convert the ADC analog input data to volts
    float getDataInVolts(float vcc  = 5.0) {
      return this->getData() * (vcc / 1023.0);
    }

    // Calculate the average value for set amount of readings.
    // This will return a more precise result.
    long getAverage() {
    
      long sum = 0;

      for (int index = 0; index < this->readings; index++) {
        
        sum += this->getData();
        delay(10);
      }
      return sum / this->readings;
    }

    // Convert the average into volts
    float getAverageInVolts(float vcc = 5.0) {
        return this->getAverage() * (vcc / 1023.0);
    }

    // Converts to Tesla.
    // 1 gauss = 0.1mT
    float getAverageInTesla(float vcc = 5.0) {
      return this->getAverage() * 0.1;
    }


};