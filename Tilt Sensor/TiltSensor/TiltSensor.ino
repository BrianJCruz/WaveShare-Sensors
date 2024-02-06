class TiltSensor {
public:
  uint8_t DOUT;
  const int NUM_READINGS = 10; // Number of readings for averaging
  const int TILT_THRESHOLD = 5; // Tilt threshold in degrees

  TiltSensor(uint8_t DOUT) {
    this->DOUT = DOUT;
    pinMode(DOUT, INPUT_PULLUP); // Enable internal pull-up resistor for the DOUT pin
  }

  // Check if the tilt sensor is shaking
  bool isShaking() {
    int avgReading = getAverageReading();
    return abs(avgReading) > TILT_THRESHOLD;
  }

  // Get the tilt angle in degrees
  int getTiltAngle() {
    return getAverageReading();
  }

  // Get the tilt direction (LEFT, RIGHT, or NONE)
  String getTiltDirection() {
    int avgReading = getAverageReading();
    if (avgReading < -TILT_THRESHOLD) {
      return "LEFT";
    } else if (avgReading > TILT_THRESHOLD) {
      return "RIGHT";
    } else {
      return "NONE";
    }
  }

private:
  // Get the average reading from the tilt sensor
  int getAverageReading() {
    int total = 0;
    for (int i = 0; i < NUM_READINGS; i++) {
      total += digitalRead(DOUT); // Read the digital value from the DOUT pin
      delay(10); // Add a small delay between readings for stability
    }
    int avg = total / NUM_READINGS;
    // Map the digital reading to an angle between -90 and 90 degrees
    return map(avg, 0, 1, -90, 90);
  }
};
