**Clase de Gestión de Memoria en Sistemas Operativos**

## **Introducción**

La gestión de memoria es una de las tareas más críticas y complejas de un sistema operativo (SO). Su objetivo principal es administrar de manera eficiente el recurso de memoria principal (RAM) para que múltiples procesos puedan ejecutarse de forma concurrente sin interferir entre sí, garantizando protección y optimizando el rendimiento del sistema.

Esta clase explorará los conceptos fundamentales, las técnicas clásicas y las implementaciones modernas de la gestión de memoria.

---

## **1\. Conceptos Fundamentales**

Antes de sumergirnos en los algoritmos, es crucial entender dos conceptos:

* Direcciones Lógicas vs. Físicas:  
  * Dirección Lógica (o Virtual): Es la dirección generada por la CPU. Es la dirección que "ve" un programa. Cada proceso tiene su propio espacio de direcciones lógicas, generalmente comenzando desde 0\.  
  * Dirección Física: Es la dirección real en la unidad de memoria física (RAM).  
* Unidad de Gestión de Memoria (MMU \- Memory Management Unit): Es un componente de hardware que se encarga de traducir las direcciones lógicas a direcciones físicas en tiempo de ejecución. Esta traducción es fundamental para la protección y la flexibilidad de la memoria.

---

## **2\. Mapa de Distribución de Memoria de un Proceso**

Cuando un programa se carga en la memoria, el sistema operativo no lo ve como un bloque monolítico. En su lugar, organiza su espacio de direcciones lógicas en varias secciones bien definidas. Esta es la distribución típica:

     \+------------------------+  \<-- Dirección más alta  
      |      Stack               |  (Variables locales, llamadas a función. Crece hacia abajo)  
      |           |                  |  
      |           v                 |  
      |                              |  
      |           ^                 |  
      |           |                  |  
      |       Heap              |  (Memoria dinámica: malloc, new. Crece hacia arriba)  
      \+-----------------------+  
      |        BSS              |  (Variables globales/estáticas no inicializadas)  
      \+-----------------------+  
      |   Data Segment   |  (Variables globales/estáticas inicializadas)  
      \+-----------------------+  
      |   Text Segment    |  (Código del programa, solo lectura)  
      \+------------------------+  \<-- Dirección más baja (0)

* Text Segment (.text): Contiene las instrucciones del programa compilado. Es de solo lectura para evitar que el proceso modifique su propio código.  
* Data Segment (.data): Almacena variables globales y estáticas que están inicializadas con un valor.  
* BSS Segment (.bss): Almacena variables globales y estáticas no inicializadas. El sistema las inicializa a cero por defecto.  
* Heap: Es la región de memoria para la asignación dinámica. En C, se gestiona con malloc() y free(); en C++, con new y delete. Crece desde las direcciones bajas hacia las altas.  
* Stack: Se utiliza para variables locales, parámetros de funciones y direcciones de retorno. Cada hilo de ejecución tiene su propia pila. Crece desde las direcciones altas hacia las bajas.

---

## **3\. Particionamiento de Memoria (Uso de la División)**

El método más básico para compartir la memoria entre procesos es dividirla en particiones.

### a) Particionamiento Fijo

La memoria se divide en un número fijo de particiones de tamaño igual o diferente. Un proceso se carga en una partición lo suficientemente grande para contenerlo.

* Ventajas: Simple de implementar.  
* Desventajas:  
  * Fragmentación Interna: Si un proceso es más pequeño que su partición, el espacio sobrante dentro de la partición se desperdicia.  
  * El número de procesos activos está limitado por el número de particiones.

### b) Particionamiento Dinámico

Las particiones se crean dinámicamente según las necesidades de los procesos. Cuando un proceso llega, se le busca un "hueco" de memoria lo suficientemente grande.

* Ventajas: No hay fragmentación interna.  
* Desventajas:  
  * Fragmentación Externa: A medida que los procesos entran y salen, la memoria se llena de pequeños huecos no contiguos que son demasiado pequeños para ser útiles, aunque la suma total de espacio libre sea grande.  
  * Requiere algoritmos más complejos para gestionar los huecos.

---

## **4\. Algoritmos Clásicos de Asignación de Memoria**

Estos algoritmos se utilizan en el particionamiento dinámico para decidir qué hueco libre asignar a un nuevo proceso.

