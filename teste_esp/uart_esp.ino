#include <SoftwareSerial.h>

#define SAMPLES 30*6

int data[SAMPLES];

SoftwareSerial TINY(12, 13);      // RX pino 12 , TX pino 13

void setup() 
{
  Serial.begin(9600);
  TINY.begin(9600);
}

void getData()
{
  unsigned long t0 = millis();
  for(int i=0; i<SAMPLES; i++)
  {
    while(1)
    {
      if(TINY.available()>=2)
      {
        uint8_t b1 = TINY.read();
        uint8_t b2 = TINY.read();
        data[i] = ((b1<<8) + b2);
        break;
      }
    } 
  }
  Serial.print("Comunication time: ");
  Serial.print(millis()-t0);
  Serial.println("ms");
}

void loop() 
{
  if(TINY.available())
  {
    getData();
    Serial.println("\n--------------\n");
    for(int i=0; i<SAMPLES; i++)
    {
      Serial.println(data[i]);
    }
    Serial.println("\n--------------\n");
  }  
}
