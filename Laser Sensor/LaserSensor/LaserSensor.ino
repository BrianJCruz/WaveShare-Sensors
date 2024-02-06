class LaserSensor {
private:
  float sensorLength = 1.27; // Sensor's length used as refference
  float timeElapsed, objectSize;
  bool obstacleDetected = false; // Detection flag

public:
  uint8_t DOUT;

  // Constructor
  LaserSensor(uint8_t DOUT) {
    this->DOUT = DOUT;
    pinMode(DOUT, INPUT);

  }

  // Detect if there's an obstacle 
  bool isObstacle() {
    if (digitalRead(DOUT) == LOW) {
      return true;
    } else {
      return false;
    }
  }
  
  // Calculate the time elapsed until the object moves
  float getTimeElapsed() {
    float startTime, endTime;
    startTime = millis();

	// Wait until there is no obstacle
    while (isObstacle()) {
    }
	
	// Store the final time
    endTime = millis();

	// millis() returns the time elapsed since the program started
	// The time elapsed before and after the object moves = endTime - startTime
	// Then convert the time in seconds
    this->timeElapsed = (endTime - startTime) / 1000.0; 
    return this->timeElapsed;
  }
	
	// Set size for velocity calculation
	// Size can be either width or length
  void setObjectSize(float size) {
    this->objectSize = size;
  }

	// Getter
  float getObjectSize() {
    return this->objectSize;
  }

  // Calculate the velocity once the object is moved
  // objectSize is needed in order to correctly calculate the distance
  float getVelocity() {
    float timeElapsed = getTimeElapsed();
    if (timeElapsed > 0) {
      return (this->sensorLength + this->getObjectSize()) / timeElapsed;
    } else {
      return 0; 
    }
  }

};