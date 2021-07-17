#include <PowerManagement.h>                             //
#include <AmebaServo.h>                                  //宣告阿米巴專用的伺服馬達函式庫(可送PWM訊號)
#include "wifi_data.h"                                   //引入WIFI連線設定之函式庫
#include "sys_api.h"
AmebaServo servo1;                                       //定義伺服馬達:servo1

void setup() {
  servo1.attach(12);
  Serial.begin(9600);                                    //設定序列埠鮑率
  while (status != WL_CONNECTED){                        //嘗試連接WIFI，若WIFI沒有連接則持續跑迴圈內容
   
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                                // 印出WIFI網路的名稱(SSID);
    
    status = WiFi.begin(ssid, pass);                     //用(使用者名稱，密碼)連線  
    delay(10000);                                        //延遲10秒
  }
  server.begin();                                        // 若連線上則跳脫迴圈啟動wifi server
  printWifiStatus();                                     // 顯示server IP，同時會得到一個網址http://xxx.xxx.xx.xx
}

void loop() {
  WiFiClient client = server.available();               // 聽取傳入的客戶，此處Client端為網址端(http)
  
  if (client) {                             // 如果你有客戶
    Serial.println("new client");                            
    String currentLine = "";                // 創建一個String保存來自 客戶端傳入的數據   
    while (client.connected()) {            // 若有連線上則持續跑迴圈內容(在客戶端連接時循環)
      if (client.available()) {             // if there's bytes to read from the client,
                                            // (如果有從客戶端讀取的字節，)
        char c = client.read();             // read a byte, then
                                            // (然後讀一個字節)
        Serial.write(c);                     // print it out the serial monitor
                                            // (將它打印出串行監視器)
        if (c == '\n') {                    // if the byte is a newline character
                                            // (如果該字節是換行符)

                                                                      // if the current line is blank, you got two newline characters in a row.
                                                                      // (如果當前行為空，則連續有兩個換行符。)
                                                                      // that's the end of the client HTTP request, so send a response:
                                                                      // (這是客戶端HTTP請求的結束，所以發送響應：)
          if (currentLine.length() == 0) {
                                                                      // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                                                                      // (HTTP標頭始終以響應代碼開頭（例如HTTP / 1.1 200 OK）)
                                                                      // and a content-type so the client knows what's coming, then a blank line:
                                                                      // (和內容類型，以便客戶端知道將要發生什麼，然後是一個空白行：)
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

                                                                      // the content of the HTTP response follows the header:
                                                                      // (HTTP響應的內容遵循標題：)
            client.print("Click  <a href=\"/H\">here</a>  let the servo work <br>");
            

            // The HTTP response ends with another blank line:
            // (HTTP響應以另一個空行結束：)
            client.println();
            // break out of the while loop:
            // (突破while循環：)'
            break;
          } else {    // if you got a newline, then clear currentLine:
                      // (如果你有換行符，那麼清除currentLine：)
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
                                 // (如果除了回車符之外還有別的東西，)
          currentLine += c;      // add it to the end of the currentLine
                                 // (將它添加到currentLine的末尾)
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        // (檢查客戶端請求是“GET / H”還是“GET / L”：)
       
        if (currentLine.endsWith("GET /H")) {
          data_update(device_ID, 1, cloud);    // 收到GET /H, 向雲端發送1
          Serial.println("servo switch is on ");
           servo1.write(0);                              
           delay(1000);          
           servo1.write(180);        
           delay(3000);   
           servo1.write(0);       
           delay(10000);        
        }
        
      }
    }
    // close the connection:
    // (關閉連接：)
    client.stop();
    Serial.println("client disonnected");
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  // (打印您所連接的網絡的SSID：)
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  // (打印您的WiFi盾牌的IP地址：)
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  // (打印接收信號強度：)
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  // (打印在瀏覽器中的位置：)
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}
