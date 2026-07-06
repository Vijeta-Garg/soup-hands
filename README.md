# Soup Hands

falling out of the world : O the world ends in 7 days!!!

*artificial hands meant to simulate long-distance hand holding : )

![Photo]()
![Schematic]()
![Zine]()

## BOM/Assembly/how it works

- 4 load cells with 4 HX711 ADCs
- 4 servos
- 2 Seeed Studio XIAO ESP32-C3

---

On the sending end we have a 3D printed hand with 4 load sensors that measure the pressure of your own hand on different areas. On the receiving end we have a glove for another person to wear, that has 4 motors pulling strings in the same places of the pressure on the other side. Both sides are connected to a laptop with WebSerial and transmitted with a central WebSocket server.

