#include <SoftwareSerial.h>

#define SAMPLES 30

SoftwareSerial TINY(12, 13);      // RX pino 12 , TX pino 13

//int data[SAMPLES][6];

void setup() 
{
  Serial.begin(9600);
  TINY.begin(9600);
}

int **dataAlloc()
{
  int **M = (int **)calloc(SAMPLES, sizeof(int *));
  for (int i=0; i<SAMPLES; ++i)
  {
    M[i] = (int *)calloc(6, sizeof(int));
  }

  return M;
}

void dataFree(int **dados)
{
  for(int i=0; i<SAMPLES; i++)
    free(dados[i]);
  free(dados);
}

void getData(int **dados)
{
  unsigned long t0 = millis();
  for(int i=0; i<SAMPLES; i++)
  {
    for(int j=0; j<6; j++)
    {
      while(1)
      {
        if(TINY.available()>=2)
        {
          dados[i][j] = ((TINY.read()<<8) + TINY.read() - 0x8000);
          break;
        }
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
    int **data = dataAlloc();
    getData(data);
    Serial.println("\n---------------------------------------------------------------\n");
    Serial.println("\t SMP \t AX \t AY \t AZ \t GX \t GY \t GZ");
    for(int i=0; i<SAMPLES; i++)
    {
      Serial.print("\t");
      Serial.print(i+1);
      for(int j=0; j<3; j++)
      {
        Serial.print("\t");
        Serial.print((float)data[i][j]/8192.0);
      }
      for(int j=3; j<6; j++)
      {
        Serial.print("\t");
        Serial.print((float)data[i][j]/131.0);
      }
      Serial.println();
    }
    Serial.println("\n---------------------------------------------------------------\n");
    dataFree(data);
  }  
}