Supongamos que tenemos los siguientes huecos de memoria: 100KB, 500KB, 200KB, 300KB, 600KB. Un nuevo proceso necesita 212KB.

### a) First-Fit (Primer Ajuste)

Asigna el primer hueco que sea lo suficientemente grande.

* Proceso: Recorre la lista de huecos y se detiene en el primero que cumpla el requisito.  
* Ejemplo: El proceso de 212KB se asignaría al hueco de 500KB.  
* Ventajas: Es muy rápido.  
* Desventajas: Tiende a dejar huecos pequeños al principio de la lista de memoria.

### b) Best-Fit (Mejor Ajuste)

Asigna el hueco más pequeño que sea lo suficientemente grande.

* Proceso: Debe buscar en toda la lista de huecos para encontrar el que deje el menor remanente.  
* Ejemplo: El proceso de 212KB se asignaría al hueco de 300KB (dejando un remanente de 88KB, el más pequeño posible).  
* Ventajas: Desperdicia la menor cantidad de espacio en la partición elegida.  
* Desventajas: Es más lento porque siempre tiene que buscar en toda la lista. Tiende a crear muchísimos huecos pequeños que son inútiles.

### c) Worst-Fit (Peor Ajuste)

Asigna el hueco más grande disponible.

* Proceso: Debe buscar en toda la lista para encontrar el hueco más grande.  
* Ejemplo: El proceso de 212KB se asignaría al hueco de 600KB.  
* Ventajas: La idea es que el hueco remanente sea lo suficientemente grande como para ser útil para otros procesos.  
* Desventajas: También es lento. Tiende a eliminar rápidamente los huecos grandes, lo que puede ser un problema para procesos grandes que lleguen más tarde.

---

## **5\. Código de Ejemplo en C/C++**

Este código ilustra el uso de la memoria dinámica (Heap) y muestra la ubicación de diferentes tipos de variables.

### Código en C (malloc y free)

```c

#include <stdio.h>
#include <stdlib.h>

int global_initialized_var = 10; // Almacenada en el segmento .data
int global_uninitialized_var;      // Almacenada en el segmento .bss

void function() {
    int stack_var = 30; // Almacenada en el Stack
    printf("Dirección de variable en Stack: %p\n", &stack_var);
}

int main() {
    static int static_var = 20; // Almacenada en .data o .bss dependiendo de inicialización

    // Asignación dinámica en el Heap
    int *heap_var = (int*) malloc(sizeof(int));
    if (heap_var == NULL) {
        fprintf(stderr, "Fallo al asignar memoria en el heap\n");
        return 1;
    }
    *heap_var = 40;

    printf("--- Mapa de Memoria (Direcciones) ---\n");
    printf("Dirección del código (main):   %p\n", main);
    printf("Dirección de var global init:    %p\n", &global_initialized_var);
    printf("Dirección de var estática:     %p\n", &static_var);
    printf("Dirección de var en Heap:        %p\n", heap_var);
    function();
    printf("--------------------------------------\n");

    // Liberar la memoria del Heap
    free(heap_var);
    heap_var = NULL; // Buena práctica para evitar punteros colgantes

    return 0;
}
```

### **Código en C++ (new y delete)**

```c++

#include <iostream>
#include <vector>

class MyObject {
public:
    MyObject() {
        std::cout << "Objeto creado en el Heap." << std::endl;
    }
    ~MyObject() {
        std::cout << "Objeto destruido." << std::endl;
    }
};

int main() {
    // Asignación de un objeto simple en el Heap
    MyObject* obj_ptr = new MyObject();

    // Asignación de un arreglo en el Heap
    int* array_ptr = new int[10]; // Asigna 10 enteros

    std::cout << "Dirección del objeto en el Heap: " << obj_ptr << std::endl;
    std::cout << "Dirección del arreglo en el Heap: " << array_ptr << std::endl;

    // La principal diferencia con C es que new/delete llaman
    // al constructor y destructor del objeto, gestionando su ciclo de vida.
    
    // Liberar la memoria
    delete obj_ptr;       // Para un solo objeto
    delete[] array_ptr;   // Para un arreglo

    obj_ptr = nullptr;
    array_ptr = nullptr;

    return 0;
}
```
---

## **6\. Comparativa de Gestión de Memoria: Linux, Windows y macOS**

Todos los sistemas operativos modernos utilizan técnicas mucho más avanzadas que el simple particionamiento, principalmente la memoria virtual con paginación por demanda.

