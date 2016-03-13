/*
    OCT Sensor
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
    SensorBase(int pin);
	virtual T readSensor() = 0;
	
protected:
	int _inputPin;
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
    HumitureDHT11(int pin);
    Humiture readSensor();
    
  private:
    DHT _dht;
    Humiture _humiture;
};

// Flame Sensor
const int FLAME_SENSOR_ANALOG = 0;
const int FLAME_SENSOR_DIGITAL = 1;

enum FlameSensorState {None = 'N', Sensored = 'S' , Detected = 'D', Cleared = 'C'};
enum FlameSensorTrend {High, Still, Low};

class FlameSensor: public SensorBase<int> {
  public:
    // type = 0 is analog sensor
    // type = 1 is digital sensor
    FlameSensor(int type, int pin, int gapThreshold);
    int readSensor();
	void evaluate(int sample);
	FlameSensorState getState();
	boolean isSensored();
    boolean isDetected();
    boolean isCleared();
	
  private:
    int _type;
	FlameSensorState _state;
	FlameSensorTrend _trend;
	short _trendCount;
	short _lastSample;
    short _gapThreshold;
};

#endif