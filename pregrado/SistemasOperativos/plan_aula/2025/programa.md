### **Plan de Estudio de Sistemas Operativos**  (16 Semanas)

### 

### **Objetivo General:**

Al finalizar el curso, el estudiante comprenderá los conceptos fundamentales de la gestión de procesos, concurrencia, manejo de memoria y sistemas de archivos en un sistema operativo moderno. Además, tendrá las bases de programación en C/C++ y una introducción a Assembler que le permitirán entender la interacción entre el software de bajo nivel y el hardware.

### 

### **Distribución de Evaluaciones:**

* ### **Parcial 1 (25%): Semana 5**
  (11-ago al 16-ago)

* ### **Parcial 2 (25%): Semana 10**
  (15-sep al 20-sep)

* ### **Parcial 3 (25%): Semana 15**
  (27-oct al 1-nov)

* ### **Trabajo Final (25%): Entre semana Semana 16 y la Semana 19**
  (3-nov al 29-nov)


### **Sugerencias para el Trabajo Final (25%)**

El trabajo debe ser práctico y permitir a los estudiantes aplicar varios de los conceptos vistos. Se asigna en la semana 7 para dar tiempo suficiente.

* **Opción 1: Simulador de Planificador de CPU.**  
  Implementar en C/C++ un programa que simule la ejecución de procesos bajo diferentes algoritmos de planificación (ej. FCFS, SJF, Round Robin). Debe manejar estados de procesos y generar estadísticas de rendimiento (tiempo de espera, tiempo de retorno).  
* **Opción 2: Implementación de un Problema de Sincronización.**  
  Resolver un problema clásico complejo como "Los Filósofos Cenando" o "El Barbero Durmiente" utilizando hilos, mutex y semáforos, garantizando que no haya deadlocks ni inanición.  
* **Opción 3: Pequeña Shell o Intérprete de Comandos.**  
  Crear una shell simple en C que pueda leer comandos del usuario, interpretarlos y ejecutarlos en procesos separados usando `fork()` y `exec()`. Debe manejar comandos simples con argumentos y la ejecución en segundo plano (`&`).

---

### **Módulo 1: Fundamentos y Nivelación (Semanas 1-4)**
Este módulo establece las bases necesarias en programación y arquitectura para poder abordar los temas complejos del curso.

| Semana | Tema Principal | Contenidos y Actividades | Hito / Evaluación |
| :---- | :---- | :---- | :---- |
| 1 | Introducción y Nivelación en C/C++ (Parte 1\) | Presentación del curso, sílabo y sistema de evaluación. Conceptos básicos de un Sistema Operativo. Repaso de C/C++: compilación (`gcc`/`g++`), variables, tipos de datos, control de flujo. Práctica: Escribir, compilar y ejecutar programas sencillos. |  |
| 2 | Nivelación en C/C++ (Parte 2\) | Punteros y Aritmética de Punteros: El concepto más crucial para SO. Gestión de Memoria Dinámica: `malloc`, `calloc`, `realloc`, `free`. Estructuras de Datos (`structs`): Agrupación de datos relacionados. |  |
| 3 | Introducción a Assembler y Arquitectura | Modelo básico de un procesador (CPU, registros, memoria). Concepto de set de instrucciones (ISA). Assembler (x86 básico): Instrucciones `MOV`, `ADD`, `SUB`, `JMP`. Registros principales (`EAX`, `EBX`, etc.). Relación entre código C y el Assembler resultante. |  |
| 4 | Procesos y Llamadas al Sistema | Concepto de Proceso y su ciclo de vida (creación, ejecución, bloqueo, terminación). Bloque de Control de Proceso (PCB). Llamadas al Sistema (System Calls): El puente entre el usuario y el kernel. Ejemplos en POSIX (`fork`, `exec`, `wait`, `exit`). Práctica: Crear un programa en C que utilice `fork()` para crear un nuevo proceso. | Repaso para Parcial 1 |

### 

### **Módulo 2: Concurrencia y Sincronización (Semanas 5-9)**
Este es el corazón de la concurrencia, uno de los temas más desafiantes y fundamentales.

