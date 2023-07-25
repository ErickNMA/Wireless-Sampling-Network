#include <SoftwareSerial.h>

SoftwareSerial HC12(0, 2);      // RX pino 0 , TX pino 2

void setup() 
{
  Serial.begin(115200);          // Porta Serial do computador 
  HC12.begin(115200);            // Porta Serial do HC12
}

unsigned long cont=0;
char buffer[10];

void loop() 
{
  delay(1000);
  sprintf(buffer, "B%d\n", cont);
  HC12.write(buffer);
  Serial.print(buffer);
  cont++;
}
