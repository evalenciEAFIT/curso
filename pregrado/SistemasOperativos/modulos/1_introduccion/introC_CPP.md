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

---

## 💾 3. Gestión de Memoria Dinámica

### **Malloc, Calloc y Free en C**

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// EJEMPLO COMPLETO DE GESTIÓN DE MEMORIA EN C
void ejemplo_memoria_dinamica_c() {
    printf("=== GESTIÓN DE MEMORIA DINÁMICA EN C ===\n\n");
    
    // 1. malloc() - ASIGNACIÓN DE MEMORIA SIN INICIALIZAR
    // malloc(sizeof(int)): solicita memoria para un entero
    // (int*)malloc(sizeof(int)): casteo a apuntador a entero
    printf("1. USO DE malloc():\n");
    int *ptr_entero = (int*)malloc(sizeof(int));
    if(ptr_entero == NULL) {
        // VERIFICACIÓN DE ERROR: malloc retorna NULL si no hay memoria
        printf("Error: No se pudo asignar memoria\n");
        return;
    }
    *ptr_entero = 100;  // Asignar valor al espacio de memoria reservado
    printf("Valor asignado con malloc: %d\n", *ptr_entero);
    
    // 2. calloc() - ASIGNACIÓN E INICIALIZACIÓN A CERO
    // calloc(tamano, sizeof(int)): asigna tamano*sizeof(int) bytes inicializados a 0
    printf("\n2. USO DE calloc():\n");
    int tamano = 5;
    int *ptr_arreglo = (int*)calloc(tamano, sizeof(int));
    if(ptr_arreglo == NULL) {
        printf("Error: No se pudo asignar memoria\n");
        free(ptr_entero);  // Liberar memoria previa antes de salir
        return;
    }
    
    printf("Arreglo inicializado con calloc: ");
    for(int i = 0; i < tamano; i++) {
        printf("%d ", ptr_arreglo[i]);  // Todos deben ser 0 por calloc
    }
    printf("\n");
    
    // MODIFICAR VALORES DEL ARREGLO
    for(int i = 0; i < tamano; i++) {
        ptr_arreglo[i] = (i + 1) * 10;  // Asignar valores 10, 20, 30, 40, 50
    }
    printf("Arreglo modificado: ");
    for(int i = 0; i < tamano; i++) {
        printf("%d ", ptr_arreglo[i]);
    }
    printf("\n");
    
    // 3. realloc() - REASIGNAR MEMORIA (EXPANDIR O REDUCIR)
    // realloc(ptr_arreglo, tamano * sizeof(int)): cambia tamaño del bloque
    printf("\n3. USO DE realloc():\n");
    tamano = 10;  // Queremos expandir a 10 elementos
    ptr_arreglo = (int*)realloc(ptr_arreglo, tamano * sizeof(int));
    if(ptr_arreglo == NULL) {
        printf("Error: No se pudo reasignar memoria\n");
        free(ptr_entero);
        return;
    }
    
    // COMPLETAR NUEVOS ELEMENTOS
    for(int i = 5; i < tamano; i++) {
        ptr_arreglo[i] = (i + 1) * 10;  // Asignar valores 60, 70, 80, 90, 100
    }
    printf("Arreglo expandido: ");
    for(int i = 0; i < tamano; i++) {
        printf("%d ", ptr_arreglo[i]);
    }
    printf("\n");
    
    // 4. LIBERAR MEMORIA - MUY IMPORTANTE PARA EVITAR MEMORY LEAKS
    // free(ptr): libera el bloque de memoria apuntado por ptr
    printf("\n4. LIBERANDO MEMORIA:\n");
    free(ptr_entero);       // Liberar memoria de entero individual
    free(ptr_arreglo);      // Liberar memoria del arreglo
    ptr_entero = NULL;      // Buena práctica: evitar dangling pointers
    ptr_arreglo = NULL;     // Buena práctica: evitar dangling pointers
    printf("Memoria liberada correctamente\n");
}

