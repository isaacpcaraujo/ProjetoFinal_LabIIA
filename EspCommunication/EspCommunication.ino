#include <ESP8266WiFi.h>

const char* ssid = "LittleRobot";
const char* password = "12345678";
WiFiServer server(80);

void blink_led() {
  digitalWrite(LED_BUILTIN, LOW);  
  delay(1000);                      
  digitalWrite(LED_BUILTIN, HIGH);  
  delay(2000);   
}

void setup() {
  Serial.begin(9600); 

  pinMode(LED_BUILTIN, OUTPUT);
  
  IPAddress staticIP(192, 168, 4, 2); // IP set to Static
  IPAddress gateway(192, 168, 4, 1);// gateway set to Static
  IPAddress subnet(255, 255, 255, 0);// subnet set to Static

  WiFi.mode(WIFI_AP);// Working mode only as Acess Point 

  WiFi.softAP(ssid, password, 2, 0);
  WiFi.config(staticIP, gateway, subnet);

  server.begin();

}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  client.print("O que o robozinho deve fazer?");
  
  // Wait until the client sends some data
  while (!client.available()) {
    delay(1);
  }

  String req = client.readStringUntil('\r');
  req = req.substring(req.indexOf("/") + 1, req.indexOf("HTTP") - 1);
  client.flush();

  // Match the request

  if (req.indexOf("start") != -1)// 192.168.4.1/600
  {
    client.print("start");
    Serial.print("start");
    blink_led();
    client.flush();
  }
  else if (req.indexOf("stop") != -1)
  {
    client.print("stop");
    Serial.print("stop");
    blink_led();
    client.flush();
  }  
  else if (req.indexOf("x") != -1)
  {
    client.print(req);
    Serial.print(req);
    blink_led();
    client.flush();
  }  
  else if (req.indexOf("y") != -1)
  {
    client.print(req);
    Serial.print(req);
    blink_led();
    client.flush();
  }  
  else if (req.indexOf("pos") != -1)
  {
    client.print(req);
    Serial.print(req);
    blink_led();
  }  
  else {
    client.flush();
    client.stop();
    return;
  }

  client.print("HTTP/1.1 200 OK\n\n");
  client.flush();
}