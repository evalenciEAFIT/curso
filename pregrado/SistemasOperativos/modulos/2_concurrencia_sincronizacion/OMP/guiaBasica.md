# Guía Paso a Paso de las Directivas de OpenMP en C y C++

OpenMP (Open Multi-Processing) es una API para programación paralela en sistemas de memoria compartida. Permite paralelizar código en C y C++ (y Fortran) utilizando directivas de preprocesador que se insertan en el código secuencial. Estas directivas comienzan con `#pragma omp` y facilitan la creación de hilos (threads) para ejecutar tareas en paralelo, mejorando el rendimiento en procesadores multicore.

Esta guía está estructurada de manera paso a paso: 
1. **Introducción y configuración inicial**.
2. **Explicación de las directivas principales**, con su sintaxis, propósito, cláusulas comunes y ejemplos en C/C++.
3. **Consejos avanzados y consideraciones**.

Los ejemplos son código fuente que puedes compilar y ejecutar. Para compilar con OpenMP:
- En GCC: `gcc -fopenmp archivo.c -o ejecutable` (para C) o `g++ -fopenmp archivo.cpp -o ejecutable` (para C++).
- Asegúrate de tener un compilador que soporte OpenMP (como GCC 4.2+ o Clang).

Nota: La sintaxis es idéntica en C y C++, salvo diferencias menores en el código base (e.g., uso de `std::cout` en C++). Usaré C para la mayoría de ejemplos, pero se aplican igual a C++.

## Paso 1: Configuración y Conceptos Básicos
- **Instalación**: OpenMP viene integrado en compiladores como GCC, Clang o MSVC. No necesitas instalar bibliotecas adicionales.
- **Inclusión**: Incluye `<omp.h>` para funciones como `omp_get_thread_num()`.
- **Conceptos clave**:
  - **Región paralela**: Bloque de código ejecutado por múltiples hilos.
  - **Cláusulas**: Modificadores como `private`, `shared` para controlar variables.
  - **Variables**: Por defecto, las variables globales son compartidas; locales son privadas.
- **Ejecución**: OpenMP usa un modelo fork-join: un hilo maestro crea hilos hijos en regiones paralelas y los une al final.

Ejemplo básico para probar OpenMP (hello world paralelo):
```c
#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel
    {
        printf("Hola desde hilo %d\n", omp_get_thread_num());
    }
    return 0;
}
```
- Compila: `gcc -fopenmp hello.c -o hello`.
- Explicación: Crea hilos (número por defecto del sistema) y cada uno imprime un mensaje. Salida típica: múltiples "Hola desde hilo X".

En C++ sería similar, usando `std::cout` en lugar de `printf`.

## Paso 2: Directivas Principales de OpenMP
Explicaremos las directivas más comunes, agrupadas por categoría. Cada una incluye:
- **Sintaxis**.
- **Explicación**.
- **Cláusulas comunes**.
- **Ejemplo**.

### 2.1 Directivas para Regiones Paralelas
#### `#pragma omp parallel`
- **Sintaxis**: `#pragma omp parallel [cláusulas] { código }`
- **Explicación**: Crea una región paralela donde el código se ejecuta en múltiples hilos. El hilo maestro (ID 0) coordina.
- **Cláusulas comunes**:
  - `num_threads(N)`: Especifica el número de hilos.
  - `private(var)`: Cada hilo tiene su copia privada de `var`.
  - `shared(var)`: `var` es compartida entre hilos.
  - `if(condición)`: Ejecuta en paralelo solo si la condición es verdadera.

Ejemplo (suma paralela simple):
```c
#include <stdio.h>
#include <omp.h>

int main() {
    int suma = 0;
    #pragma omp parallel num_threads(4) reduction(+:suma)
    {
        int id = omp_get_thread_num();
        suma += id;  // Cada hilo suma su ID
    }
    printf("Suma total: %d\n", suma);  // Ej: 0+1+2+3=6
    return 0;
}
```
- Explicación: Sin `reduction`, habría race conditions (condiciones de carrera). `reduction(+:suma)` acumula la suma de forma segura.

#### `#pragma omp for`
- **Sintaxis**: `#pragma omp for [cláusulas] for (init; cond; incr) { código }` (Debe estar dentro de una región paralela).
- **Explicación**: Divide un bucle `for` entre hilos para paralelizar iteraciones independientes.
- **Cláusulas comunes**:
  - `schedule(static|dynamic|guided|auto)`: Cómo distribuir iteraciones (static: bloques fijos; dynamic: dinámico).
  - `collapse(N)`: Combina N bucles anidados.
  - `reduction(op:var)`: Para operaciones como suma, max, etc.

Ejemplo (paralelizar un bucle de suma):
```c
#include <stdio.h>
#include <omp.h>

int main() {
    int n = 100;
    int suma = 0;
    #pragma omp parallel reduction(+:suma)
    {
        #pragma omp for
        for (int i = 1; i <= n; i++) {
            suma += i;
        }
    }
    printf("Suma de 1 a %d: %d\n", n, suma);  // 5050
    return 0;
}
```
- Explicación: El bucle se divide (e.g., hilo 0: 1-25, hilo 1: 26-50, etc.). `reduction` une los resultados.

