#include "Arduino.h"
#include <FastLED.h>

#include "FastLED_RGBW.h"

//#include <WS2812Serial.h>
//#define USE_WS2812SERIAL

#define DATA_PIN 0           //LED Strip Signal Connection 
#define ParkSignal 1          //Park Signal Connection
#define BrakeSignal 3         //Brake Signal Connection
#define LeftSignal 2          //Left Blinker Signal Connection
#define RightSignal 4         //Right Blinker Signal Connection
#define ReverseSignal 5      //Reverse Signal Connection

#define NUM_LEDS 14           //Total no of LEDs in two LED strips (eg. Use 40 here for two 20 LED strips)

//#define BlinkerLEDs NUM_LEDS/2 
#define BlinkerLEDsLeft NUM_LEDS -12 
#define BlinkerLEDsRight NUM_LEDS -10 

int BlinkerSpeed = 30;        //Turn Signal Running LED Speed. Adjust this to match with your vehicle turn signal speed.
int BlinkerOffDelay = 250;    //Turn Signal Off time. Adjust this to match with your vehicle turn signal speed.

int StartupSpeed = 55;
int ParkDetect = 0;

CRGBW leds[NUM_LEDS];
CRGB *ledsRGB = (CRGB *) &leds[0];
int r,r_dim,g,g_dim,b,b_dim,w,w_dim;


void setup() 
{
FastLED.addLeds<WS2812B, DATA_PIN, RGB>(ledsRGB, getRGBWsize(NUM_LEDS));
pinMode(ParkSignal, INPUT);
pinMode(BrakeSignal, INPUT);
pinMode(LeftSignal, INPUT);
pinMode(RightSignal, INPUT);
pinMode(ReverseSignal, INPUT);
}


