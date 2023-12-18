#include <EEPROM.h>

int ThermistorPin1 = 26;
int ThermistorPin2 = 27;
int ThermistorPin3 = 28;

float temp1, temp2, temp3;

int blue = 2;
int green = 3;
int red = 4;

float diff = 0;
String inStr = "";

int closeCounter = 0;
int wait = 3600;
int timeOut = 100;

bool ambientFlipFlop = false;

int minAmbient = 20;

bool firstRun = false;

void setup() {
  Serial.begin(300);

  EEPROM.begin(1024);


  diff = readFromEEPROM(0);
  timeOut = readFromEEPROM(32);
  wait = readFromEEPROM(64);
  minAmbient = readFromEEPROM(96);


  pinMode(5, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

  for (int i = 0; i < 10; i++) {
    digitalWrite(red, HIGH);

    delay(200);
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH);
    delay(200);
    digitalWrite(green, LOW);
    digitalWrite(blue, HIGH);
    delay(200);
    digitalWrite(blue, LOW);
    delay(400);
  }

  if (getTemp(ThermistorPin3) > minAmbient) {
    digitalWrite(5, HIGH);
    delay(500);
  }
}

void loop() {


  // digitalWrite(5, LOW);

  if (Serial.available() > 0) {
    while (Serial.available() > 0) {

      char input = Serial.read();
      inStr += input;
      Serial.println("_");
      Serial.println(input);
    }

    inStr = inStr.substring(0, inStr.length());
    handleString(inStr);
    inStr = "";
  }



  temp1 = getTemp(ThermistorPin1);
  temp2 = getTemp(ThermistorPin2);
  temp3 = getTemp(ThermistorPin3);
  //Serial.println(EEPROM.length());
  Serial.print("Temperature In: ");
  Serial.print(temp1);
  Serial.print("    Temperature Out: ");
  Serial.print(temp2);
  Serial.print("    Temperature Ambient: ");
  Serial.print(temp3);

  Serial.print("    Diff ");
  Serial.print(diff);
  Serial.print("    minAmbient: ");
  Serial.print(minAmbient);
  Serial.print("    Timeout: ");
  Serial.print(timeOut);
  Serial.print("    Wait: ");
  Serial.print(wait);

  int openTemp = temp1 - temp2 - diff;


  if (temp3 > minAmbient + 2) {
    ambientFlipFlop = true;

  } else if (temp3 < minAmbient) {
    ambientFlipFlop = false;
  }

  if (ambientFlipFlop) {
    firstRun=true;
    Serial.print("    Temperature Diffrence Currently: ");
    Serial.print(openTemp);

    Serial.println(" ");

    delay(500);
    if (openTemp > 0) {
      digitalWrite(red, HIGH);
      delay(1000);
      Serial.print("Waiting ");
      Serial.print(closeCounter);
      Serial.println("/");
      Serial.println(timeOut);

      if (closeCounter == timeOut) {
        Serial.println("Timeout HIT");
        digitalWrite(red, HIGH);
        digitalWrite(5, LOW);

        delay(wait * 1000);
        closeCounter = 0;
        digitalWrite(red, LOW);
        digitalWrite(5, HIGH);
      } else {
        closeCounter++;
        digitalWrite(red, LOW);
      }

    } else {
      delay(500);
      digitalWrite(green, HIGH);
      digitalWrite(5, HIGH);
      closeCounter = 0;
    }

    delay(500);
    digitalWrite(green, LOW);
  } else {

digitalWrite(5, LOW);
    if(firstRun){
      firstRun = false;
      digitalWrite(blue, HIGH);
      delay(120000);
      
    }
    Serial.print("  Waiting For required tempreature ");
    Serial.print(temp3);
    Serial.print("<");
    Serial.print(minAmbient);
    Serial.println("+1");
    
    delay(500);
    digitalWrite(blue, HIGH);
    delay(500);
    digitalWrite(blue, LOW);
  }
}

float getTemp(int inPin) {

  int Vo;
  float R1 = 10000;
  float logR2, R2, T, Tc, Tf;
  float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

  Vo = analogRead(inPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
  Tc = T - 273.15;
  Tf = (Tc * 9.0) / 5.0 + 32.0;
  return Tc;
}

void handleString(String inputStr) {
  Serial.println(inputStr);

  if (inputStr.equals("HELLO")) {
    Serial.println("GENERAL KANOBI");
  }


  if (inputStr.indexOf("=") != -1) {
    if (inputStr.indexOf("DIFF") != -1) {
      diff = inputStr.substring(inputStr.indexOf("=") + 1, inputStr.length()).toFloat();

      Serial.println(diff);
      writeToEEPROM(diff, 0);
    }
    if (inputStr.indexOf("TIMEOUT") != -1) {
      timeOut = inputStr.substring(inputStr.indexOf("=") + 1, inputStr.length()).toInt() / 2;

      Serial.println(timeOut * 2);
      writeToEEPROM(timeOut, 32);
    }
    if (inputStr.indexOf("WAIT") != -1) {
      wait = inputStr.substring(inputStr.indexOf("=") + 1, inputStr.length()).toInt();

      Serial.println(wait);
      writeToEEPROM(wait, 64);
    }
    if (inputStr.indexOf("MINAMBIENT") != -1) {
      minAmbient = inputStr.substring(inputStr.indexOf("=") + 1, inputStr.length()).toInt();
      writeToEEPROM(minAmbient, 96);
      Serial.println(minAmbient);
    }
  }
}

void writeToEEPROM(float value, int address) {
  // Ensure the address is valid
  if (address < 0 || address >= EEPROM.length()) {
    Serial.println("Address out of range.");
    return;
  }

  // Write the float to EEPROM
  EEPROM.put(address, value);
  EEPROM.commit();
}

float readFromEEPROM(int address) {
  float value;

  // Ensure the address is valid
  if (address < 0 || address >= EEPROM.length()) {
    Serial.println("Address out of range.");
    return 0;  // Return zero or some error value
  }

  // Read the float from EEPROM
  EEPROM.get(address, value);
  return value;
}
