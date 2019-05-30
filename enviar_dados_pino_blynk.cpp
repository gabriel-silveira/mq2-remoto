/*************************************************************
  Tratamento-de-Sensores-no-Blynk
  // Trate a leitura de sensores, 
  mostre valores convertidos 
  e controle o Módulo Relé Serial da RoboCore 
  com o aplicativo Blynk.
  
  Produtos e tutoriais:     http://robocore.net
  Siga a RoboCore:          http://facebook.com/robocore
                            http://youtube.com/robocore
                            http://instagram.com/robocore
  __________
  
  Baseado no exemplo de VirtualPinWrite da biblioteca Blynk.
  Modificado em 17 Set 2018 por RoboCore.
  
  Faça o download da última versão da biblioteca SerialRelay em:
  https://github.com/RoboCore/SerialRelay
  
**************************************************************/

#include <DHT.h>
#define DHTPIN 7 // Define em que pino está conectado o módulo DHT
#define DHTTYPE DHT22 // Define se o módulo conectado é o DHT11 ou DHT22
DHT dht(DHTPIN, DHTTYPE);

#include <SerialRelay.h>
SerialRelay relays(4, 5, 1); // (data, clock, número de módulos)

#define BLYNK_PRINT Serial


#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "seu-token-aqui";

BlynkTimer timerDHT22; // Criação do Timer do DHT22
BlynkTimer timerLDR; // Criação do Timer do LDR



void lerSensorDHT22() // Função para ler os parâmetros do DHT22
{
  float temperatura = dht.readTemperature(); // Comando da biblioteca que lê a temperatura adquirida pelo DHT22
  float umidade = dht.readHumidity(); // Comando da biblioteca que lê a umidade adquirida pelo DHT22

  if (isnan(temperatura) || isnan(umidade)) { // isnan = IS Not A Number <= verifica se o valor lido não é um número
    return;
  }
  Blynk.virtualWrite(V4, temperatura); // Escreve no pino virtual V4 do Blynk o valor da temperatura lido pelo DHT22
  Blynk.virtualWrite(V5, umidade); // Escreve no pino virtual V5 do Blynk o valor de umidade lido pelo DHT22
}



void lerLDR(){ // Função para ler o parâmetro do LDR
  int valorLDR = analogRead(A0);
  valorLDR = map(valorLDR, 0, 1023, 0, 100); // Função que mapeia os valores lidos de 0 à 1023 em 0 à 100
  Blynk.virtualWrite(V6, valorLDR); // Escreve no pino virtual V6 do Blynk o valor de luminosidade lido pelo LDR
}



BLYNK_WRITE(V0){
  int pinValue = param.asInt();
  //Serial.println(pinValue);
  if (pinValue == 1) {
    relays.SetRelay(1, SERIAL_RELAY_ON, 1);   // Liga o relé 01
  } else {
    relays.SetRelay(1, SERIAL_RELAY_OFF, 1);  // Desliga o relé 01
  }
}

BLYNK_WRITE(V1){
  int pinValue = param.asInt();
  //Serial.println(pinValue);
  if (pinValue == 1) {
    relays.SetRelay(2, SERIAL_RELAY_ON, 1);   // Liga o relé 02
  } else {
    relays.SetRelay(2, SERIAL_RELAY_OFF, 1);  // Desliga o relé 02
  }
}

BLYNK_WRITE(V2){
  int pinValue = param.asInt();
  //Serial.println(pinValue);
  if (pinValue == 1) {
    relays.SetRelay(3, SERIAL_RELAY_ON, 1);   // Liga o relé 03
  } else {
    relays.SetRelay(3, SERIAL_RELAY_OFF, 1);  // Desliga o relé 03
  }
}

BLYNK_WRITE(V3){
  int pinValue = param.asInt();
  //Serial.println(pinValue);
  if (pinValue == 1) {
    relays.SetRelay(4, SERIAL_RELAY_ON, 1);   // Liga o relé 04
  } else {
    relays.SetRelay(4, SERIAL_RELAY_OFF, 1);  // Desliga o relé 04
  }
}



void setup()
{
  Serial.begin(9600);
  dht.begin(); // Inicializa o DHT22
  Blynk.begin(auth);

  timerDHT22.setInterval(2000L, lerSensorDHT22); // Define o intervalo de tempo da leitura do DHT22 para 2000 milisegundos
  timerLDR.setInterval(100L, lerLDR); // Define o intervalo de tempo da leitura do LDR para 100 milisegundos
}



void loop()
{
  Blynk.run();
  timerDHT22.run(); // Inicializa o Timer do DHT22
  timerLDR.run(); // Inicializa o Timer do LDR
}