La memoria virtual permite que cada proceso crea que tiene un enorme espacio de memoria contiguo (ej. 2^64 bytes en un sistema de 64 bits), cuando en realidad la memoria física (RAM) se asigna en pequeños bloques llamados páginas solo cuando se necesitan (paginación por demanda).

| Característica / SO | Linux | Windows | macOS |
| :---- | :---- | :---- | :---- |
| Concepto Clave | Transparencia y eficiencia. Todo es un "archivo". | Gestión agresiva del "Working Set" de cada proceso. | Basado en el micronúcleo Mach, con un enfoque en objetos de memoria. |
| Algoritmos | Buddy System para la asignación de páginas físicas. Slab/Slub/Slob Allocators para estructuras del kernel. | Gestor de Memoria Virtual (VMM) con un complejo algoritmo de reemplazo de páginas y gestión del "working set". | VM Objects del kernel XNU. Utiliza un sistema de paginación por demanda similar a BSD. |
| Paginación | Paginación por demanda. Utiliza una partición o archivo swap. Implementa Copy-on-Write (CoW) para fork(). | Paginación por demanda con "clustering" (trae páginas adyacentes). Usa un archivo pagefile.sys. | Paginación por demanda. Usa archivos swap dinámicos. |
| Funciones Únicas | Overcommitting: Permite asignar más memoria de la disponible, confiando en que los procesos no la usarán toda. OOM Killer (Out-of-Memory Killer): Un proceso que mata a otros procesos para liberar memoria en situaciones críticas. | Memory Compression: Desde Windows 10, comprime páginas de memoria no utilizadas en la RAM en lugar de escribirlas al disco, lo cual es más rápido. SuperFetch/SysMain: Precarga aplicaciones usadas frecuentemente en la memoria. | Compressed Memory: Una de las primeras implementaciones masivas. Comprime páginas inactivas en la RAM para liberar espacio. App Nap: Pone en "siesta" a las aplicaciones que no están visibles, reduciendo su consumo de memoria y CPU. |
| Flexibilidad | Altamente configurable. Se pueden ajustar parámetros como swappiness o las políticas de overcommit. | Menos configurable para el usuario final, las políticas son mayormente automáticas y adaptativas. | Enfoque en la "experiencia de usuario", con una gestión muy automática y poco visible. |

### Resumen Comparativo:

* Linux es conocido por su eficiencia y su política de "overcommit", que puede ser muy potente pero arriesgada (de ahí el OOM Killer). Sus algoritmos internos (Buddy, Slab) son clásicos de la literatura de sistemas operativos.  
* Windows se centra en el rendimiento percibido por el usuario, con técnicas proactivas como SuperFetch y una gestión muy refinada del conjunto de trabajo de cada aplicación para minimizar los fallos de página.  
* macOS, con sus raíces en Mach y BSD, fue pionero en popularizar la compresión de memoria como una alternativa más rápida al swapping. Su gestión está muy orientada a mantener un sistema fluido y responsivo, especialmente en dispositivos con memoria limitada como los portátiles.

En la práctica, los tres sistemas han convergido en sus características principales (paginación por demanda, memoria virtual, compresión de memoria), pero difieren en las políticas y heurísticas que aplican para optimizar el rendimiento.

**¡Excelente adición\! Una parte fundamental de la gestión de memoria es saber cómo observarla en un sistema real. Aquí tienes la sección añadida al final del documento.**

---

## **7\. Herramientas para Monitorear la Memoria en Linux**

Saber cómo se está utilizando la memoria es crucial para diagnosticar problemas de rendimiento, detectar fugas de memoria (memory leaks) y planificar la capacidad del sistema. Linux ofrece una variedad de potentes herramientas de línea de comandos para este propósito.

### **a) free**

Es el comando más simple y directo para obtener una instantánea del uso de la memoria RAM y swap.

Uso:

Bash

free \-h

La opción \-h (human-readable) muestra los valores en formatos más legibles como Kilobytes (K), Megabytes (M) o Gigabytes (G).

Salida de Ejemplo:

Bash

             total        used        free      shared  buff/cache   available  
Mem:          7.7Gi       3.4Gi       1.2Gi       215Mi       3.1Gi       3.9Gi  
Swap:         2.0Gi       512Mi       1.5Gi

Interpretación de las Columnas (Fila Mem):

