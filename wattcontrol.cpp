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
// =============================================================
class Medidor {
private:
    // Energia total acumulada en kWh
    float kWhAcumulados;
    // Costo total acumulado en pesos colombianos (COP)
    float costoAcumulado;

public:
    // Constructor: el medidor inicia en 0 kWh y $0
    Medidor() {
        kWhAcumulados = 0;
        costoAcumulado = 0;
    }

    // Reinicia el medidor a 0 antes de empezar una nueva simulacion de 24 horas
    void reiniciar() {
        kWhAcumulados = 0;
        costoAcumulado = 0;
    }

    // Registra el consumo (kWh) y el costo (COP) de una hora de operacion
    void registrarHora(float kwh, float costo) {
        kWhAcumulados = kWhAcumulados + kwh;
        costoAcumulado = costoAcumulado + costo;
    }

    // Getters para consultar lo acumulado en el medidor
    float getKWh() { return kWhAcumulados; }
    float getCosto() { return costoAcumulado; }
};

// =============================================================
// RELACION 4 (ASOCIACION):
// Mercado existe por fuera del Negocio; da el precio de cada hora y los embalses
// =============================================================
class Mercado {
private:
    // Nivel actual de los embalses del pais en porcentaje (0 a 100 %)
    float nivelEmbalses;

public:
    // Constructor: inicia los embalses en el nivel por defecto (65 %)
    Mercado(float embalsesInicial) {
        nivelEmbalses = embalsesInicial;
    }

    // Actualiza el nivel de los embalses cuando el usuario ingresa una lectura manual
    void setEmbalses(float nuevoNivel) {
        nivelEmbalses = nuevoNivel;
    }

    // Devuelve el nivel actual de los embalses
    float getEmbalses() {
        return nivelEmbalses;
    }

    // Genera el precio de bolsa al azar dentro del rango real de XM para cada franja
    float precioHora(int hora) {
        int minimo, maximo;
        // Franja 1: Madrugada (0 a 6 h) -> 398 a 640 COP/kWh
        if (hora <= HORA_FIN_MADRUGADA) {
            minimo = PRECIO_MIN_MADRUGADA;
            maximo = PRECIO_MAX_MADRUGADA;
        // Franja 2: Manana y mediodia (7 a 14 h) -> 456 a 970 COP/kWh
        } else if (hora <= HORA_FIN_MANANA) {
            minimo = PRECIO_MIN_MANANA;
            maximo = PRECIO_MAX_MANANA;
        // Franja 3: Tarde (15 a 17 h) -> 596 a 1040 COP/kWh
        } else if (hora <= HORA_FIN_TARDE) {
            minimo = PRECIO_MIN_TARDE;
            maximo = PRECIO_MAX_TARDE;
        // Franja 4: Hora pico (18 a 20 h) -> 705 a 1612 COP/kWh
        } else if (hora <= PICO_FIN) {
            minimo = PRECIO_MIN_PICO;
            maximo = PRECIO_MAX_PICO;
        // Franja 5: Noche (21 a 23 h) -> 565 a 970 COP/kWh
        } else {
            minimo = PRECIO_MIN_NOCHE;
            maximo = PRECIO_MAX_NOCHE;
        }
        // Retorna un numero aleatorio entre minimo y maximo (incluidos)
        return minimo + rand() % (maximo - minimo + 1);
    }
};

// =============================================================
// Clase principal Negocio: reune el Medidor (composicion), el Mercado (asociacion)
// y las Cargas (agregacion), aplica la Regla 6 y simula las 24 horas
// =============================================================
class Negocio {
private:
    // Nombre del negocio
    string nombre;
    // COMPOSICION: el Medidor es un atributo por valor; nace y muere con el Negocio
    Medidor medidor;
    // AGREGACION: arreglo de punteros a Carga. Las cargas se crean en main() y se agregan aqui
    Carga* cargas[MAX_CARGAS];
    // Cantidad de cargas agregadas actualmente
    int numCargas;
    // ASOCIACION: puntero al Mercado. El Negocio no es dueno del Mercado, solo le pide datos
    Mercado* mercado;

