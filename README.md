# Detecção remota de incêndio com Arduino Uno e MQ-2
> **Resumo.** Este artigo descreve o projeto de um sensor de fumaça conectado à internet. O dispositivo descrito no projeto provê segurança emitindo um som e acendendo um LED vermelho de alerta quando detecta um certo nível de fumaça no ambiente. O dispositivo também envia uma mensagem para um celular ao detectar um nível crítico de gás no ambiente através do aplicativo Blynk, sendo possível desativar o alarme pelo mesmo.
## Introdução

Atualmente vivemos em uma era em que a tecnologia já faz parte do nosso cotidiano, onde um telefone celular não é utilizado apenas para realizar chamadas, mas é um dispositivo que possibilita tirar fotos, gravar voz e vídeos, jogar e ouvir música. Viajar de um estado para outro, ou até de um país para outro, tornou-se mais fácil e barato. Por isso, podemos estar longe de nossos lares, porém conectados através de nossos celulares. Existem as câmeras IPs, onde uma pessoa pode ver o que acontece em sua residência do outro lado do mundo, em tempo real, tudo isso através da tecnologia.

Na busca por maior segurança este projeto abrange o monitoramento remoto, por meio de aplicativo para celular, de possíveis focos de incêndio através de sensores de fumaça integrados com Arduino Uno e componentes conectados à internet para uso residencial. Confira abaixo as descrições da solução.
## Componentes
<table>
  <tr>
    <td align="center" colspan="2">
      <img height="350" src="https://raw.githubusercontent.com/gabriel-silveira/mq2-remoto/master/imgs/project_mq-2.jpeg" alt="Projeto MQ-2">
    </td>
  </tr>
  <tr>
    <td align="center">
      <img height="300" src="https://raw.githubusercontent.com/gabriel-silveira/mq2-remoto/master/imgs/uno.jpeg" alt="Arduino Uno">
      <p>Arduino Uno</p>
    </td>
    <td align="center">
      <img height="300" src="https://raw.githubusercontent.com/gabriel-silveira/mq2-remoto/master/imgs/ethernet-shield.jpeg" alt="Ethernet Shield">
      <p>Ethernet Shield</p>
    </td>
  </tr>
  <tr>
    <td align="center">
      <img height="120" src="https://raw.githubusercontent.com/gabriel-silveira/mq2-remoto/master/imgs/mq-2.jpeg" alt="Sensor de gás MQ-2">
      <p>Sensor de gás inflamável MQ-2</p>
    </td>
    <td align="center">
      <img height="120" src="https://raw.githubusercontent.com/gabriel-silveira/mq2-remoto/master/imgs/buzzer.jpg" alt="Buzzer 5V">
      <p>Buzzer 5V</p>
    </td>
  </tr>
  <tr>
    <td align="center">
      <img height="80" src="https://raw.githubusercontent.com/gabriel-silveira/mq2-remoto/master/imgs/red-led.jpeg" alt="LED vermelho">
      <p>LED vermelho</p>
    </td>
    <td align="center">
        <img height="120" src="https://raw.githubusercontent.com/gabriel-silveira/mq2-remoto/master/imgs/resistor-120.jpeg" alt="Resistor de 120 Ohms">
      <p>Resistor de 120 Ohms</p>
    </td>
  </tr>
</table>

## Blynk
O aplicativo Blynk conta com 3 indicadores visuais de níveis de LPG, CO e fumaça e um botão para desativar o alarme.

<p align="center">
  <img src="https://raw.githubusercontent.com/gabriel-silveira/mq2-remoto/master/imgs/blynk_app.jpeg">
</p>

## Aplicação
Importamos as bibliotecas SPI e Ethernet (para comunicação com a internet), Blynk e MQ-2:
```cpp
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <MQ2.h>
```
Definimos variáveis para os pinos e outras úteis para a aplicação:
```cpp
bool alertLigado = false;
int ledVermelho = 4;
int buzzer = 5;
int pin = A0;
int lpg, co, smoke;
MQ2 mq2(pin);


char auth[] = "insira_aqui_seu_token_de_autenticação";


#define W5100_CS  10
#define SDCARD_CS 4

SimpleTimer timer;
```
Inserimos as configurações da aplicação na função setup():
```cpp
void setup()
{
  pinMode(ledVermelho, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  Serial.begin(9600);

  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH);

  Blynk.begin(auth);

  mq2.begin();
  timer.setInterval(2000L, gas);
}
```
Está é a função que verifica os níveis de gás e envia o alerta quando um nível crítico é detectado pelo MQ-2:
```cpp
void gas() {
  float* values = mq2.read(false);

  lpg = mq2.readLPG();
  Blynk.virtualWrite(1, lpg);
  
  co = mq2.readCO();
  Blynk.virtualWrite(2, co);
  
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
```
Loop principal de processamento do Arduino:
```cpp
void loop()
{
  Blynk.run();
  timer.run();
}
```


