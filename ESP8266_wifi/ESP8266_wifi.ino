#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

/* Set these to your desired credentials. */
const char *ssid_AP = "ESP8266";
const char *password_AP = "";
MDNSResponder mdns;
ESP8266WebServer server_AP(80);


char *ssid_Clinet     = "";
char *password_Clinet = "";

int LED = 16;

const char INDEX_HTML[] =
  "<!DOCTYPE html>"
  "<html>"
  "    <head>"
  "        <meta http-equiv=\"Content-Type\" content=\"text/html; charset=Big5\">"
  "        <title>ESP8266 Web Form Demo</title>"
  "        <style>\"body { background-color: #808080; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }\"</style>"
  "    </head>"
  "    <body>"
  "        <FORM action=\"/\" method=\"post\">"
  "            <h1>輸入Wifi名稱及密碼</h1>"
  "            SSID <input type=\"text\" name=\"ssid\"/><BR>"
  "            密碼 <input type=\"text\" name=\"passwd\"/><BR>"
  "            <input type=\"submit\" value=\"送出\" />"
  "        </FORM>"
  "        <FORM action=\"/\" method=\"post\">"
  "            <h1>Set LED</h1>"
  "            <INPUT type=\"radio\" name=\"LED\" value=\"1\" />On<BR>"
  "            <INPUT type=\"radio\" name=\"LED\" value=\"0\" />Off<BR>"
  "            <INPUT type=\"submit\" value=\"Set\" />"
  "        </FORM>"
  "    </body>"
  "</html>";

void handleRoot() {
  if (server_AP.hasArg("ssid")) {
    handleSubmit();
  }
  else if (server_AP.hasArg("LED")) {
    handleSubmit_LED();
  }
  else {
    server_AP.send(200, "text/html", INDEX_HTML);
  }
}

void returnFail(String msg)
{
  server_AP.sendHeader("Connection", "close");
  server_AP.sendHeader("Access-Control-Allow-Origin", "*");
  server_AP.send(500, "text/plain", msg + "\r\n");
}

void handleSubmit()
{
  if (!server_AP.hasArg("ssid")) return returnFail("BAD ARGS");
  if (!server_AP.hasArg("passwd")) return returnFail("BAD ARGS");

  String data1 = server_AP.arg("ssid");
  ssid_Clinet = const_cast<char*>(data1.c_str());
  String data2 = server_AP.arg("passwd");
  password_Clinet = const_cast<char*>(data2.c_str());
  Serial.println("----------------------");
  Serial.print("SSID:");
  Serial.println(ssid_Clinet);
  Serial.print("Passwd:");
  Serial.println(password_Clinet);
  Serial.println("----------------------");

  Serial.println("OK");
  //  WiFi.begin(string2char(server_AP.arg("ssid")), string2char(server_AP.arg("passwd")));   //
  WiFi.begin(ssid_Clinet, password_Clinet);   //
  Serial.println("");
  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < 20) {
    delay(500);
    Serial.print(".");
    retry++;
  }
  Serial.println(WiFi.localIP());

  server_AP.send(200, "text/html", INDEX_HTML);
}

void handleSubmit_LED()
{  
  if (!server_AP.hasArg("LED")) return returnFail("BAD ARGS");
  String LEDvalue = server_AP.arg("LED");
  if (LEDvalue == "1") {
    digitalWrite(LED, HIGH);
    server_AP.send(200, "text/html", INDEX_HTML);
  }
  else if (LEDvalue == "0") {
    digitalWrite(LED, LOW);
    server_AP.send(200, "text/html", INDEX_HTML);
  }
  else {
    returnFail("Bad LED value");
  }
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid_AP, password_AP);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server_AP.on("/", handleRoot);
  server_AP.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server_AP.handleClient();
}
