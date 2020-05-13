#include <SoftwareSerial.h>
SoftwareSerial mySerial(12,13); // RX, TX  


#define RED_PIN 5
#define GREEN_PIN 4
#define BLUE_PIN 0
#define INPUT_BUFFER_SIZE 100
#define DEBUG 1 // set to 0 to disable debug mode

void SetLeds(const int Red, const int Green, const int Blue){
    analogWrite(RED_PIN, Red);
    analogWrite(GREEN_PIN, Green);
    analogWrite(BLUE_PIN, Blue);
  
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

void setup() {
  
  Serial.begin(9600);
  mySerial.begin(9600);
  // put your setup code here, to run once:
  pinMode(RED_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
}

void loop() {
  int ReadIndex = 0;
  char SerialInputString[INPUT_BUFFER_SIZE];

  if (mySerial.available()) {             
    while (mySerial.available() && ReadIndex < INPUT_BUFFER_SIZE - 1) {
       SerialInputString[ReadIndex] = mySerial.read(); 
       ReadIndex++;
       if(mySerial.available() == false) delayMicroseconds(1200);
             
    }
    Serial.println("Got input");
    SerialInputString[6] = '\0'; // put terminating character at the end of the read string
    Serial.printf("%s\n", SerialInputString);
    
    ProcessInput(SerialInputString);
  }
}
