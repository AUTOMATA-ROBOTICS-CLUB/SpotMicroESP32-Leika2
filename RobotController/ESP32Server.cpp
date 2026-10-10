#include <WiFi.h>
#include <WiFiUdp.h>

const char* apSSID = "RobotController";
const char* apPassword = "Eojd39tf!"; 

const unsigned int localPort = 4210; 

WiFiUDP udpServer;
char packetBuffer[255]; 

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(apSSID, apPassword);

  udpServer.begin(localPort);

void loop() {
  int packetSize = udpServer.parsePacket();
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = udpServer.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(udpServer.remotePort());


    int len = udpServer.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0; 
    }
    Serial.print("Payload: ");
    Serial.println(packetBuffer);

    udpServer.beginPacket(udpServer.remoteIP(), udpServer.remotePort());
    udpServer.write((const uint8_t*)"Acknowledge from ESP32 AP", 25);
    udpServer.endPacket();
  }
}
