#include <string.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>   // Include the WebServer library

#define DEBUG 1 // set to 0 to disable debug mode
#define INPUT_BUFFER_SIZE 100

#define RED_PIN 5
#define GREEN_PIN 4
#define BLUE_PIN 0


//TODO: read ssid and password from a file or environment variable
const char* ssid = "velios"; //put your wifi network name here
const char* password = "789123654"; //put your wifi password here

ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80

void handleRoot();              // function prototypes for HTTP handlers
void handleNotFound();
void handleColor();

//statically assigned ip, gateway and subnet mask
IPAddress ip(192, 168, 1, 143);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);


void setup(){
  Serial.begin(9600);
  
  pinMode(RED_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  //TODO: read ssid and password from a file or environment variable
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);
  Serial.println("Connecting");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    #ifdef DEBUG
    Serial.println(".");
    #endif
  }

  server.on("/",HTTP_GET, handleRoot);               // Call the 'handleRoot' function when a client requests URI "/"
  server.on("/color", HTTP_POST, handleColor);
  server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  server.begin();                           // Actually start the server
  
  #ifdef DEBUG
  Serial.printf("HTTP server started, open %s in a web browser\n", WiFi.localIP().toString().c_str());
  #endif
 
}

void SetLeds(const int Red, const int Green, const int Blue){
    analogWrite(RED_PIN, Red);
    analogWrite(GREEN_PIN, Green);
    analogWrite(BLUE_PIN, Blue);
  
}

void loop(){
  server.handleClient();                    // Listen for HTTP requests from clients
} 


void ProcessInput(const char Input[]){   

  #ifdef DEBUG
	Serial.println(Input);
	#endif
  
	if(strlen(Input) != 6) return;
  int Red = 0, Green = 0, Blue = 0;

  int Parsed[6];
  for(int i = 0; i < 6; i++){
    if(Input[i] >= '0' && Input[i] <= '9'){
      Parsed[i] = Input[i] - '0';
    }
    else if(Input[i] >= 'a' && Input[i] <= 'z'){
      Parsed[i] = Input[i] - 'a' + 10;  
    }
    else if(Input[i] >= 'A' && Input[i] <= 'Z'){
      Parsed[i] = Input[i] - 'A' + 10;  
    }
    else{
      Serial.println("Invalid input");
      return;  
    }
  }
 
  Red = (Parsed[0]) * 16 + (Parsed[1]);
	Green = (Parsed[2]) * 16 + (Parsed[3]);
  Blue = (Parsed[4]) * 16 + (Parsed[5]);

  #ifdef DEBUG                                                                                                                                                    
	Serial.printf("Red: %d, Green: %d, Blue: %d\n", Red, Green, Blue);
  #endif
  SetLeds(Red, Green, Blue);
}

void handleRoot() {                         // When URI / is requested, send a web page with a button to toggle the LED
  server.send(200, "text/html", "<form action=\"/color\" method=\"POST\"><label for=\"color\"></label><input name=\"color\" type=\"color\" value=\"#ff0000\"><input type=\"submit\" value=\"Submit\"></form>");
}

void handleColor(){
  if( ! server.hasArg("color") || server.arg("color") == NULL) {
    server.send(400, "text/plain", "400: Invalid Request");         // The request is invalid, so send HTTP status 400
    return;
  }
  char temp[6];
//  strcpy(server.arg("color").c_str()[1], temp);
  ProcessInput(&server.arg("color").c_str()[1]);
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303);                         // Send it back to the browser with an HTTP status 303 (See Other) to redirect
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
  
