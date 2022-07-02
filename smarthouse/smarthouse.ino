#include <NewPing.h>                //Include the NewPing library
#define PING_PIN 8    
int sound =11;
int led =10;
NewPing sonar(PING_PIN, PING_PIN ); //Create a NewPing object

void setup()
{
  pinMode(12,OUTPUT);
  pinMode(7,INPUT);
  Serial.begin(9600);
 pinMode(led,OUTPUT);
 pinMode(sound,OUTPUT);
}
void loop()
{
  if (digitalRead(7)== LOW)
  {
    digitalWrite(12,HIGH);
    
    delay(10);
  }
  else 
  {
    
    digitalWrite(12,LOW);
    delay(10);
    
  }
  unsigned int uS = sonar.ping(); 
 int cm = uS / US_ROUNDTRIP_CM;    //Get the distance in cm using the library
 Serial.println(cm);               //Print the distance on the serial monitor
 delay(50);                        //Delay 50 milliseconds for the next distance mesurement

 if(cm <= 100){
  digitalWrite(led,HIGH);
   if(cm< 30){
     digitalWrite(sound,HIGH);
   }
  }else{
   digitalWrite(led,LOW);
  digitalWrite(sound,LOW);
 }
}
