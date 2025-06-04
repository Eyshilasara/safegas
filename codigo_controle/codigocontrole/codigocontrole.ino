#include <IRremote.h> // Biblioteca IRremote


const int IR_SEND_PIN = 3;

const int BOTAO_LIGA_DESLIGA_PIN = 7; 
const int BOTAO_VOLUME_MAIS_PIN = 5;
const int BOTAO_VOLUME_MENOS_PIN = 6;

// Códigos IR específicos
#define CODIGO_LIGA_DESLIGA  0x20DF10EF
#define CODIGO_DIGITO_1      0x20DF8877
#define CODIGO_DIGITO_2      0x20DF48B7
#define CODIGO_VOLUME_MAIS   0x20DF40BF
#define CODIGO_VOLUME_MENOS  0x20DFC03F


// Variáveis para debounce dos botões
unsigned long ultimoTempoBotaoLigaDesliga = 0;
unsigned long ultimoTempoBotaoVolumeMais = 0;
unsigned long ultimoTempoBotaoVolumeMenos = 0;
unsigned long debounceDelay = 50; // Tempo de debounce em milissegundos

// Delays para a sequência de ligar e mudar de canal
const int DELAY_APOS_LIGAR = 5000;    // Tempo para a TV ligar (em ms)
const int DELAY_ENTRE_DIGITOS = 500; // Tempo entre enviar '1' e '2' (em ms)

void setup() {
  Serial.begin(9600);

  // Inicializa o emissor IR usando o método do seu código simples
  IrSender.begin(IR_SEND_PIN); // <--- ADICIONADO E AJUSTADO
  // Se IR_SEND_PIN for o mesmo que irPin (pino 3), está correto.

  // Configura os pinos dos botões como entrada com pull-up interno
  pinMode(BOTAO_LIGA_DESLIGA_PIN, INPUT_PULLUP);
  pinMode(BOTAO_VOLUME_MAIS_PIN, INPUT_PULLUP);
  pinMode(BOTAO_VOLUME_MENOS_PIN, INPUT_PULLUP);

  Serial.println("Controle Remoto LG (Canal 12) Iniciado - v2");
}

void loop() {
  // Verifica o botão Liga/Desliga
  if (digitalRead(BOTAO_LIGA_DESLIGA_PIN) == LOW) {
    if ((millis() - ultimoTempoBotaoLigaDesliga) > debounceDelay) {
      Serial.println("Botão Liga/Desliga Pressionado - Iniciando sequência...");

      Serial.println("Enviando código Liga/Desliga...");
      // Usa IrSender.sendNEC como no seu código simples
      IrSender.sendNEC(CODIGO_LIGA_DESLIGA, 32); // <--- AJUSTADO
      ultimoTempoBotaoLigaDesliga = millis();

      Serial.print("Aguardando TV ligar (");
      Serial.print(DELAY_APOS_LIGAR / 1000);
      Serial.println("s)...");
      delay(DELAY_APOS_LIGAR);

      Serial.println("Enviando código para dígito 1...");
      IrSender.sendNEC(CODIGO_DIGITO_1, 32); // <--- AJUSTADO
      delay(DELAY_ENTRE_DIGITOS);

      Serial.println("Enviando código para dígito 2...");
      IrSender.sendNEC(CODIGO_DIGITO_2, 32); // <--- AJUSTADO

      Serial.println("Sequência para Canal 12 enviada!");
      delay(1000); // Evita reenvio acidental imediato
    }
  }

  // Verifica o botão Volume+
  if (digitalRead(BOTAO_VOLUME_MAIS_PIN) == LOW) {
    if ((millis() - ultimoTempoBotaoVolumeMais) > debounceDelay) {
      Serial.println("Botão Volume+ Pressionado");
      IrSender.sendNEC(CODIGO_VOLUME_MAIS, 32); // <--- AJUSTADO
      ultimoTempoBotaoVolumeMais = millis();
      delay(200);
    }
  }

  // Verifica o botão Volume-
  if (digitalRead(BOTAO_VOLUME_MENOS_PIN) == LOW) {
    if ((millis() - ultimoTempoBotaoVolumeMenos) > debounceDelay) {
      Serial.println("Botão Volume- Pressionado");
      IrSender.sendNEC(CODIGO_VOLUME_MENOS, 32); // <--- AJUSTADO
      ultimoTempoBotaoVolumeMenos = millis();
      delay(200);
    }
  }
}