#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

const char* ssid = "final";
const char* password = "12345678";

double round2(double value) {
  return (int)(value * 100 + 0.5) / 100.0;
}

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

String webpage = "<h1 style='text-align:center;'>Water Tester Data Monitor</h1><p><span style='color:#003366;'>PH Value : <span id='rand1'>-</span></span></p><p><span style='color:#003366;'>Turbidity : <span id='rand2'>-</span></span></p><script>var Socket;function init() { Socket = new WebSocket('ws://' + window.location.hostname + ':81/'); Socket.onmessage = function (event) { processCommand(event); };}function processCommand(event) { var obj = JSON.parse(event.data); document.getElementById('rand1') .innerHTML = obj.rand1; document.getElementById('rand2') .innerHTML = obj.rand2; console.log(obj.rand1); console.log(obj.rand2);}window.onload = function (event) { init();}</script>";


int interval = 1000;
unsigned long previousMillis = 0;

StaticJsonDocument<200> doctx;
StaticJsonDocument<200> docrx;

void setup() {
   Serial.begin(9600);

  WiFi.softAP(ssid, password);
  
  Serial.println("WiFi AP configured. SSID: " + String(ssid));

  
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

   server.on("/", []() {
    server.send(200, "text\html", webpage);
   });

   server.begin();
   webSocket.begin();

}

void loop() {
  String data = Serial.readStringUntil(','); // Read until the comma delimiter
    float ph = data.toFloat(); // Convert the string to float
    data = Serial.readStringUntil('\n'); // Read until newline
    float td = data.toFloat(); // Convert the second part to float

    // Print the received data
    Serial.print("Received data1: ");
    Serial.println(ph);
    Serial.print("Received data2: ");
    Serial.println(td);
    delay(1000);
 
    
  server.handleClient();
    webSocket.loop();

    unsigned long now = millis();
    if(now - previousMillis > interval) {
      String jsonString = "";
      JsonObject object = doctx.to<JsonObject>();
      object["rand1"] = round2(ph);
      object["rand2"] = round2(td);
      serializeJson(doctx, jsonString);
      Serial.println(jsonString);
      webSocket.broadcastTXT(jsonString);
      previousMillis = now;
  // put your main code here, to run repeatedly:
    }
}