### 2.2 Directivas para Secciones y Tareas
#### `#pragma omp sections`
- **Sintaxis**: `#pragma omp sections [cláusulas] { #pragma omp section {código} ... }` (Dentro de región paralela).
- **Explicación**: Divide el código en secciones independientes, asignadas a hilos diferentes.
- **Cláusulas comunes**: `private`, `reduction`.

Ejemplo (dos secciones paralelas):
```c
#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel num_threads(2)
    {
        #pragma omp sections
        {
            #pragma omp section
            printf("Sección 1 por hilo %d\n", omp_get_thread_num());
            #pragma omp section
            printf("Sección 2 por hilo %d\n", omp_get_thread_num());
        }
    }
    return 0;
}
```
- Explicación: Cada sección se ejecuta en un hilo diferente, ideal para tareas no relacionadas.

#### `#pragma omp task`
- **Sintaxis**: `#pragma omp task [cláusulas] { código }` (Dentro de región paralela).
- **Explicación**: Crea tareas asíncronas que se ejecutan en hilos disponibles, útil para paralelismo irregular (e.g., recursión).
- **Cláusulas comunes**: `depend(in:out:var)` para dependencias.

Ejemplo (tareas en Fibonacci recursivo):
```c
#include <stdio.h>
#include <omp.h>

int fib(int n) {
    if (n < 2) return n;
    int x, y;
    #pragma omp task shared(x)
    x = fib(n-1);
    #pragma omp task shared(y)
    y = fib(n-2);
    #pragma omp taskwait  // Espera a que terminen las tareas
    return x + y;
}

int main() {
    #pragma omp parallel
    {
        #pragma omp single  // Solo un hilo inicia las tareas
        printf("Fib(6): %d\n", fib(6));  // 8
    }
    return 0;
}
```
- Explicación: Las llamadas recursivas se convierten en tareas paralelas.

### 2.3 Directivas de Sincronización
#### `#pragma omp barrier`
- **Sintaxis**: `#pragma omp barrier`
- **Explicación**: Punto de sincronización: todos los hilos esperan hasta que todos lleguen.

Ejemplo: Dentro de una región paralela, después de una fase de cómputo.

#### `#pragma omp critical`
- **Sintaxis**: `#pragma omp critical [(nombre)] { código }`
- **Explicación**: Sección crítica: solo un hilo ejecuta a la vez, evita race conditions.

Ejemplo (actualizar variable compartida):
```c
int contador = 0;
#pragma omp parallel
{
    #pragma omp for
    for (int i = 0; i < 100; i++) {
        #pragma omp critical
        contador++;  // Seguro, uno a la vez
    }
}
```

#### `#pragma omp atomic`
- **Sintaxis**: `#pragma omp atomic [read|write|update|capture] { operación }`
- **Explicación**: Operación atómica: actualiza una variable de forma indivisible, más eficiente que critical para operaciones simples.

Ejemplo:
```c
int suma = 0;
#pragma omp parallel
{
    #pragma omp for
    for (int i = 1; i <= 100; i++) {
        #pragma omp atomic
        suma += i;
    }
}
```

#### `#pragma omp single` y `#pragma omp master`
- **Sintaxis**: `#pragma omp single { código }` o `#pragma omp master { código }`
- **Explicación**: `single`: Solo un hilo ejecuta (cualquiera). `master`: Solo el hilo maestro (ID 0).

Ejemplo: Usado para inicializaciones únicas en regiones paralelas.

### 2.4 Otras Directivas Avanzadas
- `#pragma omp simd`: Para vectorización (SIMD) en bucles.
- `#pragma omp target`: Para offloading a aceleradores (e.g., GPU).
- `#pragma omp teams`: Para grupos de hilos en entornos distribuidos.

## Paso 3: Cláusulas Comunes en Detalle
- **private(var)**: Cada hilo tiene su propia copia (inicializada indefinida).
- **firstprivate(var)**: Como private, pero inicializada con valor del hilo maestro.
- **shared(var)**: Compartida, riesgo de race conditions.
- **reduction(op:var)**: Operadores: +, *, -, max, min, etc. Acumula resultados.
- **nowait**: Elimina barrera implícita al final de directivas como `for`.

## Paso 4: Consejos y Consideraciones
- **Debugging**: Usa `omp_get_num_threads()` para verificar hilos. Evita race conditions con critical/atomic/reduction.
- **Rendimiento**: Mide con `omp_get_wtime()`. No paralelices bucles pequeños (overhead).
- **Errores comunes**: Olvidar incluir `<omp.h>`, no compilar con `-fopenmp`, o ignorar dependencias de datos.
- **Versiones**: OpenMP 5.0+ añade más features como dependencias en tasks.
- **En C++**: Usa con clases, pero ten cuidado con constructores/destructores en regiones paralelas.
