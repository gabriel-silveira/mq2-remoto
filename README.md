# Detecção remota de incêndio com Arduino Uno e MQ-2
> **Resumo.** Este artigo descreve o projeto de um sensor de fumaça conectado à internet. O dispositivo descrito no projeto provê segurança emitindo um som e acendendo um LED vermelho de alerta quando detecta um certo nível de fumaça no ambiente. O dispositivo também envia uma mensagem para uma API utilizando o protocolo http ao detectar um nível crítico de gás inflamável no ambiente.
## Introdução

Atualmente vivemos em uma era em que a tecnologia já faz parte do nosso cotidiano, onde um telefone celular não é utilizado apenas para realizar chamadas, mas é um dispositivo que possibilita tirar fotos, gravar voz e vídeos, jogar e ouvir música. Viajar de um estado para outro, ou até de um país para outro, tornou-se mais fácil e barato. Por isso, podemos estar longe de nossos lares, porém conectados através de nossos celulares. Existem as câmeras IPs, onde uma pessoa pode ver o que acontece em sua residência do outro lado do mundo, em tempo real, tudo isso através da tecnologia.

Na busca por maior segurança, este projeto visa monitorar focos de incêndio através de sensores de fumaça integrados com Arduino Uno e componentes conectados à internet para uso residencial.
## Componentes
* Arduino Uno
* Ethernet Shield
* Sensor de gás inflamável MQ-2
* LED vermelho
* LED azul
* 2 resistores 120 Ohms
## Aplicação
Importamos as bibliotecas SPI e Ethernet para comunicação com a internet:
```console
#include <SPI.h>
#include <Ethernet.h>
```
Definimos variáveis para os pinos e outras úteis para a aplicação:
```
int buzzer = 8; // buzzer no pino 8

int ledAzul = 10;
int ledVermelho = 11;


int PinA0 = A0; // sensor MQ-2 (analógico A0)

int intervalo = 100;
int limite = 140; // limite para nível normal de gás

bool alertaEnviado = false;
```
