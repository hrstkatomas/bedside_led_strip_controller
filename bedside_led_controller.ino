#include <CapacitiveSensor.h>

const int mosfetPin = 3;
const int capacitiveSensor1ResistorPin = 8;
const int capacitiveSensor1WirePin = 11;
const int capacitiveSensor2ResistorPin = 9; 
const int capacitiveSensor2WirePin = 12;

const int brightnesses[] = {0, 192, 64, 10}; // pwm duty cycles - 255 is max
const int brightnessesLength = 4;
const int transitionTime = 500;

const int capacityThreshold = 1000;

CapacitiveSensor capSensor1 = CapacitiveSensor(capacitiveSensor1ResistorPin, capacitiveSensor1WirePin);
CapacitiveSensor capSensor2 = CapacitiveSensor(capacitiveSensor2ResistorPin, capacitiveSensor2WirePin);


bool holding = false;

bool sensorTouched() {
  long sensorValue1 = capSensor1.capacitiveSensor(30);
  long sensorValue2 = capSensor2.capacitiveSensor(30);
  
  bool prevHoldingValue = holding;
  holding = sensorValue1 > capacityThreshold || sensorValue2 > capacityThreshold;

  return !prevHoldingValue && holding;
}


int currentBrightnessesIndex = 0;

void transitionToNextBrightness() {
  int currentBrightness = brightnesses[currentBrightnessesIndex];
  
  currentBrightnessesIndex++;
  if (currentBrightnessesIndex >= brightnessesLength) {
    currentBrightnessesIndex = 0;
  }

  int nextBrightness = brightnesses[currentBrightnessesIndex];
  
  int brightnessDifference = currentBrightness - nextBrightness;
  int brightnessincreasing = brightnessDifference < 0;
  int delayBetweenSteps = round(transitionTime/abs(brightnessDifference));
  if(brightnessincreasing) {
    for (byte i = currentBrightness; i < nextBrightness; i++) {
      analogWrite(mosfetPin, i + 1);
      delay(delayBetweenSteps);
    } 
  } else {
    for (byte i = currentBrightness; i > nextBrightness; i--) {
      analogWrite(mosfetPin, i - 1);
      delay(delayBetweenSteps);
    } 
  }
}


void setup() {}

void loop() {  
  if (sensorTouched()) {
    transitionToNextBrightness();
  }

  delay(20);
}
