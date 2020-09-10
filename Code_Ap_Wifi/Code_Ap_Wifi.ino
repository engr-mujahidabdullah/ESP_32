#include <WiFi.h>
#include <WebServer.h>

/* Put your SSID & Password */
const char* ssid = "ESP32";  // Enter SSID here
const char* password = "12345678";  //Enter Password here
String prev_ssid;
String prev_pass;
WebServer server(80);

boolean wif = false;
boolean found;
void handlesave(){
  if (server.hasArg("USERNAME") && server.hasArg("PASSWORD")) {
    found = false;
    Serial.println("Connecting to "+server.arg("USERNAME"));
    char ssidw[server.arg("USERNAME").length()+1];
    server.arg("USERNAME").toCharArray(ssidw,server.arg("USERNAME").length()+1);
    char passwordw[server.arg("PASSWORD").length()+1];
    server.arg("PASSWORD").toCharArray(passwordw,server.arg("PASSWORD").length()+1);
    server.send(200, "text/plain", "Data sent to ESP");

    Serial.println("scan start");

    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0) {
        Serial.println("no networks found");
    } else {
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            String str(ssidw);
            if(str.equals( WiFi.SSID(i))){
              Serial.println("SSID Found");
              found = true;
              }
            delay(10);
        }
    }
    if(found){
      Serial.println("");
    
    WiFi.disconnect();
//    Serial.println(ssidw);
//    Serial.println(passwordw);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssidw, passwordw);
    Serial.println("");
  int delaay =0;
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    delaay += 500; 
    Serial.print(".");
    if (delaay >10000){
      Serial.println("Can't Connect");
      server.sendHeader("Location", "/");
      server.sendHeader("Cache-Control", "no-cache");
      server.send(301);
      if(prev_ssid == NULL && prev_pass == NULL){
      Serial.println("Turning back to AP");
      WiFi.softAP(ssid, password);}

      else{
        char id[prev_ssid.length()+1];
        char pass[prev_pass.length()+1];
        prev_ssid.toCharArray(id,prev_ssid.length()+1);
        prev_pass.toCharArray(pass,prev_pass.length()+1);
        //Serial.println(id);
        //Serial.println(pass);
        WiFi.begin(id,pass);
        while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.print("Connected Back to ");
    Serial.println(prev_ssid);
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());}
    return;}
  }
  Serial.println("Changing prev");
  prev_ssid = String(ssidw);
  prev_pass = String(passwordw);
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssidw);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  wif = true;
  }
  else{
    Serial.println("SSID Not Found");
    server.sendHeader("Location", "/");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);}
  }
    }
void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, password);
  delay(100);
  
  server.on("/", SendHTML);
  server.on("/save", handlesave);
  server.onNotFound(handle_NotFound);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  if(wif)
  {ptr +="<title>Wifi server</title>\n";}
  else
  {ptr +="<title>AP server</title>\n";}
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 7px 12px;text-decoration: none;font-size: 10px;margin: 2px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP32 Web Server</h1>\n";
  if(wif){ptr +="<h3>Using WiFi Mode</h3>\n";}
  else{
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";}
  ptr +="<form action='/save' method='POST'>";
  ptr += "Username :<input type='text' name='USERNAME' placeholder='user name' required><br>";
  ptr += "Password :<input type='password' name='PASSWORD' placeholder='password' required><br>";
  ptr += "<input type='submit' name='SUBMIT' value='Submit'></form>";
  
  ptr +="</body>\n";
  ptr +="</html>\n";
  server.send(200, "text/html", ptr);
  return ptr;
}
