# Detecção remota de incêndio com Arduino Uno e MQ-2
> **Resumo.** Este artigo descreve o projeto de um sensor de fumaça conectado à internet. O dispositivo descrito no projeto provê segurança emitindo um som e acendendo um LED vermelho de alerta quando detecta um certo nível de fumaça no ambiente. O dispositivo também envia uma mensagem para um celular ao detectar um nível crítico de gás no ambiente através do aplicativo Blynk, sendo possível desativar o alarme pelo memso.
## Introdução

Atualmente vivemos em uma era em que a tecnologia já faz parte do nosso cotidiano, onde um telefone celular não é utilizado apenas para realizar chamadas, mas é um dispositivo que possibilita tirar fotos, gravar voz e vídeos, jogar e ouvir música. Viajar de um estado para outro, ou até de um país para outro, tornou-se mais fácil e barato. Por isso, podemos estar longe de nossos lares, porém conectados através de nossos celulares. Existem as câmeras IPs, onde uma pessoa pode ver o que acontece em sua residência do outro lado do mundo, em tempo real, tudo isso através da tecnologia.

Na busca por maior segurança, este projeto visa monitorar focos de incêndio através de sensores de fumaça integrados com Arduino Uno e componentes conectados à internet para uso residencial.
## Componentes
* Arduino Uno
* Ethernet Shield
* Sensor de gás inflamável MQ-2
* LED vermelho
* 1 resistores 120 Ohms
## API
Ao detectar um nível crítico de gás inflamável no ambiente, uma requisição é enviada à API no endereço abaixo enviado **status**, onde status pode ser _true_ ou _false_, indicando para acionar ou não um alerta:
- http://dev.gabrielsilveira.com.br/alert/{status}
## Aplicação
Importamos as bibliotecas SPI, Ethernet para comunicação com a internet e para leitura do MQ-2:
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


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "f4e68a76871c42fdaa27708e5f7dd560";


#define W5100_CS  10
#define SDCARD_CS 4

SimpleTimer timer;
```
Definimos variáveis para conexão de rede:
```cpp
// declarando dados de rede
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

//byte mac[] =   {0xf0, 0x1f, 0xaf, 0x33, 0x62, 0x2f };
IPAddress ip(192,168,0,101);// ip que quer que sua placa tenha
EthernetClient client;

// URL
char server[] = "dev.gabrielsilveira.com.br";
// porta
int portaweb = 80;
```
Inserimos as configurações da aplicação na função setup():
```cpp

void setup() {
  
  // define como entrada
  pinMode(PinA0, INPUT);
  
  pinMode(buzzer, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledAzul, OUTPUT);

  // inicializa a serial
  Serial.begin(9600);
  

  Serial.println("Adquirindo IP address Usando DHCP:");

  if(!Ethernet.begin(mac)) {
    //Caso DHCP Falhe
    Serial.println("Falha ao Adquirir Ip Via DHCP, Utilizar manual");
    Ethernet.begin(mac, ip);
  }

  Serial.print("Meu IP: ");
  Serial.println(Ethernet.localIP());
}
```
Loop principal:
```cpp
void loop() {

  // recebe valor lido no pino analógico A0
  int valor_analogico = analogRead(PinA0); 

  // exibe valor lido no monitor serial
  Serial.print("Nível: ");
  Serial.println(valor_analogico);

  // se valor lido for maior que limite
  if (valor_analogico > limite) {
    
    // emite som, liga led vermelho e apaga led azul
    // tone(buzzer, 3000, intervalo / 2);
    
    digitalWrite(ledVermelho, HIGH);
    digitalWrite(ledAzul, LOW);

    // envia solicitação na API
    if(!alertaEnviado)
      enviarAlerta(true);
    
  } else {
    
    // desliga som, apaga led vermelho e acende led azul
    // noTone(buzzer);
    
    digitalWrite(ledAzul, HIGH);
    digitalWrite(ledVermelho, LOW);

    // enviarAlerta(false);
  }
  
  // intervalo em milissegundos
  delay(intervalo);
}
```
Aqui está nossa função que envia o alerta quando um nível sensível de gás é detectado pelo MQ-2:
```cpp
void enviarAlerta(bool status) {
  if(client.connect(server, portaweb)) {

    Serial.println("Enviando alerta...");
    
    // manda informações para API
    if(status) {
      client.print("GET /alert/1");
    } else {
      client.print("GET /alert/0");
    }
    
    client.println(" HTTP/1.1");
    client.print("HOST: ");
    client.println(server);
    client.println();
    client.stop();
    
    Serial.println("Enviado com Sucesso");

    // marca como enviado
    alertaEnviado = true;
    
  } else { //caso de erro ao se conectar
    // Finaliza conexão
    client.stop();
    Serial.println("falha na conexao");

    // delay(5000); //Tempo de espera antes de tentar conectar novamente     
  }
}
```