    // Resultados guardados de la ultima simulacion para mostrarlos en la Opcion 4
    float costoSinGestor;
    float kwhSinGestor;
    int horasSimuladas;
    int fallasLectura;
    bool huboEmergencia;
    bool simulacionRealizada;

public:
    // Constructor: recibe el nombre del negocio y la direccion del Mercado (Mercado* m)
    Negocio(string n, Mercado* m) : medidor() {
        nombre = n;
        mercado = m;
        numCargas = 0;
        costoSinGestor = 0;
        kwhSinGestor = 0;
        horasSimuladas = 0;
        fallasLectura = 0;
        huboEmergencia = false;
        simulacionRealizada = false;
    }

    // AGREGACION: agrega la direccion de una carga creada afuera (Carga* c)
    void agregarCarga(Carga* c) {
        if (numCargas < MAX_CARGAS) {
            cargas[numCargas] = c;
            numCargas++;
        }
    }

    // Muestra la configuracion general del negocio y sus cargas registradas
    void mostrarConfiguracion() {
        cout << "=============================================================" << endl;
        cout << "Negocio: " << nombre << " (Ultimo digito doc: 0-1)" << endl;
        cout << "Potencia contratada: " << POTENCIA_CONTRATADA << " kW | Precio caro: > $"
             << PRECIO_CARO << " COP/kWh" << endl;
        cout << "Alerta de embalses (El Nino): < " << EMBALSES_ALERTA
             << " % | Hora pico: " << PICO_INICIO << " a " << PICO_FIN << " h" << endl;
        cout << "Cargas registradas (" << numCargas << "):" << endl;
        for (int i = 0; i < numCargas; i++) {
            cout << "  - " << setw(18) << left << cargas[i]->getNombre() << right
                 << " (" << cargas[i]->getPotencia() << " kW)";
            if (cargas[i]->esAplazable()) {
                cout << " [Aplazable | Minimo: " << cargas[i]->getHorasMinimas() << " h/dia]";
            } else {
                cout << " [Critica   | Opera 24 h/dia]";
            }
            cout << endl;
        }
        cout << "=============================================================" << endl;
    }

    // Regla 6: verifica si la suma de potencias encendidas supera POTENCIA_CONTRATADA.
    // Si la supera, apaga la carga aplazable encendida de MAYOR potencia y muestra alerta.
    // Retorna el nombre de la carga apagada por tope (o "" si no hubo sobrecarga).
    string aplicarReglaPotenciaContratada(bool mostrarAlertaConsola) {
        float potenciaEncendida = 0;
        for (int i = 0; i < numCargas; i++) {
            if (cargas[i]->estaEncendida()) {
                potenciaEncendida += cargas[i]->getPotencia();
            }
        }

        // Si la potencia total supera el tope contratado (9.0 kW en la panaderia)...
        if (potenciaEncendida > POTENCIA_CONTRATADA) {
            int indiceMayorAplazable = -1;
            float mayorPotencia = -1.0;

            // Busca entre las cargas aplazables que estan encendidas la de mayor potencia
            for (int i = 0; i < numCargas; i++) {
                if (cargas[i]->estaEncendida() && cargas[i]->esAplazable()) {
                    if (cargas[i]->getPotencia() > mayorPotencia) {
                        mayorPotencia = cargas[i]->getPotencia();
                        indiceMayorAplazable = i;
                    }
                }
            }

            // Si encontro una carga aplazable encendida, la apaga y emite la alerta
            if (indiceMayorAplazable != -1) {
                string nombreApagada = cargas[indiceMayorAplazable]->getNombre();
                cargas[indiceMayorAplazable]->apagarPorTope();
                if (mostrarAlertaConsola) {
                    cout << "  [ALERTA REGLA 6] Potencia demandada (" << potenciaEncendida
                         << " kW) supera la contratada (" << POTENCIA_CONTRATADA << " kW)." << endl;
                    cout << "                   Se apaga la carga aplazable de mayor potencia: "
                         << nombreApagada << " (" << mayorPotencia << " kW)." << endl;
                }
                return nombreApagada;
            }
        }
        return "";
    }

