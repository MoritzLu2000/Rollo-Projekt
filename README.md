# Wireless Rollomodule

In diesem Projekt geht es um die Entwicklung von **kabellosen Rollosteuerungen**, die nur über ein Stromkabel versorgt werden und per **Funkfernbedienung** bedient werden können.  
Jedes Modul basiert auf einem **ESP32** und einem **Schrittmotor**, der das Rollo präzise positioniert.

Der Fokus liegt auf einer einfachen Installation ohne separate Steuerleitungen.  
Die Module können einzeln oder gemeinsam angesteuert werden.  

Später werden in diesem Repository auch die **PCB-Designs** der einzelnen Rollomodule ergänzt.

---

## Aktueller Stand

- Steuerung per Fernbedienung (ESP32 empfängt Funkbefehle)  
- Antrieb über Stepper-Motoren mit Treiberplatine  
- Nur ein Netzkabel pro Rollo nötig  
- Code für mehrere Module vorbereitet  

---

## Geplante Erweiterungen mit Steppermotoren

- Hochladen der PCB-Layouts für jedes Modul  
- Integration einer Positionskalibrierung und einen PID Regler mit einenem magnetic rotary encoder
- Einbindung in Home Assistant
- Optional: WLAN-Steuerung und App-Anbindung  
- Energiesparmodus im Stillstand  

---

## Geplante komplett kabellose Rollomodule – 5-V-Version

Dieses Projekt basiert auf dem gleichen Prinzip wie das ursprüngliche **Wireless-Rollomodule**-Projekt, zielt jedoch auf eine vollständig **kabellose Version** ab.

Jedes Modul nutzt einen **5-V-Schrittmotor** und wird von einem **ESP32** gesteuert.  
Die Energieversorgung erfolgt über einen **Li-Akku**, der über **USB-C** wieder aufgeladen werden kann.  
Damit wird kein festes Stromkabel mehr benötigt. Jedes Rollo arbeitet also autark.

**Eigenschaften der 5-V-Version:**

- 100 % kabelloser Betrieb  
- Kompakte Einheit mit Akku, Ladeelektronik, Motor und Steuerung  
- Aufladung über USB-C  
- Kommunikation per Funkfernbedienung (ESP32-basiert)  
- Gleiche Softwarebasis wie das ursprüngliche, kabelgebundene System