void loop() 
{
if(digitalRead(ReverseSignal)==1)
{ 
Reverse();
}

if(ParkDetect == 0)
{
if(digitalRead(ParkSignal)==1) 
{
ParkDetect = 1;
}
}

if(ParkDetect == 2)
{
if(digitalRead(ParkSignal)==0) 
{
ParkDetect = 3;
}
}

if(ParkDetect == 1)
{
ParkON();
ParkDetect = 2;  
}

if(ParkDetect == 3)
{
ParkOFF();
ParkDetect = 0;  
}


if(digitalRead(ParkSignal)==0)
{
if(digitalRead(ReverseSignal)==0)
{
if((digitalRead(BrakeSignal)==0)&&(digitalRead(LeftSignal)==0)&&(digitalRead(RightSignal)==0)) //Brake Light
{
AllOff();
}

if((digitalRead(BrakeSignal)==1)&&(digitalRead(LeftSignal)==0)&&(digitalRead(RightSignal)==0)) //Brake Light
{
BrakeFull();
}
    
if((digitalRead(LeftSignal)==1)&&(digitalRead(RightSignal)==0)&&(digitalRead(BrakeSignal)==0)) //Left Blinker
{
RightOff();
LeftBlinker();
LeftDim();
delay (BlinkerOffDelay);
}

if((digitalRead(RightSignal)==1)&&(digitalRead(LeftSignal)==0)&&(digitalRead(BrakeSignal)==0)) //Right Blinker
{
LeftOff();
RightBlinker();
RightDim();
delay (BlinkerOffDelay);
}

if((digitalRead(LeftSignal)==1)&&(digitalRead(RightSignal)==0)&&(digitalRead(BrakeSignal)==1)) //Left Blinker & Brake
{
LeftDim();
RightFull(); 
LeftBlinker();
LeftDim();
delay (BlinkerOffDelay);
}

if((digitalRead(RightSignal)==1)&&(digitalRead(LeftSignal)==0)&&(digitalRead(BrakeSignal)==1)) //Right Blinker & Brake
{
RightDim();
LeftFull();
RightBlinker();
RightDim();
delay (BlinkerOffDelay);
}

if((digitalRead(LeftSignal)==1)&&(digitalRead(RightSignal)==1)&&(digitalRead(BrakeSignal)==0)) //Dual Blinker / Hazard
{
MiddleOff();
//DualBlinker();
LeftDim();
RightDim();
delay (BlinkerOffDelay);
}

if((digitalRead(LeftSignal)==1)&&(digitalRead(RightSignal)==1)&&(digitalRead(BrakeSignal)==1)) //Dual Blinker / Hazard + Brake
{
LeftDim();
RightDim();
BrakeMiddle();
//DualBlinker();
LeftDim();
RightDim();
delay (BlinkerOffDelay);
}
}
}


if(ParkDetect == 2)
{
if(digitalRead(ReverseSignal)==0)
{
if((digitalRead(LeftSignal)==0)&&(digitalRead(RightSignal)==0)&&(digitalRead(BrakeSignal)==0)) //Park Light
{   
ParkFull();
}

if((digitalRead(BrakeSignal)==1)&&(digitalRead(LeftSignal)==0)&&(digitalRead(RightSignal)==0)) //Brake Light
{
BrakeFull();
}
    
if((digitalRead(LeftSignal)==1)&&(digitalRead(RightSignal)==0)&&(digitalRead(BrakeSignal)==0)) //Left Blinker
{
LeftDim();
RightLit(); 
LeftBlinker();
LeftDim();
delay (BlinkerOffDelay);
}

if((digitalRead(RightSignal)==1)&&(digitalRead(LeftSignal)==0)&&(digitalRead(BrakeSignal)==0)) //Right Blinker
{
RightDim();
LeftLit();
RightBlinker();
RightDim();
delay (BlinkerOffDelay);
}

if((digitalRead(LeftSignal)==1)&&(digitalRead(RightSignal)==0)&&(digitalRead(BrakeSignal)==1)) //Left Blinker & Brake
{
LeftDim();
RightFull(); 
LeftBlinker();
LeftDim();
delay (BlinkerOffDelay);
}

if((digitalRead(RightSignal)==1)&&(digitalRead(LeftSignal)==0)&&(digitalRead(BrakeSignal)==1)) //Right Blinker & Brake
{
RightDim();
LeftFull();
RightBlinker();
RightDim();
delay (BlinkerOffDelay);
}

if((digitalRead(LeftSignal)==1)&&(digitalRead(RightSignal)==1)&&(digitalRead(BrakeSignal)==0)) //Dual Blinker / Hazard
{
LeftDim();
RightDim();
ParkMiddle();
//DualBlinker();
LeftDim();
RightDim();
delay (BlinkerOffDelay);
}

if((digitalRead(LeftSignal)==1)&&(digitalRead(RightSignal)==1)&&(digitalRead(BrakeSignal)==1)) //Dual Blinker / Hazard + Brake
{
LeftDim();
RightDim();
BrakeMiddle();
//DualBlinker();
LeftDim();
RightDim();
delay (BlinkerOffDelay);
}
}
}


}

void ParkON()
{
  for (int j = ((NUM_LEDS/2)-1); j >= 0; j--)
  {
    leds[j] = CRGBW(30, 0, 0, 0);
    leds[j+1] = CRGBW(0, 0, 0, 0);
    leds[(NUM_LEDS/2-1)+((NUM_LEDS/2)-j)] = CRGBW(30, 0, 0, 0);
    leds[(NUM_LEDS/2-1)+((NUM_LEDS/2)-j)-1] = CRGBW(0, 0, 0, 0);
    FastLED.show();
    delay (StartupSpeed);    
  }
  
  for (int i = 0; i < (NUM_LEDS/2); i++)
  {
    leds[i] = CRGBW(30, 0, 0, 0);
    leds[(NUM_LEDS-1)-i] = CRGBW(30, 0, 0, 0);
    FastLED.show();
    delay (StartupSpeed);    
  }
  

  for (int j = ((NUM_LEDS/2)-1); j >= 0; j--)
  {
    leds[j] = CRGBW(255, 0, 0, 0);
    leds[(NUM_LEDS/2-1)+((NUM_LEDS/2)-j)] = CRGBW(255, 0, 0, 0);
    FastLED.show();
    delay (StartupSpeed);    
  }

  for (int j = 255; j >= 60; j--)
  {  
    for (int i = 0; i < NUM_LEDS; i++)
    {
    leds[i] = CRGBW(j, 0, 0, 0);
    }
    FastLED.show();  
    delay (5);
  }
}