    // Opcion 2: Evalua y muestra que haria cada carga con la lectura manual (hora, precio, embalses)
    void evaluarDecisionManual(int hora, float precio, float embalses) {
        cout << fixed << setprecision(1);
        cout << endl << "--- Evaluacion puntual de cargas ---" << endl;
        cout << "Condiciones: Hora " << hora << ":00 | Precio $" << setprecision(0) << precio
             << " COP/kWh | Embalses " << setprecision(1) << embalses << " %" << endl;

        // Avisos de contexto segun las reglas
        if (embalses < EMBALSES_ALERTA) {
            cout << "Estado del sistema: ALERTA FENOMENO DEL NINO (embalses < "
                 << EMBALSES_ALERTA << " %)" << endl;
        }
        if (hora >= PICO_INICIO && hora <= PICO_FIN) {
            cout << "Estado horario:     HORA PICO (" << PICO_INICIO << " a " << PICO_FIN << " h)" << endl;
        }
        if (precio > PRECIO_CARO) {
            cout << "Estado de tarifa:   ENERGIA CARA (> $" << setprecision(0) << PRECIO_CARO << " COP/kWh)" << endl;
        }

        // Reinicia temporalmente las cargas para evaluar la hora puntual en limpio
        for (int i = 0; i < numCargas; i++) {
            cargas[i]->reiniciarDia();
        }

        // Paso A: Cada carga toma su decision inicial mediante POLIMORFISMO
        for (int i = 0; i < numCargas; i++) {
            if (cargas[i]->debeEncender(hora, precio, embalses)) {
                cargas[i]->encender();
            } else {
                cargas[i]->apagar();
            }
        }

        // Paso B: El Negocio aplica la Regla 6 (tope de potencia contratada = 9.0 kW)
        cout << setprecision(1);
        string apagadaPorTope = aplicarReglaPotenciaContratada(true);

        // Paso C: Muestra la decision final de cada carga y la potencia total resultante
        float potenciaFinal = 0;
        float costoHoraFinal = 0;
        for (int i = 0; i < numCargas; i++) {
            bool on = cargas[i]->estaEncendida();
            cout << "  " << setw(18) << left << cargas[i]->getNombre() << right
                 << " (" << setprecision(1) << cargas[i]->getPotencia() << " kW): ";
            if (on) {
                cout << "ENCENDER" << endl;
                potenciaFinal += cargas[i]->getPotencia();
                costoHoraFinal += cargas[i]->costoHora(precio);
            } else if (cargas[i]->getNombre() == apagadaPorTope) {
                cout << "APLAZAR / APAGAR (por Regla 6: tope de potencia contratada)" << endl;
            } else {
                cout << "APLAZAR / APAGAR (por regla de precio, hora pico o embalses)" << endl;
            }
        }
        cout << "Potencia total encendida: " << setprecision(1) << potenciaFinal
             << " kW de " << POTENCIA_CONTRATADA << " kW contratados" << endl;
        cout << "Costo estimado en esta hora: $" << setprecision(0) << costoHoraFinal << " COP" << endl;
    }

