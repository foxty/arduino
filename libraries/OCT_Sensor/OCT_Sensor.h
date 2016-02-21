/*
    OCT Sensor 类库，为Octopus项目提供专用类库，主要封装各种传感器并加入软均衡算法。
*/
#ifndef OCT_Sensor_h
#define OCT_Sensor_h

#include "DHT.h"

// Structure for Humiture(For DHT11 DHT22)
struct Humiture {
  float humidity;
  float temperature;
};

// Base class of Sensor
template<typename T>
class SensorBase {
public:
    SensorBase(byte pin);
	virtual T readSensor() = 0;
	
protected:
	byte _inputPin;
    T _samples[10];
};

// LM35 Temperature Sensor
class TemperatureLM35: public SensorBase<float> {
public:
    TemperatureLM35(int pin);
    float readSensor();
};

// DHT11
class HumitureDHT11: public SensorBase<Humiture> {
public:
    HumitureDHT11(byte pin);
    Humiture readSensor();
    
  private:
    DHT _dht;
    Humiture _humiture;
};

// Flame Sensor
class FlameSensor: public SensorBase<int> {
  public:
    FlameSensor(byte pin, int t);
    int readSensor();
    boolean isOverThreshold();
	
  private:
    int _threshold;
};

#endif