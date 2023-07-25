#include <SoftwareSerial.h>

SoftwareSerial HC12(2, 3);      // RX pino 2 , TX pino 3

void setup() 
{
  Serial.begin(9600);          // Porta Serial do computador 
  HC12.begin(9600);            // Porta Serial do HC12
}
void loop() 
{
  if(HC12.available()) // Se HC12 receber dados
  {   
    Serial.write(HC12.read()); // Escreve os dados na Serial
  }
  if(Serial.available()) // Se a Serial receber dados
  { 
    HC12.write(Serial.read()); // Escreve os dados no HC12
    HC12.flush();              // Aguarda os dados, depois limpa a memória
  }
}