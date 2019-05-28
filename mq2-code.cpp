int buzzer = 8; // buzzer no pino 8

int ledVermelho = 10;
int ledAzul = 12;

int PinA0 = A0; // sensor MQ-2 (analógico A0)

int intervalo = 500;
int limite = 1000; // limite para nível normal de gás

void setup() {
  
  // define como entrada
  pinMode(PinA0, INPUT);
  
  pinMode(buzzer, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledAzul, OUTPUT);

  // inicializa a serial
  Serial.begin(9600);
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
    tone(buzzer, 3000, intervalo / 2);
    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledAzul, HIGH);
    
  } else {
    
    // desliga som, apaga led vermelho e acende led azul
    noTone(buzzer);
    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledAzul, LOW);
    
  }

  // intervalo em milissegundos
  delay(intervalo);
}
