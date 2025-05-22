# Sistema de Irrigação Automatizada com ESP32 e MQTT

Este projeto apresenta um sistema de irrigação de baixo custo que utiliza um sensor de umidade do solo e um atuador (bomba representada por um LED) para controlar automaticamente a irrigação de plantas. A comunicação é realizada via Wi-Fi e protocolo MQTT, permitindo controle remoto por meio de comandos e monitoramento em tempo real.

---

## Funcionamento

O sistema realiza leituras periódicas da umidade do solo usando um sensor analógico. Quando a umidade estiver abaixo de um limiar definido (30%), o sistema aciona uma bomba por 10 segundos para irrigação (simulada por um LED).

Além do funcionamento automático, o sistema aceita comandos MQTT remotos:
- `"LIGAR"` força a irrigação independentemente da umidade.
- `"DESLIGAR"` desativa a bomba manualmente.

Todos os dados são enviados e recebidos por meio do protocolo MQTT usando o broker público HiveMQ (`broker.hivemq.com`).

---

## Estrutura do Projeto

/root

├── sketch.ino # Código principal do sistema

├── diagram.json # Esquema de montagem no Wokwi

├── soil-moisture-sensor.chip.json # Definição do sensor customizado

├── soil-moisture-sensor.chip.c # Lógica de simulação do sensor

├── README.md # Documentação do projeto



---

## Interfaces e Comunicação

- **Protocolo:** MQTT
- **Broker público:** `broker.hivemq.com`
- **Tópicos utilizados:**
  - `plantas/umidade` (publicação do valor da umidade em %)
  - `plantas/status` (publicação do estado do sistema)
  - `plantas/comando` (assinatura para receber comandos: "LIGAR", "DESLIGAR")

---

## Hardware Utilizado

| Componente                | Função                                          |
|--------------------------|--------------------------------------------------|
| ESP32 DevKit C V4        | Microcontrolador com Wi-Fi                      |
| Sensor de Umidade (A0)   | Leitura da umidade do solo                      |
| LED (Verde)              | Simulação da bomba de irrigação                |
| Resistor de 330 ohm      | Proteção do LED                                 |
| Conexão Wi-Fi (Wokwi)    | Comunicação com broker MQTT via TCP/IP         |

---

## Requisitos para Simulação

- Navegador com suporte ao [Wokwi Simulator](https://wokwi.com/)
- Conexão com a internet
- MQTT Web Client (como [HiveMQ Web Client](https://www.hivemq.com/demos/websocket-client/)) para enviar comandos

---

## Como Reproduzir

1. Acesse o link do projeto no Wokwi: [Simular no Wokwi](https://wokwi.com/projects/431602835355957249)

2. Publique mensagens MQTT com os tópicos e payloads:
   - `plantas/comando` → `"LIGAR"` ou `"DESLIGAR"`

3. Observe o funcionamento automático ou forçado no monitor serial.

---

## Exemplos de Teste

- Altere o valor do sensor no Wokwi (slider) para simular umidade baixa ou alta.
- Envie comandos MQTT e verifique o tempo de resposta.

---

## Licença

Distribuído gratuitamente para fins acadêmicos. Todos os direitos reservados ao autor.

