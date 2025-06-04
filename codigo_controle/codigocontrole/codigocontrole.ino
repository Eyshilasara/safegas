#include <IRremote.h> // Biblioteca IRremote

// Defina o pino do emissor IR
const int IR_SEND_PIN = 3;

// Defina os pinos dos botões
const int BOTAO_LIGA_DESLIGA_PIN = 7;
const int BOTAO_VOLUME_MAIS_PIN = 5; 
const int BOTAO_VOLUME_MENOS_PIN = 6; 


#define CODIGO_LIGA_DESLIGA  0x20DF10EF 

#define CODIGO_SETA_BAIXO    0x20DF827D 
#define CODIGO_OK_SELECIONAR 0x20DF22DD 
#define CODIGO_CANAL_MAIS    0x20DF00FF 

// Códigos de Volume (UNIVERSAL)
#define CODIGO_VOLUME_MAIS_BTN   0x20DF40BF
#define CODIGO_VOLUME_MENOS_BTN  0x20DFC03F

// Variáveis para debounce dos botões
unsigned long ultimoTempoBotaoLigaDesliga = 0;
unsigned long ultimoTempoBotaoVolumeMais = 0;
unsigned long ultimoTempoBotaoVolumeMenos = 0;
unsigned long debounceDelay = 50; // Tempo de debounce em milissegundos

// Delays
const int DELAY_APOS_LIGAR = 5000;        // Tempo para a TV ligar (em ms) - ajuste se necessário
const int DELAY_ENTRE_ACOES_MENU = 5000;   // Tempo entre os comandos de navegação no menu (ajuste se necessário)

void setup() {
  Serial.begin(9600);

  // Inicializa o emissor IR
  IrSender.begin(IR_SEND_PIN);

  // Configura os pinos dos botões como entrada com pull-up interno
  pinMode(BOTAO_LIGA_DESLIGA_PIN, INPUT_PULLUP);
  pinMode(BOTAO_VOLUME_MAIS_PIN, INPUT_PULLUP);
  pinMode(BOTAO_VOLUME_MENOS_PIN, INPUT_PULLUP);

  Serial.println("Controle Remoto LG (Navegação Menu - Seus Códigos) Iniciado - v4");
}

void loop() {
  // Verifica o botão Liga/Desliga
  if (digitalRead(BOTAO_LIGA_DESLIGA_PIN) == LOW) {
    if ((millis() - ultimoTempoBotaoLigaDesliga) > debounceDelay) {
      Serial.println("Botão Liga/Desliga Pressionado - Iniciando sequência...");
      ultimoTempoBotaoLigaDesliga = millis();

      // 1. Envia o código de Liga/Desliga
      Serial.println("Enviando código Liga/Desliga...");
      IrSender.sendNEC(CODIGO_LIGA_DESLIGA, 32);

      // 2. Espera a TV ligar
      Serial.print("Aguardando TV ligar (");
      Serial.print(DELAY_APOS_LIGAR / 1000);
      Serial.println("s)...");
      delay(DELAY_APOS_LIGAR);

      // 3. Envia comando "Seta para Baixo" 
      Serial.println("Enviando Seta para Baixo (Seu Código)...");
      IrSender.sendNEC(CODIGO_SETA_BAIXO, 32);
      delay(DELAY_ENTRE_ACOES_MENU);

      // 4. Envia comando "OK/Selecionar" 
      Serial.println("Enviando OK/Selecionar (Seu Código)...");
      IrSender.sendNEC(CODIGO_OK_SELECIONAR, 32);
      delay(DELAY_ENTRE_ACOES_MENU);

      // 5. Envia comando "Canal para Cima" 
      Serial.println("Enviando Canal para Cima (Seu Código)...");
      IrSender.sendNEC(CODIGO_CANAL_MAIS, 32);

      Serial.println("Sequência de navegação de menu (Seus Códigos) enviada!");
      delay(1000); // Evita reenvio acidental imediato da sequência completa
    }
  }

  // Verifica o botão Volume+
  if (digitalRead(BOTAO_VOLUME_MAIS_PIN) == LOW) {
    if ((millis() - ultimoTempoBotaoVolumeMais) > debounceDelay) {
      Serial.println("Botão Volume+ Pressionado");
      IrSender.sendNEC(CODIGO_VOLUME_MAIS_BTN, 32);
      ultimoTempoBotaoVolumeMais = millis();
      delay(200);
    }
  }

  // Verifica o botão Volume-
  if (digitalRead(BOTAO_VOLUME_MENOS_PIN) == LOW) {
    if ((millis() - ultimoTempoBotaoVolumeMenos) > debounceDelay) {
      Serial.println("Botão Volume- Pressionado");
      IrSender.sendNEC(CODIGO_VOLUME_MENOS_BTN, 32);
      ultimoTempoBotaoVolumeMenos = millis();
      delay(200);
    }
  }
}
