#include "OCT_Sensor.h"

// SensorBase
template <typename T>
SensorBase<T>::SensorBase(int pin) {
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
HumitureDHT11::HumitureDHT11(int pin):SensorBase(pin), _dht(pin, DHT11) {
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
FlameSensor::FlameSensor(int type, int pin, int t):SensorBase(pin), _type(type), _gapThreshold(t) {
	_state = None;
}

int FlameSensor::readSensor() {
    short val = (_type == FLAME_SENSOR_ANALOG ? analogRead(_inputPin) : digitalRead(_inputPin));
	evaluate(val);
	return val;
}

void FlameSensor::evaluate(int sample) {
	switch(_state) {
		case None:
		_state = Sensored;
		break;
		
		case Sensored:
		if (sample - _lastSample >= _gapThreshold) {
			_state = Detected;
		}
		break;
		
		case Detected:
		if(_trend == Low && _trendCount > 5) {
			_state = Cleared;
		}
		break;
		
		case Cleared:
		if (sample - _lastSample >= _gapThreshold) {
			_state = Detected;
		} else {
			_state = Sensored;
		}
		break;
	}
	if (sample > _lastSample && _trend != High) {
		_trend = High; 
		_trendCount = 0;
	} 
	if (sample < _lastSample && _trend != Low) {
		_trend = Low;
		_trendCount = 0;
	} 
	if (sample == _lastSample && _trend != Still) {
		_trend = Still;
		_trendCount = 0;
	}
	_trendCount ++;
	_lastSample = sample;
}

FlameSensorState FlameSensor::getState() {
	return _state;
}

boolean FlameSensor::isSensored() {
	return _state == Sensored;
}

boolean FlameSensor::isDetected() {
	return _state == Detected;
}

boolean FlameSensor::isCleared() {
    // if last two sample's decrese _gapThreshold, then flame detected;
    return _state == Cleared;
}
