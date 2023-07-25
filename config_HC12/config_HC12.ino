#include <SoftwareSerial.h>

SoftwareSerial HC12(25, 26);      // RX pino 25 , TX pino 26

void setup() 
{
  
  Serial.begin(115200);          // Porta Serial do computador 
  HC12.begin(9600);            // Porta Serial do HC12
  pinMode(27, OUTPUT);
  digitalWrite(27, LOW);
  delay(550);
  HC12.write("AT+B115200");
  delay(50);
  while(HC12.available())
  {
    Serial.write(HC12.read());
  }
  HC12.write("AT+C002");
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
  digitalWrite(27, HIGH);
  HC12.begin(115200);
  Serial.println("LOOP");
}
void loop() {
  if(HC12.available()) 
  {   // Se o HC12 receber dados
    Serial.write(HC12.read()); // Escreve os dados na Serial
  }
  if(Serial.available()) 
  { // Se a Serial receber dados
    HC12.write(Serial.read()); // Escreve os dados no HC12
    HC12.flush();              // Aguarda os dados, depois limpa a memória
  }
}