* total: Memoria RAM total instalada.  
* used: Memoria actualmente en uso. Se calcula como total \- free \- buff/cache.  
* free: Memoria que no está siendo utilizada para absolutamente nada. Este número por sí solo puede ser engañoso.  
* shared: Memoria compartida (principalmente por tmpfs).  
* buff/cache: Memoria utilizada por el kernel para búferes y caché de disco. ¡Esta memoria puede ser liberada instantáneamente si una aplicación la necesita\!  
* available: La métrica más importante. Es una estimación de cuánta memoria está realmente disponible para iniciar nuevas aplicaciones sin necesidad de empezar a usar la swap. Considera la memoria free más la buff/cache que se puede reclamar.

### **b) top y htop**

top es un monitor de sistema interactivo que muestra una lista de los procesos que más recursos consumen, actualizada en tiempo real. htop es una versión mejorada y más amigable de top.

Uso:

Bash

top  
\# O, si está instalado:  
htop

Salida de Ejemplo (top):

Bash

top \- 15:30:45 up 2 days,  4:11,  1 user,  load average: 0.15, 0.20, 0.25  
Tasks: 250 total,   1 running, 249 sleeping,   0 stopped,   0 zombie  
%Cpu(s):  1.5 us,  0.5 sy,  0.0 ni, 98.0 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st  
MiB Mem :   7945.4 total,   1250.2 free,   3550.8 used,   3144.4 buff/cache  
MiB Swap:   2048.0 total,   1536.0 free,    512.0 used.   3950.6 avail Mem 

    PID USER      PR  NI    VIRT    RES    SHR S  %CPU  %MEM     TIME+ COMMAND  
   1234 user      20   0   2.5g   512m    48m S   5.0   6.4   2:35.10 firefox  
   5678 root      20   0   1.2g   128m    32m S   1.2   1.6   0:15.80 Xorg

Columnas de Memoria Clave:

* VIRT (Virtual Memory): Memoria virtual total usada por el proceso. Incluye todo: código, datos, bibliotecas compartidas y memoria que ha sido mapeada en el espacio de direcciones pero no necesariamente está en RAM (puede estar en swap).  
* RES (Resident Memory): Memoria física (RAM) real que está utilizando el proceso. Es una métrica más realista del impacto del proceso en la RAM.  
* SHR (Shared Memory): Memoria compartida con otros procesos (ej. bibliotecas comunes).  
* %MEM: Porcentaje de la memoria física (RES) que está usando el proceso.

### **c) vmstat**

vmstat (Virtual Memory Statistics) reporta información sobre procesos, memoria, paginación, E/S de bloques, traps y actividad de la CPU. Es excelente para ver cómo evoluciona el sistema en el tiempo.

Uso:

Para obtener una actualización cada 2 segundos, 5 veces:

Bash

vmstat 2 5

Salida de Ejemplo:

Bash

procs \-----------memory---------- \---swap-- \-----io---- \-system-- \------cpu-----  
 r  b   swpd   free   buff  cache   si   so    bi    bo   in   cs us sy id wa st  
 1  0 524288 1280200 3219800 3219800    2    5    50    60  100  200  5  2 93  0  0  
 0  0 524288 1280100 3219800 3219800    0    0     0    20  102  205  2  1 97  0  0  
 ...

Columnas de Memoria y Swap Clave:

* swpd: Cantidad de memoria virtual utilizada (en swap).  
* free, buff, cache: Similar al comando free.  
* si (Swap In): Cantidad de memoria leída desde el disco de swap (por segundo). Un valor consistentemente alto indica que el sistema no tiene suficiente RAM.  
* so (Swap Out): Cantidad de memoria escrita al disco de swap (por segundo). Igualmente, un valor alto es señal de falta de RAM.

### **d) /proc/meminfo**

Este no es un comando, sino un archivo virtual que proporciona una gran cantidad de información detallada directamente desde el kernel. De hecho, herramientas como free y top obtienen sus datos de este archivo.

Uso:

Bash

cat /proc/meminfo

Salida de Ejemplo (extracto):

Bash

MemTotal:        8136084 kB  
MemFree:         1279840 kB  
MemAvailable:    4045448 kB  
Buffers:          321980 kB  
Cached:          2898020 kB  
SwapCached:        12340 kB  
Active:          3500000 kB  
Inactive:        2500000 kB  
...  
Slab:             220000 kB  
...