    // Opcion 3: Simula un dia completo (24 horas), comparando Con Gestor vs Sin Gestor
    void simularDia() {
        // Reinicia el medidor (composicion), las cargas (agregacion) y las variables del resumen
        medidor.reiniciar();
        for (int i = 0; i < numCargas; i++) {
            cargas[i]->reiniciarDia();
        }
        costoSinGestor = 0;
        kwhSinGestor = 0;
        horasSimuladas = 0;
        fallasLectura = 0;
        huboEmergencia = false;
        simulacionRealizada = true;

        // Obtiene el nivel actual de los embalses desde el Mercado (asociacion)
        float embalses = mercado->getEmbalses();

        // Calcula la potencia total si las 3 cargas estuvieran encendidas sin gestor (10.4 kW)
        float potenciaTotalSinGestor = 0;
        for (int i = 0; i < numCargas; i++) {
            potenciaTotalSinGestor += cargas[i]->getPotencia();
        }

        cout << endl;
        cout << "===== Simulacion de 24 horas - " << nombre << " =====" << endl;
        cout << "Nivel de embalses: " << fixed << setprecision(1) << embalses << " %";
        if (embalses < EMBALSES_ALERTA) {
            cout << " [ALERTA FENOMENO DEL NINO < " << EMBALSES_ALERTA << " %]";
        }
        cout << endl;

        // Encabezado de la tabla horaria
        cout << fixed << setprecision(0);
        cout << setw(5) << "Hora" << setw(8) << "Precio" << setw(9) << "Embalses";
        for (int i = 0; i < numCargas; i++) {
            cout << setw(19) << cargas[i]->getNombre();
        }
        cout << setw(8) << "kW_On" << setw(9) << "Costo" << "  Observacion" << endl;
        cout << "-------------------------------------------------------------------------------------------------------------" << endl;

        // Ciclo for para simular las 24 horas del dia (h = 0, 1, ..., 23)
        for (int h = 0; h < HORAS_DIA; h++) {
            // ASOCIACION: el Negocio le pide al Mercado el precio de bolsa de la hora h
            float precio = mercado->precioHora(h);

            // break: si el precio supera 1500 COP/kWh Y los embalses estan por debajo del 40 %,
            // se declara emergencia de racionamiento y termina la simulacion de inmediato
            if (precio > PRECIO_EMERGENCIA && embalses < EMBALSES_ALERTA) {
                cout << setw(5) << h << setw(8) << precio << setw(8) << setprecision(0) << embalses << "%"
                     << "  EMERGENCIA DE RACIONAMIENTO (precio > $1500 y embalses < 40 %): se detiene la simulacion" << endl;
                huboEmergencia = true;
                break;
            }

            // Esta hora si se simula: suma 1 al contador de horas simuladas
            horasSimuladas++;

            // Acumula el escenario SIN GESTOR: las 3 cargas encendidas durante toda la hora
            kwhSinGestor += potenciaTotalSinGestor;
            costoSinGestor += potenciaTotalSinGestor * precio;

            // continue: 1 de cada 20 horas falla la lectura del precio;
            // en esa hora solo funciona la carga critica y se salta a la siguiente hora
            if (rand() % FALLA_CADA == 0) {
                fallasLectura++;
                float kwhFalla = 0;
                float costoFalla = 0;
                for (int i = 0; i < numCargas; i++) {
                    if (!cargas[i]->esAplazable()) {
                        cargas[i]->encender();
                        kwhFalla += cargas[i]->kwhHora();
                        costoFalla += cargas[i]->costoHora(precio);
                    } else {
                        cargas[i]->apagar();
                    }
                }
                // El medidor registra el consumo de la carga critica en esa hora
                medidor.registrarHora(kwhFalla, costoFalla);

                cout << setw(5) << h << setw(8) << "--" << setw(8) << setprecision(0) << embalses << "%";
                for (int i = 0; i < numCargas; i++) {
                    cout << setw(19) << (cargas[i]->estaEncendida() ? "ON" : "off");
                }
                cout << setw(8) << setprecision(1) << kwhFalla
                     << setw(9) << setprecision(0) << costoFalla
                     << "  falla de lectura: solo carga critica ON" << endl;
                continue;
            }

            // Escenario CON GESTOR: cada carga decide con POLIMORFISMO si debe encender
            for (int i = 0; i < numCargas; i++) {
                if (cargas[i]->debeEncender(h, precio, embalses)) {
                    cargas[i]->encender();
                } else {
                    cargas[i]->apagar();
                }
            }

            // Se aplica la Regla 6: si las cargas encendidas superan 9.0 kW,
            // apaga la aplazable de mayor potencia (sin desordenar la tabla)
            string apagadaRegla6 = aplicarReglaPotenciaContratada(false);

            // Calcula los kWh y el costo de esta hora con gestor y los registra en el Medidor
            float kwhHoraCon = 0;
            float costoHoraCon = 0;
            for (int i = 0; i < numCargas; i++) {
                kwhHoraCon += cargas[i]->kwhHora();
                costoHoraCon += cargas[i]->costoHora(precio);
            }
            // COMPOSICION: el Medidor del negocio acumula los kWh y el costo
            medidor.registrarHora(kwhHoraCon, costoHoraCon);

            // Construye la observacion de la fila para que la tabla sea muy clara
            string nota = "";
            if (apagadaRegla6 != "") {
                nota = "Regla 6: tope 9 kW -> apaga " + apagadaRegla6;
            } else if (embalses < EMBALSES_ALERTA) {
                nota = "Regla 2: embalses < 40 % (aplaza)";
            } else if (h >= PICO_INICIO && h <= PICO_FIN) {
                nota = "Regla 3: hora pico 18-20 h (aplaza)";
            } else if (precio > PRECIO_CARO) {
                nota = "Regla 4: precio > $800 (aplaza)";
            }

            // Imprime la fila de la hora h en la tabla
            cout << setw(5) << h << setw(8) << setprecision(0) << precio
                 << setw(8) << setprecision(0) << embalses << "%";
            for (int i = 0; i < numCargas; i++) {
                cout << setw(19) << (cargas[i]->estaEncendida() ? "ON" : "off");
            }
            cout << setw(8) << setprecision(1) << kwhHoraCon
                 << setw(9) << setprecision(0) << costoHoraCon
                 << "  " << nota << endl;
        }

        cout << "-------------------------------------------------------------------------------------------------------------" << endl;
        cout << "Horas simuladas: " << horasSimuladas << " de " << HORAS_DIA
             << " | fallas de lectura: " << fallasLectura << endl;
        cout << "Costo con gestor: $" << fixed << setprecision(0) << medidor.getCosto()
             << " COP (" << setprecision(1) << medidor.getKWh() << " kWh)" << endl;
        cout << "Costo sin gestor: $" << setprecision(0) << costoSinGestor
             << " COP (" << setprecision(1) << kwhSinGestor << " kWh)" << endl;
        cout << "Simulacion terminada. Consulte el detalle completo y alertas en la Opcion 4." << endl;
    }

