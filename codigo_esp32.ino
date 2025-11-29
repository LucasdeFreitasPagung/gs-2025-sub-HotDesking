#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>

// --- Configurações de Hardware ---
#define PIN_SERVO 13
#define PIN_PIR 12

// Pinos dos Botões (B1, B2, B3, B4)
const int pinosBotoes[4] = {26, 27, 14, 25}; 

// --- Configurações de Wi-Fi e Servidor ---
const char* ssid = "Wokwi-GUEST"; // Rede padrão do Wokwi
const char* password = "";
// URL para teste (Webhook.site ou similar para ver o POST chegando)
const char* serverUrl = "https://httpbin.org/post"; 

// --- Objetos ---
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo meuServo;

// --- Variáveis Globais ---
String senhaDigitada = "";
const String SENHA_MESTRA = "1234"; // Senha correta: Botão 1, 2, 3, 4

// --- Declaração de Funções (Modularização) ---
void setupWiFi();
void verificarBotoes();
void processarTentativa();
void liberarAcesso();
void negarAcesso();
void estacaoOcupada();
void enviarDadosIoT(String status);

void setup() {
  Serial.begin(115200);

  // Inicializa LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("GS 2025 - IoT");
  lcd.setCursor(0, 1);
  lcd.print("Iniciando...");

  // Inicializa Servo
  meuServo.attach(PIN_SERVO);
  meuServo.write(0); // Começa trancado

  // Inicializa PIR
  pinMode(PIN_PIR, INPUT);

  // Inicializa Botões com Pull-up interno (sem resistor externo necessário)
  for(int i = 0; i < 4; i++) {
    pinMode(pinosBotoes[i], INPUT_PULLUP);
  }

  setupWiFi();

  lcd.clear();
  lcd.print("Digite a Senha:");
}

void loop() {
  verificarBotoes();
  delay(100); // Pequeno delay para estabilidade
}

// --- Implementação das Funções ---

void setupWiFi() {
  lcd.clear();
  lcd.print("Conectando WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Conectado!");
  lcd.clear();
  lcd.print("WiFi OK!");
  delay(1000);
}

void verificarBotoes() {
  for(int i = 0; i < 4; i++) {
    // Se o botão for pressionado (LOW por causa do PULLUP)
    if(digitalRead(pinosBotoes[i]) == LOW) {
      senhaDigitada += String(i + 1); // Adiciona "1", "2", "3" ou "4"
      
      // Feedback visual
      lcd.setCursor(0, 1);
      lcd.print(senhaDigitada);
      Serial.println("Tecla: " + String(i+1));
      
      delay(300); // Debounce simples (evita leitura duplicada)
      
      // Se digitou 4 números, verifica
      if(senhaDigitada.length() == 4) {
        processarTentativa();
        senhaDigitada = ""; // Limpa para próxima tentativa
        lcd.clear();
        lcd.print("Digite a Senha:");
      }
    }
  }
}

void processarTentativa() {
  lcd.clear();
  if (senhaDigitada == SENHA_MESTRA) {
    // Senha correta, agora checa o PIR
    bool ocupado = digitalRead(PIN_PIR);
    
    if (ocupado) {
      estacaoOcupada();
    } else {
      liberarAcesso();
    }
  } else {
    negarAcesso();
  }
  delay(2000); // Tempo para ler a mensagem
}

void liberarAcesso() {
  lcd.print("Acesso Liberado");
  meuServo.write(90); // Abre a "porta"
  Serial.println("Acesso Liberado - Enviando HTTP POST...");
  enviarDadosIoT("LIBERADO");
  delay(3000);
  meuServo.write(0); // Tranca novamente
}

void estacaoOcupada() {
  lcd.print("Estacao Ocupada");
  meuServo.write(0); // Mantém trancado
  Serial.println("Tentativa válida, mas estação ocupada.");
}

void negarAcesso() {
  lcd.print("Acesso Negado");
  meuServo.write(0);
  Serial.println("Senha Incorreta!");
}

void enviarDadosIoT(String statusAcesso) {
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");
    
    // JSON simulado com timestamp (pode vir de um servidor NTP)
    String jsonPayload = "{\"status\":\"" + statusAcesso + "\", \"device\":\"ESP32_01\", \"timestamp\": 1716300000}";
    
    int httpResponseCode = http.POST(jsonPayload);
    
    if(httpResponseCode > 0){
      String response = http.getString();
      Serial.println("Resposta HTTP: " + String(httpResponseCode));
    } else {
      Serial.print("Erro no envio HTTP: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi Desconectado");
  }
}
