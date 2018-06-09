#define DEBUG 1  //Print to Serial
#define DUDEPIN 12 // Pin to Relay
#define REFRESHTIME 1 //MINUTES
#define WHILE_TO 5000 // milliseconds
#define PRINTDEBUG(STR) \
  {  \
    if (DEBUG) Serial.println(STR); \
  }
#define BIAS 1
#include <ESP8266WiFi.h>
#include "TheDudeParams.h" // Change this file params


//INIT
const char* ssid     = MY_SSID;
const char* password = MY_PWD;
const char* host = MY_HOST; 
const char* hostIP = MY_HOSTIP;
String url = PAGE_URL;
WiFiServer server(80);

// Fucntion to connect WiFi
void connectWifi(const char* ssid, const char* password) {
  int WiFiCounter = 0;
  // We start by connecting to a WiFi network
  PRINTDEBUG("Connecting to ");
  PRINTDEBUG(ssid);
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED && WiFiCounter < 30) {
    
    delay(1000);
    WiFiCounter++;
    
    PRINTDEBUG(".");
  }

        PRINTDEBUG("");
        PRINTDEBUG("WiFi connected");
        PRINTDEBUG("IP address: ");
        PRINTDEBUG(WiFi.localIP());
    
}


// Inifinite loop - Causes to reset self
void resetSelf() {
  PRINTDEBUG("Reseting");
  while (1) {}
}

void setup() {
  if (DEBUG) Serial.begin(115200);  //Start Serial
  delay(10);
  connectWifi(ssid, password); // Start WiFi
  server.begin();  // Start Server
  delay(10);
  PRINTDEBUG("End of Setup");
}



void loop() {

  PRINTDEBUG("Start Loop");
  delay(5000);
  delay(10);
  int connectFails = 0;

  while ((WiFi.status() != WL_CONNECTED)) {
    PRINTDEBUG("WIFI Failed ammore");
    connectWifi(ssid, password);
    connectFails++;
    if (connectFails > 4) {
      resetSelf();  // If 2 minutes passed with no connection - Reset Self
    }
  }


// Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  } else {
  // We now create a URI for the request
  String url = "/ewsf/report.php/";
  url += "?myState=";
  url += 20;

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");

  }
}
