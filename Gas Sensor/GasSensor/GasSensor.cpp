#include "Arduino.h"
#include "GasSensor.h"

GasSensor::GasSensor(int pin) {
	_pin = pin;
	Ro = -1.0;
}

void GasSensor::begin(){
	Ro = MQCalibration();
	Serial.print("Ro: ");
	Serial.print(Ro);
	Serial.println(" kohm");
}

void GasSensor::close(){
	Ro = -1.0;
	values[0] = 0.0;
	values[1] = 0.0;
	values[2] = 0.0;
}

bool GasSensor::checkCalibration() {
	if (Ro < 0.0) {
		Serial.println("Device not calibrated, call GasSensor::begin before reading any value.");
		return false;
	}

	return true;
}

float* GasSensor::read(bool print){
	if (!checkCalibration()) return NULL;

	values[0] = MQGetPercentage(LPGCurve);
	values[1] = MQGetPercentage(COCurve);
	values[2] = MQGetPercentage(SmokeCurve);

	lastReadTime = millis();

	if (print){
		Serial.print(lastReadTime);
		Serial.print("ms - LPG:");
		Serial.print(values[0], 5);
		Serial.print("ppm\t");
		Serial.print("CO:");
		Serial.print(values[1], 5);
		Serial.print("ppm\t");
		Serial.print("SMOKE:");
		Serial.print(values[2], 5);
		Serial.print("ppm\n");
	}

	return values;
}

float GasSensor::readLPG(){
	if (!checkCalibration()) return 0.0;

    if (millis() < (lastReadTime + READ_DELAY) && values[0] > 0)
        return values[0];
    else
        return (values[0] = MQGetPercentage(LPGCurve));
}

float GasSensor::readCO(){
	if (!checkCalibration()) return 0.0;

    if (millis() < (lastReadTime + READ_DELAY) && values[1] > 0)
        return values[1];
    else
        return (values[1] = MQGetPercentage(COCurve));
}

float GasSensor::readSmoke(){
	if (!checkCalibration()) return 0.0;

    if (millis() < (lastReadTime + READ_DELAY) && values[2] > 0)
        return values[2];
    else
        return (values[2] = MQGetPercentage(SmokeCurve));
}

float GasSensor::MQResistanceCalculation(int raw_adc) {
	float flt_adc = (float) raw_adc;
	return RL_VALUE * (1023.0 - flt_adc) / flt_adc;
}

float GasSensor::MQCalibration() {
	float val = 0.0;

	// take multiple samples
	for (int i = 0; i < CALIBARAION_SAMPLE_TIMES; i++) {
		val += MQResistanceCalculation(analogRead(_pin));
		delay(CALIBRATION_SAMPLE_INTERVAL);
	}

	//calculate the average value
	val = val / ((float) CALIBARAION_SAMPLE_TIMES);

	//divided by RO_CLEAN_AIR_FACTOR yields the Ro according to the chart in the datasheet 
	val = val / RO_CLEAN_AIR_FACTOR;

	return val; 
}

float GasSensor::MQRead() {
	float rs = 0.0;

	for (int i = 0; i < READ_SAMPLE_TIMES; i++) {
		rs += MQResistanceCalculation(analogRead(_pin));
		delay(READ_SAMPLE_INTERVAL);
	}

	return rs / ((float) READ_SAMPLE_TIMES);  // return the average
}

float GasSensor::MQGetPercentage(float *pcurve) {
	float rs_ro_ratio = MQRead() / Ro;
	return pow(10.0, ((log(rs_ro_ratio) - pcurve[1]) / pcurve[2]) + pcurve[0]);
}
