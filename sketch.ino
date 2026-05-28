#include <Arduino.h>
#include <WiFi.h>
#include <ThingSpeak.h>
#include "Config.h"
#include "MisClases.h"

Led       led(PIN_LED);
Boton     boton(PIN_BOTON);
LDR       ldr(PIN_LDR);
SensorDHT dht(PIN_DHT, DHT22);

WiFiClient cliente;
unsigned long ultimoEnvio = 0;

void conectarWiFi() {
  Serial.print("\n[WiFi] Conectando a: ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int intentos = 0;
  while (WiFi.status() != WL_CONNECTED && intentos < 20) {
    delay(500);
    Serial.print(".");
    intentos++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WiFi] Conectado!");
    Serial.print("[WiFi] IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n[WiFi] FALLO. Reiniciando...");
    ESP.restart();
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("=== Estacion Ambiental IoT - ESP32 ===");

  led.begin();
  boton.begin();
  ldr.begin();
  dht.begin();
  Serial.println("[OK] Sensores y actuadores listos");

  // Parpadeo de arranque
  for (int i = 0; i < 3; i++) {
    led.encender(); delay(150);
    led.apagar();   delay(150);
  }

  conectarWiFi();
  ThingSpeak.begin(cliente);
  Serial.println("[OK] ThingSpeak listo\n");
}
void loop() {
  int   luz   = ldr.leer();
  float luz_p = ldr.porcentaje();
  float temp  = dht.temperatura();
  float hum   = dht.humedad();

  // Apaga el LED automáticamente si la temp baja
  if (temp != -999.0 && temp <= TEMP_ALERTA && led.estadoLed()) {
    led.apagar();
    Serial.println("[OK] Temperatura normal - LED apagado");
  }

  // Enciende el LED si la temp sube
  if (temp != -999.0 && temp > TEMP_ALERTA && !led.estadoLed()) {
    led.encender();
    Serial.println("[ALERTA] Temperatura alta - LED encendido");
  }

  // Botón apaga alerta manualmente
  if (digitalRead(PIN_BOTON) == LOW) {
    led.apagar();
    Serial.println("[RESET] Boton presionado - LED apagado");
    delay(300);
  }

  // Monitor serial
  Serial.println("-----------------------------------");
  Serial.print("[LDR]  Crudo: "); Serial.print(luz);
  Serial.print("  |  Luz: ");     Serial.print(luz_p, 1); Serial.println(" %");
  if (temp != -999.0) {
    Serial.print("[DHT]  Temp: "); Serial.print(temp, 1);
    Serial.print(" C  |  Hum: ");  Serial.print(hum, 1); Serial.println(" %");
  }
  Serial.print("[LED]  Estado: ");
  Serial.println(led.estadoLed() ? "ENCENDIDO" : "apagado");

  // Envío ThingSpeak
  if (millis() - ultimoEnvio >= INTERVALO_MS) {
    if (WiFi.status() != WL_CONNECTED) conectarWiFi();

    ThingSpeak.setField(1, (int)luz_p);
    ThingSpeak.setField(2, temp);
    ThingSpeak.setField(3, hum);

    int codigo = ThingSpeak.writeFields(CHANNEL_ID, THINGSPEAK_API_KEY);

    if (codigo == 200) {
      Serial.println("[TS] Datos enviados OK");
    } else {
      Serial.print("[TS] Error: "); Serial.println(codigo);
    }

    ultimoEnvio = millis();
  }

  delay(1000);
}
