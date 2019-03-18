//www.elegoo.com
//2016.12.08
#include "SR04.h"
#include <SoftwareSerial.h>

#define TRIG_PIN 12
#define ECHO_PIN 11
#define LED 4

SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
SoftwareSerial SIM900(7, 8); 

long a;
int i=0;
bool msgsent = false;

void setup() {
  //let the gsm connect to the network
    SIM900.begin(19200);
    delay(20000);

    Serial.begin(9600);
    pinMode(LED, OUTPUT);
    
    SIM900.print("AT+CMGF=1\r"); 
    delay(100);
}


bool sendSMS() {
  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  SIM900.println("AT+CMGS = \"+33626445365\""); 
  delay(100);
  
  // REPLACE WITH YOUR OWN SMS MESSAGE CONTENT
  SIM900.println("La poubelle est pleine !"); 
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  SIM900.println((char)26); 
  delay(100);
  SIM900.println();
  // Give module time to send SMS
  delay(5000); 
  return true;
}

void loop() {
    a=sr04.Distance();
    Serial.print(a);
    Serial.println(" cm");
    while(a < 15){
      //on fait clignoter la led
      digitalWrite(LED, HIGH);
      delay(500);
      digitalWrite(LED, LOW);
      delay(500);
      i++;
      
      a=sr04.Distance();
      Serial.print(i);
      Serial.println(" seconde");
      
      //si on a attendu plus de 5sec on envoie un sms
      if(i >= 5 && msgsent == false){
        digitalWrite(LED, HIGH);
        msgsent = sendSMS();
        if(msgsent == true)
          Serial.println("Message envoyé !");
         digitalWrite(LED, LOW);
      }
    }
    i=0;
    digitalWrite(LED, LOW);
    delay(1000);
}
