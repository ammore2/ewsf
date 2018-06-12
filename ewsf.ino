
#include <ESP8266WebServer.h>
 
ESP8266WebServer server(80);

const char* host = "192.168.1.6";
 
void setup() {
 
  Serial.begin(115200);
  WiFi.begin("Tenda_D15D48", "NgAhBaH888");  //Connect to the WiFi network
 
  while (WiFi.status() != WL_CONNECTED) {  //Wait for connection
 
    delay(500);
    Serial.println("Waiting to connect…");
 
  }
 
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //Print the local IP
 
  server.on("/other", []() {   //Define the handling function for the path
 
    server.send(200, "text / plain", "Other URL");
 
  });


   server.on("/one", []() {   //Define the handling function for the path
 
    server.send(200, "text / plain", "Other URL");
    Serial.print("One");
  });


    server.on("/two", []() {   //Define the handling function for the path
 
    server.send(200, "text / plain", "Other URL");
    Serial.print("Two");
 
  });

    server.on("/three", []() {   //Define the handling function for the path

     Serial.print("Three");
     server.send(200, "text / plain", "Other URL");
 
  });
 
  server.on("/", handleRootPath);    //Associate the handler function to the path
  server.begin();                    //Start the server
  Serial.println("Server listening");
 
}
 
void loop() {
 
  server.handleClient();         //Handling of incoming requests

   // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  } else {
    // We now create a URI for the request
    String url = "http://192.168.1.6/test/handle.blade.php";
    url += "?status=";
        url += 3650;

    Serial.print("Requesting URL: ");
    Serial.println(url);

//     This will send the request to the server
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
    }

    Serial.println();
    Serial.println("closing connection");

  }
}
 
void handleRootPath() {            //Handler for the rooth path
 
  server.send(200, "text/plain", "Hello world");
 
}