| Semana | Tema Principal | Contenidos y Actividades | Hito / Evaluación |
| :---- | :---- | :---- | :---- |
| 5 | Examen y Fundamentos de Concurrencia | Evaluación Parcial 1 (25%): Cubre temas de las semanas 1-4. Introducción a la concurrencia: hilos (threads) vs. procesos. Condiciones de Carrera (Race Conditions) y la necesidad de sincronización. | Parcial 1 |
| 6 | Sincronización: Secciones Críticas y Mutex | El problema de la Sección Crítica. Soluciones de software (Algoritmo de Peterson). Mutex (Locks): Mecanismo de exclusión mutua para proteger secciones críticas. Práctica: Programa en C con hilos (pthreads) que muestre una condición de carrera y cómo solucionarla con un mutex. |  |
| 7 | Sincronización Avanzada: Semáforos | Semáforos: Contadores para controlar el acceso a recursos. Problemas clásicos de sincronización: Productor-Consumidor, Lectores-Escritores. Práctica: Implementar una solución al problema Productor-Consumidor usando semáforos. | Asignación del Trabajo Final |
| 8 | Interbloqueo (Deadlocks) | Concepto de interbloqueo y sus 4 condiciones necesarias. Estrategias para manejar deadlocks: prevención, evitación (Algoritmo del Banquero), detección y recuperación. inanición (starvation). |  |
| 9 | Repaso de Concurrencia | Sesión de repaso de todos los mecanismos de sincronización (mutex, semáforos). Discusión de casos de uso y comparación de herramientas. Asesoría y seguimiento del trabajo final. | Repaso para Parcial 2 |

### 

### **Módulo 3: Gestión de Memoria y Almacenamiento (Semanas 10-14)**
Este módulo cubre cómo el SO gestiona el recurso de la memoria y el almacenamiento persistente.

| Semana | Tema Principal | Contenidos y Actividades | Hito / Evaluación |
| :---- | :---- | :---- | :---- |
| 10 | Examen y Gestión de Memoria Básica | Evaluación Parcial 2 (25%): Cubre temas de concurrencia (semanas 5-9). Abstracción de memoria: Espacios de direcciones. Asignación contigua, fragmentación (interna y externa). | Parcial 2 |
| 11 | Memoria Virtual: Paginación | Concepto de paginación: Páginas, marcos de página (frames). Tablas de páginas. Translation Lookaside Buffer (TLB): Acelerando la traducción de direcciones. |  |
| 12 | Memoria Virtual: Segmentación y Paginación por Demanda | Segmentación y combinación con paginación. Paginación por demanda (Demand Paging): Cargar páginas sólo cuando se necesitan. Manejo de fallos de página (Page Faults). Algoritmos de reemplazo de páginas (FIFO, LRU, Óptimo). | Avance/Revisión del Trabajo Final |
| 13 | Sistemas de Archivos (File Systems) | Concepto de archivo, atributos, operaciones. Estructura de directorios (árbol, grafo acíclico). Métodos de acceso a archivos (secuencial, directo). |  |
| 14 | Implementación de Sistemas de Archivos y E/S | Métodos de asignación de espacio en disco (contiguo, enlazado, indexado). Gestión del espacio libre. Fundamentos de E/S (I/O): Polling, interrupciones, DMA. |  |

### 

### **Módulo 4: Cierre y Evaluación Final (Semanas 15-16)**

| Semana | Tema Principal | Contenidos y Actividades | Hito / Evaluación |
| :---- | :---- | :---- | :---- |
| 15 | Examen Parcial y Repaso General | Evaluación Parcial 3 (25%): Cubre gestión de memoria y sistemas de archivos (semanas 10-14). Repaso integrador del curso: cómo se relacionan los procesos, la memoria y los archivos. | Parcial 3 |
| 16 \- 19 | Presentación del Trabajo Final | Entrega y/o presentación de los trabajos finales. Discusión de los resultados y lecciones aprendidas. Cierre del curso. | Entrega del Trabajo Final (25%) |

