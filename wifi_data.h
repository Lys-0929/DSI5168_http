#include <HttpClient.h>
#include <WiFi.h>
#include <WiFiClient.h>

char ssid[] = "home296";     // 要連線的SSID名稱
char pass[] = "88888888";         // wifi密碼
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
String cloud = "test";

int status = WL_IDLE_STATUS;
WiFiServer server(80);            //  定義wifi server與其端口80
WiFiClient client;

int device_ID = 1;

char server_IP[] = "www.dweet.io";
const uint16_t port = 80;

void data_update(int ID, int count, String cloud) {
  client.stop();
  if (client.connect(server_IP, 80)) {
    
    Serial.println("connected to server");
    String s = "POST /dweet/for/";
    s.concat(cloud);
    s.concat("?DSI5168_");
    s.concat(ID);
    String tempt = s;
    tempt.concat("=");
    String tempt1 = tempt;
    tempt1.concat(count);
    String s1 = tempt1;

    Serial.println(s1);
    client.println(s1);
    client.println("Host: www.dweet.io");
    client.println("Connection: close");
    client.println();
    client.stop();
    delay(2000);
  } else if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
  }
  Serial.println();
}