void ParkOFF()
{
  for (int j = 60; j <= 255; j++)
  {  
    for (int i = 0; i < NUM_LEDS; i++)
    {
    leds[i] = CRGBW(j, 0, 0, 0);
    }
    FastLED.show();  
    delay (5);
  }

  for (int j = 0; j <= ((NUM_LEDS/2)-1); j++)
  {
    leds[j] = CRGBW(255, 0, 0, 0);
    leds[j-1] = CRGBW(30, 0, 0, 0);
    leds[(NUM_LEDS-1)-j] = CRGBW(255, 0, 0, 0);
    leds[(NUM_LEDS)-j] = CRGBW(30, 0, 0, 0);
    FastLED.show();
    delay (StartupSpeed);    
  }

  leds[((NUM_LEDS/2)-1)] = CRGBW(30, 0, 0, 0);
  leds[((NUM_LEDS/2)-1)+1] = CRGBW(30, 0, 0, 0);
  FastLED.show();

    for (int j = ((NUM_LEDS/2)-1); j >= 0; j--)
  {
    leds[j] = CRGBW(30, 0, 0, 0);
    leds[j+1] = CRGBW(0, 0, 0, 0);
    leds[(NUM_LEDS/2-1)+((NUM_LEDS/2)-j)] = CRGBW(30, 0, 0, 0);
    leds[(NUM_LEDS/2-1)+((NUM_LEDS/2)-j)-1] = CRGBW(0, 0, 0, 0);
    FastLED.show();
    delay (StartupSpeed);    
  }
  
  for (int j = 0; j <= ((NUM_LEDS/2)-1); j++)
  {
    leds[j] = CRGBW(30, 0, 0, 0);
    leds[j-1] = CRGBW(0, 0, 0, 0);
    leds[(NUM_LEDS-1)-j] = CRGBW(30, 0, 0, 0);
    leds[(NUM_LEDS)-j] = CRGBW(0, 0, 0, 0);
    FastLED.show();
    delay (StartupSpeed);    
  }
}

void AllOff()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGBW(0, 0, 0, 0);
  }
    FastLED.show();  
}

void LeftOff()
{
  for (int i = 0; i < (NUM_LEDS - BlinkerLEDsRight); i++)
  {
    leds[i] = CRGBW(0, 0, 0, 0);
  }
    FastLED.show();
}

void RightOff()
{
  for (int i = 0; i < (NUM_LEDS - BlinkerLEDsRight); i++)
  {
    leds[i] = CRGBW(0, 0, 0, 0);
  }
    FastLED.show();
}

void MiddleOff()
{
  for (int i = (BlinkerLEDsLeft + BlinkerLEDsRight); i < (NUM_LEDS - (BlinkerLEDsLeft + BlinkerLEDsRight)); i++)
  {
    leds[i] = CRGBW(0, 0, 0, 0);
  }
    FastLED.show();  
}

void Reverse()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGBW(255, 255, 255,255);
  }
    FastLED.show();  
}

void BrakeFull()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGBW(255, 0, 0, 0);
  }
    FastLED.show();  
}

void BrakeMiddle()
{
  for (int i = (BlinkerLEDsLeft + BlinkerLEDsRight); i < (NUM_LEDS - (BlinkerLEDsLeft + BlinkerLEDsRight)); i++)
  {
    leds[i] = CRGBW(255, 0, 0, 0);
  }
    FastLED.show();  
}

void ParkFull()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGBW(60, 0, 0, 0);
  }
    FastLED.show();
}

void ParkMiddle()
{
  for (int i = (BlinkerLEDsLeft + BlinkerLEDsRight); i < (NUM_LEDS - (BlinkerLEDsLeft + BlinkerLEDsRight)); i++)
  {
    leds[i] = CRGBW(60, 0, 0, 0);
  }
    FastLED.show();  
}

