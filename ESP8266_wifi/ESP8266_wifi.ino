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

const char INDEX_HTML[] =
  "<!DOCTYPE html>"
  "<html>"
  " <head>"
  "    <meta name = \"viewport\" content = \"width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0\">"
  "   <title>ESP8266 Web Form Demo</title>"
  "   <style>"
  "     \"body { background-color: #808080; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }\""
  "   </style>"
  " </head>"
  " <body>"
  "   <FORM action=\"/\" method=\"post\">"
  "     <h1>輸入Wifi名稱及密碼</h1>"
  "     SSID <input type=\"text\" name=\"ssid\"/><BR>"
  "     密碼 <input type=\"text\" name=\"passwd\"/><BR>"
  "     <input type=\"submit\" value=\"送出\" />"
  "   </FORM>"
  " </body>"
  "</html>";

void handleRoot() {
  if (server_AP.hasArg("ssid")) {
    handleSubmit();
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
  ssid_Clinet = string2char(server_AP.arg("ssid"));
//  temp.toCharArray(ssid_Clinet, temp.length() + 1);

  if (!server_AP.hasArg("passwd")) return returnFail("BAD ARGS");
  password_Clinet = string2char(server_AP.arg("passwd"));
//  temp.toCharArray(password_Clinet, temp.length() + 1);

  Serial.println(ssid_Clinet);
  Serial.println(password_Clinet);

  Serial.println("OK");
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
void setup() {
  delay(1000);
  Serial.begin(115200);
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

char* string2char(String command){
    if(command.length()!=0){
        char *p = const_cast<char*>(command.c_str());
        return p;
    }
}
