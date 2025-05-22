#include <WiFi.h>
#include <PubSubClient.h>

// ====== CONFIG WIFI ======
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""
#define WIFI_CHANNEL 6

// ====== CONFIG MQTT ======
const char* mqtt_server = "broker.hivemq.com";
const char* topic_umidade = "plantas/umidade";
const char* topic_status = "plantas/status";
const char* topic_comando = "plantas/comando";

WiFiClient espClient;
PubSubClient client(espClient);

// ====== PINOS ======
#define SENSOR_UMIDADE 34  // SIG do chip customizado
#define PINO_RELE 19       // LED azul = bomba

unsigned long ultimoEnvio = 0;
const long intervaloEnvio = 10000;

void setup() {
  Serial.begin(115200);
  Serial.println("Sistema de irrigacao iniciado.");

  pinMode(SENSOR_UMIDADE, INPUT);
  pinMode(PINO_RELE, OUTPUT);
  digitalWrite(PINO_RELE, LOW);

  conectarWiFi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  unsigned long agora = millis();
  if (agora - ultimoEnvio >= intervaloEnvio) {
    ultimoEnvio = agora;

    int leitura = analogRead(SENSOR_UMIDADE);
    float umidade = map(leitura, 1023, 0, 0, 100);

    // Envia umidade via MQTT
    char buffer[10];
    dtostrf(umidade, 4, 1, buffer);
    client.publish(topic_umidade, buffer);

    // Mostra no monitor serial
    Serial.print("Umidade: ");
    Serial.print(umidade);
    Serial.println("%");

    // Irrigação automática
    if (umidade > 30) {

      digitalWrite(PINO_RELE, LOW);
      client.publish(topic_status, "SOLO_UMIDO");

    } else {

      digitalWrite(PINO_RELE, HIGH);
      client.publish(topic_status, "BOMBA_LIGADA");
      delay(10000);  // regando
      digitalWrite(PINO_RELE, LOW);
      client.publish(topic_status, "BOMBA_DESLIGADA");

    }
  }
}

void conectarWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD, WIFI_CHANNEL);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" conectado!");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Reconectando ao MQTT...");
    String clientId = "espClient-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str()))
     {
      Serial.println(" conectado.");
      client.subscribe(topic_comando);
    } else {
      Serial.print(" falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando em 5s...");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {

  String comando;
  for (int i = 0; i < length; i++) {
    comando += (char)payload[i];
  }

  Serial.println(comando);

  // Compare o conteúdo recebido
  if (String(topic) == topic_comando && comando == "LIGAR") {
    Serial.println("Comando LIGAR reconhecido, acionando bomba...");
    digitalWrite(PINO_RELE, HIGH);
    client.publish(topic_status, "BOMBA_LIGADA_FORCADA");
    delay(10000);
    digitalWrite(PINO_RELE, LOW);
    client.publish(topic_status, "BOMBA_DESLIGADA");
  }

    if (String(topic) == topic_comando && comando == "DESLIGAR") {
    Serial.println("Comando DESLIGAR reconhecido, desativando bomba...");
    digitalWrite(PINO_RELE, LOW);
    client.publish(topic_status, "BOMBA_DESLIGADA");
    delay(10000);
    digitalWrite(PINO_RELE, HIGH);
    client.publish(topic_status, "BOMBA_LIGADA_FORCADA");
  }
}
