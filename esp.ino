#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

int R = 0; 
int G = 0; 
int B = 0; 
bool validRGB = true;

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
    String inputMessage;
    String inputParam;
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      R = inputMessage.toInt();
      validRGB = isRGB(R);
    }
    if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_2;
      G = inputMessage.toInt(); 
      validRGB = isRGB(G);
    }
    if (request->hasParam(PARAM_INPUT_3)) {
      inputMessage = request->getParam(PARAM_INPUT_3)->value();
      inputParam = PARAM_INPUT_3;
      B = inputMessage.toInt();
      validRGB = isRGB(B);
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    request->send(200, "text/html", "<br><a href=\"/\">Return to Home Page</a>");
  });
  server.onNotFound(notFound);
  server.begin();
}


bool isRGB(const int & rgb) {
  return (rgb>=0 && rgb<=255);
}


void loop() {
  analogWrite(23, R);
  analogWrite(22, G);
  analogWrite(21, B);
  delay(1000); 
}
