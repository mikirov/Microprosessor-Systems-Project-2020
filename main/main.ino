#include <string.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>


#define DEBUG 1 // set to 0 to disable debug mode
#define INPUT_BUFFER_SIZE 100
#define RX_PIN 3
#define TX_PIN 1
#define RED_PIN 5
#define GREEN_PIN 4
#define BLUE_PIN 0


//TODO: read ssid and password from a file or environment variable
const char* ssid = "It hurs when IP"; //put your wifi network name here
const char* password = "12345678"; //put your wifi password here

//statically assigned ip, gateway and subnet mask
IPAddress ip(192, 168, 1, 143);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

int Red = 0, Green = 0, Blue = 0;

//web server on port 80
WiFiServer server(80);
WiFiClient client;

void setup(){
	Serial.begin(152000);
  
	pinMode(TX_PIN, OUTPUT);
	pinMode(RX_PIN, INPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  
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
  #ifdef DEBUG
  Serial.print("Connected to ");
  Serial.println(ssid);

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  #endif
  
  //Start the TCP server
  server.begin();
  
  #ifdef DEBUG
  Serial.printf("Web server started, open %s in a web browser\n", WiFi.localIP().toString().c_str());
  #endif
 
}

void SetLids(){
    analogWrite(RED_PIN, Red);
    analogWrite(GREEN_PIN, Green);
    analogWrite(BLUE_PIN, Blue);
  
}

void loop(){
	HandleBluetoothInput();
	HandleWifiInput();
  SetLeds();
} 

void HandleBluetoothInput(){
	int ReadIndex = 0;
	char SerialInputString[INPUT_BUFFER_SIZE];

	if (Serial.available() > 0) {             
		while (Serial.available() > 0 && ReadIndex < INPUT_BUFFER_SIZE - 1) {
			 SerialInputString[ReadIndex] = Serial.read(); 
			 ReadIndex++;
       if(Serial.available() == false) delayMicroseconds(1200);
             
		}
		SerialInputString[ReadIndex] = '\0'; // put terminating character at the end of the read string
		ProcessInput(SerialInputString);
	}

}

void HandleWifiInput(){
  client = server.available();
  if(client && client.connected()){
    char WifiInputString[INPUT_BUFFER_SIZE];
    int ReadIndex = 0;
    while(client.available() > 0 && ReadIndex < INPUT_BUFFER_SIZE - 1){
        WifiInputString[ReadIndex] = static_cast<char>(client.read());
        ReadIndex++;
    }
    WifiInputString[ReadIndex] = '\0';
    ProcessInput(WifiInputString);
    
    client.stop();
  }
}
  
void ProcessInput(const char Input[]){   

  #ifdef DEBUG
	Serial.println(Input);
	#endif
  
	if(strlen(Input) != 6) return;

  Red = Input[0] * 16 + Input[1];
	Green = Input[2] * 16 + Input[3];
  Blue = Input[4] * 16 + Input[5];

  #ifdef DEBUG
	Serial.printf("Red: %d, Green: %d, Blue: %d", Red, Green, Blue);
  #endif
}
  
