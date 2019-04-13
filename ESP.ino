//Program odpowiedzialny za sterowanie modułem ESP8266
//Ustawia Access Point chroniony hasłem
//Nasłuchuje przychodzących pakietów danych
//Przesyła je do Arduino sterującego robotem
#include<ESP8266WiFi.h>
#include<WiFiUdp.h>

//Definicje WiFi
const char* ssid = "EspAP";
const char* password = "pass12345"; //musi być dłuższy niż 7 znaków
WiFiUDP Udp;
unsigned int localUdpPort = 4210;
char incomingPacket[255]; //bufer dla nadchodzących pakietów
char* replyPacket[255]; //odpowiedź na pakiet


void setup() {

   Serial.begin(115200);
   delay(10);

   //Ustawienie trybu i hasła WiFi
   WiFi.mode(WIFI_AP);
   WiFi.softAP(ssid, password, 1, false);
  

   Udp.begin(localUdpPort);
   //Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.softAPIP().toString().c_str(), localUdpPort);
}

void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    //Odbieranie przychodzących pakietów UDP
    //Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    //Serial.printf("UDP packet contents: %s\n", incomingPacket);
    Serial.printf("%s\n", incomingPacket);

    //Wyślij odpowiedź na port i adres IP z którego otrzymano pakiet
    *replyPacket = incomingPacket;
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(*replyPacket);
    Udp.endPacket();
  }
}
