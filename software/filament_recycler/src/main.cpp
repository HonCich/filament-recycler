#include <Arduino.h>
#include <HardwareSerial.h>
#include "Wire.h"
#include "PCA9685.h"
#include "HX711.h"
#include "AccelStepper.h"
#include "MultiStepper.h"


const uint8_t reflectivesensor0 = 34;
const uint8_t PCA_OE = 25;
const uint8_t temp0 = 32;
const uint8_t temp1 = 33;
const uint8_t step0 = 12;
const uint8_t dir0 = 5;
const uint8_t step1 = 17;
const uint8_t dir1 = 0;

const float c1 = 0.0007157730988481782;
const float c2 = 0.0002145860245372531;
const float c3 = 1.0991444718982721e-7;

PCA9685 PCA(0x70);
AccelStepper stepper0(AccelStepper::DRIVER, step0, dir0);

void outputEnable (bool enable);
void fiveVDrive (char output, int value);
void twentyfourVDrive (char output, int value);
int reflectiveRead ();
float tempRead (char temp);

void outputEnable (bool enable)
{
  if(enable==1)
  {
    digitalWrite(PCA_OE, LOW);
    PCA.setON(12);
    PCA.setON(15);
  }
  else
  {
    PCA.setOFF(15);
    PCA.setOFF(12);
    digitalWrite(PCA_OE, HIGH);
  }
}

void fiveVDrive (char output, int value)
{
  char pin;
  switch(output)
  {
    case 0:
      pin = 8;
      break;
    case 1:
      pin = 9;
      break;
    case 2:
      pin = 10;
      break;
    case 3:
      pin = 11;
      break;
    case 4:
      pin = 0;
      break;
    case 5:
     pin = 1;
      break;
  }
  PCA.setPWM(pin,0+value*40.95);
}

void twentyfourVDrive (char output, int value)
{
  char pin;
  switch (output)
  {
    case 0:
      pin = 2;
      break;
    case 1:
      pin = 3;
      break;
    case 2:
      pin = 4;
      break;
    case 3:
      pin = 5;
      break;
    case 4:
      pin = 6;
      break;
    case 5:
      pin = 7;
      break;
  }
  PCA.setPWM(pin,0+value*40.95);
}

int reflectiveRead ()
{
  PCA.setOFF(14);
  int noise = analogRead(reflectivesensor0);
  PCA.setON(14);
  return analogRead(reflectivesensor0)-noise;
}

float tempRead (char temp)
{
  float R,T,logR;
  switch(temp)
  {
    case 0:
       R = 100000 / (4095.0 / analogRead(temp0) - 1.0);
      break;
    case 1:
      R = 100000 / (4095.0 / analogRead(temp1) - 1.0);
      break;
  }
  logR = log(R);
  T = (1 / (c1 + c2 * logR + c3 * logR * logR * logR)) - 273.15;
  Serial.println(T);
  return(T);
}

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(115200);
  Wire.begin(); 
  PCA.begin();

  pinMode (PCA_OE, OUTPUT);
  outputEnable(1);
  stepper0.setAcceleration(200.0);
  stepper0.setSpeed(100);
  stepper0.move(1000);
}


int x = 0;

void loop() {
  // put your main code here, to run repeatedly
    
    stepper0.run();

}