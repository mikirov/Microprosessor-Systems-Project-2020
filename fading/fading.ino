
#define RED_PIN 5
#define GREEN_PIN 4
#define BLUE_PIN 0

void setup() {
  // put your setup code here, to run once:
  pinMode(RED_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
   for(int i=0; i<255; i++){
    analogWrite(RED_PIN, i);
    analogWrite(BLUE_PIN, i);
    analogWrite(GREEN_PIN, i);
    delay(5);
  }
  for(int i=255; i>0; i--){
    analogWrite(RED_PIN, i);
    analogWrite(BLUE_PIN, i);
    analogWrite(GREEN_PIN, i);
    delay(5);
  } 
}