// EJEMPLO DE MANEJO DE STRINGS DINÁMICOS
void ejemplo_strings_dinamicos() {
    printf("\n=== STRINGS DINÁMICOS ===\n");
    
    // ASIGNAR MEMORIA PARA UN STRING DINÁMICO
    // malloc(50 * sizeof(char)): espacio para 50 caracteres
    char *nombre = (char*)malloc(50 * sizeof(char));
    if(nombre == NULL) {
        printf("Error de memoria\n");
        return;
    }
    
    // COPIAR STRING A MEMORIA DINÁMICA
    // strcpy(destino, origen): copia string origen a destino
    strcpy(nombre, "Juan Pérez");
    printf("Nombre: %s\n", nombre);
    
    // REASIGNAR PARA UN STRING MÁS LARGO
    // realloc(nombre, 100 * sizeof(char)): expandir a 100 caracteres
    nombre = (char*)realloc(nombre, 100 * sizeof(char));
    if(nombre == NULL) {
        printf("Error de realloc\n");
        return;
    }
    
    // CONCATENAR STRING
    // strcat(destino, origen): concatena origen al final de destino
    strcat(nombre, " - Desarrollador Senior");
    printf("Nombre actualizado: %s\n", nombre);
    
    // LIBERAR MEMORIA DEL STRING
    free(nombre);
    nombre = NULL;  // Evitar dangling pointer
    printf("String dinámico liberado\n");
}

// EJEMPLO DE MATRICES DINÁMICAS
void ejemplo_matrices_dinamicas() {
    printf("\n=== MATRICES DINÁMICAS ===\n");
    
    int filas = 3, columnas = 4;
    
    // MÉTODO 1: MATRIZ DE APUNTADORES A FILAS
    // malloc(filas * sizeof(int*)): array de apuntadores a filas
    int **matriz = (int**)malloc(filas * sizeof(int*));
    if(matriz == NULL) {
        printf("Error asignando filas\n");
        return;
    }
    
    // ASIGNAR CADA FILA INDIVIDUALMENTE
    for(int i = 0; i < filas; i++) {
        matriz[i] = (int*)malloc(columnas * sizeof(int));  // Asignar columna i
        if(matriz[i] == NULL) {
            printf("Error asignando columna %d\n", i);
            // LIBERAR MEMORIA PREVIAMENTE ASIGNADA EN CASO DE ERROR
            for(int j = 0; j < i; j++) {
                free(matriz[j]);  // Liberar filas ya asignadas
            }
            free(matriz);         // Liberar array de apuntadores
            return;
        }
    }
    
    // INICIALIZAR MATRIZ CON VALORES SECUENCIALES
    int valor = 1;
    for(int i = 0; i < filas; i++) {
        for(int j = 0; j < columnas; j++) {
            matriz[i][j] = valor++;  // Asignar valores 1, 2, 3, ...
        }
    }
    
    // IMPRIMIR MATRIZ
    printf("Matriz %dx%d:\n", filas, columnas);
    for(int i = 0; i < filas; i++) {
        for(int j = 0; j < columnas; j++) {
            printf("%3d ", matriz[i][j]);  // Formato de 3 caracteres
        }
        printf("\n");
    }
    
    // LIBERAR MEMORIA (IMPORTANTE: EN ORDEN INVERSO)
    // Primero liberar cada fila individualmente
    for(int i = 0; i < filas; i++) {
        free(matriz[i]);  // Liberar cada fila
    }
    // Luego liberar el array de apuntadores
    free(matriz);         // Liberar array de apuntadores a filas
    matriz = NULL;        // Evitar dangling pointer
    printf("Matriz dinámica liberada\n");
}
```

### **New y Delete en C++**

```cpp
#include <iostream>
#include <new>  // Para std::bad_alloc - manejo de errores de memoria
using namespace std;

