// =============================================================
// WattControl - Reto Extra TinkerCad Circuits (Arduino UNO)
// Panaderia El Trigo de Oro (Potencia contratada: 9.0 kW)
// =============================================================
// Conexiones en TinkerCad:
//   - Potenciometro: terminal central al pin A0, extremos a 5V y GND
//   - LED 1 (Verde - Nevera 1.2 kW):     Pin 13 -> resistencia 220 ohm -> LED -> GND
//   - LED 2 (Amarillo - Horno 7.0 kW):   Pin 12 -> resistencia 220 ohm -> LED -> GND
//   - LED 3 (Azul/Rojo - Amasadora 2.2 kW): Pin 11 -> resistencia 220 ohm -> LED -> GND
// =============================================================

const int PIN_POT_PRECIO = A0;
const int PIN_LED_NEVERA = 13;
const int PIN_LED_HORNO = 12;
const int PIN_LED_AMASADORA = 11;

const float POTENCIA_NEVERA = 1.2;
const float POTENCIA_HORNO = 7.0;
const float POTENCIA_AMASADORA = 2.2;
const float POTENCIA_CONTRATADA = 9.0;

const float PRECIO_CARO = 800.0;
const float PRECIO_EMERGENCIA = 1500.0;
const int PICO_INICIO = 18;
const int PICO_FIN = 20;

int horaActual = 0;

void setup() {
  pinMode(PIN_LED_NEVERA, OUTPUT);
  pinMode(PIN_LED_HORNO, OUTPUT);
  pinMode(PIN_LED_AMASADORA, OUTPUT);

  Serial.begin(9600);
  Serial.println("====================================================");
  Serial.println("  WATTCONTROL ARDUINO - PANADERIA EL TRIGO DE ORO");
  Serial.println("  Gire el potenciometro para variar el precio XM");
  Serial.println("====================================================");
}

void loop() {
  // 1. Leer potenciometro (0..1023) y mapear a precio XM real (350..1650 COP/kWh)
  int lecturaPot = analogRead(PIN_POT_PRECIO);
  float precioXM = map(lecturaPot, 0, 1023, 350, 1650);

  // 2. Evaluar reglas de WattControl
  bool horaPico = (horaActual >= PICO_INICIO && horaActual <= PICO_FIN);
  bool precioCaro = (precioXM > PRECIO_CARO);
  bool emergencia = (precioXM > PRECIO_EMERGENCIA);

  // Regla 1: Nevera (CargaCritica) siempre encendida
  bool neveraOn = true;
  // Reglas 3, 4 y 5: Cargas aplazables se apagan en hora pico, precio caro o emergencia
  bool hornoOn = (!horaPico && !precioCaro && !emergencia);
  bool amasadoraOn = (!horaPico && !precioCaro && !emergencia);

  // Regla 6: Tope de potencia contratada (9.0 kW)
  // Nevera (1.2) + Horno (7.0) + Amasadora (2.2) = 10.4 kW > 9.0 kW
  // Si las tres quieren encender, se apaga la aplazable de mayor potencia (Horno)
  String notaRegla6 = "";
  float potenciaTotal = POTENCIA_NEVERA + (hornoOn ? POTENCIA_HORNO : 0) + (amasadoraOn ? POTENCIA_AMASADORA : 0);
  if (potenciaTotal > POTENCIA_CONTRATADA && hornoOn) {
    hornoOn = false;
    notaRegla6 = " [Regla 6: Tope 9kW -> Horno OFF]";
  }

  // 3. Activar los 3 LEDs segun las decisiones
  digitalWrite(PIN_LED_NEVERA, neveraOn ? HIGH : LOW);
  digitalWrite(PIN_LED_HORNO, hornoOn ? HIGH : LOW);
  digitalWrite(PIN_LED_AMASADORA, amasadoraOn ? HIGH : LOW);

  // 4. Calcular potencia final y costo de la hora
  float potenciaFinal = POTENCIA_NEVERA + (hornoOn ? POTENCIA_HORNO : 0) + (amasadoraOn ? POTENCIA_AMASADORA : 0);
  float costoHora = potenciaFinal * precioXM;

  // 5. Mostrar en el Monitor Serial
  Serial.print("Hora ");
  if (horaActual < 10) Serial.print("0");
  Serial.print(horaActual);
  Serial.print(":00 | Precio XM: $");
  Serial.print((int)precioXM);
  Serial.print(" COP/kWh | Nevera: ON | Horno: ");
  Serial.print(hornoOn ? "ON " : "OFF");
  Serial.print(" | Amasadora: ");
  Serial.print(amasadoraOn ? "ON " : "OFF");
  Serial.print(" | kW: ");
  Serial.print(potenciaFinal, 1);
  Serial.print(" | Costo: $");
  Serial.print((long)costoHora);
  Serial.print(" COP");
  if (emergencia) Serial.print(" [EMERGENCIA > $1500]");
  else if (horaPico) Serial.print(" [HORA PICO 18-20h]");
  else if (precioCaro) Serial.print(" [PRECIO CARO > $800]");
  Serial.println(notaRegla6);

  // Avanzar a la siguiente hora cada 2 segundos
  horaActual = (horaActual + 1) % 24;
  delay(2000);
}
