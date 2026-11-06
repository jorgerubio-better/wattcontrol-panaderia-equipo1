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

int main() {
    return 0;
}
