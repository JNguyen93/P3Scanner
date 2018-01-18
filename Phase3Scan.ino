const byte photo = A0;
const byte motorPin1 = 2;
const byte motorPin2 = 3;
const byte motorPin3 = 4;
const byte motorPin4 = 5;
const byte greenLED = 9;
const byte redLED = 10;
const byte relay = 8;
const byte lineLaser = 6;
const byte RFIDreader = 7;
const int threshold = 50;

int photoValue = 0;
boolean correctVial = false;
int motorSpeed = 1200;
int count = 0;
int countsperrev = 90;
int lookup[8] = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001};

void setup() {
  Serial.begin(9600);
  Serial.print("Scanner Online");
  pinMode(lineLaser, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(photo, INPUT);
  pinMode(correctVial, INPUT);
}

void loop() {
  photoValue = analogRead(photo);
  Serial.print("Photosensor Value: ");
  Serial.println(photoValue);
  
  if (photoValue <= threshold){
    scan();
    correctVial = digitalRead(RFIDreader);
    if (correctVial){
      digitalWrite(greenLED, HIGH);
      digitalWrite(relay, HIGH);
      delay(5000);
      digitalWrite(relay, LOW);
      digitalWrite(greenLED, LOW);
    }
    else{
      digitalWrite(redLED, HIGH);
      delay(1000);
      digitalWrite(redLED, LOW);
      digitalWrite(relay, LOW);
    }
  }
  
}

void scan(){
  boolean finished = false;
  digitalWrite(lineLaser, HIGH);
  while(!finished){
    Serial.print("Count: ");
    Serial.println(count);
    if (count < countsperrev)
      clockwise();
    else if (count == countsperrev * 2){
      count = 0;
      finished = true;
    }
    else
      counterclockwise();
    if (!finished){
      count++;
    }
  }
  digitalWrite(lineLaser, LOW);
}

void counterclockwise(){
  for (int i = 0; i < 8; i++){
    setOutput(i);
    delayMicroseconds(motorSpeed);
  }
}

void clockwise(){
  for (int i = 7; i >= 0; i--){
    setOutput(i);
    delayMicroseconds(motorSpeed);
  }
}

void setOutput(int out){
  digitalWrite(motorPin1, bitRead(lookup[out], 0));
  digitalWrite(motorPin2, bitRead(lookup[out], 1));
  digitalWrite(motorPin3, bitRead(lookup[out], 2));
  digitalWrite(motorPin4, bitRead(lookup[out], 3));
}

