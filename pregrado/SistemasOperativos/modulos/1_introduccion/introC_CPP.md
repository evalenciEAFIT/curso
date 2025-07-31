# Guía Completa de C y C++ con GCC/G++

## 📋 Descripción General

Esta guía completa cubre el uso de C y C++ con sus respectivos compiladores GCC y G++, explicando apuntadores, estructuras de datos, colecciones, programación orientada a objetos y gestión de memoria dinámica.

## 🎯 Contenidos

1. **Compilación con GCC y G++**
2. **Apuntadores en C y C++**
3. **Estructuras (struct) en C**
4. **Clases y Objetos en C++**
5. **Colecciones y Contenedores**
6. **Gestión de Memoria Dinámica**
7. **Ejemplos Prácticos**

---

## 🛠️ 1. Compilación con GCC y G++

### **GCC - GNU Compiler Collection para C**

```bash
# COMPILACIÓN BÁSICA
# gcc archivo.c -o programa
# - gcc: compilador para C
# - archivo.c: archivo fuente en C
# - -o programa: nombre del ejecutable de salida

# COMPILACIÓN CON WARNINGS
# gcc -Wall -Wextra archivo.c -o programa
# - -Wall: mostrar todos los warnings importantes
# - -Wextra: mostrar warnings adicionales

# COMPILACIÓN CON OPTIMIZACIÓN
# gcc -O2 archivo.c -o programa
# - -O2: nivel de optimización moderado (balance entre velocidad y tamaño)

# COMPILACIÓN CON DEBUGGING
# gcc -g archivo.c -o programa
# - -g: incluir información de debugging para usar con gdb

# COMPILACIÓN MÚLTIPLES ARCHIVOS
# gcc archivo1.c archivo2.c -o programa
# - Compila varios archivos fuente en un solo ejecutable

# COMPILACIÓN CON BIBLIOTECAS EXTERNAS
# gcc archivo.c -lm -o programa
# - -lm: enlazar con la biblioteca matemática (libm)
```

### **G++ - GNU Compiler Collection para C++**

```bash
# COMPILACIÓN BÁSICA
# g++ archivo.cpp -o programa
# - g++: compilador para C++
# - archivo.cpp: archivo fuente en C++
# - -o programa: nombre del ejecutable de salida

# COMPILACIÓN CON ESTÁNDARES MODERNOS
# g++ -std=c++17 archivo.cpp -o programa
# - -std=c++17: usar el estándar C++17
# g++ -std=c++20 archivo.cpp -o programa
# - -std=c++20: usar el estándar C++20

# COMPILACIÓN CON WARNINGS Y DEBUGGING
# g++ -Wall -Wextra -g archivo.cpp -o programa
# - Combinación de flags para desarrollo seguro

# COMPILACIÓN SEPARADA (COMPILACIÓN Y ENLAZADO)
# g++ -c archivo1.cpp          # Genera archivo1.o
# - -c: solo compilar (no enlazar), genera archivo objeto
# g++ -c archivo2.cpp          # Genera archivo2.o
# g++ archivo1.o archivo2.o -o programa  # Enlaza objetos
# - Enlazar archivos objeto para crear ejecutable final
```

### **Opciones Comunes de Compilación**

| Opción | Descripción |
|--------|-------------|
| `-Wall` | Mostrar todos los warnings importantes para detectar posibles errores |
| `-Wextra` | Warnings adicionales que pueden ayudar a mejorar la calidad del código |
| `-g` | Información de debugging necesaria para usar depuradores como gdb |
| `-O0` | Sin optimización (por defecto) - útil para debugging |
| `-O1`, `-O2`, `-O3` | Niveles de optimización progresivamente más agresivos |
| `-std=c99/c11` | Estándar de C a usar (C99 o C11) |
| `-std=c++11/c++17/c++20` | Estándar de C++ a usar |
| `-c` | Solo compilación (sin enlazar) - genera archivos objeto |
| `-I` | Directorio de includes para buscar archivos de cabecera |
| `-L` | Directorio de bibliotecas para buscar bibliotecas enlazables |
| `-l` | Biblioteca a enlazar (ej: -lm para math library) |

---

## 🔗 2. Apuntadores en C y C++

### **¿Qué son los apuntadores?**

Los apuntadores son variables que almacenan direcciones de memoria en lugar de valores directos. Son fundamentales para la gestión dinámica de memoria y el acceso eficiente a datos.

```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    // DECLARACIÓN DE APUNTADORES
    // int numero = 42;            // Variable entera normal
    // int *ptr_numero;            // Apuntador a entero (almacena dirección de memoria)
    // int **ptr_doble;            // Apuntador a apuntador (apuntador a dirección de apuntador)
    
    int numero = 42;            // Variable entera con valor 42
    int *ptr_numero;            // Declaramos apuntador a entero
    int **ptr_doble;            // Declaramos apuntador a apuntador
    
    // ASIGNACIÓN DE DIRECCIONES
    // &numero: operador de dirección (obtiene la dirección de memoria de numero)
    // ptr_numero = &numero: asignamos la dirección de numero al apuntador
    // ptr_doble = &ptr_numero: asignamos dirección de ptr_numero a ptr_doble
    
    ptr_numero = &numero;       // ptr_numero ahora contiene la dirección de numero
    ptr_doble = &ptr_numero;    // ptr_doble contiene dirección de ptr_numero
    
    // ACCESO A VALORES
    // *ptr_numero: operador de desreferencia (obtiene valor en la dirección apuntada)
    // **ptr_doble: doble desreferencia (valor apuntado por el apuntador apuntado)
    
    printf("Valor de numero: %d\n", numero);           // 42 - valor directo
    printf("Dirección de numero: %p\n", &numero);      // Dirección en memoria hexadecimal
    printf("Valor de ptr_numero: %p\n", ptr_numero);   // Misma dirección que &numero
    printf("Valor apuntado: %d\n", *ptr_numero);       // 42 - valor en dirección apuntada
    printf("Valor doble apuntado: %d\n", **ptr_doble); // 42 - desreferencia doble
    
    return 0;
}
```

### **Operaciones con Apuntadores**

```c++
#include <iostream>

void ejemplo_apuntadores() {
    // ARREGLO DE ENTEROS PARA DEMOSTRAR ARITMÉTICA DE APUNTADORES
    int arreglo[] = {10, 20, 30, 40, 50};
    int *ptr = arreglo;  // Apuntador al primer elemento (equivalente a &arreglo[0])
    
    std::cout << "Elementos del arreglo usando apuntadores:\n";
    
    // ARITMÉTICA DE APUNTADORES
    // ptr + i: avanza i posiciones desde la dirección apuntada
    // *(ptr + i): desreferencia la dirección en la posición i
    for(int i = 0; i < 5; i++) {
        // Mostramos el valor y la dirección de cada elemento
        std::cout << "Elemento " << i << ": " << *(ptr + i) 
                  << " (dirección: " << (ptr + i) << ")\n";
    }
    
    // MODIFICACIÓN USANDO APUNTADORES
    // *ptr = 100: modifica el valor en la dirección apuntada por ptr
    // *(ptr + 2) = 300: modifica el valor en la dirección ptr + 2 (tercer elemento)
    
    *ptr = 100;          // Modifica arreglo[0] (primer elemento)
    *(ptr + 2) = 300;    // Modifica arreglo[2] (tercer elemento)
    
    std::cout << "\nArreglo modificado: ";
    for(int i = 0; i < 5; i++) {
        std::cout << arreglo[i] << " ";  // Imprimir arreglo modificado
    }
    std::cout << "\n";
}

// APUNTADORES A FUNCIONES
// Las funciones también tienen direcciones en memoria
int sumar(int a, int b) { return a + b; }
int multiplicar(int a, int b) { return a * b; }

void ejemplo_funciones_apuntadores() {
    // DECLARACIÓN DE APUNTADOR A FUNCIÓN
    // int (*operacion)(int, int): apuntador a función que recibe dos int y retorna int
    int (*operacion)(int, int);
    
    // ASIGNAR FUNCIONES A APUNTADORES
    // operacion = sumar: asignamos dirección de la función sumar
    operacion = sumar;
    std::cout << "Suma: " << operacion(5, 3) << "\n";  // 8 - llamamos función a través de apuntador
    
    operacion = multiplicar;
    std::cout << "Multiplicación: " << operacion(5, 3) << "\n";  // 15 - llamamos otra función
}

int main() {
    ejemplo_apuntadores();
    ejemplo_funciones_apuntadores();
    return 0;
}

```
Reto [**reto.md**](./reto.md) 

---

## 💾 3. Gestión de Memoria Dinámica

### **Malloc, Calloc y Free en C**

