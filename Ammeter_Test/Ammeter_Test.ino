

const int ammeter1Pin = 15;


void setup() {
  Serial.begin(115200);

pinMode(ammeter1Pin, INPUT);
  

}

void loop() {
  // put your main code here, to run repeatedly:


int input = analogRead(ammeter1Pin);

Serial.println(input);
delay(5);



}
