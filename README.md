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

## Geplante Erweiterungen

- Hochladen der PCB-Layouts für jedes Modul  
- Integration einer Positionskalibrierung und einen PID Regler mit einenem magnetic rotary encoder
- Einbindung in Home Assistant
- Optional: WLAN-Steuerung und App-Anbindung  
- Energiesparmodus im Stillstand  

---


