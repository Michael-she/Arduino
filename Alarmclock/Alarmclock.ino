// - Lien vidéo: https://youtu.be/g5hJh01psXk
// - Time Library: https://github.com/3tawi/PicoEspTime
// - Getting Started Raspberry Pi Pico : https://youtu.be/KiM6-Hmxf3s
// - Find All "Great Projects" Videos : https://www.youtube.com/c/GreatProjects

#include <PicoEspTime.h>

PicoEspTime rtc;

const int buzzerPin = 10;

const int muteSW1 = 21;
const int muteSW2 = 20;

const int light = 22;

const int relayPin = 2;

uint32_t lastTime; 

bool armState = true;
int swState = 0;

void setup() {
  pinMode(relayPin, OUTPUT);
  pinMode(light, OUTPUT);
  pinMode(muteSW1, INPUT);
  pinMode(muteSW2, INPUT);

  swState = updateSWState();


  Serial.begin(115200);
 rtc.adjust(13, 0, 0, 2024, 1, 1); // 01:26:21 03 Jun 2022
// rtc.adjust(1654219721); // Friday, June 03 2022 01:28:41
armState = true;
delay(1000);
}
void loop() {
   if(millis() - lastTime >= 1000) {
    rtc.read(); 
    PrintTime();
   
    Serial.print("ARM STATE: ");
    Serial.println(armState);

   lastTime = millis();
    }

if(swState != updateSWState()){
swState = updateSWState();
Serial.println("Switch Changed");
armState = false;

digitalWrite(relayPin , HIGH);
delay(1000);
}


}
void PrintTime()
{ 
  Serial.println(rtc.getTime("%A, %B %d %Y %H:%M:%S"));   // (String) returns time with specified format   
// formating options  https://www.cplusplus.com/reference/ctime/strftime/

      Serial.println(""); 
      Serial.println(rtc.getEpoch());         //  (long)    1647216003
      Serial.println(rtc.second);             //  (int)     30    (0-59)
      Serial.println(rtc.minute);             //  (int)     58    (0-59)
      Serial.println(rtc.hour);               //  (int)     0     (0-23)
      Serial.println(rtc.dayOfMonth);         //  (int)     14    (1-31)
      Serial.println(rtc.month);              //  (int)     3     (1-12)
      Serial.println(rtc.year);               //  (int)     2022
      Serial.println(rtc.dayOfWeek);          //  (int)     1     (0-6)
      Serial.println(""); 


if (rtc.hour <= 12){
    digitalWrite(relayPin , HIGH);
    armState = true;
  }else if (armState){
    digitalWrite(relayPin , LOW);
  }

} 


int updateSWState(){

  const int pos1 = digitalRead(muteSW1);
  const int pos2 = digitalRead(muteSW2);

  Serial.print("Switch States: ");
  Serial.print(pos1);
  Serial.print("   ");
  Serial.print(pos2);
  Serial.println();
  

  if(pos1 == HIGH){
    return 1;
  }
  else if(pos2 == HIGH){
    return 2;
  }
  else{
    return 0;
  }




}