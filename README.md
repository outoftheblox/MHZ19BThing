# MH-Z18B Thing
A MH-Z18B CO2 sensor thing communicating over MQTT.

## Hardware
- Wemos D1 mini MCU
- MH-Z19B CO2 sensor
- 5V & 3.3V stable power supply board

## Wiring
- PowerBoard 5V - BreadBoard 5V line 
- PowerBoard GND - BreadBoard GND line
- PowerBoard 3V3 - BreadBoard 3V3 line 
- PowerBoard GND - BreadBoard GND line

- BreadBoard 5V line - D1Mini 5V
- BreadBoard 3V3 line - D1Mini 3V3
- BreadBoard GND line - D1Mini GND

- BreadBoard 5V line - MH-Z19B Vcc
- BreadBoard GND line - MH-Z19B GND

- D1-mini A0 - MH-Z19B Vo
- D1-mini D1 - MH-Z19B Rx
- D1-mini D2 - MH-Z19B Tx
- D1-mini D5 - MH-Z19B PWM

## Resources
https://emariete.com/en/sensor-co2-mh-z19b/
https://revspace.nl/MHZ19
