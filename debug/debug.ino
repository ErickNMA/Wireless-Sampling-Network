#include <SoftwareSerial.h>

SoftwareSerial TINY(12, 13);      // RX pino 12 , TX pino 13

#define SAMPLES 30 // samples to receive
#define timeout SAMPLES*20 // receiving timeout

int received[SAMPLES];

void setup() 
{
  Serial.begin(9600);
  TINY.begin(9600);

  for(int i=0; i<SAMPLES; i++)
  {
    received[i] = -1;
  }

  delay(2000);
}

bool checksum(uint8_t *dados)
{
  return ((dados[2]^dados[1])==dados[0]);
}

bool getData()
{
  unsigned long t0 = millis();
  // Aguarda o sinal de start:
  Serial.println("Waiting start...");
  while(1)
  {
    if(TINY.available())
    {
      if(TINY.read()==0xff)
      {
        if(TINY.read()==0xff)
        {
          break;
        }
      }
    }
    if((millis()-t0)>timeout)
    {
      Serial.println("Timed out!");
      return false;
    }
  }
  int i = 0, errcont=0;
  Serial.println("Waiting data...");
  do
  {    
    if(TINY.available()>=3)
    {
      uint8_t b1 = TINY.read();
      uint8_t b2 = TINY.read();
      uint8_t b3 = TINY.read();
      uint8_t cksm = (b1^b2); 
      //Send confirmation:
      TINY.write(b1^b2);
      // Confere a integridade do dado:
      if(b3==cksm)
      {
        Serial.println("DATA RECEIVED!");
        received[i] = ((b1<<8) + b2 - 0x8000);
        i++;
      }else
      {
        errcont++;
        Serial.println("DATA LOSS!");
      }
    }
    if((millis()-t0)>timeout)
    {
      Serial.println("Timed out!");
      return false;
    }
  } while(i<SAMPLES);
  Serial.println("Successfull!");
  Serial.print("Wrong attempts: ");
  Serial.println(errcont);
  Serial.print("Comunication time: ");
  Serial.print(millis()-t0);
  Serial.println("ms");
  return true;
}

void loop() 
{
  if(TINY.available())
  {
    if(getData())
    {
      Serial.println("\n--------------\n");
      for(int j=0; j<SAMPLES; j++)
      {
        Serial.println(received[j]);
      }
      Serial.println("\n--------------\n");
    }    
  }
  delay(3000);
}