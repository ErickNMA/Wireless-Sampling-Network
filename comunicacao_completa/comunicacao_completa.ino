#include "SoftwareSerial.h"
#include <string.h>

SoftwareSerial HC12(25, 26);  // 25 -> RX | 26 -> TX
#define SET 27

void setup() 
{
  Serial.begin(115200);
  HC12.begin(9600);
  pinMode(SET, OUTPUT);
  digitalWrite(SET, LOW);

  // Configuração HC12:
  delay(550);  
  HC12.write("AT+B115200");
  delay(50);
  while(HC12.available())
  {
    Serial.write(HC12.read());
  } 
  HC12.write("AT+C001");
  delay(50);
  while(HC12.available())
  {
    Serial.write(HC12.read());
  } 
  HC12.write("AT+P4");
  delay(50);
  while(HC12.available())
  {
    Serial.write(HC12.read());
  }
  HC12.end();
  digitalWrite(SET, HIGH);
  Serial.print("DONE!");
}

char buff[10];

void setChannel(char *ch)
{
  HC12.begin(9600);
  digitalWrite(SET, LOW);
  delay(550);
  sprintf(buff, "AT+C%s", ch);
  HC12.write(buff);
  delay(50);
  while(HC12.available())
  {
    Serial.write(HC12.read());
  }
  HC12.end();
  digitalWrite(SET, HIGH);
}

void getData()
{
  HC12.begin(115200);
  //HC12.write("START");
  while(1)
  {
    if(HC12.available())
    {
      while(HC12.available())
      {
        Serial.write(HC12.read());
      }
      break;      
    }
  }
  HC12.flush();
  HC12.end();
}

char chans[2][4] = {"002", "003"};

void loop() 
{
  for(int i=0; i<2; i++)
  {
    setChannel(chans[i]);
    Serial.print("\n\n\nLeitura do HC12-");
    Serial.print(chans[i]);
    Serial.println(": ");
    getData();
  }
  delay(1000);
}
