/******************************************************************
 Created with PLATFORMIO IDE for VSCODE - 7.05.2020
 Project     : Ap_http
 Libraries   : 
 Author      : Mujahid Abdullah
 Description : 
******************************************************************/

#include <Arduino.h>
#include <WiFi.h> 
#include <WebServer.h>

//declare functions
String display_page();
void handlerProcess();
void http_request_text(String req);
void start_up(const char *ssid_,const char *pass_);
void Start_Ap();

// Access point network credentials
const char* Ap_ssid     = "ESP32-Access-Point";
const char* Ap_password = "123456789";
int Ap_port = 80;

// WiFi network credentials
const char* ssid  ;
const char* password ;
int port = 2200;

//Object Instance
WebServer Ap_server(Ap_port);
WiFiClient check_client;

// globals
String header;
bool wifi_web = false;
bool ap_web = false ;

void setup() 
{
  Serial.begin(115200);
  //setting Access point
  Serial.println("Setting Access Point …");
  WiFi.softAP(Ap_ssid);

  Ap_server.on("/",display_page);
  Ap_server.on("/get",HTTP_POST, handlerProcess);
  Ap_server.on("/Ap", Start_Ap);

  
  //Getting Ip of Access Point
  IPAddress IP = WiFi.softAPIP();
  Serial.println("AP IP address: ");
  Serial.println(IP);
  Ap_server.begin();
  ap_web = true;
}

void loop() 
{
  Ap_server.handleClient();
}

String display_page()
{
  String page = "<!DOCTYPE html>\n";
      page += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
      if(ap_web == true) page += "<title>Access Point WEB SERVER</title>"; else page += "<title>WiFi WEB SERVER</title>";
      page +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
      page +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
      page +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
      page +="</style>\n";
      page +=    "<html>";
      page +=     "<head>";
      page +=     "</head>";
      page +=       "<body>";
      if(ap_web == true) page += "<h2>Access Point WEB SERVER</h2>"; else page += "<h2>WiFi WEB SERVER</h2>";
      page += "<form action=\"/get\" method =\"post\">"; page += "<label for=\"user\">User name:</label><br>"; page += "<input type=\"text\" id=\"user\" name=\"user\" placeholder=\"ENTER SSID\"><br><br>";
      page += "<label for=\"pass\">Password:</label><br>"; page += "<input type=\"text\" id=\"pass\" name=\"pass\"><br><br>";
      page +=  "<input type=\"submit\" name='SUBMIT' value=\"SUBMIT\">";
      if(wifi_web == true) page += "<input type=\"submit\" formaction=\"/Ap\" value=\"Start as Access Point\">"; page += "</form>";
      page +=     "</body>";
      page +=     "</html>";
      Ap_server.send(200, "text/html", page);
      return page;
}

void http_request_text(String req)
{
  Ap_server.send(200, "text/html", req);
}

void handlerProcess()
{
  if(Ap_server.hasArg("user") && Ap_server.hasArg("pass"))
  {
    String ssid_ = Ap_server.arg("user"); 
    String password_ = Ap_server.arg("pass"); 
    char _ssid_[ssid_.length()+1]; char _password_[password_.length()+1];
    ssid_.toCharArray(_ssid_, ssid_.length()+1);
    password_.toCharArray(_password_, password_.length()+1);

      int network = WiFi.scanNetworks();
      if(network == 0) http_request_text("No Network found");
      else
      {
        for(int i = 0; i< network; i++)
        {
          String ssid_name = WiFi.SSID(i);
          delay(100);
          if(!ssid_name.compareTo(ssid_))
          {
            WiFi.disconnect();
            ap_web = false;
              WiFi.softAPdisconnect(true);
              WiFi.mode(WIFI_STA);
              start_up(_ssid_,_password_);
              Serial.println(WiFi.localIP());
               wifi_web = true; 
          }    
          else
          {
            http_request_text("Network not found");
          }
          
        }
      }
  }
}

void start_up(const char *ssid_,const char *pass_)
{
    delay(10);
    Serial.println("Connecting to ");
    Serial.println(ssid_);
    WiFi.begin(ssid_, pass_);
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
}

void Start_Ap()
{
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_AP_STA);
  Serial.println("Setting Access Point …");
  WiFi.softAP(Ap_ssid);
 IPAddress IP = WiFi.softAPIP();
 Serial.println("AP IP address: ");
 Serial.println(IP);
 http_request_text("Access point Started");

} 

 