```c++
#include <iostream>  // QUÉ: Incluye la biblioteca de entrada/salida para operaciones de consola
                     // POR QUÉ: Necesaria para usar std::cout y mostrar texto en pantalla
                     // CÓMO: Proporciona funciones para impresión formateada
#include <cstdlib>   // QUÉ: Incluye la biblioteca estándar para gestión de memoria
                     // POR QUÉ: Proporciona std::nothrow para manejar fallos de asignación
                     // CÓMO: Permite usar new/delete con manejo seguro de errores
#include <cstring>   // QUÉ: Incluye la biblioteca para manipulación de cadenas estilo C
                     // POR QUÉ: Necesaria para funciones como std::strcpy y std::strcat
                     // CÓMO: Facilita operaciones con cadenas de caracteres

// QUÉ: Función para demostrar la gestión de memoria dinámica en C++
// POR QUÉ: Mostrar cómo asignar, modificar y liberar memoria dinámicamente
// CÓMO: Usa new/delete para enteros y arreglos, con verificación de errores
// HACER: Ejecutar para entender la asignación dinámica y evitar memory leaks
void ejemplo_memoria_dinamica_cpp() {
    // QUÉ: Imprimir encabezado de la sección
    // POR QUÉ: Indicar claramente el inicio de la demostración
    // CÓMO: Usa std::cout para mostrar texto formateado
    // HACER: Asegurar que el usuario identifique la sección
    std::cout << "=== GESTIÓN DE MEMORIA DINÁMICA EN C++ ===\n\n";
    
    // QUÉ: Asignar memoria para un solo entero con new
    // POR QUÉ: Demostrar asignación dinámica de una variable
    // CÓMO: Usa new con std::nothrow para evitar excepciones en fallo
    // HACER: Verificar siempre si la asignación fue exitosa
    std::cout << "1. USO DE new:\n";
    int* ptr_entero = new (std::nothrow) int;
    if (ptr_entero == nullptr) {
        // QUÉ: Manejar fallo de asignación
        // POR QUÉ: Evitar dereferenciar un puntero nulo
        // CÓMO: Imprime mensaje de error y retorna
        // HACER: Salir de la función para evitar errores
        std::cout << "Error: No se pudo asignar memoria\n";
        return;
    }
    // QUÉ: Asignar un valor al espacio reservado
    // POR QUÉ: Mostrar cómo usar memoria dinámica
    // CÓMO: Desreferencia el puntero para asignar un valor
    // HACER: Usar el puntero para almacenar datos
    *ptr_entero = 100;
    std::cout << "Valor asignado con new: " << *ptr_entero << "\n";
    
    // QUÉ: Asignar e inicializar un arreglo de enteros
    // POR QUÉ: Demostrar asignación de arreglos con inicialización a cero
    // CÓMO: Usa new[] con inicialización de llaves para valores en cero
    // HACER: Verificar que el arreglo se inicialice correctamente
    std::cout << "\n2. USO DE new[]:\n";
    int tamano = 5;
    int* ptr_arreglo = new (std::nothrow) int[tamano]{};
    if (ptr_arreglo == nullptr) {
        // QUÉ: Manejar fallo de asignación del arreglo
        // POR QUÉ: Evitar memory leaks y errores
        // CÓMO: Libera memoria previa y retorna
        // HACER: Asegurar limpieza antes de salir
        std::cout << "Error: No se pudo asignar memoria\n";
        delete ptr_entero;
        return;
    }
    
    // QUÉ: Imprimir valores iniciales del arreglo
    // POR QUÉ: Verificar que new[] inicializó a cero
    // CÓMO: Recorre el arreglo e imprime cada elemento
    // HACER: Confirmar que todos los valores son cero
    std::cout << "Arreglo inicializado con new[]: ";
    for (int i = 0; i < tamano; i++) {
        std::cout << ptr_arreglo[i] << " ";
    }
    std::cout << "\n";
    
    // QUÉ: Modificar valores del arreglo
    // POR QUÉ: Mostrar cómo actualizar un arreglo dinámico
    // CÓMO: Asigna valores (10, 20, 30, 40, 50) en un bucle
    // HACER: Usar índices para modificar elementos
    for (int i = 0; i < tamano; i++) {
        ptr_arreglo[i] = (i + 1) * 10;
    }
    // QUÉ: Imprimir arreglo modificado
    // POR QUÉ: Mostrar los nuevos valores
    // CÓMO: Recorre el arreglo e imprime cada elemento
    // HACER: Verificar que los valores se actualizaron
    std::cout << "Arreglo modificado: ";
    for (int i = 0; i < tamano; i++) {
        std::cout << ptr_arreglo[i] << " ";
    }
    std::cout << "\n";
    
    // QUÉ: Reasignar memoria para un arreglo más grande
    // POR QUÉ: Demostrar cómo expandir un arreglo dinámico
    // CÓMO: Crea un nuevo arreglo, copia datos y libera el viejo
    // HACER: Manejar cuidadosamente la copia y liberación
    std::cout << "\n3. USO DE REASIGNACIÓN:\n";
    int nuevo_tamano = 10;
    int* temp_arreglo = new (std::nothrow) int[nuevo_tamano]{};
    if (temp_arreglo == nullptr) {
        // QUÉ: Manejar fallo de reasignación
        // POR QUÉ: Evitar memory leaks y errores
        // CÓMO: Libera memoria existente y retorna
        // HACER: Asegurar limpieza antes de salir
        std::cout << "Error: No se pudo reasignar memoria\n";
        delete ptr_entero;
        delete[] ptr_arreglo;
        return;
    }
    
    // QUÉ: Copiar datos existentes al nuevo arreglo
    // POR QUÉ: Preservar los valores originales
    // CÓMO: Copia elemento por elemento del arreglo viejo
    // HACER: Asegurar que todos los datos se transfieran
    for (int i = 0; i < tamano; i++) {
        temp_arreglo[i] = ptr_arreglo[i];
    }
    // QUÉ: Inicializar nuevos elementos
    // POR QUÉ: Asignar valores significativos a la nueva memoria
    // CÓMO: Asigna valores (60, 70, 80, 90, 100) a elementos nuevos
    // HACER: Completar el arreglo expandido
    for (int i = tamano; i < nuevo_tamano; i++) {
        temp_arreglo[i] = (i + 1) * 10;
    }
    // QUÉ: Liberar arreglo viejo y actualizar puntero
    // POR QUÉ: Evitar memory leaks y usar nueva memoria
    // CÓMO: Usa delete[] y asigna el nuevo arreglo al puntero
    // HACER: Asegurar que el puntero apunte a la nueva memoria
    delete[] ptr_arreglo;
    ptr_arreglo = temp_arreglo;
    tamano = nuevo_tamano;
    
    // QUÉ: Imprimir arreglo expandido
    // POR QUÉ: Verificar reasignación y nuevos valores
    // CÓMO: Recorre el arreglo e imprime cada elemento
    // HACER: Confirmar que la expansión fue exitosa
    std::cout << "Arreglo expandido: ";
    for (int i = 0; i < tamano; i++) {
        std::cout << ptr_arreglo[i] << " ";
    }
    std::cout << "\n";
    
    // QUÉ: Liberar toda la memoria asignada
    // POR QUÉ: Evitar memory leaks
    // CÓMO: Usa delete para el entero y delete[] para el arreglo
    // HACER: Establecer punteros a nullptr tras liberar
    std::cout << "\n4. LIBERANDO MEMORIA:\n";
    delete ptr_entero;
    delete[] ptr_arreglo;
    ptr_entero = nullptr;
    ptr_arreglo = nullptr;
    std::cout << "Memoria liberada correctamente\n";
}

// QUÉ: Función para demostrar manejo de cadenas dinámicas estilo C
// POR QUÉ: Mostrar gestión de memoria con cadenas
// CÓMO: Usa new/delete con funciones de cadenas estilo C
// HACER: Ejecutar para entender manipulación de cadenas dinámicas
void ejemplo_strings_dinamicos() {
    // QUÉ: Imprimir encabezado de la sección
    // POR QUÉ: Indicar inicio de la demostración de cadenas
    // CÓMO: Usa std::cout para mostrar texto
    // HACER: Asegurar claridad en la salida
    std::cout << "\n=== STRINGS DINÁMICOS ===\n";
    
    // QUÉ: Asignar memoria para una cadena estilo C
    // POR QUÉ: Almacenar una cadena dinámica
    // CÓMO: Usa new para un arreglo de 50 caracteres
    // HACER: Verificar si la asignación fue exitosa
    char* nombre = new (std::nothrow) char[50];
    if (nombre == nullptr) {
        // QUÉ: Manejar fallo de asignación
        // POR QUÉ: Evitar errores de puntero nulo
        // CÓMO: Imprime error y retorna
        // HACER: Salir para evitar problemas
        std::cout << "Error de memoria\n";
        return;
    }
    
    // QUÉ: Copiar una cadena a la memoria asignada
    // POR QUÉ: Inicializar la cadena dinámica
    // CÓMO: Usa std::strcpy para copiar "Juan Pérez"
    // HACER: Asegurar que la copia sea correcta
    std::strcpy(nombre, "Juan Pérez");
    std::cout << "Nombre: " << nombre << "\n";
    
    // QUÉ: Reasignar memoria para una cadena más larga
    // POR QUÉ: Permitir almacenar una cadena más extensa
    // CÓMO: Crea nuevo arreglo, copia y concatena
    // HACER: Verificar fallo de asignación
    char* temp_nombre = new (std::nothrow) char[100];
    if (temp_nombre == nullptr) {
        // QUÉ: Manejar fallo de reasignación
        // POR QUÉ: Evitar memory leaks
        // CÓMO: Libera memoria original y retorna
        // HACER: Asegurar limpieza antes de salir
        std::cout << "Error de realloc\n";
        delete[] nombre;
        return;
    }
    
    // QUÉ: Copiar y concatenar cadenas
    // POR QUÉ: Actualizar la cadena con texto adicional
    // CÓMO: Usa std::strcpy y std::strcat
    // HACER: Asegurar que la concatenación sea correcta
    std::strcpy(temp_nombre, nombre);
    std::strcat(temp_nombre, " - Desarrollador Senior");
    // QUÉ: Liberar cadena vieja y actualizar puntero
    // POR QUÉ: Evitar memory leaks y usar nueva memoria
    // CÓMO: Usa delete[] y asigna nuevo arreglo
    // HACER: Asegurar que el puntero apunte a la nueva memoria
    delete[] nombre;
    nombre = temp_nombre;
    std::cout << "Nombre actualizado: " << nombre << "\n";
    
    // QUÉ: Liberar memoria de la cadena
    // POR QUÉ: Evitar memory leaks
    // CÓMO: Usa delete[] y establece puntero a nullptr
    // HACER: Confirmar liberación para evitar dangling pointers
    delete[] nombre;
    nombre = nullptr;
    std::cout << "String dinámico liberado\n";
}

// QUÉ: Función para demostrar asignación de matrices dinámicas
// POR QUÉ: Mostrar cómo crear y gestionar una matriz 2D
// CÓMO: Usa un arreglo de punteros para filas, cada una con columnas
// HACER: Ejecutar para entender matrices dinámicas
void ejemplo_matrices_dinamicas() {
    // QUÉ: Imprimir encabezado de la sección
    // POR QUÉ: Indicar inicio de la demostración de matrices
    // CÓMO: Usa std::cout para mostrar texto
    // HACER: Asegurar claridad en la salida
    std::cout << "\n=== MATRICES DINÁMICAS ===\n";
    
    // QUÉ: Definir dimensiones de la matriz
    // POR QUÉ: Crear una matriz 3x4
    // CÓMO: Declara variables para filas y columnas
    // HACER: Ajustar dimensiones según necesidad
    int filas = 3, columnas = 4;
    
    // QUÉ: Asignar arreglo de punteros a filas
    // POR QUÉ: Almacenar punteros a cada fila de la matriz
    // CÓMO: Usa new para un arreglo de punteros a enteros
    // HACER: Verificar si la asignación fue exitosa
    int** matriz = new (std::nothrow) int*[filas];
    if (matriz == nullptr) {
        // QUÉ: Manejar fallo de asignación
        // POR QUÉ: Evitar errores de puntero nulo
        // CÓMO: Imprime error y retorna
        // HACER: Salir para evitar problemas
        std::cout << "Error asignando filas\n";
        return;
    }
    
    // QUÉ: Asignar memoria para cada fila
    // POR QUÉ: Crear espacio para las columnas de cada fila
    // CÓMO: Usa new para cada fila, inicializando a cero
    // HACER: Verificar fallos y liberar memoria previa si ocurre
    for (int i = 0; i < filas; i++) {
        matriz[i] = new (std::nothrow) int[columnas]{};
        if (matriz[i] == nullptr) {
            // QUÉ: Manejar fallo de asignación de columna
            // POR QUÉ: Evitar memory leaks
            // CÓMO: Libera filas ya asignadas y el arreglo de punteros
            // HACER: Asegurar limpieza antes de salir
            std::cout << "Error asignando columna " << i << "\n";
            for (int j = 0; j < i; j++) {
                delete[] matriz[j];
            }
            delete[] matriz;
            return;
        }
    }
    
    // QUÉ: Inicializar matriz con valores secuenciales
    // POR QUÉ: Llenar la matriz con datos significativos
    // CÓMO: Usa bucles anidados para asignar valores 1, 2, 3, ...
    // HACER: Asegurar que los valores se asignen correctamente
    int valor = 1;
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            matriz[i][j] = valor++;
        }
    }
    
    // QUÉ: Imprimir la matriz
    // POR QUÉ: Mostrar el contenido de la matriz
    // CÓMO: Usa bucles anidados con formato de 3 caracteres
    // HACER: Verificar que la matriz se muestre correctamente
    std::cout << "Matriz " << filas << "x" << columnas << ":\n";
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            std::cout.width(3);
            std::cout << matriz[i][j] << " ";
        }
        std::cout << "\n";
    }
    
    // QUÉ: Liberar memoria de la matriz
    // POR QUÉ: Evitar memory leaks
    // CÓMO: Libera cada fila y luego el arreglo de punteros
    // HACER: Establecer puntero a nullptr tras liberar
    for (int i = 0; i < filas; i++) {
        delete[] matriz[i];
    }
    delete[] matriz;
    matriz = nullptr;
    std::cout << "Matriz dinámica liberada\n";
}

// QUÉ: Función principal, punto de entrada del programa
// POR QUÉ: Requerida por el compilador para iniciar la ejecución
// CÓMO: Llama a las funciones de demostración y retorna 0
// HACER: Ejecutar todas las demostraciones en orden
int main() {
    ejemplo_memoria_dinamica_cpp();
    ejemplo_strings_dinamicos();
    ejemplo_matrices_dinamicas();
    return 0;
}
```

### **New y Delete en C++**

