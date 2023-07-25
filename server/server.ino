#include <ESP8266WiFi.h>

String ssid = "ESP01";
String password = "12345678";

WiFiServer server(80);

IPAddress ip(192, 168, 10, 40);
IPAddress gateway(192, 168, 10, 1);
IPAddress subnet(255, 255, 255, 0);

void setup() 
{
  Serial.begin(115200);
  
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(ssid, password, 2, 1, 5);
  delay(500);
  Serial.print("AP IP address: ");
  Serial.println(ip);
  server.begin();
 
  Serial.println("ESP-01 Server Initialized");
  Serial.print("IP: ");       Serial.println(WiFi.softAPIP());
  Serial.print("SSID: ");     Serial.println(WiFi.SSID());
  Serial.print("Signal: ");   Serial.println(WiFi.RSSI());
}

char buffer[10];

void loop() 
{
  WiFiClient client = server.available();

  if(client) 
  {
    client.readBytes(buffer, 4);
    Serial.write(buffer);
    Serial.println();
  }
}