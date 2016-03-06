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
const int FLAME_SENSOR_ANALOG = 0;
const int FLAME_SENSOR_DIGITAL = 1;
const int SAMPLE_SIZE = 5;

enum FlameSensorState {None = 0, Sensored, Detected, Cleared};

class FlameSensor: public SensorBase<int> {
  public:
    // type = 0 is analog sensor
    // type = 1 is digital sensor
    FlameSensor(byte type, byte pin, int gapThreshold);
    int readSensor();
	void evaluate();
	boolean isSensored();
    boolean isDetected();
    boolean isCleared();
	
  private:
    byte _type;
	FlameSensorState _state;
    int _samplePos;
    int _samples[SAMPLE_SIZE] = {-1 ,-1 ,-1, -1, -1};
    int _gapThreshold;
};

#endif