void LeftBlinker()
{
  for (int i = (BlinkerLEDsLeft-1); i >= 0; i--)
  {
    leds[i] = CRGBW(255, 0, 0, 0);  // SK6812
    FastLED.show();
    delay (BlinkerSpeed);

    leds[i] = CRGBW(255, 255, 0, 0);  // SK6812
    FastLED.show();
    delay (BlinkerSpeed);
    
    leds[i] = CRGBW(255, 255, 255, 0);  // SK6812
    FastLED.show();
    delay (BlinkerSpeed);
    
    leds[i] = CRGBW(255, 255, 255, 255);  // SK6812
    FastLED.show();
    delay (BlinkerSpeed);      
  }
}

void LeftDim()
{
  for (int i = 0; i < BlinkerLEDsLeft; i++)
  {
    leds[i] = CRGBW(0, 0, 0, 0);
  }
    FastLED.show();
}

void LeftLit()
{
  for (int i = 0; i < (NUM_LEDS - BlinkerLEDsLeft); i++)
  {
    leds[i] = CRGBW(75, 0, 0, 0);
  }
    FastLED.show();
}

void LeftFull()
{
  for (int i = 0; i < (NUM_LEDS - BlinkerLEDsLeft); i++)
  {
    leds[i] = CRGBW(255, 0, 0, 0);
  }
    FastLED.show();
}

void RightBlinker()
{
  for (int i = (BlinkerLEDsRight -1); i >= 2; i--)
  {
    leds[i] = CRGBW(255, 0, 0, 0);  // SK6812
    FastLED.show();
    delay (BlinkerSpeed);

    leds[i] = CRGBW(255, 255, 0, 0);  // SK6812
    FastLED.show();
    delay (BlinkerSpeed);
    
    leds[i] = CRGBW(255, 255, 255, 0);  // SK6812
    FastLED.show();
    delay (BlinkerSpeed);
    
    leds[i] = CRGBW(255, 255, 255, 255);  // SK6812
    FastLED.show();
    delay (BlinkerSpeed);
  }
}

void RightDim()
{
   for (int i = 1; i < BlinkerLEDsRight; i++)
  {
    leds[i] = CRGBW(0, 0, 0, 0);
  }
    FastLED.show();
}

void RightLit()
{
  for (int i = BlinkerLEDsRight; i < NUM_LEDS; i++)
  {
    leds[i] = CRGBW(75, 0, 0, 0);
  }
    FastLED.show();
}

void RightFull()
{
  for (int i = (BlinkerLEDsLeft + BlinkerLEDsRight) ; i < NUM_LEDS; i++)
  {
    leds[i] = CRGBW(255, 0, 0, 0);
  }
    FastLED.show();
}

/*
void DualBlinker()
{
  for (int i = BlinkerLEDsLeft -1; i >= 0; i--)
  {
    leds[i] = CRGBW(255, 0, 0, 0);  // SK6812
    FastLED.show();
    delay (BlinkerSpeed);

    leds[i] = CRGBW(255, 255, 0, 0);  // SK6812
    FastLED.show();
    delay (BlinkerSpeed);
    
    leds[i] = CRGBW(255, 255, 255, 0);  // SK6812
    FastLED.show();
    delay (BlinkerSpeed);
    
    leds[i] = CRGBW(255, 255, 255, 255);  // SK6812
    FastLED.show();
    delay (BlinkerSpeed);
    
  }
  for (int i = BlinkerLEDsRight -1; i >= 2; i--)
  {
    leds[i] = CRGBW(255, 0, 0, 0);  // SK6812
    FastLED.show();
    delay (BlinkerSpeed);

    leds[i] = CRGBW(255, 255, 0, 0);  // SK6812
    FastLED.show();
    delay (BlinkerSpeed);
    
    leds[i] = CRGBW(255, 255, 255, 0);  // SK6812
    FastLED.show();
    delay (BlinkerSpeed);
    
    leds[i] = CRGBW(255, 255, 255, 255);  // SK6812
    FastLED.show();
    delay (BlinkerSpeed);
    
  }
}
*/
