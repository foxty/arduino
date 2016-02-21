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
FlameSensor::FlameSensor(byte pin, int t):SensorBase(pin), _threshold(t) {}

int FlameSensor::readSensor() {
    return analogRead(_inputPin);
}

boolean FlameSensor::isOverThreshold() {
    int val = readSensor();
    return val >= _threshold;
}
