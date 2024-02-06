class InfraredRSensor {

  public:

    uint8_t DOUT, AOUT;

    InfraredRSensor(uint8_t DOUT, uint8_t AOUT) {
      
      this-> DOUT = DOUT;
      this-> AOUT = AOUT;

      pinMode( this->AOUT, INPUT );
      pinMode( this->AOUT, INPUT );

    }

    // Returns true if there's an object detected within the range.
    bool isObstacle() {

      if( digitalRead(DOUT) == LOW ) {
        return true;
      } else {
        return false;
      }
    }

    int getReflection(){
      return AOUT;
    } 



};
