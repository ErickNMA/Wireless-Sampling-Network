#include <SoftwareSerial.h>

SoftwareSerial ESP(1, 4);      // RX pino 1 , TX pino 4

#define SAMPLES 30
#define timeout (SAMPLES*30)

int pacote[SAMPLES];

void setup() 
{
  ESP.begin(9600);

  for(int j=0; j<SAMPLES; j++)
  {
    pacote[j] = (j+1);
  }

  sendData(pacote);
}

int i=0;

void sendData(int *data)
{
  //Start bytes:
  ESP.write(0xff);
  ESP.write(0xff);
  i = 0;
  do 
  {    
    int send = (data[i] + 0x8000);
    uint8_t b1 = (send>>8);
    uint8_t b2 = (send&0xff);
    uint8_t cksm = (b1^b2);    
    // Envio do inteiro como dois bytes:      
    ESP.write(b1);
    ESP.write(b2);
    //Envio do checksum:
    ESP.write(cksm);
    unsigned long t0 = millis();
    while(!ESP.available())
    {
      if(ESP.read()==cksm)
      {
        i++;
      }
      /*
      if((millis()-t0)>timeout)
      {
        ESP.flush();
        //Start bytes:
        ESP.write(0xff);
        ESP.write(0xff);
      }
      */
      break;
    }
  } while(i<SAMPLES);
}

void loop() 
{

}