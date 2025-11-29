# 🏢 Sistema de Controle de Acesso e Reserva (Hot Desking) - Sub - GS - 2025

**Sub - Global Solution - O Futuro do Trabalho**

## 📖 Descrição do Projeto
Este projeto foi desenvolvido como parte da Global Solution 2025, focado em **IoT e Cidades Inteligentes** aplicados ao contexto de **Espaços de Trabalho Híbridos**.

O sistema simula uma estação de "Hot Desking" (mesa compartilhada). Ele gerencia o acesso físico à estação garantindo que:
1. O usuário possua a credencial correta (Simulação de senha).
2. A estação esteja fisicamente vazia antes de liberar a reserva (Monitoramento via Sensor PIR).
3. O registro de uso seja enviado para a nuvem para auditoria de gestão (Integração IoT via HTTP POST).

## 🛠️ Componentes Utilizados
* **Microcontrolador:** ESP32 DevKit V1
* **Entrada de Dados:** 4x Push Buttons (Simulando teclado matricial/senha)
* **Sensores:** 1x Sensor de Movimento PIR (HC-SR501)
* **Atuadores:** 1x Micro Servo Motor (Simulando tranca da mesa)
* **Display:** 1x LCD 16x2 com interface I2C
* **Software/Simulador:** Wokwi Online Simulator

## 🚀 Funcionalidades

### 1. Autenticação Local
* O usuário insere uma senha de 4 dígitos através dos botões.
* **Senha Mestra configurada:** `1-2-3-4`.
* Feedback visual imediato no display LCD.

### 2. Validação de Disponibilidade Física
* Mesmo com a senha correta, o sistema consulta o **Sensor PIR**.
* Se houver alguém na mesa (PIR = HIGH), o sistema nega o acesso e exibe "Estação Ocupada". Isso evita conflitos de reserva.

### 3. Conectividade IoT (Smart Building)
* Ao liberar o acesso, o ESP32 conecta-se ao Wi-Fi.
* Envia uma requisição **HTTP POST** para um endpoint (API).
* Envia um JSON contendo o status da liberação e timestamp para controle de RH/Facilities.

## 🔌 Esquema de Ligação (Circuito)

> **Observação:** A imagem do circuito deve ser visualizada diretamente no simulador Wokwi ou no arquivo de imagem anexado a este repositório.

* **LCD:** Pinos SDA (D21) e SCL (D22).
* **Servo:** Pino D13.
* **PIR:** Pino D12.
* **Botões:** Pinos D26, D27, D14, D25 (Configurados com INPUT_PULLUP).

## 💻 Como Rodar o Projeto

### Pré-requisitos
* Acesso ao simulador [Wokwi](https://wokwi.com/).

### Passo a Passo
1.  Acesse o link do projeto: **[https://wokwi.com/projects/448979004625757185]**
2.  No Wokwi, certifique-se de que as bibliotecas `LiquidCrystal I2C` e `ESP32Servo` estão instaladas no Library Manager.
3.  Clique no botão **"Start the Simulation"** (Play).
4.  **Teste de Acesso:**
    * Digite a senha `1-2-3-4` clicando nos botões correspondentes.
    * Se o PIR não detectar movimento, o servo girará 90° e o acesso será liberado.
5.  **Teste de Bloqueio:**
    * Clique no sensor PIR (para simular movimento).
    * Tente digitar a senha novamente. O sistema exibirá "Estação Ocupada".

## 📡 Integração HTTP
O sistema envia dados para o endpoint de teste: `https://httpbin.org/post`.
Exemplo de Payload JSON enviado:
```json
{
  "status": "LIBERADO",
  "device": "ESP32_01",
  "timestamp": 1716300000
}

Nome do Aluno: Lucas de Freitas Pagung
RM:553242
Turma:3ESPR