// EJEMPLO COMPLETO DE GESTIÓN DE MEMORIA EN C++
void ejemplo_memoria_dinamica_cpp() {
    cout << "=== GESTIÓN DE MEMORIA DINÁMICA EN C++ ===" << endl << endl;
    
    try {
        // 1. new - ASIGNACIÓN DE MEMORIA PARA TIPOS BÁSICOS
        // new int(42): asigna memoria para int e inicializa con 42
        // new double: asigna memoria para double (valor indeterminado)
        cout << "1. USO DE new PARA TIPOS BÁSICOS:" << endl;
        int *ptr_entero = new int(42);  // Asignar e inicializar con 42
        double *ptr_double = new double;  // Solo asignar (valor indeterminado)
        *ptr_double = 3.14159;  // Asignar valor después
        
        cout << "Entero: " << *ptr_entero << endl;
        cout << "Double: " << *ptr_double << endl;
        
        // 2. new[] - ASIGNACIÓN DE ARREGLOS DINÁMICOS
        // new int[tamano]: asigna memoria para arreglo de tamano enteros
        // new int[tamano]{1, 2, 3, 4, 5}: inicialización uniforme
        cout << "\n2. USO DE new[] PARA ARREGLOS:" << endl;
        int tamano = 5;
        int *ptr_arreglo = new int[tamano]{1, 2, 3, 4, 5};  // Inicialización uniforme
        
        cout << "Arreglo: ";
        for(int i = 0; i < tamano; i++) {
            cout << ptr_arreglo[i] << " ";
        }
        cout << endl;
        
        // MODIFICAR VALORES DEL ARREGLO
        for(int i = 0; i < tamano; i++) {
            ptr_arreglo[i] *= 2;  // Multiplicar cada elemento por 2
        }
        cout << "Arreglo modificado: ";
        for(int i = 0; i < tamano; i++) {
            cout << ptr_arreglo[i] << " ";
        }
        cout << endl;
        
        // 3. delete y delete[] - LIBERAR MEMORIA
        // delete ptr: libera memoria asignada con new
        // delete[] ptr_arreglo: libera memoria de arreglo asignado con new[]
        cout << "\n3. LIBERANDO MEMORIA:" << endl;
        delete ptr_entero;      // Para memoria asignada con new (objeto único)
        delete ptr_double;      // Para memoria asignada con new (objeto único)
        delete[] ptr_arreglo;   // Para arreglos asignados con new[] (múltiples objetos)
        
        ptr_entero = nullptr;   // Buena práctica en C++ (nullptr en lugar de NULL)
        ptr_double = nullptr;
        ptr_arreglo = nullptr;
        cout << "Memoria liberada correctamente" << endl;
        
    } catch(const bad_alloc& e) {
        // MANEJO DE EXCEPCIONES: capturar errores de asignación de memoria
        cout << "Error de asignación de memoria: " << e.what() << endl;
    }
}

// EJEMPLO DE CLASES CON MEMORIA DINÁMICA
class GestorStrings {
private:
    char* datos;      // Apuntador a datos dinámicos
    size_t longitud;  // Longitud del string

public:
    // CONSTRUCTOR
    // GestorStrings(const char* str = ""): constructor con parámetro por defecto
    GestorStrings(const char* str = "") {
        longitud = strlen(str);  // Calcular longitud del string de entrada
        datos = new char[longitud + 1];  // +1 para el null terminator '\0'
        strcpy(datos, str);      // Copiar string a memoria dinámica
        cout << "Constructor llamado para: " << datos << endl;
    }
    
    // CONSTRUCTOR DE COPIA - MUY IMPORTANTE PARA EVITAR SHALLOW COPY
    // GestorStrings(const GestorStrings& otro): copia profunda del objeto
    GestorStrings(const GestorStrings& otro) {
        longitud = otro.longitud;           // Copiar longitud
        datos = new char[longitud + 1];     // Asignar nueva memoria
        strcpy(datos, otro.datos);          // Copiar datos (copia profunda)
        cout << "Constructor de copia llamado para: " << datos << endl;
    }
    
    // OPERADOR DE ASIGNACIÓN - EVITAR AUTO-ASIGNACIÓN Y MEMORY LEAKS
    // GestorStrings& operator=(const GestorStrings& otro): asignación segura
    GestorStrings& operator=(const GestorStrings& otro) {
        if(this != &otro) {  // Verificar auto-asignación (objeto = objeto)
            delete[] datos;  // Liberar memoria existente para evitar memory leak
            longitud = otro.longitud;        // Copiar nueva longitud
            datos = new char[longitud + 1];  // Asignar nueva memoria
            strcpy(datos, otro.datos);       // Copiar nuevos datos
            cout << "Operador de asignación llamado para: " << datos << endl;
        }
        return *this;  // Retornar referencia al objeto actual (*this)
    }
    
    // DESTRUCTOR - LIBERAR MEMORIA AUTOMÁTICAMENTE AL DESTRUIR OBJETO
    // ~GestorStrings(): se llama automáticamente al salir de scope
    ~GestorStrings() {
        cout << "Destructor llamado para: " << datos << endl;
        delete[] datos;    // Liberar memoria dinámica asignada
        datos = nullptr;   // Evitar dangling pointer
    }
    
    // MÉTODOS DE ACCESO
    const char* getDatos() const { return datos; }
    size_t getLongitud() const { return longitud; }
    
