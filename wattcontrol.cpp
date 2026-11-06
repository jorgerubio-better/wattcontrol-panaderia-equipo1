// =============================================================
// WattControl v1 - Gestor de energia de una Panaderia (El Nino 2026)
// Curso: Programacion Orientada a Objetos - IU Digital de Antioquia
// Docente: Carlos Arturo Castro Castro
// Equipo:
//   1. Jorge Alejandro Rubio Giraldo (Ultimo digito documento: 0-1 -> Panaderia)
//   2. Luisa Fernanda Parra Arboleda
//   3. Maribel Roman Ospina
//   4. Deivis Alfonso Yance Rosado
// =============================================================

// --- Librerias: codigo que ya existe y que se trae al programa -------------
// iostream: cout (escribir en pantalla) y cin (leer del teclado)
#include <iostream>
// iomanip: setw (ancho de columna) y setprecision (cantidad de decimales)
#include <iomanip>
// string: el tipo string, para guardar textos
#include <string>
// cstdlib: rand() y srand(), para generar numeros al azar
#include <cstdlib>

// Permite escribir cout en vez de std::cout (todo lo de la libreria esta en "std")
using namespace std;

// --- Directivas del preprocesador ------------------------------------------
// Antes de compilar, el preprocesador cambia cada nombre por su valor en todo el archivo.
// No llevan = ni ; al final.
// Horas de la simulacion: un dia completo
#define HORAS_DIA 24
// Cuantas cargas puede tener el negocio como maximo (tamano del arreglo en Negocio)
#define MAX_CARGAS 3
// Semilla de rand(): con la misma semilla, todos obtienen los mismos precios al azar
#define SEMILLA 2038

// --- Constantes: todos los valores del negocio (Panaderia, digitos 0-1) ----
// const = no se pueden cambiar despues. Si cambia un valor, se cambia solo aqui.
// Potencia en kW de la carga critica: Nevera de insumos (funciona 24 h)
const float POTENCIA_NEVERA = 1.2;
// Potencia en kW de la carga aplazable 1: Horno electrico
const float POTENCIA_HORNO = 7.0;
// Potencia en kW de la carga aplazable 2: Amasadora industrial
const float POTENCIA_AMASADORA = 2.2;
// Potencia maxima contratada por la panaderia (kW) - Regla 6
const float POTENCIA_CONTRATADA = 9.0;
// Precio considerado "caro" para la panaderia (COP/kWh) - Regla 4
const float PRECIO_CARO = 800.0;
// Precio de emergencia de racionamiento (COP/kWh) - Paso 7 (break)
const float PRECIO_EMERGENCIA = 1500.0;
// Nivel de los embalses (%) por debajo del cual hay alerta de El Nino - Regla 2
const float EMBALSES_ALERTA = 40.0;
// Nivel inicial de los embalses (%) al arrancar el programa
const float EMBALSES_INICIAL = 65.0;
// Precio inicial por defecto para pruebas manuales (COP/kWh)
const float PRECIO_INICIAL = 550.0;
// Hora inicial por defecto para pruebas manuales (0 a 23)
const int HORA_INICIAL = 10;
// La hora pico empieza a las 18 h (6 pm)... - Regla 3
const int PICO_INICIO = 18;
// ...y termina a las 20 h (la hora 20 esta incluida: hasta las 9 pm)
const int PICO_FIN = 20;
// Limites validos para la lectura manual
const int HORA_MINIMA = 0;
const int HORA_MAXIMA = 23;
const float EMBALSES_MINIMO = 0.0;
const float EMBALSES_MAXIMO = 100.0;
// Horas minimas de operacion diaria para cumplir la produccion de pan
const int HORAS_MIN_HORNO = 6;
const int HORAS_MIN_AMASADORA = 4;
// Horas de turno diario maximo de cada equipo aplazable (evita uso innecesario)
const int HORAS_TURNO_HORNO = 10;
const int HORAS_TURNO_AMASADORA = 4;
// 1 de cada 20 lecturas del precio falla (continue)
const int FALLA_CADA = 20;

// Rangos reales de precios de bolsa XM (COP/kWh) por franja horaria
const int HORA_FIN_MADRUGADA = 6;
const int PRECIO_MIN_MADRUGADA = 398;
const int PRECIO_MAX_MADRUGADA = 640;

const int HORA_FIN_MANANA = 14;
const int PRECIO_MIN_MANANA = 456;
const int PRECIO_MAX_MANANA = 970;

const int HORA_FIN_TARDE = 17;
const int PRECIO_MIN_TARDE = 596;
const int PRECIO_MAX_TARDE = 1040;

const int PRECIO_MIN_PICO = 705;
const int PRECIO_MAX_PICO = 1612;

const int PRECIO_MIN_NOCHE = 565;
const int PRECIO_MAX_NOCHE = 970;

// =============================================================
// Clase base abstracta Carga: lo que tienen en comun todas las cargas
// =============================================================
class Carga {
// protected: estos atributos los ven esta clase y sus hijas (CargaCritica, CargaAplazable),
// pero no el resto del programa
protected:
    // El nombre del equipo: "Nevera", "Horno", "Amasadora"
    string nombre;
    // La potencia electrica en kW
    float potenciaKW;
    // true si esta encendida en esta hora; false si esta apagada
    bool encendida;
    // Cuantas horas ha funcionado en el dia (para el resumen y horas minimas)
    int horasUso;

// public: lo que puede usar cualquier parte del programa
public:
    // Constructor: recibe el nombre (n) y la potencia en kW (p)
    Carga(string n, float p) {
        nombre = n;
        potenciaKW = p;
        encendida = false;
        horasUso = 0;
    }