```cpp
#include <iostream>  // QUÉ: Incluye la biblioteca de entrada/salida para consola
                     // POR QUÉ: Permite usar std::cout para mostrar texto
                     // CÓMO: Proporciona funciones para impresión formateada
                     // HACER: Usar para mostrar resultados en pantalla
#include <new>       // QUÉ: Incluye la biblioteca para manejo de excepciones de memoria
                     // POR QUÉ: Proporciona std::bad_alloc y std::nothrow
                     // CÓMO: Permite manejar errores de asignación de memoria
                     // HACER: Usar std::nothrow para asignaciones seguras
#include <cstring>   // QUÉ: Incluye la biblioteca para funciones de cadenas estilo C
                     // POR QUÉ: Necesaria para std::strlen, std::strcpy, std::strcat
                     // CÓMO: Facilita operaciones con cadenas dinámicas
                     // HACER: Usar para manipular cadenas estilo C

// QUÉ: Función para demostrar la gestión de memoria dinámica con tipos básicos y arreglos
// POR QUÉ: Mostrar cómo asignar, modificar y liberar memoria en C++
// CÓMO: Usa new/delete con manejo de excepciones y std::nothrow
// HACER: Ejecutar para entender asignación dinámica y manejo de errores
void ejemplo_memoria_dinamica_cpp() {
    // QUÉ: Imprimir encabezado de la sección
    // POR QUÉ: Indicar el inicio de la demostración
    // CÓMO: Usa std::cout con salto de línea para claridad
    // HACER: Asegurar que la salida sea clara
    std::cout << "=== GESTIÓN DE MEMORIA DINÁMICA EN C++ ===\n\n";

    // QUÉ: Asignar memoria para tipos básicos usando new
    // POR QUÉ: Demostrar asignación de variables individuales
    // CÓMO: Usa new con std::nothrow para evitar excepciones
    // HACER: Verificar asignación y manejar fallos
    std::cout << "1. USO DE new PARA TIPOS BÁSICOS:\n";
    int* ptr_entero = new (std::nothrow) int(42);  // Inicializa con 42
    if (ptr_entero == nullptr) {
        // QUÉ: Manejar fallo de asignación
        // POR QUÉ: Evitar dereferenciar puntero nulo
        // CÓMO: Imprime error y retorna
        // HACER: Salir para evitar errores
        std::cout << "Error: No se pudo asignar memoria para entero\n";
        return;
    }
    double* ptr_double = new (std::nothrow) double;  // Sin inicialización
    if (ptr_double == nullptr) {
        // QUÉ: Manejar fallo de asignación
        // POR QUÉ: Evitar memory leaks y errores
        // CÓMO: Libera memoria previa y retorna
        // HACER: Asegurar limpieza antes de salir
        std::cout << "Error: No se pudo asignar memoria para double\n";
        delete ptr_entero;
        return;
    }
    // QUÉ: Asignar valor al double
    // POR QUÉ: Inicializar memoria asignada dinámicamente
    // CÓMO: Desreferencia el puntero para asignar valor
    // HACER: Usar el puntero para almacenar datos
    *ptr_double = 3.14159;

    // QUÉ: Mostrar valores asignados
    // POR QUÉ: Verificar que la asignación e inicialización sean correctas
    // CÓMO: Usa std::cout para imprimir valores
    // HACER: Confirmar que los valores son los esperados
    std::cout << "Entero: " << *ptr_entero << "\n";
    std::cout << "Double: " << *ptr_double << "\n";

    // QUÉ: Asignar e inicializar un arreglo dinámico
    // POR QUÉ: Demostrar creación de arreglos con inicialización
    // CÓMO: Usa new[] con lista de inicialización
    // HACER: Verificar asignación e inicialización
    std::cout << "\n2. USO DE new[] PARA ARREGLOS:\n";
    int tamano = 5;
    int* ptr_arreglo = new (std::nothrow) int[tamano]{1, 2, 3, 4, 5};
    if (ptr_arreglo == nullptr) {
        // QUÉ: Manejar fallo de asignación
        // POR QUÉ: Evitar errores y memory leaks
        // CÓMO: Libera memoria previa y retorna
        // HACER: Asegurar limpieza antes de salir
        std::cout << "Error: No se pudo asignar memoria para arreglo\n";
        delete ptr_entero;
        delete ptr_double;
        return;
    }

    // QUÉ: Imprimir valores iniciales del arreglo
    // POR QUÉ: Verificar inicialización correcta
    // CÓMO: Recorre el arreglo con un bucle
    // HACER: Confirmar que los valores son 1, 2, 3, 4, 5
    std::cout << "Arreglo: ";
    for (int i = 0; i < tamano; i++) {
        std::cout << ptr_arreglo[i] << " ";
    }
    std::cout << "\n";

    // QUÉ: Modificar valores del arreglo
    // POR QUÉ: Mostrar cómo actualizar elementos dinámicos
    // CÓMO: Multiplica cada elemento por 2
    // HACER: Verificar que los valores se actualicen
    for (int i = 0; i < tamano; i++) {
        ptr_arreglo[i] *= 2;
    }
    // QUÉ: Imprimir arreglo modificado
    // POR QUÉ: Confirmar que los valores se duplicaron
    // CÓMO: Recorre el arreglo con un bucle
    // HACER: Verificar valores 2, 4, 6, 8, 10
    std::cout << "Arreglo modificado: ";
    for (int i = 0; i < tamano; i++) {
        std::cout << ptr_arreglo[i] << " ";
    }
    std::cout << "\n";

    // QUÉ: Liberar memoria asignada
    // POR QUÉ: Evitar memory leaks
    // CÓMO: Usa delete para variables y delete[] para arreglos
    // HACER: Establecer punteros a nullptr tras liberar
    std::cout << "\n3. LIBERANDO MEMORIA:\n";
    delete ptr_entero;
    delete ptr_double;
    delete[] ptr_arreglo;
    ptr_entero = nullptr;
    ptr_double = nullptr;
    ptr_arreglo = nullptr;
    std::cout << "Memoria liberada correctamente\n";
}

// QUÉ: Clase para manejar cadenas dinámicas
// POR QUÉ: Demostrar gestión de memoria en objetos con RAII
// CÓMO: Implementa constructor, copia, asignación y destructor
// HACER: Usar para gestionar cadenas dinámicas de forma segura
class GestorStrings {
private:
    char* datos;      // QUÉ: Puntero a datos dinámicos
                      // POR QUÉ: Almacena la cadena dinámica
                      // CÓMO: Apunta a memoria asignada con new
                      // HACER: Gestionar cuidadosamente para evitar leaks
    size_t longitud;  // QUÉ: Almacena la longitud de la cadena
                      // POR QUÉ: Facilitar operaciones con la cadena
                      // CÓMO: Calculada con std::strlen
                      // HACER: Actualizar al modificar la cadena

public:
    // QUÉ: Constructor para inicializar la cadena
    // POR QUÉ: Crear un objeto con una cadena inicial
    // CÓMO: Asigna memoria y copia la cadena
    // HACER: Pasar una cadena inicial o usar valor por defecto
    GestorStrings(const char* str = "") {
        // QUÉ: Calcular longitud y asignar memoria
        // POR QUÉ: Asegurar espacio suficiente para la cadena
        // CÓMO: Usa std::strlen y new con std::nothrow
        // HACER: Verificar asignación
        longitud = std::strlen(str);
        datos = new (std::nothrow) char[longitud + 1];
        if (datos == nullptr) {
            std::cout << "Error: No se pudo asignar memoria en constructor\n";
            longitud = 0;
            return;
        }
        // QUÉ: Copiar la cadena inicial
        // POR QUÉ: Inicializar el objeto con datos
        // CÓMO: Usa std::strcpy para copiar
        // HACER: Asegurar que la copia sea correcta
        std::strcpy(datos, str);
        std::cout << "Constructor llamado para: " << datos << "\n";
    }

    // QUÉ: Constructor de copia
    // POR QUÉ: Evitar copias superficiales (shallow copy)
    // CÓMO: Crea una copia profunda de los datos
    // HACER: Usar para duplicar objetos de forma segura
    GestorStrings(const GestorStrings& otro) {
        // QUÉ: Copiar longitud y asignar nueva memoria
        // POR QUÉ: Garantizar copia independiente
        // CÓMO: Usa new y std::strcpy
        // HACER: Verificar asignación
        longitud = otro.longitud;
        datos = new (std::nothrow) char[longitud + 1];
        if (datos == nullptr) {
            std::cout << "Error: No se pudo asignar memoria en copia\n";
            longitud = 0;
            return;
        }
        std::strcpy(datos, otro.datos);
        std::cout << "Constructor de copia llamado para: " << datos << "\n";
    }

    // QUÉ: Operador de asignación
    // POR QUÉ: Permitir asignación segura entre objetos
    // CÓMO: Libera memoria existente, copia datos
    // HACER: Verificar auto-asignación y manejar memoria
    GestorStrings& operator=(const GestorStrings& otro) {
        // QUÉ: Verificar auto-asignación
        // POR QUÉ: Evitar liberar y copiar innecesariamente
        // CÓMO: Compara punteros de objetos
        // HACER: No modificar si es el mismo objeto
        if (this != &otro) {
            // QUÉ: Liberar memoria existente
            // POR QUÉ: Evitar memory leaks
            // CÓMO: Usa delete[]
            // HACER: Asegurar liberación antes de nueva asignación
            delete[] datos;
            // QUÉ: Copiar longitud y datos
            // POR QUÉ: Crear una copia profunda
            // CÓMO: Usa new y std::strcpy
            // HACER: Verificar asignación
            longitud = otro.longitud;
            datos = new (std::nothrow) char[longitud + 1];
            if (datos == nullptr) {
                std::cout << "Error: No se pudo asignar memoria en asignación\n";
                longitud = 0;
                return *this;
            }
            std::strcpy(datos, otro.datos);
            std::cout << "Operador de asignación llamado para: " << datos << "\n";
        }
        return *this;
    }

    // QUÉ: Destructor para liberar memoria
    // POR QUÉ: Evitar memory leaks al destruir el objeto
    // CÓMO: Usa delete[] y establece puntero a nullptr
    // HACER: Asegurar que se llame automáticamente al salir del scope
    ~GestorStrings() {
        std::cout << "Destructor llamado para: " << (datos ? datos : "nullptr") << "\n";
        delete[] datos;
        datos = nullptr;
    }

    // QUÉ: Métodos de acceso para datos y longitud
    // POR QUÉ: Permitir acceso seguro a los miembros privados
    // CÓMO: Retorna puntero a datos y longitud
    // HACER: Usar para consultar estado del objeto
    const char* getDatos() const { return datos; }
    size_t getLongitud() const { return longitud; }

    // QUÉ: Método para concatenar cadenas
    // POR QUÉ: Extender la cadena actual con otra
    // CÓMO: Reasigna memoria, copia y concatena
    // HACER: Verificar asignación y actualizar longitud
    void concatenar(const char* str) {
        // QUÉ: Calcular nueva longitud
        // POR QUÉ: Determinar espacio necesario
        // CÓMO: Suma longitud actual y nueva cadena
        // HACER: Asegurar espacio suficiente
        size_t nueva_longitud = longitud + std::strlen(str);
        char* nuevo_datos = new (std::nothrow) char[nueva_longitud + 1];
        if (nuevo_datos == nullptr) {
            std::cout << "Error: No se pudo asignar memoria para concatenar\n";
            return;
        }
        // QUÉ: Copiar y concatenar datos
        // POR QUÉ: Actualizar contenido de la cadena
        // CÓMO: Usa std::strcpy y std::strcat
        // HACER: Asegurar que la concatenación sea correcta
        std::strcpy(nuevo_datos, datos);
        std::strcat(nuevo_datos, str);
        // QUÉ: Liberar memoria antigua y actualizar
        // POR QUÉ: Evitar memory leaks
        // CÓMO: Usa delete[] y actualiza puntero y longitud
        // HACER: Asegurar que el puntero apunte a nueva memoria
        delete[] datos;
        datos = nuevo_datos;
        longitud = nueva_longitud;
    }
};

// QUÉ: Función para demostrar uso de la clase GestorStrings
// POR QUÉ: Mostrar gestión de memoria en objetos con RAII
// CÓMO: Crea objetos, usa copia, asignación y concatenación
// HACER: Ejecutar para ver ciclo de vida de objetos
void ejemplo_clases_con_memoria() {
    // QUÉ: Imprimir encabezado de la sección
    // POR QUÉ: Indicar inicio de la demostración
    // CÓMO: Usa std::cout para claridad
    // HACER: Asegurar que la salida sea clara
    std::cout << "\n=== CLASES CON MEMORIA DINÁMICA ===\n";

    {
        // QUÉ: Crear objetos en un scope local
        // POR QUÉ: Demostrar construcción, copia y asignación
        // CÓMO: Instancia objetos y usa métodos
        // HACER: Observar cómo se llaman constructores y destructores
        GestorStrings str1("Hola");
        GestorStrings str2 = str1;  // Constructor de copia
        GestorStrings str3;         // Constructor por defecto
        str3 = str1;                // Operador de asignación

        // QUÉ: Concatenar una cadena
        // POR QUÉ: Mostrar modificación dinámica
        // CÓMO: Llama al método concatenar
        // HACER: Verificar que la cadena se actualice
        str1.concatenar(" Mundo");
        std::cout << "str1 después de concatenar: " << str1.getDatos() << "\n";
    }
    // QUÉ: Imprimir mensaje tras salir del scope
    // POR QUÉ: Confirmar que los destructores se llamaron
    // CÓMO: Usa std::cout tras el scope
    // HACER: Verificar que los objetos se destruyeron
    std::cout << "Objetos destruidos\n";
}

// QUÉ: Función para demostrar manejo de excepciones en asignación
// POR QUÉ: Mostrar cómo manejar fallos de memoria
// CÓMO: Usa try-catch y std::nothrow
// HACER: Ejecutar para entender manejo de errores
void ejemplo_excepciones_memoria() {
    // QUÉ: Imprimir encabezado de la sección
    // POR QUÉ: Indicar inicio de la demostración
    // CÓMO: Usa std::cout para claridad
    // HACER: Asegurar que la salida sea clara
    std::cout << "\n=== MANEJO DE EXCEPCIONES EN MEMORIA ===\n";

    // QUÉ: Intentar asignar memoria grande con new
    // POR QUÉ: Probar manejo de excepciones con std::bad_alloc
    // CÓMO: Usa try-catch para capturar fallo
    // HACER: Observar cómo se maneja el error
    try {
        size_t gran_tamano = 1000000000000ULL;  // 1TB
        int* ptr_grande = new int[gran_tamano];
        std::cout << "Memoria grande asignada (no debería llegar aquí)\n";
        delete[] ptr_grande;
    } catch (const std::bad_alloc& e) {
        // QUÉ: Capturar excepción de asignación
        // POR QUÉ: Manejar fallos de memoria de forma segura
        // CÓMO: Imprime mensaje de error
        // HACER: Verificar que se captura la excepción
        std::cout << "Capturada excepción de memoria: " << e.what() << "\n";
    }

    // QUÉ: Asignar memoria con std::nothrow
    // POR QUÉ: Demostrar asignación sin excepciones
    // CÓMO: Usa new(std::nothrow) y verifica nullptr
    // HACER: Verificar resultado de la asignación
    int* ptr_seguro = new (std::nothrow) int[1000];
    if (ptr_seguro == nullptr) {
        // QUÉ: Manejar fallo de asignación
        // POR QUÉ: Evitar errores con puntero nulo
        // CÓMO: Imprime mensaje de error
        // HACER: Confirmar manejo seguro
        std::cout << "No se pudo asignar memoria (nothrow)\n";
    } else {
        // QUÉ: Confirmar asignación exitosa
        // POR QUÉ: Mostrar que la memoria se asignó
        // CÓMO: Imprime mensaje y libera memoria
        // HACER: Liberar memoria para evitar leaks
        std::cout << "Memoria asignada con éxito (nothrow)\n";
        delete[] ptr_seguro;
    }
}

// QUÉ: Función principal, punto de entrada del programa
// POR QUÉ: Requerida por el compilador para iniciar ejecución
// CÓMO: Llama a las funciones de demostración
// HACER: Ejecutar todas las demostraciones en orden
int main() {
    ejemplo_memoria_dinamica_cpp();
    ejemplo_clases_con_memoria();
    ejemplo_excepciones_memoria();
    return 0;
}
```