    // MÉTODO PARA CONCATENAR STRINGS
    void concatenar(const char* str) {
        size_t nueva_longitud = longitud + strlen(str);  // Calcular nueva longitud
        char* nuevo_datos = new char[nueva_longitud + 1];  // Asignar nueva memoria
        
        strcpy(nuevo_datos, datos);    // Copiar datos actuales
        strcat(nuevo_datos, str);      // Concatenar nuevo string
        
        delete[] datos;                // Liberar memoria antigua
        datos = nuevo_datos;           // Asignar nueva memoria
        longitud = nueva_longitud;     // Actualizar longitud
    }
};

void ejemplo_clases_con_memoria() {
    cout << "\n=== CLASES CON MEMORIA DINÁMICA ===" << endl;
    
    {
        // CREAR OBJETOS EN SCOPE LOCAL
        GestorStrings str1("Hola");        // Constructor normal
        GestorStrings str2 = str1;         // Constructor de copia (shallow copy evitada)
        GestorStrings str3;                // Constructor por defecto
        str3 = str1;                       // Operador de asignación
        
        str1.concatenar(" Mundo");         // Concatenar string
        cout << "str1 después de concatenar: " << str1.getDatos() << endl;
        
        // AL SALIR DEL SCOPE {}, SE LLAMAN AUTOMÁTICAMENTE LOS DESTRUCTORES
    }
    cout << "Objetos destruidos" << endl;
}

// EJEMPLO DE MANEJO DE EXCEPCIONES EN MEMORIA
void ejemplo_excepciones_memoria() {
    cout << "\n=== MANEJO DE EXCEPCIONES EN MEMORIA ===" << endl;
    
    try {
        // INTENTAR ASIGNAR MUCHA MEMORIA (PROBABLEMENTE FALLARÁ)
        size_t gran_tamano = 1000000000000ULL;  // 1TB - muy grande
        int *ptr_grande = new int[gran_tamano]; // new lanzará bad_alloc si falla
        
        // ESTO PROBABLEMENTE LANZARÁ bad_alloc
        delete[] ptr_grande;  // Liberar si se asignó (no llegará aquí)
        
    } catch(const bad_alloc& e) {
        // CAPTURAR EXCEPCIÓN DE ASIGNACIÓN DE MEMORIA
        cout << "Capturada excepción de memoria: " << e.what() << endl;
    }
    
    // new(nothrow) - NO LANZA EXCEPCIONES, RETORNA nullptr SI FALLA
    int *ptr_seguro = new(nothrow) int[1000];  // new con nothrow
    if(ptr_seguro == nullptr) {
        cout << "No se pudo asignar memoria (nothrow)" << endl;
    } else {
        cout << "Memoria asignada con éxito (nothrow)" << endl;
        delete[] ptr_seguro;  // Liberar memoria asignada
    }
}
```

### **Buenas Prácticas de Gestión de Memoria**

```cpp
#include <iostream>
#include <memory>  // Para smart pointers (C++11+)
using namespace std;

// EJEMPLO DE BUENAS PRÁCTICAS CON SMART POINTERS
void ejemplo_smart_pointers() {
    cout << "\n=== SMART POINTERS (C++11+) ===" << endl;
    
    // unique_ptr - PROPIEDAD ÚNICA (NO SE PUEDE COPIAR, SOLO MOVER)
    {
        // make_unique<int>(42): crear unique_ptr de forma segura
        unique_ptr<int> ptr_unica = make_unique<int>(42);
        cout << "Valor unique_ptr: " << *ptr_unica << endl;  // Desreferenciar
        
        // TRANSFERENCIA DE PROPIEDAD CON move()
        unique_ptr<int> ptr2 = move(ptr_unica);  // Mover propiedad a ptr2
        cout << "Valor después de move: " << *ptr2 << endl;
        // ptr_unica ya no es válida (nullptr)
        
        // NO NECESITA delete EXPLÍCITO - SE LIBERA AUTOMÁTICAMENTE
    }  // Se libera automáticamente al salir del scope
    
    // shared_ptr - PROPIEDAD COMPARTIDA (REFERENCIA CONTADA)
    {
        // make_shared<int>(100): crear shared_ptr de forma eficiente
        shared_ptr<int> ptr_compartido1 = make_shared<int>(100);
        {
            shared_ptr<int> ptr_compartido2 = ptr_compartido1;  // Compartir propiedad
            cout << "Contador de referencias: " << ptr_compartido1.use_count() << endl;
            cout << "Valor compartido: " << *ptr_compartido1 << endl;
        }  // ptr_compartido2 se destruye aquí, contador disminuye
        cout << "Contador después de scope: " << ptr_compartido1.use_count() << endl;
    }  // ptr_compartido1 se destruye aquí, memoria se libera
    
    // weak_ptr - REFERENCIA DÉBIL (EVITA CICLOS DE REFERENCIA)
    {
        shared_ptr<int> ptr_original = make_shared<int>(200);
        weak_ptr<int> ptr_debil = ptr_original;  // Referencia débil (no incrementa contador)
        
        // lock(): verificar si el objeto aún existe y obtener shared_ptr temporal
        if(auto ptr = ptr_debil.lock()) {  // Verificar si aún existe
            cout << "Valor desde weak_ptr: " << *ptr << endl;
        }
    }
}

