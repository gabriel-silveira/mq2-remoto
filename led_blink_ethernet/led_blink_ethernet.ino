/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Social networks:            http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example shows how to use Arduino Ethernet shield (W5100)
  to connect your project to Blynk.

  NOTE: Pins 10, 11, 12 and 13 are reserved for Ethernet module.
        DON'T use them in your sketch directly!

  WARNING: If you have an SD card, you may need to disable it
        by setting pin 4 to HIGH. Read more here:
        https://www.arduino.cc/en/Main/ArduinoEthernetShield

  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <MQ2.h>

bool alertLigado = false;
int ledVermelho = 4;
int buzzer = 5;
int pin = A0;
int lpg, co, smoke;
MQ2 mq2(pin);


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "f4e68a76871c42fdaa27708e5f7dd560";


#define W5100_CS  10
#define SDCARD_CS 4

SimpleTimer timer;

void setup()
{
  pinMode(ledVermelho, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  // Debug console
  Serial.begin(9600);

  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH); // Deselect the SD card

  Blynk.begin(auth);
  // You can also specify server:
  //Blynk.begin(auth, "blynk-cloud.com", 80);
  //Blynk.begin(auth, IPAddress(192,168,1,100), 8080);
  // For more options, see Boards_Ethernet/Arduino_Ethernet_Manual example

  mq2.begin();
  timer.setInterval(2000L, gas);
}



void gas() {
  /*read the values from the sensor, it returns
    an array which contains 3 values.
    1 = LPG in ppm
    2 = CO in ppm
    3 = SMOKE in ppm
  */
  float* values = mq2.read(false); //set it false if you don't want to print the values in the Serial

  //lpg = values[0];
  lpg = mq2.readLPG();
  Blynk.virtualWrite(1, lpg);
  
  //co = values[1];
  co = mq2.readCO();
  Blynk.virtualWrite(2, co);
  
  //smoke = values[2];
  smoke = mq2.readSmoke();
  Blynk.virtualWrite(3, smoke);

  // desligou pelo botão do Blynk
  if(alertLigado && !digitalRead(ledVermelho)) {
    
    // desliga apito
    noTone(buzzer);
    
    // marca o alerta como desligado
    alertLigado = false;
  }

  // atingiu um nivel crítico de gás no ambiente
  if(co > 5000) {
    
    // se o alerta não estiver ativado...
    if(!alertLigado) {
      
      // apita
      tone(buzzer, 1000);
      
      // liga led
      digitalWrite(ledVermelho, HIGH);

      // marca como "alerta ligado"
      alertLigado = true;
    }
  } else {
    // e o alerta está desligado
    if(!alertLigado) {
      
      // desliga led
      digitalWrite(ledVermelho, LOW);
      
      // desliga apito
      noTone(buzzer);
    }
  }
  
  Serial.println(lpg);
  Serial.println(co);
  Serial.println(smoke);
}

void loop()
{
  Blynk.run();
  timer.run();
}
