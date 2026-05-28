# Estación Ambiental IoT con ESP32

Sistema de monitoreo ambiental en tiempo real desarrollado con ESP32,
que lee temperatura, humedad y luz, y envía los datos a ThingSpeak cada 15 segundos.

## Integrantes
- Hugo Alejandro Rodriguez Montenegro
- Andres Felipe Bolaños Zuñiga
- Jarol Herney Medina Perafan

## Descripción
El sistema lee tres sensores ambientales y reporta los datos a la nube:
- **LDR**: mide la intensidad de luz (0-100%)
- **DHT22**: mide temperatura (°C) y humedad relativa (%)
- **LED**: se enciende automáticamente si la temperatura supera 30°C
- **Botón**: apaga la alerta del LED manualmente

## Arquitectura del código
El proyecto aplica Programación Orientada a Objetos con cuatro clases definidas en MisClases.h:

| Clase | Descripción |
|---|---|
| `Led` | Controla el LED de alerta |
| `Boton` | Lee el botón con detección de flanco |
| `LDR` | Lee la luz por ADC y calcula porcentaje |
| `SensorDHT` | Lee temperatura y humedad del DHT22 |

## Pines utilizados
| Componente | Pin ESP32 |
|---|---|
| LDR | GPIO34 (ADC1) |
| DHT22 | GPIO4 |
| LED | GPIO2 |
| Botón | GPIO15 |

## Plataforma
- Simulado en [Wokwi](https://wokwi.com)
- Datos enviados a [ThingSpeak](https://thingspeak.com) (Channel ID: 3395823)

## Librerías
- DHT sensor library (Adafruit)
- ThingSpeak (MathWorks)