### **Buenas Prácticas de Gestión de Memoria**

```cpp
#include <iostream>  // QUÉ: Incluye la biblioteca de entrada/salida para consola
                     // POR QUÉ: Permite usar std::cout para mostrar texto
                     // CÓMO: Proporciona funciones para impresión formateada
                     // HACER: Usar para mostrar resultados en pantalla
#include <memory>    // QUÉ: Incluye la biblioteca para smart pointers
                     // POR QUÉ: Proporciona std::unique_ptr, std::shared_ptr, std::weak_ptr
                     // CÓMO: Facilita la gestión automática de memoria
                     // HACER: Usar para evitar memory leaks
#include <string>    // QUÉ: Incluye la biblioteca para std::string
                     // POR QUÉ: Necesaria para el campo nombre en GestorRecurso
                     // CÓMO: Proporciona una clase para manejar cadenas
                     // HACER: Usar para nombres de recursos

// QUÉ: Función para demostrar el uso de smart pointers (C++11+)
// POR QUÉ: Mostrar cómo gestionar memoria automáticamente
// CÓMO: Usa unique_ptr, shared_ptr y weak_ptr con make_unique/make_shared
// HACER: Ejecutar para entender la gestión automática de memoria
void ejemplo_smart_pointers() {
    // QUÉ: Imprimir encabezado de la sección
    // POR QUÉ: Indicar el inicio de la demostración
    // CÓMO: Usa std::cout con saltos de línea para claridad
    // HACER: Asegurar que la salida sea clara
    std::cout << "\n=== SMART POINTERS (C++11+) ===\n";

    // QUÉ: Demostrar unique_ptr para propiedad única
    // POR QUÉ: Mostrar cómo manejar recursos no compartibles
    // CÓMO: Usa make_unique para crear y mover propiedad
    // HACER: Observar la transferencia de propiedad
    {
        // QUÉ: Crear un unique_ptr con valor inicial
        // POR QUÉ: Asegurar asignación segura sin delete explícito
        // CÓMO: Usa std::make_unique para inicializar con 42
        // HACER: Verificar que el valor sea accesible
        std::unique_ptr<int> ptr_unica = std::make_unique<int>(42);
        std::cout << "Valor unique_ptr: " << *ptr_unica << "\n";

        // QUÉ: Transferir propiedad a otro unique_ptr
        // POR QUÉ: Demostrar que unique_ptr no se puede copiar
        // CÓMO: Usa std::move para transferir propiedad
        // HACER: Confirmar que ptr_unica queda inválido
        std::unique_ptr<int> ptr2 = std::move(ptr_unica);
        std::cout << "Valor después de move (move(ptr_unica)): " << *ptr2 << "\n";
        // Nota: ptr_unica es nullptr tras move, no se debe dereferenciar
    }  // QUÉ: Liberar memoria automáticamente
       // POR QUÉ: unique_ptr libera memoria al salir del scope
       // CÓMO: El destructor de unique_ptr llama a delete
       // HACER: No es necesario delete explícito

    // QUÉ: Demostrar shared_ptr para propiedad compartida
    // POR QUÉ: Mostrar cómo múltiples punteros comparten un recurso
    // CÓMO: Usa make_shared y verifica el contador de referencias
    // HACER: Observar el conteo de referencias
    {
        // QUÉ: Crear un shared_ptr con valor inicial
        // POR QUÉ: Asegurar asignación eficiente y compartida
        // CÓMO: Usa std::make_shared para inicializar con 100
        // HACER: Verificar que el valor sea accesible
        std::shared_ptr<int> ptr_compartido1 = std::make_shared<int>(100);
        {
            // QUÉ: Compartir propiedad con otro shared_ptr
            // POR QUÉ: Demostrar incremento del contador de referencias
            // CÓMO: Asigna ptr_compartido1 a ptr_compartido2
            // HACER: Verificar el contador de referencias
            std::shared_ptr<int> ptr_compartido2 = ptr_compartido1;
            std::cout << "Contador de referencias: " << ptr_compartido1.use_count() << "\n";
            std::cout << "Valor compartido: " << *ptr_compartido1 << "\n";
        }  // QUÉ: Liberar ptr_compartido2
           // POR QUÉ: Disminuye el contador de referencias
           // CÓMO: El destructor de ptr_compartido2 reduce use_count
           // HACER: Confirmar que el contador disminuye
        std::cout << "Contador después de scope: " << ptr_compartido1.use_count() << "\n";
    }  // QUÉ: Liberar memoria compartida
       // POR QUÉ: El último shared_ptr libera la memoria
       // CÓMO: El destructor de shared_ptr llama a delete cuando use_count es 0
       // HACER: No es necesario delete explícito

    // QUÉ: Demostrar weak_ptr para referencias débiles
    // POR QUÉ: Evitar ciclos de referencia y verificar validez
    // CÓMO: Usa weak_ptr con lock() para acceder al recurso
    // HACER: Verificar si el recurso aún existe
    {
        // QUÉ: Crear shared_ptr y asociar weak_ptr
        // POR QUÉ: Mostrar referencias débiles sin incrementar contador
        // CÓMO: Usa make_shared y asigna a weak_ptr
        // HACER: Verificar acceso seguro al recurso
        std::shared_ptr<int> ptr_original = std::make_shared<int>(200);
        std::weak_ptr<int> ptr_debil = ptr_original;
        // QUÉ: Acceder al recurso con lock()
        // POR QUÉ: Verificar si el recurso aún existe
        // CÓMO: lock() retorna un shared_ptr temporal
        // HACER: Comprobar validez antes de dereferenciar
        if (auto ptr = ptr_debil.lock()) {
            std::cout << "Valor desde weak_ptr: " << *ptr << "\n";
        } else {
            std::cout << "Recurso de weak_ptr no disponible\n";
        }
    }  // QUÉ: Liberar recursos
       // POR QUÉ: weak_ptr no afecta la vida del recurso
       // CÓMO: ptr_original libera la memoria al salir del scope
       // HACER: Confirmar que weak_ptr no causa leaks
}

// QUÉ: Clase para implementar el patrón RAII
// POR QUÉ: Garantizar la liberación automática de recursos
// CÓMO: Usa constructor, destructor y semántica de movimiento
// HACER: Usar para gestionar recursos de forma segura
class GestorRecurso {
private:
    int* recurso;        // QUÉ: Puntero a recurso dinámico
                         // POR QUÉ: Representa un recurso gestionado
                         // CÓMO: Apunta a memoria asignada con new
                         // HACER: Gestionar cuidadosamente para evitar leaks
    std::string nombre;  // QUÉ: Nombre identificador del recurso
                         // POR QUÉ: Facilitar trazabilidad en salida
                         // CÓMO: Usa std::string para manejo seguro
                         // HACER: Usar para identificar el recurso

public:
    // QUÉ: Constructor para adquirir recurso
    // POR QUÉ: Inicializar el objeto con un recurso dinámico
    // CÓMO: Asigna memoria con new y std::nothrow
    // HACER: Verificar asignación y establecer nombre
    GestorRecurso(const std::string& n) : nombre(n), recurso(nullptr) {
        recurso = new (std::nothrow) int(0);
        if (recurso == nullptr) {
            // QUÉ: Manejar fallo de asignación
            // POR QUÉ: Evitar errores con puntero nulo
            // CÓMO: Imprime error
            // HACER: Informar al usuario del fallo
            std::cout << "Error: No se pudo asignar recurso para " << nombre << "\n";
            return;
        }
        std::cout << "Recurso " << nombre << " adquirido\n";
    }

    // QUÉ: Destructor para liberar recurso
    // POR QUÉ: Garantizar liberación automática al salir del scope
    // CÓMO: Usa delete y verifica validez del puntero
    // HACER: Asegurar que se llame automáticamente
    ~GestorRecurso() {
        if (recurso) {
            std::cout << "Recurso " << nombre << " liberado\n";
            delete recurso;
            recurso = nullptr;
        }
    }

    // QUÉ: Eliminar constructor de copia
    // POR QUÉ: Evitar copias accidentales que causen doble liberación
    // CÓMO: Usa = delete para deshabilitar copia
    // HACER: Usar movimiento en lugar de copia
    GestorRecurso(const GestorRecurso&) = delete;
    GestorRecurso& operator=(const GestorRecurso&) = delete;

    // QUÉ: Constructor de movimiento
    // POR QUÉ: Permitir transferencia segura de propiedad
    // CÓMO: Transfiere recurso y nombre, deja origen en estado válido
    // HACER: Usar std::move para transferencias
    GestorRecurso(GestorRecurso&& otro) noexcept 
        : recurso(otro.recurso), nombre(std::move(otro.nombre)) {
        otro.recurso = nullptr;
        std::cout << "Recurso " << nombre << " movido\n";
    }

    // QUÉ: Operador de asignación por movimiento
    // POR QUÉ: Completar semántica de movimiento para RAII
    // CÓMO: Libera recurso actual, transfiere desde otro
    // HACER: Usar para asignaciones seguras
    GestorRecurso& operator=(GestorRecurso&& otro) noexcept {
        if (this != &otro) {
            // QUÉ: Liberar recurso actual
            // POR QUÉ: Evitar memory leaks
            // CÓMO: Usa delete si recurso existe
            // HACER: Asegurar liberación antes de transferencia
            delete recurso;
            // QUÉ: Transferir recurso y nombre
            // POR QUÉ: Tomar propiedad del recurso de otro
            // CÓMO: Asigna puntero y mueve nombre
            // HACER: Dejar otro en estado válido
            recurso = otro.recurso;
            nombre = std::move(otro.nombre);
            otro.recurso = nullptr;
            std::cout << "Recurso " << nombre << " asignado por movimiento\n";
        }
        return *this;
    }

    // QUÉ: Método para usar el recurso
    // POR QUÉ: Demostrar interacción con el recurso
    // CÓMO: Incrementa el valor del recurso si es válido
    // HACER: Verificar validez antes de usar
    void usar() {
        if (recurso) {
            (*recurso)++;
            std::cout << "Usando recurso " << nombre << ": " << *recurso << "\n";
        } else {
            std::cout << "Recurso " << nombre << " no válido\n";
        }
    }
};

// QUÉ: Función para demostrar el patrón RAII
// POR QUÉ: Mostrar gestión automática de recursos
// CÓMO: Crea objetos GestorRecurso y usa sus métodos
// HACER: Ejecutar para observar ciclo de vida de recursos
void ejemplo_raii() {
    // QUÉ: Imprimir encabezado de la sección
    // POR QUÉ: Indicar inicio de la demostración
    // CÓMO: Usa std::cout para claridad
    // HACER: Asegurar que la salida sea clara
    std::cout << "\n=== PATRÓN RAII ===\n";

    {
        // QUÉ: Crear objetos que gestionan recursos
        // POR QUÉ: Demostrar adquisición y liberación automática
        // CÓMO: Instancia objetos GestorRecurso
        // HACER: Usar y observar liberación al salir del scope
        GestorRecurso recurso1("Archivo1");
        GestorRecurso recurso2("Archivo2");
        // QUÉ: Usar los recursos
        // POR QUÉ: Mostrar funcionalidad del recurso
        // CÓMO: Llama al método usar
        // HACER: Verificar que los valores se incrementan
        recurso1.usar();
        recurso1.usar();
        recurso2.usar();
    }
    // QUÉ: Confirmar liberación de recursos
    // POR QUÉ: Verificar que los destructores se llamaron
    // CÓMO: Imprime mensaje tras salir del scope
    // HACER: Confirmar que no hay memory leaks
    std::cout << "Todos los recursos liberados\n";
}

// QUÉ: Función para demostrar manejo seguro de memoria con raw pointers
// POR QUÉ: Mostrar buenas prácticas con punteros crudos
// CÓMO: Usa inicialización, verificación y liberación explícita
// HACER: Ejecutar para entender manejo manual de memoria
void ejemplo_manejo_seguro() {
    // QUÉ: Imprimir encabezado de la sección
    // POR QUÉ: Indicar inicio de la demostración
    // CÓMO: Usa std::cout para claridad
    // HACER: Asegurar que la salida sea clara
    std::cout << "\n=== MANEJO SEGURO DE MEMORIA ===\n";

    // QUÉ: Asignar memoria para un arreglo
    // POR QUÉ: Demostrar verificación de asignación
    // CÓMO: Usa new con std::nothrow
    // HACER: Verificar si la asignación fue exitosa
    int* ptr = new (std::nothrow) int[1000];
    if (ptr == nullptr) {
        // QUÉ: Manejar fallo de asignación
        // POR QUÉ: Evitar errores con puntero nulo
        // CÓMO: Imprime error y retorna
        // HACER: Salir para evitar problemas
        std::cout << "Error: No se pudo asignar memoria para arreglo\n";
        return;
    }

    // QUÉ: Inicializar apuntadores a nullptr
    // POR QUÉ: Evitar valores basura y dangling pointers
    // CÓMO: Declara punteros y asigna nullptr
    // HACER: Siempre inicializar punteros
    int* ptr1 = nullptr;
    int* ptr2 = nullptr;

    // QUÉ: Asignar memoria para enteros individuales
    // POR QUÉ: Demostrar asignación y uso seguro
    // CÓMO: Usa new con std::nothrow y verifica
    // HACER: Verificar asignaciones antes de usar
    ptr1 = new (std::nothrow) int(10);
    if (ptr1 == nullptr) {
        // QUÉ: Manejar fallo de asignación
        // POR QUÉ: Evitar memory leaks y errores
        // CÓMO: Libera memoria previa y retorna
        // HACER: Asegurar limpieza antes de salir
        std::cout << "Error: No se pudo asignar memoria para ptr1\n";
        delete[] ptr;
        return;
    }
    ptr2 = new (std::nothrow) int(20);
    if (ptr2 == nullptr) {
        // QUÉ: Manejar fallo de asignación
        // POR QUÉ: Evitar memory leaks y errores
        // CÓMO: Libera memoria previa y retorna
        // HACER: Asegurar limpieza antes de salir
        std::cout << "Error: No se pudo asignar memoria para ptr2\n";
        delete ptr1;
        delete[] ptr;
        return;
    }

    // QUÉ: Usar los recursos asignados
    // POR QUÉ: Mostrar acceso a memoria dinámica
    // CÓMO: Imprime valores de los punteros
    // HACER: Verificar que los valores sean correctos
    std::cout << "Valores: " << *ptr1 << ", " << *ptr2 << "\n";

    // QUÉ: Liberar memoria asignada
    // POR QUÉ: Evitar memory leaks
    // CÓMO: Usa delete y delete[] para liberar
    // HACER: Establecer punteros a nullptr tras liberar
    delete ptr1;
    delete ptr2;
    delete[] ptr;
    ptr1 = nullptr;
    ptr2 = nullptr;
    ptr = nullptr;
    std::cout << "Memoria manejada de forma segura\n";
}

// QUÉ: Función principal, punto de entrada del programa
// POR QUÉ: Requerida por el compilador para iniciar ejecución
// CÓMO: Llama a las funciones de demostración
// HACER: Ejecutar todas las demostraciones en orden
int main() {
    ejemplo_smart_pointers();
    ejemplo_raii();
    ejemplo_manejo_seguro();
    return 0;
}
```

