#include <SPI.h>
#include <Ethernet.h>


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


int buzzer = 8; // buzzer no pino 8

int ledAzul = 10;
int ledVermelho = 11;


int PinA0 = A0; // sensor MQ-2 (analógico A0)

int intervalo = 100;
int limite = 140; // limite para nível normal de gás

bool alertaEnviado = false;


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


// declarando dados de rede
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

//byte mac[] =   {0xf0, 0x1f, 0xaf, 0x33, 0x62, 0x2f };
IPAddress ip(192,168,0,101);// ip que quer que sua placa tenha
EthernetClient client;

// URL
char server[] = "dev.gabrielsilveira.com.br";
// porta
int portaweb = 80;


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


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
