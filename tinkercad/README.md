# 🔌 Guía de Montaje en TinkerCad Circuits (Reto Extra EA3)

- 👉 **Enlace público del circuito:** [https://www.tinkercad.com/things/9GOirWZ5FvB-circuito-tinkercad?sharecode=K-Z3pdnRC6QesqsSHOuCuKrVLk0OzmnR5VBd9xhRUMU](https://www.tinkercad.com/things/9GOirWZ5FvB-circuito-tinkercad?sharecode=K-Z3pdnRC6QesqsSHOuCuKrVLk0OzmnR5VBd9xhRUMU)

1. Abrir **TinkerCad Circuits** y crear un nuevo circuito.
2. Agregar un **Arduino UNO R3**, una **Protoboard**, **1 Potenciómetro**, **3 LEDs** (verde, amarillo, rojo) y **3 resistencias de 220 Ω**.
3. Conectar el **Potenciómetro**:
   - Terminal izquierdo a **5V**
   - Terminal central (wiper) a **A0**
   - Terminal derecho a **GND**
4. Conectar los **3 LEDs** (cátodo a **GND** a través de resistencia de `220 Ω`):
   - **LED Verde (Nevera 1.2 kW):** ánodo al pin **13**
   - **LED Amarillo (Horno 7.0 kW):** ánodo al pin **12**
   - **LED Rojo (Amasadora 2.2 kW):** ánodo al pin **11**
5. Abrir la pestaña **Código -> Texto**, pegar el contenido de `wattcontrol_arduino.ino`, iniciar la simulación y abrir el **Monitor en serie**.
6. Girar el potenciómetro para simular precios XM entre `$350` y `$1.650 COP/kWh` y observar cómo se encienden/apagan los LEDs según las reglas de WattControl.