Nota: 
# RAII (Resource Acquisition Is Initialization) en Sistemas Operativos

## Definición
**RAII** (Resource Acquisition Is Initialization) es un paradigma de programación utilizado en lenguajes como **C++** para gestionar recursos de manera segura y automática. Aunque no es exclusivo de los sistemas operativos, es ampliamente utilizado en el desarrollo de software que interactúa con recursos del sistema (memoria, archivos, sockets, mutexes, etc.).

## Principio Básico
- **Adquisición del recurso durante la inicialización** (generalmente en el constructor de un objeto).
- **Liberación automática al salir del ámbito** (generalmente en el destructor).

## Aplicaciones en Sistemas Operativos
RAII ayuda a manejar recursos del sistema operativo de forma segura, evitando fugas (*memory leaks*, *handle leaks*) y garantizando liberación incluso en casos de excepciones.

### Ejemplos Comunes
1. **Gestión de Memoria**
   - Uso de `std::unique_ptr` o `std::shared_ptr` en C++ para liberación automática.
   - Sin RAII: riesgo de fugas por olvidar `free()` o `delete`.

2. **Archivos y Descriptores**
   - Clases como `std::fstream` cierran automáticamente el archivo al destruirse.
   - Sin RAII: necesidad de llamar manualmente a `close()`.

3. **Sincronización (Mutexes)**
   - `std::lock_guard` adquiere y libera automáticamente un mutex.
   - Sin RAII: riesgo de *deadlocks* por liberación olvidada.

4. **Handles del Sistema (Windows/Linux)**
   - Objetos como `HANDLE` (Windows) o descriptores de archivo (Linux) pueden gestionarse con RAII.

## Ventajas de RAII
✅ **Seguridad**: Elimina fugas de recursos.  
✅ **Excepción-safe**: Libera recursos incluso si ocurren errores.  
✅ **Código más limpio**: Reduce errores humanos al evitar liberación manual.  

## Ejemplo en C++ (Mutex con RAII)
```cpp
#include <mutex>

void ejemplo_seguro() {
    std::mutex mtx;
    std::lock_guard<std::mutex> lock(mtx); // Bloquea en construcción

    // Sección crítica...
    // El mutex se libera automáticamente al salir del ámbito.
}
```

---

## 🏗️ 4. Estructuras (struct) en C

### **Definición y Uso Básico**

```c
#include <iostream>   // Qué: Biblioteca estándar de E/S
                      // Para qué: Usar cout, cin y otras funciones de E/S
#include <memory>     // Qué: Smart pointers
                      // Para qué: Gestión automática de memoria (unique_ptr, shared_ptr)
#include <fstream>    // Qué: Manejo de archivos
                      // Para qué: Leer/escribir archivos con RAII
#include <mutex>      // Qué: Sincronización de hilos
                      // Para qué: Proteger secciones críticas
#include <vector>     // Qué: Contenedor dinámico
                      // Para qué: Almacenar colecciones de objetos
#include <string>     // Qué: Cadena de caracteres segura
                      // Para qué: Manejo moderno de strings
#include <cstdio>     // Qué: Funciones de C estándar
                      // Para qué: Manejar FILE* para integración con código C
#include <stdexcept>  // Qué: Excepciones estándar
                      // Para qué: Lanzar errores con mensajes descriptivos

// Ejemplo 1: Gestión de memoria con smart pointers
void ejemplo_memoria_raii() {
    // Qué: Creación de un unique_ptr
    // Para qué: Demostrar gestión automática de memoria
    // Cómo: Usando make_unique para asignación segura
    // Hacer: Usar siempre smart pointers en lugar de new/delete
    
    std::unique_ptr<int> ptr_int = std::make_unique<int>(42);
    std::cout << "Valor: " << *ptr_int << std::endl;
    // El destructor libera automáticamente la memoria aquí
}

// Ejemplo 2: Gestión de archivos con RAII
void ejemplo_archivos_raii() {
    // Qué: Uso de ofstream
    // Para qué: Mostrar manejo seguro de archivos
    // Cómo: El destructor cierra el archivo automáticamente
    // Hacer: Preferir streams de C++ sobre funciones de C
    
    std::ofstream archivo("ejemplo.txt");
    if (archivo.is_open()) {
        archivo << "Datos guardados automáticamente\n";
        // No necesita close() explícito - RAII lo maneja
    }
}

// Ejemplo 3: Gestión de mutex con RAII
void ejemplo_mutex_raii() {
    // Qué: Uso de lock_guard
    // Para qué: Sincronización segura entre hilos
    // Cómo: Bloquea en construcción, desbloquea en destrucción
    // Hacer: Usar siempre RAII para manejo de mutex
    
    static std::mutex mtx;
    
    {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Sección crítica protegida\n";
        // Mutex se libera automáticamente al salir del ámbito
    }
}

// Ejemplo 4: RAII para recursos estilo C (FILE*)
class FileRAII {
    // Qué: Wrapper RAII para FILE*
    // Para qué: Integrar recursos C en paradigma RAII
    // Cómo: Constructor abre, destructor cierra
    // Hacer: Crear wrappers para recursos no-RAII
    
    FILE* file;
public:
    FileRAII(const char* filename, const char* mode) 
        : file(fopen(filename, mode)) {
        if (!file) throw std::runtime_error("No se pudo abrir el archivo");
    }
    
    ~FileRAII() {
        if (file) fclose(file);
    }
    
    void write(const std::string& data) {
        if (fputs(data.c_str(), file) == EOF) {
            throw std::runtime_error("Error al escribir");
        }
    }
};

void ejemplo_file_raii() {
    // Qué: Uso del wrapper FileRAII
    // Para qué: Manejo seguro de archivos estilo C
    // Cómo: Mediante bloque try-catch
    // Hacer: Siempre usar try-catch con recursos
    
    try {
        FileRAII archivo("datos.txt", "w");
        archivo.write("Datos guardados con RAII\n");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

// Ejemplo 5: Estructuras mejoradas con RAII
struct Persona {
    // Qué: Estructura con miembros RAII
    // Para qué: Mostrar diseño moderno de estructuras
    // Cómo: Usando std::string y constructor
    // Hacer: Preferir std::string sobre char[]
    
    std::string nombre;
    int edad;
    float altura;
    
    Persona(const std::string& n, int e, float a) 
        : nombre(n), edad(e), altura(a) {}
    
    void mostrar() const {
        std::cout << nombre << ", " << edad << " años, " 
                  << altura << "m\n";
    }
};

void ejemplo_estructuras_mejoradas() {
    // Qué: Uso de vector y estructuras RAII
    // Para qué: Demostrar colecciones seguras
    // Cómo: Usando emplace_back para construcción in-place
    // Hacer: Usar contenedores STL con tipos RAII
    
    Persona juan("Juan Pérez", 25, 1.75f);
    juan.mostrar();
    
    std::vector<Persona> personas;
    personas.emplace_back("María López", 30, 1.65f);
    personas.emplace_back("Carlos Ruiz", 28, 1.80f);
    
    for (const auto& p : personas) {
        p.mostrar();
    }
}

int main() {
    // Qué: Función principal
    // Para qué: Ejecutar ejemplos demostrativos
    // Cómo: Llamando a cada función de ejemplo
    // Hacer: Organizar código en funciones específicas
    
    std::cout << "=== Ejemplo RAII Memoria ===\n";
    ejemplo_memoria_raii();
    
    std::cout << "\n=== Ejemplo RAII Archivos ===\n";
    ejemplo_archivos_raii();
    
    std::cout << "\n=== Ejemplo RAII Mutex ===\n";
    ejemplo_mutex_raii();
    
    std::cout << "\n=== Ejemplo RAII FILE* ===\n";
    ejemplo_file_raii();
    
    std::cout << "\n=== Ejemplo Estructuras Mejoradas ===\n";
    ejemplo_estructuras_mejoradas();
    
    return 0;
}
```

### **Arreglos de Estructuras**