    // Opcion 4: Muestra el resumen de la ultima simulacion, el ahorro y la alerta de produccion
    void mostrarResumen() {
        if (!simulacionRealizada) {
            cout << "Aun no se ha ejecutado ninguna simulacion. Use primero la Opcion 3." << endl;
            return;
        }

        cout << endl << "===== Resumen de la ultima simulacion: " << nombre << " =====" << endl;
        cout << "Horas simuladas: " << horasSimuladas << " de " << HORAS_DIA
             << " | Fallas de lectura (continue): " << fallasLectura << endl;
        if (huboEmergencia) {
            cout << "Estado del dia:  DETENIDO POR EMERGENCIA DE RACIONAMIENTO (break)" << endl;
        }

        cout << endl << "Desempeno por carga:" << endl;
        bool produccionCompleta = true;
        for (int i = 0; i < numCargas; i++) {
            float kwhCarga = cargas[i]->getHorasUso() * cargas[i]->getPotencia();
            cout << "  - " << setw(18) << left << cargas[i]->getNombre() << right
                 << ": " << setw(2) << cargas[i]->getHorasUso() << " h encendida | "
                 << fixed << setprecision(1) << setw(6) << kwhCarga << " kWh";

            if (cargas[i]->esAplazable()) {
                cout << " (Minimo requerido: " << cargas[i]->getHorasMinimas() << " h -> ";
                if (cargas[i]->cumplioHorasMinimas()) {
                    cout << "CUMPLIDO)";
                } else {
                    cout << "NO CUMPLIDO)";
                    produccionCompleta = false;
                }
            } else {
                cout << " (Carga critica)";
            }
            cout << endl;
        }

        // Si alguna carga aplazable no cumplio sus horas minimas, muestra la alerta obligatoria
        if (!produccionCompleta) {
            cout << endl << "  [ALERTA] produccion incompleta: una o mas cargas aplazables no alcanzaron" << endl;
            cout << "           sus horas minimas de operacion diaria." << endl;
        } else {
            cout << endl << "  [OK] Produccion diaria completada: todas las cargas cumplieron sus horas minimas." << endl;
        }

        // Calculo del ahorro en pesos y en porcentaje usando el Medidor (composicion)
        float costoCon = medidor.getCosto();
        float ahorroPesos = costoSinGestor - costoCon;
        float ahorroPorcentaje = 0;
        if (costoSinGestor > 0) {
            ahorroPorcentaje = (ahorroPesos / costoSinGestor) * 100.0;
        }

        cout << fixed << setprecision(0);
        cout << endl << "Balance economico y energetico:" << endl;
        cout << "  Consumo con gestor: " << setprecision(1) << medidor.getKWh() << " kWh" << endl;
        cout << "  Consumo sin gestor: " << setprecision(1) << kwhSinGestor << " kWh" << endl;
        cout << "  Costo con gestor:   $" << setprecision(0) << costoCon << " COP" << endl;
        cout << "  Costo sin gestor:   $" << setprecision(0) << costoSinGestor << " COP" << endl;
        if (horasSimuladas > 0) {
            cout << "  Costo promedio/h:   $" << setprecision(0) << (costoCon / horasSimuladas) << " COP/h" << endl;
        }
        cout << "  Ahorro obtenido:    $" << setprecision(0) << ahorroPesos << " COP ("
             << setprecision(1) << ahorroPorcentaje << " %)" << endl;
    }
};

