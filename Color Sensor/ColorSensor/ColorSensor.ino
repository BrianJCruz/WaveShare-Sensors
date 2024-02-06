#include "Arduino.h"

class ColorSensor{

  public:
    
    // Colors to filter
    enum Color {RED, BLUE, GREEN, WHITE, NONE};

    // Frequency levels to recive
    enum Frequency {OFF, LOW1, NORMAL, FULL};
    uint8_t S0, S1, S2, S3, output;
    
    
    ColorSensor(uint8_t S0, uint8_t S1, uint8_t S2, uint8_t S3, uint8_t output, int freq = 3) : S0(S0), S1(S1), S2(S2), S3(S3), output(output) {

      pinMode(S0, OUTPUT);
      pinMode(S1, OUTPUT);
      pinMode(S2, OUTPUT);
      pinMode(S3, OUTPUT);
      pinMode(output, INPUT);

      setColorFrequency(freq);

    }

    // Controls the light intensity that is received/reflected (Increases or decreases the frequency).
    // ColorSensor::OFF,  ColorSensor::LOW1,  ColorSensor::NORMAL (Default),  ColorSensor::FULL
    void setColorFrequency(Frequency option){
      
      switch(option){

        case OFF:
          digitalWrite(S0, LOW);
          digitalWrite(S1, LOW);
          break;
        
        case LOW1:
          digitalWrite(S0, LOW);
          digitalWrite(S1, HIGH);
          break;
        
        case NORMAL:
          digitalWrite(S0, HIGH);
          digitalWrite(S1, LOW);
          break;
        
        case FULL:
          digitalWrite(S0, HIGH);
          digitalWrite(S1, HIGH);
          break;

        default:
          digitalWrite(S0, HIGH);
          digitalWrite(S1, HIGH);
          break;
      }

    }

    // Read the frequency of the desired color.
    // color: ColorSensor::RED, ColorSensor::BLUE, ColorSensor::GREEN, ColorSensor::CLEAR.
    int getColorFreq(Color color){
      
      switch(color){

        case RED:
          digitalWrite(S2, LOW);
          digitalWrite(S3, LOW);
          break;

        case BLUE:
          digitalWrite(S2, LOW);
          digitalWrite(S3, HIGH);
          break;

        case GREEN:
          digitalWrite(S2, HIGH);
          digitalWrite(S3, HIGH);
          break;

        case WHITE:
          digitalWrite(S2, HIGH);
          digitalWrite(S3, LOW);
          break;
      }

      unsigned long duration;

      duration = pulseIn(output, LOW);

      if (duration != 0) {
        return 1000 / duration;
      } else {
        return 0;
      }
  }

  // Returns a Color type enumarator { RED,  GREEN,  BLUE,  WHITE,  NONE }.
  Color detectColor(){

    int red, green, blue, white;

    red = getColorFreq(RED);
    green = getColorFreq(GREEN); 
    blue =  getColorFreq(BLUE);
    white = getColorFreq(WHITE);
    

    if (white > 55 && white <= 200) {
      return WHITE;

    } else if ( (red > 12  && red < 24  && green >= 7 && green < 14 && blue > 7 && blue < 12) ) {
      return RED;

    } else if ( (blue >= 10  && blue <= 17  && green >= 5 && green < 17 && (red >= 6 && red < 17) || (red >= 10 && red <= 12) ) ) {
      return BLUE;

    } else if ( ( (green >= 15 && green < 30) || green == blue || green == blue - 1 || blue == green - 1 ) && red > 8 && red <= 19 && blue >= 8 && blue <= 27) {
      return GREEN;

    } else {
      return NONE;
    }

  }

  // Returns the average of the colors detected to get a more precise result. Use this instead of "detectColor()".
  Color getAverageColor() {

    int colorList[5];
    int totalSum, color; 

    for(int i = 0; i < 5; i++) {
      
      colorList[i] = detectColor();
      totalSum += colorList[i];
    }

    color = totalSum / 5;

    switch(color) {

      case 0:
        return RED;
        break;
      case 1:
        return BLUE;
        break;
      case 2:
        return GREEN;
        break;
      case 3:
        return WHITE;
        break;
    }
  }

};
