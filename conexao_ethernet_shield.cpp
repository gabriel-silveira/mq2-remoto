#include <SPI.h>
#include <Ethernet.h>

// declarando dados de rede
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

//byte mac[] =   {0xf0, 0x1f, 0xaf, 0x33, 0x62, 0x2f };
IPAddress ip(192,168,0,101);// ip que quer que sua placa tenha
EthernetClient client;

// URL
char server[] = "www.gabrielsilveira.com.br";
// porta
int portaweb = 80;

void setup() {
	// inicializa comunicação monitor serial
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
  atualizarStatus();
} // fim do loop


void atualizarStatus(){
	if(client.connect(server, portaweb)) {

		Serial.println("conectado");
				
		// manda informações para API
		// informação que quer mandar
		client.print("GET /api/alerta/");
		client.println(" HTTP/1.1");
		client.print("HOST: ");
		client.println(server);
		client.println();
		client.stop();
		Serial.println("Enviado com Sucesso");
	} else { //caso de erro ao se conectar
		// Finaliza conexão
		client.stop();
		Serial.println("falha na conexao");

		delay(5000); //Tempo de espera antes de tentar conectar novamente     
	}
}
