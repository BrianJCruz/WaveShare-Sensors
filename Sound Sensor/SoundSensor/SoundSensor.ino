class SoundSensor {
  public:
    uint8_t AOUT, DOUT; // Analog and digital output pins
    int threshold; // Noise threshold
    int lastValue; // Last noise value

    // Constructor to initialize the sensor
    SoundSensor(uint8_t AOUT, uint8_t DOUT) {
      this->AOUT = AOUT;
      this->DOUT = DOUT;
      pinMode(AOUT, INPUT); // Set the analog output pin as input
      pinMode(DOUT, INPUT); // Set the digital output pin as input
      threshold = 512; // Set default threshold to half of maximum value (1023/2)
      lastValue = 0; // Initialize last noise value to 0
    }

    // Method to set noise threshold
    void setThreshold(int threshold) {
      this->threshold = threshold;
    }

    // Method to get noise frequency
    int getNoise() {
      int value = analogRead(AOUT); // Read the analog value from the sensor
      if (digitalRead(DOUT) == LOW) { // If there is noise detected
        return value; // Return the noise frequency
      } else {
        return 0; // Otherwise, return 0
      }
    }

    // Method to check if noise is above threshold
    bool isAboveThreshold() {
      return getNoise() > threshold;
    }

    // Method to check if noise is below threshold
    bool isBelowThreshold() {
      return getNoise() < threshold;
    }

    // Method to detect changes in sound intensity
    bool hasSoundIntensityChanged(int delta) {
      int currentValue = getNoise(); // Get current noise value
      if (abs(currentValue - lastValue) > delta) { // If the difference between current and last value is greater than delta
        lastValue = currentValue; // Update last value
        return true; // Return true to indicate that sound intensity has changed
      } else {
        lastValue = currentValue; // Update last value
        return false; // Otherwise, return false to indicate that sound intensity has not changed
      }
    }
};
