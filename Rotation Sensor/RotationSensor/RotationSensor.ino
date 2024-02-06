class RotationSensor {

public:
	
  // Fields
  uint8_t SIA, SIB, SW;

  // Constructor
  RotationSensor(uint8_t SIA, uint8_t SIB, uint8_t SW) {

    this-> SIA = SIA;
    this-> SIB = SIB;
    this-> SW = SW;

    pinMode(SIA, INPUT);
    pinMode(SIB, INPUT);
    pinMode(SW, INPUT);
	
	// When the voltage of the SIA and SIB pins change, interrupt and call rotationISR or reverseRotationISR respectively 
    attachInterrupt(digitalPinToInterrupt(SIA), rotationISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(SIB), reverseRotationISR, CHANGE);
  
    setInstance(this);
	
	// Get rotation time to determine the direction and the increse or decrease in degrees
    time = millis();
    
  }

  // Retrieve the rotation value.
  int getDegrees() {

    if ( ( digitalRead(SW) == LOW ) && i == 1) {
      i = 0;
      count = 0;
      while (digitalRead(SW) == LOW);
    }

    if (num != count) {
      i = 1;
      num = count;
      return num;
    }

  }

  // Show the degrees in the Serial Monitor.
  void displayDegrees(bool state){

    if (state) {
      if (count != serialValue) {
        Serial.println(count);
        serialValue = count;
      }
    }

  }

  static void setInstance(RotationSensor* sensor) {
    instance = sensor;
  }

private:

  static RotationSensor* instance;
  unsigned long time = 0;
  long count = 0;
  long num = 0;
  long previousCount = 0;
  int i = 0;
  int serialValue = 0;

  // Since the rotation method is set to private and attachInterrupt() requires a static method,
  // this method calls rotation() method without setting rotation as static
  static void rotationISR() {
    if (instance)
      instance->rotation();
  }
	
  // Since the rotation method is set to private and attachInterrupt() requires a static method,
  // this method calls reverseRotation() method without setting rotation as static
  static void reverseRotationISR() {
    if (instance)
      instance->reverseRotation();
  }

  void rotation() {
    previousCount = count - 1;
    if ( (millis() - time) > 15 && count != previousCount) {
      count++;
    }
    time = millis();
  }

  void reverseRotation() {

    previousCount = count + 1;
    if ( (millis() - time) > 15 && count != previousCount) {
      count--;
    }
    time = millis();
  }
};

RotationSensor* RotationSensor::instance = nullptr;
