#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <SimpleTimer.h>
#include <MQ2.h>


// altere para o pino que você utiliza
int pin = A0;
int lpg, co, smoke;

MQ2 mq2(pin);

// você deve obter o Auth token no aplicativo Blynk
// Vá nas configurações do projeto
char auth[] = "f375eacff6884389b7d553c7b8e62513";
char server[] = "10.5.51.5";

SimpleTimer timer;

void setup() {
  Blynk.begin(auth, server, 8080);
  mq2.begin();
  timer.setInterval(2000L, gas);
}

void gas() {

  /* lê os valores do sensor e retorna um array contendo 3 valores
    1 = LPG in ppm
    2 = CO in ppm
    3 = SMOKE in ppm
  */
  
  // defina para "false" se você não quer exibir os valores no serial
  float* values = mq2.read(true); 

  //lpg = values[0];
  lpg = mq2.readLPG();
  Blynk.virtualWrite(1, lpg);
  //co = values[1];
  co = mq2.readCO();
  Blynk.virtualWrite(2, co);
  //smoke = values[2];
  smoke = mq2.readSmoke();
  Blynk.virtualWrite(3, smoke);
}

void loop()
{
  Blynk.run();
  timer.run();
}