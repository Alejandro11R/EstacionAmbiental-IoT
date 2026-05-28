#ifndef MIS_CLASES_H
#define MIS_CLASES_H

#include <DHT.h>

class Led {
private:
  int pin;
  bool activo;
public:
  Led(int p) { pin = p; activo = false; }

  void begin() {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  void encender() { digitalWrite(pin, HIGH); activo = true; }
  void apagar()   { digitalWrite(pin, LOW);  activo = false; }

  bool estadoLed() { return activo; }
};


class Boton {
private:
  int pin;
  bool estadoAnterior;
public:
  Boton(int p) : pin(p), estadoAnterior(false) {}

  void begin() { pinMode(pin, INPUT_PULLUP); }

  bool presionado() {
    bool estado = (digitalRead(pin) == LOW);
    if (estado && !estadoAnterior) {
      estadoAnterior = true;
      return true;
    }
    if (!estado) estadoAnterior = false;
    return false;
  }
};


class LDR {
private:
  int pin;
public:
  LDR(int p) { pin = p; }

  void begin() { pinMode(pin, INPUT); }

  int leer() { return analogRead(pin); }

  float porcentaje() { return (leer() / 4095.0) * 100.0; }
};


class SensorDHT {
private:
  DHT dht;
  int pin;
public:
  SensorDHT(int p, int tipo = DHT22) : dht(p, tipo) { pin = p; }

  void begin() { dht.begin(); }

  float temperatura() {
    float t = dht.readTemperature();
    if (isnan(t)) { Serial.println("[DHT] Error temperatura"); return -999.0; }
    return t;
  }

  float humedad() {
    float h = dht.readHumidity();
    if (isnan(h)) { Serial.println("[DHT] Error humedad"); return -999.0; }
    return h;
  }
};

#endif
