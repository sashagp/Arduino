// Pin configuration for ULN2003 driver
int Pin1 = 26; // IN1
int Pin2 = 25; // IN2  
int Pin3 = 33; // IN3
int Pin4 = 32; // IN4

// Step sequence for 28BYJ-48 stepper motor
int pole1[] = {0, 0, 0, 0, 0, 1, 1, 1, 0};
int pole2[] = {0, 0, 0, 1, 1, 1, 0, 0, 0};
int pole3[] = {0, 1, 1, 1, 0, 0, 0, 0, 0};
int pole4[] = {1, 1, 0, 0, 0, 0, 0, 1, 0};

int poleStep = 0;
int dirStatus = 3; // 1 = CCW, 2 = CW, 3 = STOP

String buttonTitle1[] = {"CCW", "CW"};
String buttonTitle2[] = {"CCW", "CW"};
String argId[] = {"ccw", "cw"};

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char *ssid = "wifi";
const char *password = "hotspottt";

WebServer server(80);

// Handles main page UI
void handleRoot() {
  String HTML = "<!DOCTYPE html>\
<html>\
<head>\
<title>Robojax 28BYJ-48 Motor Control</title>\
<meta name='viewport' content='width=device-width, initial-scale=1'>\
<style>\
html,body{width:100%;height:100%;margin:0}\
*{box-sizing:border-box}\
.colorAll{background-color:#90ee90}\
.colorBtn{background-color:#add8e6}\
.angleButton,a{font-size:30px;border:1px solid #ccc;display:table-caption;padding:7px 10px;text-decoration:none;cursor:pointer;padding:5px 6px 7px 10px}\
a{display:block}\
.btn{margin:5px;border:none;display:inline-block;vertical-align:middle;text-align:center;white-space:nowrap}\
</style>\
</head>\
<body>\
<h1>Robojax 28BYJ-48 Stepper Motor Control </h1>";

  if (dirStatus == 2) {
    HTML += "<h2><span style='background-color: #FFFF00'>Motor Running in CW</span></h2>";
  } else if (dirStatus == 1) {
    HTML += "<h2><span style='background-color: #FFFF00'>Motor Running in CCW</span></h2>";
  } else {
    HTML += "<h2><span style='background-color: #FFFF00'>Motor OFF</span></h2>";
  }

  // CCW button
  HTML += "<div class='btn'><a class='angleButton' style='background-color:";
  HTML += (dirStatus == 1) ? "#f56464" : "#90ee90";
  HTML += "' href='/motor?";
  HTML += argId[0];
  HTML += (dirStatus == 1) ? "=off'>" : "=on'>";
  HTML += (dirStatus == 1) ? buttonTitle1[0] : buttonTitle2[0];
  HTML += "</a></div>";

  // CW button
  HTML += "<div class='btn'><a class='angleButton' style='background-color:";
  HTML += (dirStatus == 2) ? "#f56464" : "#90ee90";
  HTML += "' href='/motor?";
  HTML += argId[1];
  HTML += (dirStatus == 2) ? "=off'>" : "=on'>";
  HTML += (dirStatus == 2) ? buttonTitle1[1] : buttonTitle2[1];
  HTML += "</a></div>";

  HTML += "</body></html>";
  server.send(200, "text/html", HTML);
}

// Handles 404 error
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: " + server.uri() + "\n";
  message += "Method: " + String((server.method() == HTTP_GET) ? "GET" : "POST") + "\n";
  message += "Arguments: " + String(server.args()) + "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}

// Setup function
void setup() {
  pinMode(Pin1, OUTPUT);
  pinMode(Pin2, OUTPUT);
  pinMode(Pin3, OUTPUT);
  pinMode(Pin4, OUTPUT);

  Serial.begin(115200);
  Serial.println("Robojax 28BYJ-48 Stepper Motor Control");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: http://");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("robojaxESP32")) {
    Serial.println("MDNS responder started");
    Serial.println("Access via http://robojaxESP32");
  }

  server.on("/", handleRoot);
  server.on("/motor", HTTP_GET, motorControl);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

// Main loop
void loop() {
  server.handleClient();

  if (dirStatus == 1) { // CCW
    poleStep++;
    driveStepper(poleStep);
  } else if (dirStatus == 2) { // CW
    poleStep--;
    driveStepper(poleStep);
  } else {
    driveStepper(8); // Off step
  }

  if (poleStep > 7) poleStep = 0;
  if (poleStep < 0) poleStep = 7;

  delay(1);
}

// Handle motor control requests
void motorControl() {
  if (server.arg(argId[0]) == "on") {
    dirStatus = 1; // CCW
  } else if (server.arg(argId[0]) == "off") {
    dirStatus = 3; // OFF
  } else if (server.arg(argId[1]) == "on") {
    dirStatus = 2; // CW
  } else if (server.arg(argId[1]) == "off") {
    dirStatus = 3; // OFF
  }

  handleRoot(); // Reload UI
}

// Sends signal to the motor coils
void driveStepper(int c) {
  digitalWrite(Pin1, pole1[c]);
  digitalWrite(Pin2, pole2[c]);
  digitalWrite(Pin3, pole3[c]);
  digitalWrite(Pin4, pole4[c]);
}