```c
#include <stdio.h>
#include <string.h>

// DEFINIR ESTRUCTURA PARA ESTUDIANTES
typedef struct {
    int id;              // Identificador único del estudiante
    char nombre[50];     // Nombre del estudiante (array de caracteres)
    float calificacion;  // Calificación del estudiante
} Estudiante;

void ejemplo_arreglos_estructuras() {
    // ARREGLO DE ESTRUCTURAS CON INICIALIZACIÓN
    // Estudiante estudiantes[3] = {...}: arreglo de 3 estructuras inicializadas
    Estudiante estudiantes[3] = {
        {1, "Ana Martínez", 9.5},    // Primer estudiante
        {2, "Carlos Ruiz", 8.7},     // Segundo estudiante
        {3, "Elena García", 9.2}     // Tercer estudiante
    };
    
    // ACCESO A ELEMENTOS DEL ARREGLO DE ESTRUCTURAS
    printf("Lista de estudiantes:\n");
    for(int i = 0; i < 3; i++) {
        // estudiantes[i].miembro: acceder a miembro de estructura en posición i
        printf("ID: %d, Nombre: %s, Calificación: %.1f\n",
               estudiantes[i].id,           // ID del estudiante i
               estudiantes[i].nombre,       // Nombre del estudiante i
               estudiantes[i].calificacion); // Calificación del estudiante i
    }
    
    // ARREGLO DINÁMICO DE ESTRUCTURAS
    // malloc(num_estudiantes * sizeof(Estudiante)): memoria para arreglo dinámico
    int num_estudiantes = 2;
    Estudiante *ptr_estudiantes = 
        (Estudiante*)malloc(num_estudiantes * sizeof(Estudiante));
    
    if(ptr_estudiantes != NULL) {
        // INICIALIZAR DINÁMICAMENTE CADA ESTRUCTURA EN EL ARREGLO
        // ptr_estudiantes[0]: primer elemento del arreglo dinámico
        ptr_estudiantes[0].id = 4;                           // Asignar ID
        strcpy(ptr_estudiantes[0].nombre, "David Silva");    // Copiar nombre
        ptr_estudiantes[0].calificacion = 8.9;               // Asignar calificación
        
        ptr_estudiantes[1].id = 5;                           // Asignar ID
        strcpy(ptr_estudiantes[1].nombre, "Fernanda López"); // Copiar nombre
        ptr_estudiantes[1].calificacion = 9.1;               // Asignar calificación
        
        printf("\nEstudiantes dinámicos:\n");
        for(int i = 0; i < num_estudiantes; i++) {
            // Imprimir estudiantes del arreglo dinámico
            printf("ID: %d, Nombre: %s, Calificación: %.1f\n",
                   ptr_estudiantes[i].id,
                   ptr_estudiantes[i].nombre,
                   ptr_estudiantes[i].calificacion);
        }
        
        free(ptr_estudiantes);  // Liberar memoria del arreglo dinámico
    }
}
```

---

## 🧱 5. Clases y Objetos en C++

### **Conceptos Básicos de Clases**

```cpp
#include <iostream>
#include <string>
using namespace std;

// DEFINICIÓN DE CLASE BÁSICA CON ENCAPSULACIÓN
class Persona {
private:
    // MIEMBROS PRIVADOS - Solo accesibles desde dentro de la clase
    string nombre;      // Nombre de la persona (string de C++)
    int edad;           // Edad de la persona (entero)
    string direccion;   // Dirección de la persona (string de C++)

public:
    // CONSTRUCTOR POR DEFECTO - Se llama cuando no se proporcionan argumentos
    // Persona() : nombre("Desconocido"), edad(0), direccion("Sin dirección") {}
    // Lista de inicialización: nombre("Desconocido"), edad(0), direccion("Sin dirección")
    Persona() : nombre("Desconocido"), edad(0), direccion("Sin dirección") {}
    
    // CONSTRUCTOR CON PARÁMETROS - Se llama cuando se proporcionan argumentos
    // Persona(string n, int e, string d) : nombre(n), edad(e), direccion(d) {}
    // Inicializa miembros con valores proporcionados
    Persona(string n, int e, string d) : nombre(n), edad(e), direccion(d) {}
    
    // CONSTRUCTOR DE COPIA - Se llama cuando se crea copia de otro objeto
    // const Persona& otra: referencia constante al objeto a copiar
    Persona(const Persona& otra) : 
        nombre(otra.nombre), edad(otra.edad), direccion(otra.direccion) {}
    
    // DESTRUCTOR - Se llama automáticamente cuando el objeto se destruye
    // virtual ~Persona(): virtual para permitir polimorfismo en herencia
    ~Persona() {
        cout << "Destruyendo persona: " << nombre << endl;
    }
    
    // MÉTODOS GETTER (ACCESORES) - Métodos para obtener valores privados
    // const string& getNombre() const: retorna referencia constante para eficiencia
    string getNombre() const { return nombre; }      // Obtener nombre
    int getEdad() const { return edad; }             // Obtener edad
    string getDireccion() const { return direccion; } // Obtener dirección
    
    // MÉTODOS SETTER (MODIFICADORES) - Métodos para modificar valores privados
    void setNombre(string n) { nombre = n; }         // Modificar nombre
    void setEdad(int e) { edad = e; }                // Modificar edad
    void setDireccion(string d) { direccion = d; }   // Modificar dirección
    
    // MÉTODOS DE COMPORTAMIENTO - Funcionalidad específica de la clase
    void saludar() const {
        // Método const: no modifica estado del objeto
        cout << "Hola, soy " << nombre << " y tengo " << edad << " años." << endl;
    }
    
    void cumplirAnios() {
        // Método no-const: modifica estado del objeto (edad++)
        edad++;  // Incrementar edad
        cout << nombre << " ahora tiene " << edad << " años." << endl;
    }
    
    // SOBRECARGA DE OPERADORES - Definir comportamiento de operadores
    // bool operator==(const Persona& otra) const: comparar igualdad
    bool operator==(const Persona& otra) const {
        // Comparar nombre y edad para determinar igualdad
        return nombre == otra.nombre && edad == otra.edad;
    }
    
    // SOBRECARGA DEL OPERADOR << - Permitir cout << objeto
    // friend: función no miembro con acceso a miembros privados
    // ostream& operator<<(ostream& os, const Persona& p): operador de inserción
    friend ostream& operator<<(ostream& os, const Persona& p) {
        // Formatear salida del objeto Persona
        os << "Persona(Nombre: " << p.nombre 
           << ", Edad: " << p.edad 
           << ", Dirección: " << p.direccion << ")";
        return os;  // Retornar stream para encadenamiento
    }
};

// EJEMPLO DE USO DE LA CLASE PERSONA
void ejemplo_clases_basicas() {
    cout << "=== EJEMPLO DE CLASES BÁSICAS ===" << endl;
    
    // CREACIÓN DE OBJETOS USANDO DIFERENTES CONSTRUCTORES
    Persona persona1;                           // Constructor por defecto
    Persona persona2("María López", 25, "Calle Principal 123");  // Constructor con parámetros
    Persona persona3(persona2);                 // Constructor de copia
    
    // USO DE MÉTODOS DE COMPORTAMIENTO
    persona1.saludar();  // Saludar con valores por defecto
    persona2.saludar();  // Saludar con valores proporcionados
    persona3.saludar();  // Saludar con valores copiados
    
    // MODIFICACIÓN DE ATRIBUTOS USANDO SETTERS
    persona1.setNombre("Juan Pérez");      // Modificar nombre
    persona1.setEdad(30);                  // Modificar edad
    persona1.setDireccion("Avenida Siempre Viva 456");  // Modificar dirección
    persona1.saludar();                    // Saludar con valores modificados
    
    // COMPARACIÓN DE OBJETOS USANDO OPERADOR SOBRECARGADO
    if(persona2 == persona3) {
        cout << "persona2 y persona3 son iguales" << endl;
    }
    
    // USO DEL OPERADOR SOBRECARGADO << PARA SALIDA
    cout << "Información de persona1: " << persona1 << endl;
    cout << "Información de persona2: " << persona2 << endl;
}
```

### **Herencia en C++**

```cpp
#include <iostream>
#include <vector>
using namespace std;

// CLASE BASE - Define comportamiento común para todas las derivadas
class Vehiculo {
protected:
    // MIEMBROS PROTEGIDOS - Accesibles desde clase base y derivadas
    string marca;       // Marca del vehículo
    string modelo;      // Modelo del vehículo
    int anio;           // Año del vehículo
    bool encendido;     // Estado de encendido/apagado

public:
    // CONSTRUCTOR DE CLASE BASE
    // Vehiculo(string m, string mod, int a): inicializar miembros protegidos
    Vehiculo(string m, string mod, int a) : 
        marca(m), modelo(mod), anio(a), encendido(false) {}
    
    // DESTRUCTOR VIRTUAL - MUY IMPORTANTE PARA POLIMORFISMO
    // virtual ~Vehiculo() = default;: permite destrucción correcta de derivadas
    virtual ~Vehiculo() = default;  // Destructor virtual para polimorfismo
    
    // MÉTODOS VIRTUALES - Pueden ser sobrescritos en clases derivadas
    // virtual void encender(): método virtual para encender vehículo
    virtual void encender() {
        encendido = true;  // Cambiar estado a encendido
        cout << marca << " " << modelo << " encendido." << endl;
    }
    
    // virtual void apagar(): método virtual para apagar vehículo
    virtual void apagar() {
        encendido = false;  // Cambiar estado a apagado
        cout << marca << " " << modelo << " apagado." << endl;
    }
    
    // virtual void mostrarInfo() const: método virtual para mostrar información
    virtual void mostrarInfo() const {
        cout << "Vehículo: " << marca << " " << modelo 
             << " (" << anio << ")" << endl;
    }
    
    // MÉTODOS GETTER - Acceso a miembros protegidos
    string getMarca() const { return marca; }
    string getModelo() const { return modelo; }
    int getAnio() const { return anio; }
    bool estaEncendido() const { return encendido; }
};

// CLASE DERIVADA - Hereda de Vehiculo y añade funcionalidad específica
class Coche : public Vehiculo {
private:
    // MIEMBROS PRIVADOS ESPECÍFICOS DE COCHE
    int numeroPuertas;        // Número de puertas del coche
    string tipoCombustible;   // Tipo de combustible del coche

public:
    // CONSTRUCTOR DE CLASE DERIVADA
    // Coche(...): Vehiculo(marca, modelo, anio): llamar constructor de base
    Coche(string marca, string modelo, int anio, int puertas, string combustible) :
        Vehiculo(marca, modelo, anio),  // Llamar constructor de clase base
        numeroPuertas(puertas),         // Inicializar miembros propios
        tipoCombustible(combustible) {}
    
    // SOBRESCRITURA DE MÉTODOS VIRTUALES
    // void encender() override: sobrescribir método virtual de base
    void encender() override {
        Vehiculo::encender();  // Llamar al método de la clase base
        cout << "Sistema de encendido del coche activado." << endl;
    }
    
    // void mostrarInfo() const override: sobrescribir método virtual de base
    void mostrarInfo() const override {
        Vehiculo::mostrarInfo();  // Llamar al método de la clase base
        cout << "Puertas: " << numeroPuertas 
             << ", Combustible: " << tipoCombustible << endl;
    }
    
    // MÉTODOS ESPECÍFICOS DE COCHE
    void abrirMaletero() {
        cout << "Maletero del " << marca << " abierto." << endl;
    }
    
    void cerrarMaletero() {
        cout << "Maletero del " << marca << " cerrado." << endl;
    }
};

// OTRA CLASE DERIVADA - Diferente tipo de vehículo
class Moto : public Vehiculo {
private:
    // MIEMBROS PRIVADOS ESPECÍFICOS DE MOTO
    int cilindrada;       // Cilindrada de la moto en cc
    bool tieneSidecar;    // Indica si tiene sidecar

public:
    // CONSTRUCTOR DE MOTO
    Moto(string marca, string modelo, int anio, int cil, bool sidecar) :
        Vehiculo(marca, modelo, anio),  // Llamar constructor de clase base
        cilindrada(cil),                // Inicializar miembros propios
        tieneSidecar(sidecar) {}
    
    // SOBRESCRITURA DE MÉTODOS VIRTUALES
    void encender() override {
        Vehiculo::encender();  // Llamar al método de la clase base
        cout << "Motor de la moto " << marca << " en marcha." << endl;
    }
    
    void mostrarInfo() const override {
        Vehiculo::mostrarInfo();  // Llamar al método de la clase base
        cout << "Cilindrada: " << cilindrada << "cc";
        if(tieneSidecar) {
            cout << ", Con sidecar";
        }
        cout << endl;
    }
    
    // MÉTODO ESPECÍFICO DE MOTO
    void hacerCaballito() {
        if(estaEncendido()) {  // Verificar que esté encendida
            cout << "¡Haciendo caballito con la " << marca << "!" << endl;
        } else {
            cout << "Primero enciende la moto." << endl;
        }
    }
};

// EJEMPLO DE HERENCIA Y POLIMORFISMO
void ejemplo_herencia() {
    cout << "\n=== EJEMPLO DE HERENCIA ===" << endl;
    
    // CREACIÓN DE OBJETOS DE CLASES DERIVADAS
    Coche miCoche("Toyota", "Corolla", 2020, 4, "Gasolina");
    Moto miMoto("Yamaha", "R1", 2021, 1000, false);
    
    // POLIMORFISMO: USO DE APUNTADORES A LA CLASE BASE
    // vector<Vehiculo*> vehiculos: vector de apuntadores a Vehiculo base
    vector<Vehiculo*> vehiculos;
    vehiculos.push_back(&miCoche);  // Agregar dirección de coche
    vehiculos.push_back(&miMoto);   // Agregar dirección de moto
    
    // LLAMADAS POLIMÓRFICAS - Mismo código, comportamiento diferente
    cout << "\nInformación de vehículos:" << endl;
    for(Vehiculo* v : vehiculos) {
        v->mostrarInfo();  // Llama al método correcto según tipo real
        v->encender();     // Llama al método correcto según tipo real
        cout << endl;
    }
    
    // USO DE MÉTODOS ESPECÍFICOS (NECESITA CASTING O APUNTADORES ESPECÍFICOS)
    miCoche.abrirMaletero();  // Método específico de Coche
    miMoto.hacerCaballito();  // Método específico de Moto
}
```

