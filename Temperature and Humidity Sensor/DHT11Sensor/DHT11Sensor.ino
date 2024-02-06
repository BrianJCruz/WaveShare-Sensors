class DHT11Sensor {

  public: 
  
    enum ERROR { DHTLIB_ERROR_TIMEOUT, DHTLIB_ERROR_CHECKSUM, DHTLIB_OK };

    float humidity, temperature;
    uint8_t DOUT;
	
	// MAIN CONSTRUCTOR
    DHT11Sensor(uint8_t DOUT){
      
      this-> DOUT = DOUT;
      
    }

	// GETTER - HUMIDITY FIELD
    float getHumidity(){
      return this->humidity;
    }
	
	// GETTER - TEMPERATURE FIELD
    float getTemperature(){
      return this->temperature;
    }

    // Get the sensor's status and data
    int read() {
      // BUFFER TO RECEIVE
      uint8_t bits[5];
      uint8_t cnt = 7;
      uint8_t idx = 0;

      // EMPTY BUFFER
      for (int i=0; i< 5; i++) bits[i] = 0;

      // REQUEST SAMPLE
      pinMode(DOUT, OUTPUT);
      digitalWrite(DOUT, LOW);
      delay(18);
      digitalWrite(DOUT, HIGH);
      delayMicroseconds(40);
      pinMode(DOUT, INPUT);

      // ACKNOWLEDGE or TIMEOUT
      unsigned int loopCnt = 10000;
      while(digitalRead(DOUT) == LOW)
        if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

      loopCnt = 10000;
      while(digitalRead(DOUT) == HIGH)
        if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

      // READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
      for (int i=0; i<40; i++) {
        loopCnt = 10000;
        while(digitalRead(DOUT) == LOW)
          if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

        unsigned long t = micros();

        loopCnt = 10000;
        while(digitalRead(DOUT) == HIGH)
          if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

        if ((micros() - t) > 40) bits[idx] |= (1 << cnt);
        if (cnt == 0)   // next byte?
        {
          cnt = 7;    // restart at MSB
          idx++;      // next byte
        }
        else cnt--;
      }

      // WRITE TO RIGHT VARS
            // as bits[1] and bits[3] are allways zero they are omitted in formulas.
      humidity    = bits[0]; 
      temperature = bits[2]; 

      uint8_t sum = bits[0] + bits[2];  

      if (bits[4] != sum) return DHTLIB_ERROR_CHECKSUM;
      return DHTLIB_OK;
    }

    // Convert from Celcius to Farenheit
    static double toFahrenheit(double celsius)  {
            return 1.8 * celsius + 32;
    }   

    // Convert from Celcius to Kelvin
    static double toKelvin(double celsius) {
            return celsius + 273.15;
    }    

    // Calculate the dew Point at which air must be cooled to become saturated.
    static double getDewPoint(double celsius, double humidity) {
      
            double A0= 373.15/(273.15 + celsius);
            double SUM = -7.90298 * (A0-1);
            SUM += 5.02808 * log10(A0);
            SUM += -1.3816e-7 * (pow(10, (11.344*(1-1/A0)))-1) ;
            SUM += 8.1328e-3 * (pow(10,(-3.49149*(A0-1)))-1) ;
            SUM += log10(1013.246);
            double VP = pow(10, SUM-3) * humidity;
            double T = log(VP/0.61078);   // temp var
            return (241.88 * T) / (17.558-T);
    }

    // Second method to calculate the dew point faster but with less precision. 
    static double getDewPointFast(double celsius, double humidity) {

            double a = 17.271;
            double b = 237.7;
            double temp = (a * celsius) / (b + celsius) + log(humidity/100);
            double Td = (b * temp) / (a - temp);
            return Td;
    }

    // Display a custom message showing all necessary data.
    // freq -> frequency in miliseconds.
    void toString(int freq = 2000) {

      Serial.println("\n");
      
      int chk = read();

      Serial.print("Read sensor: ");
      switch (chk) {
        case DHTLIB_OK: 
                    Serial.println("OK"); 
                    break;
        case DHTLIB_ERROR_CHECKSUM: 
                    Serial.println("Checksum error"); 
                    break;
        case DHTLIB_ERROR_TIMEOUT: 
                    Serial.println("Time out error"); 
                    break;
        default: 
                    Serial.println("Unknown error"); 
                    break;
      }

      Serial.print("Humidity (%): ");
      Serial.println( humidity , 2);

      Serial.print("Temperature (oC): ");
      Serial.println( temperature , 2);

      Serial.print("Temperature (oF): ");
      Serial.println( toFahrenheit(temperature), 2);

      Serial.print("Temperature (K): ");
      Serial.println( toKelvin(temperature), 2);

      Serial.print("Dew Point (oC): ");
      Serial.println( getDewPoint(temperature, humidity) );

      Serial.print("Dew PointFast (oC): ");
      Serial.println( getDewPointFast(temperature, humidity) );

      delay(freq);

    }
};
