#include <SoftwareSerial.h>

#define SAMPLES 30*6

int data[SAMPLES];

SoftwareSerial ESP(1, 4);      // RX pino 1 , TX pino 4

void sendData(int *dados)
{
  for(int i=0; i<SAMPLES; i++)
  {
    ESP.write(data[i]>>8);
    ESP.write(data[i]&0xff);
  }
}

void setup() 
{
  ESP.begin(9600);

  for(int i=0; i<SAMPLES; i++)
  {
    data[i] = (i+1);
  }

  sendData(data);
}

void loop() 
{

}