// EJEMPLO DE PATRÓN RAII (RESOURCE ACQUISITION IS INITIALIZATION)
class GestorRecurso {
private:
    int* recurso;     // Recurso gestionado (memoria dinámica)
    string nombre;    // Nombre identificador del recurso

public:
    // CONSTRUCTOR - ADQUIRIR RECURSO AL CREAR OBJETO
    GestorRecurso(const string& n) : nombre(n) {
        recurso = new int(0);  // Adquirir recurso (memoria)
        cout << "Recurso " << nombre << " adquirido" << endl;
    }
    
    // DESTRUCTOR - LIBERAR RECURSO AL DESTRUIR OBJETO
    ~GestorRecurso() {
        delete recurso;  // Liberar recurso automáticamente
        cout << "Recurso " << nombre << " liberado" << endl;
    }
    
    // NO PERMITIR COPIA (C++11) - EVITAR DOBLE LIBERACIÓN
    GestorRecurso(const GestorRecurso&) = delete;  // Eliminar constructor de copia
    GestorRecurso& operator=(const GestorRecurso&) = delete;  // Eliminar operador =
    
    // PERMITIR MOVIMIENTO (C++11) - TRANSFERIR PROPIEDAD DE FORMA EFICIENTE
    GestorRecurso(GestorRecurso&& otro) noexcept 
        : recurso(otro.recurso), nombre(move(otro.nombre)) {
        otro.recurso = nullptr;  // Dejar objeto origen en estado válido
        cout << "Recurso " << nombre << " movido" << endl;
    }
    
    // MÉTODO PARA USAR EL RECURSO
    void usar() {
        if(recurso) {  // Verificar que el recurso sea válido
            (*recurso)++;  // Usar recurso (incrementar contador)
            cout << "Usando recurso " << nombre << ": " << *recurso << endl;
        }
    }
};

void ejemplo_raii() {
    cout << "\n=== PATRÓN RAII ===" << endl;
    
    {
        // CREAR OBJETOS QUE GESTIONAN RECURSOS AUTOMÁTICAMENTE
        GestorRecurso recurso1("Archivo1");  // Adquirir recurso
        GestorRecurso recurso2("Archivo2");  // Adquirir recurso
        
        // USAR RECURSOS
        recurso1.usar();
        recurso1.usar();
        recurso2.usar();
        
        // LOS RECURSOS SE LIBERAN AUTOMÁTICAMENTE AL SALIR DEL SCOPE
    }
    cout << "Todos los recursos liberados" << endl;
}

