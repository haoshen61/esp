#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

int R = 0; 
int G = 0; 
int B = 0; 
bool blink; 

char* PARAM_INPUT_1 = "red";
char* PARAM_INPUT_2 = "green";
char* PARAM_INPUT_3 = "blue";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<body>

<h2>Costomize Your LED Color!</h2>

<form action="/get">
  <label for="red">Red:</label><br>
  <input type="text" id="red" name="red" value=""><br>
  <label for="green">Green:</label><br>
  <input type="text" id="green" name="green" green=""><br>
  <label for="blue">Blue:</label><br>
  <input type="text" id="blue" name="blue" green=""><br><br>
  <input type="submit" value="Submit">
</form> 

</body>
</html>
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {
  Serial.begin(115200);
  pinMode(23, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(21, OUTPUT);
  WiFi.mode(WIFI_STA);
  WiFi.begin("Wokwi-GUEST", "", 6);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    int newColor;
    blink = false; 
    R = 0; 
    G = 0; 
    B = 0; 
    if (request->hasParam(PARAM_INPUT_1)) {
      newColor = request->getParam(PARAM_INPUT_1)->value().toInt();
      R = newColor % 256; 
      if (newColor < 0 || newColor > 255) {
        blink = true; 
      }
    }
    if (request->hasParam(PARAM_INPUT_2)) {
      newColor = request->getParam(PARAM_INPUT_2)->value().toInt();
      G = newColor % 256; 
      if (newColor < 0 || newColor > 255) {
        blink = true; 
      }
    }
    if (request->hasParam(PARAM_INPUT_3)) {
      newColor = request->getParam(PARAM_INPUT_3)->value().toInt();
      B = newColor % 256; 
      if (newColor < 0 || newColor > 255) {
        blink = true; 
      }
    }
    request->send(200, "text/html", "<br><a href=\"/\">Return to Home Page</a>");
  });
  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  analogWrite(23, R);
  analogWrite(22, G);
  analogWrite(21, B);
  delay(1000); 
  if (blink) {
    analogWrite(23, 0);
    analogWrite(22, 0);
    analogWrite(21, 0);
    delay(1000); 
  }
}