// =============================================================
// Funcion principal main(): crea los objetos, conecta las relaciones y ejecuta el menu
// =============================================================
int main() {
    // Inicializa la semilla de numeros aleatorios para que la simulacion sea reproducible
    srand(SEMILLA);

    // ASOCIACION: el objeto Mercado se crea de forma independiente en main()
    Mercado mercado(EMBALSES_INICIAL);

    // HERENCIA y AGREGACION: las 3 cargas de la panaderia se crean afuera del Negocio
    CargaCritica nevera("Nevera de insumos", POTENCIA_NEVERA);
    CargaAplazable horno("Horno electrico", POTENCIA_HORNO, HORAS_MIN_HORNO, HORAS_TURNO_HORNO);
    CargaAplazable amasadora("Amasadora", POTENCIA_AMASADORA, HORAS_MIN_AMASADORA, HORAS_TURNO_AMASADORA);

    // Se crea el Negocio pasandole la direccion del Mercado (&mercado -> Asociacion)
    // Internamente el Negocio construye su propio Medidor (Composicion)
    Negocio panaderia("Panaderia El Trigo de Oro", &mercado);

    // Se agregan las direcciones de las cargas al Negocio (&nevera, &horno, &amasadora -> Agregacion)
    panaderia.agregarCarga(&nevera);
    panaderia.agregarCarga(&horno);
    panaderia.agregarCarga(&amasadora);

    // Muestra la ficha inicial del negocio y sus 3 cargas
    panaderia.mostrarConfiguracion();

    // Variables para la lectura manual (inician con valores validos por defecto)
    int horaManual = HORA_INICIAL;
    float precioManual = PRECIO_INICIAL;
    float embalsesManual = EMBALSES_INICIAL;
    int opcion;

    // Menu interactivo con do-while y switch (con default)
    do {
        cout << endl << "===== Menu Principal WattControl =====" << endl;
        cout << "1. Ingresar lectura manual (hora, precio, embalses)" << endl;
        cout << "2. Evaluar decision de cada carga" << endl;
        cout << "3. Simular un dia completo (24 h)" << endl;
        cout << "4. Ver resumen de la ultima simulacion" << endl;
        cout << "0. Salir" << endl;
        cout << "Opcion: ";
        cin >> opcion;

        switch (opcion) {
        // Opcion 1: Ingresar lectura manual validando cada dato con do-while
        case 1:
            do {
                cout << "Hora del dia (0 a 23): ";
                cin >> horaManual;
                if (horaManual < HORA_MINIMA || horaManual > HORA_MAXIMA) {
                    cout << "  Error: la hora debe estar entre 0 y 23." << endl;
                }
            } while (horaManual < HORA_MINIMA || horaManual > HORA_MAXIMA);

            do {
                cout << "Precio de bolsa (COP/kWh, mayor que 0): ";
                cin >> precioManual;
                if (precioManual <= 0) {
                    cout << "  Error: el precio debe ser mayor que 0." << endl;
                }
            } while (precioManual <= 0);

            do {
                cout << "Nivel de los embalses (0 a 100 %): ";
                cin >> embalsesManual;
                if (embalsesManual < EMBALSES_MINIMO || embalsesManual > EMBALSES_MAXIMO) {
                    cout << "  Error: el nivel de los embalses debe estar entre 0 y 100 %." << endl;
                }
            } while (embalsesManual < EMBALSES_MINIMO || embalsesManual > EMBALSES_MAXIMO);

            // Actualiza el nivel de embalses en el Mercado para la siguiente simulacion
            mercado.setEmbalses(embalsesManual);
            cout << "Lectura manual guardada correctamente (embalses actualizados a "
                 << fixed << setprecision(1) << embalsesManual << " %)." << endl;
            break;

        // Opcion 2: Evaluar la decision de cada carga con la lectura manual vigente
        case 2:
            panaderia.evaluarDecisionManual(horaManual, precioManual, embalsesManual);
            break;

int main() {
    return 0;
}
