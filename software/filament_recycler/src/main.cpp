#include <Arduino.h>
#include <HardwareSerial.h>
#include "AccelStepper.h"
#include "Wire.h"
#include "PCA9685.h"
#include "HX711.h"

PCA9685 PCA(0x70);

static uint8_t reflectivesensor0 = 34;
static uint8_t PCA_OE = 25;
static uint8_t temp0 = 32;
static uint8_t temp1 = 33;

static float c1 = 0.0007157730988481782;
static float c2 = 0.0002145860245372531;
static float c3 = 1.0991444718982721e-7;

void outputEnable (bool enable);
void fiveVDrive (char output, bool value);
void twentyfourVDrive (char output, bool value);
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

void fiveVDrive (char output, bool value)
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
  if(value==1)
  {
    PCA.setON(pin);
  }
  else
  {
    PCA.setOFF(pin);
  }
  PCA.setPWM(pin,4095-value*40.95,value*40.95);
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
  PCA.setPWM(pin,4095-value*40.95,value*40.95);
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
      R = 100000*(1023/analogRead(temp0)-1);
      break;
    case 1:
      R = 100000*(1023/analogRead(temp1)-1);
      break;
  }
  logR = log(R);
  T = (1/(c1+c2*logR+c3*logR*logR*logR));
  return(T);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin(); 
  PCA.begin();
  pinMode (PCA_OE, OUTPUT);
  outputEnable(1);
}

void loop() {
  // put your main code here, to run repeatedly
  
}