    // Destructor virtual: obligatorio en una clase base con metodos virtuales
    virtual ~Carga() {}

    // POLIMORFISMO: metodo virtual puro (= 0).
    // Cada clase hija (CargaCritica y CargaAplazable) escribe su propia regla.
    // Hace que Carga sea una clase ABSTRACTA (no se pueden crear objetos Carga directos).
    virtual bool debeEncender(int hora, float precio, float embalses) = 0;

    // Indica si la carga es aplazable (para que la Regla 6 sepa cuales puede apagar)
    virtual bool esAplazable() {
        return false;
    }

    // Devuelve las horas minimas diarias requeridas (0 para cargas criticas)
    virtual int getHorasMinimas() {
        return 0;
    }

    // Indica si la carga cumplio sus horas minimas del dia
    virtual bool cumplioHorasMinimas() {
        return true;
    }

    // Enciende la carga durante esta hora y suma 1 a las horas de uso
    void encender() {
        encendida = true;
        horasUso++;
    }

    // Apaga la carga cuando la Regla 6 (tope de potencia contratada) la desconecta
    // en esa misma hora, descontando la hora que se acababa de sumar en encender()
    void apagarPorTope() {
        if (encendida) {
            encendida = false;
            if (horasUso > 0) {
                horasUso--;
            }
        }
    }

    // Apaga la carga normalmente al evaluar una nueva hora
    void apagar() {
        encendida = false;
    }

    // Reinicia el contador de horas y el estado al comenzar un nuevo dia
    void reiniciarDia() {
        encendida = false;
        horasUso = 0;
    }

    // Costo de esta hora: potenciaKW x precio si esta encendida; 0 si esta apagada
    float costoHora(float precio) {
        if (encendida) {
            return potenciaKW * precio;
        }
        return 0;
    }

    // Energia consumida en esta hora (en 1 hora, kWh = kW x 1 h)
    float kwhHora() {
        if (encendida) {
            return potenciaKW;
        }
        return 0;
    }

    // Getters: permiten consultar los atributos protegidos sin modificarlos
    string getNombre() { return nombre; }
    float getPotencia() { return potenciaKW; }
    bool estaEncendida() { return encendida; }
    int getHorasUso() { return horasUso; }
};

// =============================================================
// RELACION 1 (HERENCIA) y RELACION 2 (POLIMORFISMO):
// CargaCritica ES UNA Carga que nunca se apaga (Regla 1)
// =============================================================
class CargaCritica : public Carga {
public:
    // Constructor: le pasa el nombre y la potencia al constructor de Carga
    CargaCritica(string n, float p) : Carga(n, p) {}

    // Regla 1: la carga critica (Nevera de insumos) siempre debe estar encendida,
    // sin importar la hora, el precio ni el nivel de los embalses
    bool debeEncender(int hora, float precio, float embalses) override {
        (void)hora;
        (void)precio;
        (void)embalses;
        return true;
    }
};

// =============================================================
// RELACION 1 (HERENCIA) y RELACION 2 (POLIMORFISMO):
// CargaAplazable ES UNA Carga que se aplaza segun las Reglas 2 a 5
// =============================================================
class CargaAplazable : public Carga {
private:
    // Minimo de horas diarias que debe operar para no tener produccion incompleta
    int horasMinimas;
    // Turno maximo de horas que requiere operar en el dia
    int horasTurnoMax;

public:
    // Constructor: recibe nombre, potencia, horas minimas y horas de turno maximo
    CargaAplazable(string n, float p, int hMin, int hMax) : Carga(n, p) {
        horasMinimas = hMin;
        horasTurnoMax = hMax;
    }

    // Reglas 2 a 5 expresadas como una condicion logica compuesta con &&, || y !:
    //   - Regla 2: si embalses < 40 % (alerta de El Nino), se aplaza.
    //   - Regla 3: si es hora pico (18 a 20 h), se aplaza.
    //   - Regla 4: si el precio supera el precio "caro" (> 800 COP/kWh), se aplaza.
    //   - Regla 5: en cualquier otro caso (si aun no ha completado su turno diario), se enciende.
    bool debeEncender(int hora, float precio, float embalses) override {
        bool alertaNino = (embalses < EMBALSES_ALERTA);
        bool horaPico = (hora >= PICO_INICIO && hora <= PICO_FIN);
        bool precioCaro = (precio > PRECIO_CARO);
        bool turnoCompleto = (horasUso >= horasTurnoMax);

        // Se aplaza si ocurre cualquiera de las condiciones de restriccion (||);
        // se enciende si NO (!) se debe aplazar y aun le faltan horas de su turno (&&)
        bool debeAplazar = alertaNino || horaPico || precioCaro;
        return !debeAplazar && !turnoCompleto;
    }

    // Sobreescribe esAplazable() para indicar que la Regla 6 puede apagar esta carga
    bool esAplazable() override {
        return true;
    }

    // Devuelve el minimo de horas diarias exigido para esta carga
    int getHorasMinimas() override {
        return horasMinimas;
    }

    // Verifica si al final del dia cumplio con las horas minimas
    bool cumplioHorasMinimas() override {
        return horasUso >= horasMinimas;
    }
};

// =============================================================
// RELACION 3 (COMPOSICION):
// Medidor es parte interna del Negocio; acumula los kWh y el costo

int main() {
    return 0;
}