---

## 📦 6. Colecciones y Contenedores en C++

### **STL - Standard Template Library**

```cpp
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>
#include <string>
using namespace std;

// EJEMPLO DE VECTOR (ARREGLO DINÁMICO)
void ejemplo_vector() {
    cout << "\n=== EJEMPLO DE VECTOR ===" << endl;
    
    // DECLARACIÓN E INICIALIZACIÓN DE VECTORES
    // vector<int> numeros = {...}: vector inicializado con valores
    vector<int> numeros = {1, 2, 3, 4, 5};
    // vector<string> nombres(3, "Desconocido"): 3 elementos con valor "Desconocido"
    vector<string> nombres(3, "Desconocido");
    // vector<double> decimales;: vector vacío
    vector<double> decimales;
    
    // AGREGAR ELEMENTOS AL FINAL DEL VECTOR
    // push_back(elemento): añade elemento al final
    numeros.push_back(6);  // Añadir 6 al final
    numeros.push_back(7);  // Añadir 7 al final
    
    // ACCESO A ELEMENTOS DEL VECTOR
    // front(): primer elemento del vector
    cout << "Primer elemento: " << numeros.front() << endl;
    // back(): último elemento del vector
    cout << "Último elemento: " << numeros.back() << endl;
    // numeros[2]: acceso por índice (posición 2)
    cout << "Elemento en posición 2: " << numeros[2] << endl;
    
    // ITERACIÓN SOBRE ELEMENTOS DEL VECTOR
    cout << "Elementos del vector: ";
    // for(const auto& num : numeros): range-based for loop (C++11)
    for(const auto& num : numeros) {
        cout << num << " ";
    }
    cout << endl;
    
    // TAMAÑO Y CAPACIDAD DEL VECTOR
    // size(): número de elementos actuales
    cout << "Tamaño: " << numeros.size() << endl;
    // capacity(): capacidad total asignada (puede ser mayor que size)
    cout << "Capacidad: " << numeros.capacity() << endl;
    
    // ELIMINAR ELEMENTOS DEL VECTOR
    // pop_back(): elimina el último elemento
    numeros.pop_back();  // Elimina el 7
    // erase(iterador): elimina elemento en posición específica
    numeros.erase(numeros.begin() + 1);  // Elimina el segundo elemento (índice 1)
    
    cout << "Después de eliminar: ";
    for(size_t i = 0; i < numeros.size(); i++) {
        cout << numeros[i] << " ";
    }
    cout << endl;
    
    // ORDENAR VECTOR
    // sort(inicio, fin): ordena elementos en rango
    sort(numeros.begin(), numeros.end());  // Orden ascendente
    cout << "Vector ordenado: ";
    for(const auto& num : numeros) {
        cout << num << " ";
    }
    cout << endl;
}

// EJEMPLO DE LIST (LISTA DOBLEMENTE ENLAZADA)
void ejemplo_list() {
    cout << "\n=== EJEMPLO DE LIST ===" << endl;
    
    // DECLARACIÓN E INICIALIZACIÓN DE LISTA
    list<int> lista = {10, 20, 30, 40, 50};
    
    // AGREGAR ELEMENTOS EN DIFERENTES POSICIONES
    // push_front(elemento): añade al inicio de la lista
    lista.push_front(5);    // Añadir 5 al inicio
    // push_back(elemento): añade al final de la lista
    lista.push_back(60);    // Añadir 60 al final
    // insert(iterador, elemento): inserta en posición específica
    lista.insert(++lista.begin(), 15);  // Insertar 15 después del primer elemento
    
    // ITERACIÓN SOBRE ELEMENTOS DE LA LISTA
    cout << "Elementos de la lista: ";
    for(const auto& elem : lista) {
        cout << elem << " ";
    }
    cout << endl;
    
    // ELIMINAR ELEMENTOS DE LA LISTA
    // remove(valor): elimina todos los elementos con ese valor
    lista.remove(20);       // Elimina todos los elementos con valor 20
    // pop_front(): elimina el primer elemento
    lista.pop_front();      // Elimina el primer elemento (5)
    // pop_back(): elimina el último elemento
    lista.pop_back();       // Elimina el último elemento (60)
    
    cout << "Después de eliminar: ";
    for(const auto& elem : lista) {
        cout << elem << " ";
    }
    cout << endl;
}

// EJEMPLO DE MAP (DICCIONARIO CLAVE-VALOR)
void ejemplo_map() {
    cout << "\n=== EJEMPLO DE MAP ===" << endl;
    
    // DECLARACIÓN E INICIALIZACIÓN DE MAP
    // map<string, int> edades;: mapa de string a int (clave-valor)
    map<string, int> edades;
    // map<string, string> telefonos = {...}: inicialización con pares
    map<string, string> telefonos = {
        {"Juan", "123-456-7890"},      // Par clave-valor
        {"María", "098-765-4321"},     // Par clave-valor
        {"Pedro", "555-123-4567"}      // Par clave-valor
    };
    
    // AGREGAR ELEMENTOS AL MAP
    // mapa[clave] = valor: inserta o actualiza par clave-valor
    edades["Ana"] = 25;                // Insertar par "Ana" -> 25
    edades["Carlos"] = 30;             // Insertar par "Carlos" -> 30
    edades["Elena"] = 28;              // Insertar par "Elena" -> 28
    // insert(make_pair(clave, valor)): insertar par usando make_pair
    edades.insert(make_pair("David", 35));  // Insertar par "David" -> 35
    
    // ACCESO A ELEMENTOS DEL MAP
    // mapa[clave]: acceso al valor asociado con clave
    cout << "Teléfono de María: " << telefonos["María"] << endl;
    cout << "Edad de Ana: " << edades["Ana"] << endl;
    
    // VERIFICAR EXISTENCIA DE CLAVE EN MAP
    // find(clave): busca clave y retorna iterador
    if(edades.find("Carlos") != edades.end()) {
        // end(): iterador que apunta después del último elemento
        cout << "Carlos tiene " << edades["Carlos"] << " años." << endl;
    }
    
    // ITERACIÓN SOBRE ELEMENTOS DEL MAP
    cout << "Lista de edades:" << endl;
    // for(const auto& par : mapa): iterar sobre pares clave-valor
    for(const auto& par : edades) {
        // par.first: clave, par.second: valor
        cout << par.first << ": " << par.second << " años" << endl;
    }
    
    // ELIMINAR ELEMENTOS DEL MAP
    // erase(clave): elimina elemento con clave específica
    edades.erase("Elena");
    cout << "Después de eliminar a Elena:" << endl;
    for(const auto& par : edades) {
        cout << par.first << ": " << par.second << " años" << endl;
    }
}

// EJEMPLO DE SET (CONJUNTO ORDENADO SIN DUPLICADOS)
void ejemplo_set() {
    cout << "\n=== EJEMPLO DE SET ===" << endl;
    
    // DECLARACIÓN E INICIALIZACIÓN DE SET
    // set<int> numeros = {...}: elementos duplicados se eliminan automáticamente
    set<int> numeros = {5, 2, 8, 1, 9, 3, 7, 2, 5};
    // Resultado: {1, 2, 3, 5, 7, 8, 9} - ordenados y sin duplicados
    
    cout << "Elementos únicos ordenados: ";
    for(const auto& num : numeros) {
        cout << num << " ";
    }
    cout << endl;
    
    // AGREGAR ELEMENTOS AL SET
    // insert(elemento): añade elemento si no existe
    numeros.insert(4);   // Añade 4
    numeros.insert(6);   // Añade 6
    numeros.insert(1);   // No se añade (ya existe)
    
    cout << "Después de insertar: ";
    for(const auto& num : numeros) {
        cout << num << " ";
    }
    cout << endl;
    
    // BUSCAR ELEMENTOS EN EL SET
    // find(elemento): busca elemento y retorna iterador
    if(numeros.find(7) != numeros.end()) {
        cout << "El número 7 está en el conjunto." << endl;
    }
    
    // ELIMINAR ELEMENTOS DEL SET
    // erase(elemento): elimina elemento específico
    numeros.erase(3);
    cout << "Después de eliminar el 3: ";
    for(const auto& num : numeros) {
        cout << num << " ";
    }
    cout << endl;
}

// EJEMPLO DE QUEUE (COLA) Y STACK (PILA)
void ejemplo_queue_stack() {
    cout << "\n=== EJEMPLO DE QUEUE Y STACK ===" << endl;
    
    // QUEUE (FIFO - FIRST IN, FIRST OUT)
    // queue<string> cola;: cola de strings
    queue<string> cola;
    // push(elemento): añade elemento al final de la cola
    cola.push("Primero");   // Añadir "Primero"
    cola.push("Segundo");   // Añadir "Segundo"
    cola.push("Tercero");   // Añadir "Tercero"
    
    cout << "Queue (FIFO):" << endl;
    // while(!cola.empty()): mientras la cola no esté vacía
    while(!cola.empty()) {
        // front(): obtener primer elemento
        cout << "Atendiendo: " << cola.front() << endl;
        // pop(): eliminar primer elemento
        cola.pop();
    }
    
    // STACK (LIFO - LAST IN, FIRST OUT)
    // stack<int> pila;: pila de enteros
    stack<int> pila;
    // push(elemento): añade elemento a la cima de la pila
    pila.push(1);  // Añadir 1
    pila.push(2);  // Añadir 2
    pila.push(3);  // Añadir 3
    pila.push(4);  // Añadir 4
    
    cout << "\nStack (LIFO):" << endl;
    // while(!pila.empty()): mientras la pila no esté vacía
    while(!pila.empty()) {
        // top(): obtener elemento de la cima
        cout << "Sacando: " << pila.top() << endl;
        // pop(): eliminar elemento de la cima
        pila.pop();
    }
}
```

---

## 🎯 7. Ejemplos Prácticos Completos

### **Ejemplo 1: Sistema de Gestión de Estudiantes**

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
using namespace std;

// CLASE ESTUDIANTE CON GESTIÓN DE CALIFICACIONES
class Estudiante {
private:
    int id;                        // Identificador único del estudiante
    string nombre;                 // Nombre del estudiante
    vector<double> calificaciones; // Vector de calificaciones del estudiante

public:
    // CONSTRUCTOR CON ID Y NOMBRE
    Estudiante(int i, string n) : id(i), nombre(n) {}
    
    // GETTERS - MÉTODOS DE ACCESO A MIEMBROS PRIVADOS
    int getId() const { return id; }  // Obtener ID
    string getNombre() const { return nombre; }  // Obtener nombre
    const vector<double>& getCalificaciones() const { return calificaciones; }  // Obtener calificaciones
    
    // MÉTODOS PARA MANEJAR CALIFICACIONES
    void agregarCalificacion(double cal) {
        // VALIDAR QUE LA CALIFICACIÓN ESTÉ EN RANGO VÁLIDO (0-10)
        if(cal >= 0 && cal <= 10) {
            calificaciones.push_back(cal);  // Añadir calificación al vector
        }
    }
    
    // CALCULAR PROMEDIO DE CALIFICACIONES
    double getPromedio() const {
        if(calificaciones.empty()) return 0.0;  // Retornar 0 si no hay calificaciones
        double suma = 0;
        // SUMAR TODAS LAS CALIFICACIONES
        for(double cal : calificaciones) {
            suma += cal;
        }
        // RETORNAR PROMEDIO (SUMA / CANTIDAD)
        return suma / calificaciones.size();
    }
    
    // MOSTRAR INFORMACIÓN DEL ESTUDIANTE
    void mostrarInfo() const {
        cout << "ID: " << id << ", Nombre: " << nombre 
             << ", Promedio: " << fixed << setprecision(2) << getPromedio() << endl;
    }
};

// CLASE SISTEMA DE GESTIÓN DE ESTUDIANTES
class SistemaGestion {
private:
    vector<Estudiante> estudiantes;  // Vector de estudiantes gestionados

public:
    // AGREGAR NUEVO ESTUDIANTE AL SISTEMA
    void agregarEstudiante(int id, string nombre) {
        // emplace_back: construye objeto directamente en el vector (más eficiente)
        estudiantes.emplace_back(id, nombre);
    }
    
    // BUSCAR ESTUDIANTE POR ID
    Estudiante* buscarEstudiante(int id) {
        // RECORRER VECTOR BUSCANDO ESTUDIANTE CON ID ESPECIFICADO
        for(auto& est : estudiantes) {
            if(est.getId() == id) {
                return &est;  // Retornar apuntador al estudiante encontrado
            }
        }
        return nullptr;  // Retornar nullptr si no se encuentra
    }
    
