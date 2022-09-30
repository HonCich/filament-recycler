#include <Arduino.h>
#include <HardwareSerial.h>
#include "Wire.h"
#include "PCA9685.h"
#include "HX711.h"
#include "AccelStepper.h"
#include "MultiStepper.h"


const uint8_t reflectivesensor0 = 34;
const uint8_t PCA_OE = 25;
const uint8_t stepperON = 12;
const uint8_t LEDsign = 15;
const uint8_t step0 = 12;
const uint8_t dir0 = 5;
const uint8_t step1 = 17;
const uint8_t dir1 = 0;
char tempPin[2]={32,33};
char fiveVPin[6]={8,9,10,11,0,1};
char twentyFourVPin[6]={2,3,4,5,6,7};

int prevTime = 0;

const float c1 = 0.0007157730988481782;
const float c2 = 0.0002145860245372531;
const float c3 = 1.0991444718982721e-7;

PCA9685 PCA(0x70);
AccelStepper stepper0(AccelStepper::DRIVER, step0, dir0);
AccelStepper stepper1(AccelStepper::DRIVER, step1, dir1);

void outputEnable (bool enable);
void fiveVDrive (uint8_t output, int value);
void twentyFourVDrive (uint8_t output, int value);
int reflectiveRead ();
float tempRead (uint8_t temp);

void outputEnable (bool enable)
{
    digitalWrite(PCA_OE, !enable);
    PCA.digitalWrite(stepperON,enable);
    PCA.digitalWrite(LEDsign,enable);
}

void fiveVDrive (uint8_t output, int value)
{
  PCA.setPWM(fiveVPin[output],0+value*40.95);
}

void twentyFourVDrive (uint8_t output, int value)
{
  PCA.setPWM(twentyFourVPin[output],0+value*40.95);
}

int reflectiveRead ()
{
  PCA.setON(14);
  return analogRead(reflectivesensor0);
}

float tempRead (uint8_t temp)
{
  float R,T,logR;
  R = 100000 / (4095.0 / analogRead(tempPin[temp]) - 1.0);
  logR = log(R);
  T = (1 / (c1 + c2 * logR + c3 * logR * logR * logR)) - 273.15;
  Serial.println(T);
  return(T);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin(); 
  PCA.begin();

  outputEnable(1);
  stepper0.setAcceleration(200.0);
  stepper0.setMaxSpeed(1000);
  stepper0.setSpeed(200);
  stepper1.setAcceleration(200.0);
  stepper1.setMaxSpeed(1000);
  stepper1.setSpeed(2);

  twentyFourVDrive(0,80);
  twentyFourVDrive(1,80);
  /*while (tempRead(0)<260)
  {
    delay(1000);
  }
  
  prevTime = millis();*/
}


void loop() {
  // put your main code here, to run repeatedly
  /*if (millis()-prevTime>=1000)
  {
    if(tempRead(0)<300)
    {
      twentyFourVDrive(0,100);
      twentyFourVDrive(1,100);
    }
    else
    {
      twentyFourVDrive(0,0);
      twentyFourVDrive(1,0);
    }
    prevTime=millis();
  }*/
  
stepper0.runSpeed();
stepper1.runSpeed();
}