#include "OCT_Sensor.h"

// SensorBase
template <typename T>
SensorBase<T>::SensorBase(byte pin) {
    _inputPin = pin;
    pinMode(_inputPin, INPUT);
}

// LM35
TemperatureLM35::TemperatureLM35(int pin):SensorBase(pin){}

float TemperatureLM35::readSensor() {
    int val = analogRead(_inputPin);
    float temp = (val * 0.0048828125 * 100);
    return temp;
}

// HumitureDHT11
HumitureDHT11::HumitureDHT11(byte pin):SensorBase(pin), _dht(pin, DHT11) {
    _dht.begin();
}

Humiture HumitureDHT11::readSensor() {
    float humidity = _dht.readHumidity();
    float temperature = _dht.readTemperature();
    _humiture.humidity = humidity;
    _humiture.temperature = temperature;
    if (isnan(humidity)) {
        Serial.println("Failed to read humidity from DHT sensor!");
        _humiture.humidity = -1;
    }
    if (isnan(temperature)) {
        Serial.println("Failed to read temperature from DHT sensor!");
        _humiture.temperature = -1;
    }
    return _humiture;
}

// FlameSensor
FlameSensor::FlameSensor(byte type, byte pin, int t):SensorBase(pin), _type(type), _gapThreshold(t) {
    _samplePos = 0;
	_state = None;
}

int FlameSensor::readSensor() {
    int val = (_type == FLAME_SENSOR_ANALOG ? analogRead(_inputPin) : digitalRead(_inputPin));
    if(_samplePos >= SAMPLE_SIZE) _samplePos = 0;
	_samples[_samplePos++] = val;
	evaluate();
	return val;
}

void FlameSensor::evaluate() {
	
}

boolean FlameSensor::isSensored() {
	return false;
}

boolean FlameSensor::isDetected() {
    // if last two sample's increase _gapThreshold, then flame detected;
    boolean result = false;
    return result;
}

boolean FlameSensor::isCleared() {
    // if last two sample's decrese _gapThreshold, then flame detected;
    boolean result = false;
    return result;
}
