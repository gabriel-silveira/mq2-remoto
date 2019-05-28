int buzzer = 8; // PINO UTILIZADO PELO BUZZER
int ledVermelho = 10;
int ledAzul = 12;
int PinA0 = A0; // PINO UTILIZADO PELO SENSOR DE GÁS MQ-2
int intervalo = 500;
int limite = 200; //DEFININDO UM VALOR LIMITE (NÍVEL DE GÁS NORMAL)

void setup() {
  pinMode(PinA0, INPUT); //DEFINE O PINO COMO ENTRADA
  pinMode(buzzer, OUTPUT); //DEFINE O PINO COMO SAÍDA
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledAzul, OUTPUT);
  Serial.begin(9600); //INICIALIZA A SERIAL
}

void loop() {
  
  int valor_analogico = analogRead(PinA0); // VARIÁVEL RECEBE O VALOR LIDO NO PINO ANALÓGICO
  
  Serial.print("Nível: "); // EXIBE O TEXTO NO MONITOR SERIAL
  Serial.println(valor_analogico); // MOSTRA NO MONITOR SERIAL O VALOR LIDO DO PINO ANALÓGICO
  
  if (valor_analogico > limite){ //SE VALOR LIDO NO PINO ANALÓGICO FOR MAIOR QUE O VALOR LIMITE, FAZ
    tone(buzzer, 3000, intervalo / 2); // ATIVA O BUZZER E O MESMO EMITE O SINAL SONORO
    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledAzul, HIGH);
    
  } else { //SENÃO, FAZ
    noTone(buzzer); //BUZZER DESLIGADO
    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledAzul, LOW);
  }
  
  delay(intervalo); //INTERVALO DE X MILISSEGUNDOS
}