// EJEMPLO DE MANEJO SEGURO DE MEMORIA
void ejemplo_manejo_seguro() {
    cout << "\n=== MANEJO SEGURO DE MEMORIA ===" << endl;
    
    // 1. SIEMPRE VERIFICAR ASIGNACIÓN DE MEMORIA
    int* ptr = nullptr;  // Inicializar a nullptr
    try {
        ptr = new int[1000];  // Intentar asignar memoria
    } catch(const bad_alloc&) {
        cout << "Error de asignación de memoria" << endl;
        return;  // Salir si falla
    }
    
    // 2. INICIALIZAR APUNTADORES A nullptr
    int* ptr1 = nullptr;  // Buena práctica: evitar valores basura
    int* ptr2 = nullptr;  // Buena práctica: evitar dangling pointers
    
    try {
        ptr1 = new int(10);   // Asignar memoria para primer entero
        ptr2 = new int(20);   // Asignar memoria para segundo entero
        
        // 3. USAR RECURSOS
        cout << "Valores: " << *ptr1 << ", " << *ptr2 << endl;
        
    } catch(...) {
        // 4. LIBERAR MEMORIA EN CASO DE EXCEPCIÓN (CLEANUP)
        delete ptr1;  // Liberar memoria si fue asignada
        delete ptr2;  // Liberar memoria si fue asignada
        delete[] ptr; // Liberar memoria si fue asignada
        throw;        // Re-lanzar la excepción original
    }
    
    // 5. LIBERAR MEMORIA NORMALMENTE
    delete ptr1;   // Liberar primer entero
    delete ptr2;   // Liberar segundo entero
    delete[] ptr;  // Liberar arreglo
    
    // 6. EVITAR DANGLING POINTERS
    ptr1 = nullptr;  // Evitar usar apuntador después de liberar
    ptr2 = nullptr;  // Evitar usar apuntador después de liberar
    ptr = nullptr;   // Evitar usar apuntador después de liberar
    
    cout << "Memoria manejada de forma segura" << endl;
}
```

---

## 🏗️ 4. Estructuras (struct) en C

### **Definición y Uso Básico**

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// DEFINICIÓN DE ESTRUCTURA BÁSICA
// struct Persona: define una estructura llamada Persona
struct Persona {
    char nombre[50];  // Array de caracteres para el nombre (máximo 49 caracteres + '\0')
    int edad;         // Entero para la edad
    float altura;     // Flotante para la altura
};

// TYPEDEF PARA SIMPLIFICAR - CREA UN ALIAS PARA LA ESTRUCTURA
// typedef struct {...} Coche: crea alias 'Coche' para la estructura
typedef struct {
    char marca[30];      // Marca del coche (máximo 29 caracteres)
    char modelo[30];     // Modelo del coche (máximo 29 caracteres)
    int anio;            // Año del coche
    float precio;        // Precio del coche
} Coche;

// ESTRUCTURAS ANIDADAS - UNA ESTRUCTURA QUE CONTIENE OTRAS ESTRUCTURAS
// typedef struct {...} Conductor: estructura que combina Persona y Coche
typedef struct {
    struct Persona conductor;  // Persona como miembro (usando struct Persona)
    Coche vehiculo;            // Coche como miembro (usando typedef Coche)
    int velocidad_actual;      // Velocidad actual del conductor
} Conductor;

void ejemplo_estructuras_basicas() {
    // DECLARACIÓN E INICIALIZACIÓN DE ESTRUCTURAS
    // struct Persona juan = {...}: inicializar estructura con valores
    struct Persona juan = {"Juan Pérez", 25, 1.75};
    // Coche mi_coche = {...}: inicializar estructura typedef con valores
    Coche mi_coche = {"Toyota", "Corolla", 2020, 25000.0};
    
    // ACCESO A MIEMBROS DE ESTRUCTURAS
    // estructura.miembro: acceso a miembros usando operador punto
    printf("Persona: %s, Edad: %d, Altura: %.2f\n", 
           juan.nombre, juan.edad, juan.altura);
    
    printf("Coche: %s %s (%d) - $%.2f\n", 
           mi_coche.marca, mi_coche.modelo, 
           mi_coche.anio, mi_coche.precio);
    
    // MODIFICACIÓN DE VALORES EN ESTRUCTURAS
    // strcpy(destino, origen): copiar string a array de caracteres
    strcpy(juan.nombre, "Juan García");  // Modificar nombre
    juan.edad = 26;                      // Modificar edad
    
    printf("Persona modificada: %s, Edad: %d\n", 
           juan.nombre, juan.edad);
}

// APUNTADORES A ESTRUCTURAS
void ejemplo_apuntadores_estructuras() {
    struct Persona *ptr_persona;        // Apuntador a estructura Persona
    struct Persona maria = {"María López", 30, 1.65};  // Estructura normal
    
    ptr_persona = &maria;  // Asignar dirección de maria al apuntador
    
    // DOS FORMAS DE ACCEDER A MIEMBROS CON APUNTADORES
    // (*ptr_persona).nombre: sintaxis explícita (desreferenciar y acceder)
    printf("Nombre: %s\n", (*ptr_persona).nombre);  // Sintaxis explícita
    // ptr_persona->edad: sintaxis flecha (más común y legible)
    printf("Edad: %d\n", ptr_persona->edad);        // Sintaxis flecha (más común)
    
    // MODIFICACIÓN USANDO APUNTADOR
    // ptr_persona->altura = 1.68: modificar miembro a través de apuntador
    ptr_persona->altura = 1.68;  // Modificar altura usando apuntador
    printf("Altura modificada: %.2f\n", ptr_persona->altura);
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
