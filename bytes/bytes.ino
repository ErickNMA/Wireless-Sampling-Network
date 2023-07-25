void setup() 
{
  Serial.begin(115200);

  delay(3000);

  int num = 3;

  int send = (num + 32768);

  byte b1 = highByte(send);
  byte b2 = lowByte(send);

  Serial.println(b1);
  Serial.println(b2);

  int received = (b2 + (b1<<8) - 32768);

  Serial.println(received);
}

void loop() 
{

}
