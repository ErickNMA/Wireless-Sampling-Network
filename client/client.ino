#include <ESP8266WiFi.h>

String ssid = "ESP01";
String password = "12345678";

IPAddress server(192,168,10,40);
WiFiClient client;

void setup() 
{ 
  Serial.begin(115200);
  delay(200);
  Serial.println("");
  Serial.print("Waiting connection: ");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("ESP-01 Client Connected");
  Serial.print("IP: ");       Serial.println(WiFi.softAPIP());
  Serial.print("SSID: ");     Serial.println(WiFi.SSID());
  Serial.print("Signal: ");   Serial.println(WiFi.RSSI());
}

char buffer[10];

void loop() 
{
  client.connect(server, 80);
  sprintf(buffer, "%lu", millis());
  client.write(buffer);
  client.stop();
}