    // MOSTRAR TODOS LOS ESTUDIANTES
    void mostrarTodos() const {
        cout << "\n=== LISTA DE ESTUDIANTES ===" << endl;
        // ITERAR SOBRE TODOS LOS ESTUDIANTES Y MOSTRAR SU INFORMACIÓN
        for(const auto& est : estudiantes) {
            est.mostrarInfo();
        }
    }
    
    // MOSTRAR N MEJORES PROMEDIOS
    void mostrarMejoresPromedios(int n) const {
        // CREAR COPIA DEL VECTOR PARA ORDENAR (NO MODIFICAR ORIGINAL)
        vector<Estudiante> copia = estudiantes;
        
        // ORDENAR COPIA POR PROMEDIO DESCENDENTE
        sort(copia.begin(), copia.end(), 
             [](const Estudiante& a, const Estudiante& b) {
                 // FUNCIÓN LAMBDA PARA COMPARAR PROMEDIOS
                 return a.getPromedio() > b.getPromedio();
             });
        
        cout << "\n=== MEJORES PROMEDIOS (Top " << n << ") ===" << endl;
        // MOSTRAR LOS PRIMEROS N ESTUDIANTES (O MENOS SI HAY MENOS ESTUDIANTES)
        int count = min(n, static_cast<int>(copia.size()));
        for(int i = 0; i < count; i++) {
            cout << (i + 1) << ". ";
            copia[i].mostrarInfo();
        }
    }
};

// EJEMPLO DE USO DEL SISTEMA DE GESTIÓN DE ESTUDIANTES
void ejemplo_sistema_estudiantes() {
    cout << "\n=== SISTEMA DE GESTIÓN DE ESTUDIANTES ===" << endl;
    
    SistemaGestion sistema;
    
    // AGREGAR ESTUDIANTES AL SISTEMA
    sistema.agregarEstudiante(1, "Ana Martínez");
    sistema.agregarEstudiante(2, "Carlos Ruiz");
    sistema.agregarEstudiante(3, "Elena García");
    sistema.agregarEstudiante(4, "David Silva");
    
    // AGREGAR CALIFICACIONES A LOS ESTUDIANTES
    sistema.buscarEstudiante(1)->agregarCalificacion(9.5);
    sistema.buscarEstudiante(1)->agregarCalificacion(8.7);
    sistema.buscarEstudiante(1)->agregarCalificacion(9.2);
    
    sistema.buscarEstudiante(2)->agregarCalificacion(7.8);
    sistema.buscarEstudiante(2)->agregarCalificacion(8.5);
    sistema.buscarEstudiante(2)->agregarCalificacion(8.0);
    
    sistema.buscarEstudiante(3)->agregarCalificacion(9.8);
    sistema.buscarEstudiante(3)->agregarCalificacion(9.6);
    sistema.buscarEstudiante(3)->agregarCalificacion(9.9);
    
    sistema.buscarEstudiante(4)->agregarCalificacion(8.2);
    sistema.buscarEstudiante(4)->agregarCalificacion(7.9);
    sistema.buscarEstudiante(4)->agregarCalificacion(8.4);
    
    // MOSTRAR RESULTADOS DEL SISTEMA
    sistema.mostrarTodos();              // Mostrar todos los estudiantes
    sistema.mostrarMejoresPromedios(3);  // Mostrar top 3 promedios
}
```

### **Ejemplo 2: Sistema de Biblioteca con Apuntadores**

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// DEFINICIÓN DE ESTRUCTURA LIBRO CON APUNTADORES
typedef struct Libro {
    int id;                    // Identificador único del libro
    char titulo[100];          // Título del libro (máximo 99 caracteres)
    char autor[50];            // Autor del libro (máximo 49 caracteres)
    int disponible;            // Estado: 1 = disponible, 0 = prestado
    struct Libro* siguiente;   // Apuntador al siguiente libro (lista enlazada)
} Libro;

// DEFINICIÓN DE ESTRUCTURA BIBLIOTECA
typedef struct {
    Libro* cabeza;        // Apuntador al primer libro de la lista
    int total_libros;     // Contador total de libros en la biblioteca
} Biblioteca;

// FUNCIONES PARA MANEJAR LA BIBLIOTECA

// CREAR NUEVA BIBLIOTECA VACÍA
Biblioteca* crear_biblioteca() {
    // malloc(sizeof(Biblioteca)): asignar memoria para estructura Biblioteca
    Biblioteca* b = (Biblioteca*)malloc(sizeof(Biblioteca));
    if(b != NULL) {
        // INICIALIZAR MIEMBROS DE LA BIBLIOTECA
        b->cabeza = NULL;      // No hay libros inicialmente
        b->total_libros = 0;   // Contador en cero
    }
    return b;  // Retornar apuntador a la biblioteca creada
}

// AGREGAR NUEVO LIBRO A LA BIBLIOTECA
void agregar_libro(Biblioteca* b, int id, const char* titulo, const char* autor) {
    // malloc(sizeof(Libro)): asignar memoria para nuevo libro
    Libro* nuevo = (Libro*)malloc(sizeof(Libro));
    if(nuevo != NULL) {
        // INICIALIZAR MIEMBROS DEL NUEVO LIBRO
        nuevo->id = id;                           // Asignar ID
        strncpy(nuevo->titulo, titulo, 99);       // Copiar título (seguro)
        strncpy(nuevo->autor, autor, 49);         // Copiar autor (seguro)
        nuevo->titulo[99] = '\0';                 // Asegurar null terminator
        nuevo->autor[49] = '\0';                  // Asegurar null terminator
        nuevo->disponible = 1;                    // Marcar como disponible
        nuevo->siguiente = b->cabeza;             // Enlazar con lista existente
        b->cabeza = nuevo;                        // Nuevo libro es la cabeza
        b->total_libros++;                        // Incrementar contador
    }
}

// MOSTRAR TODOS LOS LIBROS DE LA BIBLIOTECA
void mostrar_libros(const Biblioteca* b) {
    printf("\n=== CATÁLOGO DE LIBROS ===\n");
    // RECORRER LISTA ENLAZADA DE LIBROS
    Libro* actual = b->cabeza;  // Comenzar desde la cabeza
    while(actual != NULL) {
        // MOSTRAR INFORMACIÓN DE CADA LIBRO
        printf("ID: %d, Título: %s, Autor: %s, %s\n",
               actual->id,                    // ID del libro
               actual->titulo,                // Título del libro
               actual->autor,                 // Autor del libro
               actual->disponible ? "Disponible" : "Prestado");  // Estado
        actual = actual->siguiente;  // Mover al siguiente libro
    }
    printf("Total de libros: %d\n", b->total_libros);
}

// BUSCAR LIBRO POR ID
Libro* buscar_libro(const Biblioteca* b, int id) {
    // RECORRER LISTA BUSCANDO LIBRO CON ID ESPECIFICADO
    Libro* actual = b->cabeza;
    while(actual != NULL) {
        if(actual->id == id) {
            return actual;  // Retornar apuntador al libro encontrado
        }
        actual = actual->siguiente;  // Mover al siguiente libro
    }
    return NULL;  // Retornar NULL si no se encuentra
}

// PRESTAR LIBRO (MARCAR COMO NO DISPONIBLE)
int prestar_libro(Biblioteca* b, int id) {
    Libro* libro = buscar_libro(b, id);  // Buscar libro por ID
    if(libro != NULL && libro->disponible) {
        libro->disponible = 0;  // Marcar como prestado
        return 1;               // Retornar éxito (1)
    }
    return 0;  // Retornar error (0) si no se puede prestar
}

// DEVOLVER LIBRO (MARCAR COMO DISPONIBLE)
int devolver_libro(Biblioteca* b, int id) {
    Libro* libro = buscar_libro(b, id);  // Buscar libro por ID
    if(libro != NULL && !libro->disponible) {
        libro->disponible = 1;  // Marcar como disponible
        return 1;               // Retornar éxito (1)
    }
    return 0;  // Retornar error (0) si no se puede devolver
}

// LIBERAR TODA LA MEMORIA DE LA BIBLIOTECA
void liberar_biblioteca(Biblioteca* b) {
    Libro* actual = b->cabeza;
    // RECORRER Y LIBERAR CADA LIBRO DE LA LISTA
    while(actual != NULL) {
        Libro* temp = actual;        // Guardar apuntador actual
        actual = actual->siguiente;  // Mover al siguiente antes de liberar
        free(temp);                  // Liberar memoria del libro actual
    }
    free(b);  // Liberar memoria de la estructura Biblioteca
}

// EJEMPLO DE USO DEL SISTEMA DE BIBLIOTECA EN C
void ejemplo_biblioteca_c() {
    printf("\n=== SISTEMA DE BIBLIOTECA EN C ===\n");
    
    Biblioteca* mi_biblioteca = crear_biblioteca();  // Crear biblioteca vacía
    
    // AGREGAR LIBROS A LA BIBLIOTECA
    agregar_libro(mi_biblioteca, 1, "Cien años de soledad", "Gabriel García Márquez");
    agregar_libro(mi_biblioteca, 2, "Don Quijote de la Mancha", "Miguel de Cervantes");
    agregar_libro(mi_biblioteca, 3, "1984", "George Orwell");
    agregar_libro(mi_biblioteca, 4, "El principito", "Antoine de Saint-Exupéry");
    
    // MOSTRAR CATÁLOGO INICIAL
    mostrar_libros(mi_biblioteca);
    
    // PRESTAR LIBROS
    printf("\nPrestando libros...\n");
    if(prestar_libro(mi_biblioteca, 1)) {
        printf("Libro 1 prestado exitosamente.\n");
    }
    if(prestar_libro(mi_biblioteca, 3)) {
        printf("Libro 3 prestado exitosamente.\n");
    }
    
    // MOSTRAR ESTADO ACTUAL
    mostrar_libros(mi_biblioteca);
    
    // DEVOLVER LIBROS
    printf("\nDevolviendo libros...\n");
    if(devolver_libro(mi_biblioteca, 1)) {
        printf("Libro 1 devuelto exitosamente.\n");
    }
    
    // MOSTRAR ESTADO FINAL
    mostrar_libros(mi_biblioteca);
    
    // LIBERAR MEMORIA DE LA BIBLIOTECA
    liberar_biblioteca(mi_biblioteca);
}
```

---

## 📚 Resumen de Conceptos Clave

### **Apuntadores**
- **Ventajas**: Control directo de memoria, eficiencia, estructuras dinámicas
- **Usos**: Listas enlazadas, árboles, gestión de memoria dinámica
- **Precauciones**: Dangling pointers, memory leaks, segmentation faults

### **Gestión de Memoria**
- **C**: malloc/calloc/realloc/free
- **C++**: new/new[]/delete/delete[] y smart pointers
- **Buenas prácticas**: RAII, inicializar a nullptr, verificar asignación

### **Estructuras/Clases**
- **Struct**: Datos agrupados, acceso público por defecto
- **Class**: Datos + métodos, encapsulación, herencia, polimorfismo

### **Contenedores STL**
- **Vector**: Arreglo dinámico, acceso aleatorio rápido
- **List**: Lista doblemente enlazada, inserciones/eliminaciones eficientes
- **Map**: Diccionario, pares clave-valor ordenados
- **Set**: Conjunto ordenado sin duplicados
- **Queue/Stack**: Colas y pilas para estructuras FIFO/LIFO

### **Errores Comunes en Memoria**
1. **Memory Leaks**: No liberar memoria asignada
2. **Dangling Pointers**: Usar apuntadores después de liberar memoria
3. **Buffer Overflows**: Escribir fuera de los límites asignados
4. **Double Free**: Liberar la misma memoria dos veces
5. **Use After Free**: Usar memoria después de liberarla

### **Buenas Prácticas**
1. **Gestión de Memoria**: Siempre liberar memoria asignada dinámicamente
2. **Inicialización**: Inicializar apuntadores a NULL/nullptr
3. **Validación**: Verificar NULL antes de desreferenciar
4. **Encapsulación**: Usar getters/setters en C++
5. **RAII**: Resource Acquisition Is Initialization en C++
6. **Smart Pointers**: Usar unique_ptr, shared_ptr en C++ moderno

---

## 🎯 Conclusión

Esta guía cubre los conceptos fundamentales de C y C++ con énfasis en:
- **Apuntadores**: Control preciso de memoria
- **Gestión de Memoria**: Asignación y liberación segura
- **Estructuras/Clases**: Organización de datos complejos
- **Colecciones**: Manejo eficiente de datos
- **Integración**: Uso conjunto de C y C++

La gestión de memoria es crucial para el desarrollo eficiente y seguro en ambos lenguajes. Mientras C requiere manejo manual explícito, C++ ofrece herramientas modernas como smart pointers que facilitan la gestión automática y segura de recursos.
