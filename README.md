# 🥖 WattControl: Gestor Inteligente de Energía para Panadería

![C++](https://img.shields.io/badge/Lenguaje-C%2B%2B17-00599C?logo=c%2B%2B&logoColor=white)
![Paradigma](https://img.shields.io/badge/Paradigma-POO%20(5%20Relaciones)-orange)
![Institución](https://img.shields.io/badge/Universidad-IU%20Digital%20de%20Antioquia-blue)
![Evaluación](https://img.shields.io/badge/Entrega-EA2%20(Tag%20ea2)-green)
![Ahorro](https://img.shields.io/badge/Ahorro%20Energ%C3%A9tico-63.6%25-brightgreen)

Sistema de gestión energética en C++ diseñado para la **Panadería El Trigo de Oro**, orientado a optimizar los costos de consumo eléctrico en el mercado de bolsa de XM (Colombia), garantizando el 100 % de las metas de producción de pan y protegiendo el negocio contra contingencias de sobrecarga y racionamiento por sequía (Fenómeno del Niño).

---

## 🏛️ Información Académica

- **Institución:** Institución Universitaria Digital de Antioquia (IU Digital)
- **Facultad:** Facultad de Ingeniería · Ingeniería de Software
- **Asignatura:** Programación Orientada a Objetos (`PREICA2602B020043`)
- **Docente:** Prof. Carlos Arturo Castro Castro
- **Evaluación:** Evaluación de Aprendizaje 2 (EA2)
- **Negocio Asignado:** **Panadería** (Último dígito de documento: `0 – 1`)
- **Repositorio:** `wattcontrol-panaderia-equipo1` (Etiqueta oficial: `ea2`)

---

## 👥 Equipo de Desarrollo (Equipo 1)

| Integrante | Usuario GitHub | Rama de Trabajo | Aportes Técnicos y Relaciones POO Sustentadas |
| :--- | :---: | :--- | :--- |
| **Jorge Alejandro Rubio Giraldo** *(Creador del repo)* | [`@jorgerubio-better`](https://github.com/jorgerubio-better) | `feature/clases-carga-herencia` | Directivas `#define`, constantes `const` sin números mágicos, diseño de la clase base abstracta `Carga` (`virtual debeEncender = 0`) e implementación de `CargaCritica` y `CargaAplazable` (**Herencia** y **Polimorfismo**). |
| **Luisa Fernanda Parra Arboleda** | [`@luisaparra25`](https://github.com/luisaparra25) | `feature/medidor-y-mercado` | Implementación de la clase `Medidor` (**Composición** por valor en `Negocio`) y de la clase `Mercado` (**Asociación** mediante puntero `Mercado*`) con generación estocástica de precios XM por franja horaria. |
| **Maribel Román Ospina** | [`@mariospina03-wq`](https://github.com/mariospina03-wq) | `feature/negocio-y-simulacion` | Clase `Negocio` con arreglo polimórfico de punteros `Carga*` (**Agregación**), Regla 6 de potencia contratada (tope 9,0 kW) y simulación de 24 horas con ciclo `for`, `continue` por falla de lectura y `break` por emergencia. |
| **Deivis Alfonso Yancé Rosado** | [`@deivisJVC`](https://github.com/deivisJVC) | `feature/resumen-y-menu` | Método `mostrarResumen()` con balance de ahorro (63,6 %), alerta de producción incompleta, función `main()` integradora y menú interactivo `do-while`/`switch` con validaciones de entrada. |

---

## 🥐 Escenario del Negocio y Parámetros Operativos

En cumplimiento con los dígitos asignados (`0 – 1`), el sistema modela los requerimientos operativos de la **Panadería El Trigo de Oro**:

- **Potencia Máxima Contratada:** `9.0 kW` (Regla 6).
- **Umbral de Tarifa Cara:** `> $800 COP/kWh` (Regla 4).
- **Hora Pico Nacional:** `18:00 a 20:00 h` (Regla 3).
- **Alerta Fenómeno del Niño:** Nivel de embalses `< 40.0 %` (Regla 2).
- **Emergencia de Racionamiento:** Precio `> $1500 COP/kWh` **y** embalses `< 40.0 %` (Corte preventivo con sentencia `break`).

### Cargas Eléctricas Modeladas:
1. **Nevera de Insumos (`1.2 kW`):** `CargaCritica`. Almacena levaduras, lácteos y materias primas perecederas. Opera **24 horas continuas** (Regla 1).
2. **Horno Eléctrico Rotativo (`7.0 kW`):** `CargaAplazable 1`. Requiere mínimo **6 horas diarias** de cocción para cumplir las metas de despacho.
3. **Amasadora Industrial (`2.2 kW`):** `CargaAplazable 2`. Requiere mínimo **4 horas diarias** en la madrugada para preparación de lotes de masa.

---

## 🏗️ Arquitectura de Software: Las 5 Relaciones de POO

El sistema implementa de forma explícita las 5 relaciones canónicas de la Programación Orientada a Objetos:

1. **Herencia:** `CargaCritica : public Carga` y `CargaAplazable : public Carga`.
2. **Polimorfismo:** Sobrescritura (`override`) del método virtual puro `virtual bool debeEncender(int hora, float precio, float embalses) = 0;` con resolución dinámica en tiempo de ejecución.
3. **Composición:** El objeto `Medidor medidor;` es un atributo por valor dentro de `Negocio`; nace y muere junto con la instancia de la panadería.
4. **Asociación:** El atributo `Mercado* mercado;` dentro de `Negocio` vincula el mercado eléctrico nacional como una entidad externa mediante puntero.
5. **Agregación:** El arreglo `Carga* cargas[MAX_CARGAS];` almacena punteros a cargas que se instancian en `main()` y se incorporan al negocio con `agregarCarga()`.

---

## 🚀 Compilación y Ejecución

El código fuente está contenido en un único archivo modular `wattcontrol.cpp` sin dependencias externas, compatible con el estándar C++17 o superior.

### En Linux / macOS:
```bash
# Compilación con banderas de optimización y advertencias estrictas
clang++ -std=c++17 -Wall -Wextra wattcontrol.cpp -o wattcontrol

# Ejecución
./wattcontrol
```

### En Windows (MinGW / GCC):
```cmd
g++ -std=c++17 -Wall -Wextra wattcontrol.cpp -o wattcontrol.exe
wattcontrol.exe
```

---

## 📊 Resultados de la Simulación (Día Normal vs. Sin Gestor)

| Métrica | Sin Gestor (Operación Ciega) | Con WattControl (Optimizado) | Impacto / Ahorro |
| :--- | :---: | :---: | :---: |
| **Nevera de Insumos** | 24 h (28.8 kWh) | 24 h (28.8 kWh) | 100 % operativa (Crítica) |
| **Horno Eléctrico** | 24 h (168.0 kWh) | 10 h (70.0 kWh) | Meta diaria $\ge$ 6 h **Cumplida** |
| **Amasadora Industrial** | 24 h (52.8 kWh) | 4 h (8.8 kWh) | Meta diaria $\ge$ 4 h **Cumplida** |
| **Consumo Diario** | **249.6 kWh** | **107.6 kWh** | **- 56,9 % kWh** |
| **Costo Diario** | **$185.754 COP** | **$67.695 COP** | **Ahorro de $118.059 COP/día** |
| **Porcentaje de Ahorro** | Base de comparación | **63,6 % de ahorro** | **$3.541.770 COP / mes** |

---

## 🏷️ Etiqueta de Entrega (Git Tag)

El repositorio cuenta con la etiqueta de versión obligatoria:
```bash
git checkout ea2
```
Esta etiqueta apunta al commit donde se integraron las 4 ramas `feature/...` con sus 12 commits de equipo.
