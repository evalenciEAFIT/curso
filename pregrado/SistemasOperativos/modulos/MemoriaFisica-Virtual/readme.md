# Memoria Física y Virtual en Linux: Allocator y Buddy System

## Introducción

En Linux, la gestión de memoria es clave para ejecutar programas de forma eficiente y segura. Este documento explica de forma simple la **memoria física** y la **memoria virtual**, destacando el papel del **allocator** (asignador de memoria) y el **Buddy System** (Sistema de Compañeros). Incluye un ejemplo en C++ que usa un allocator basado en Buddy System para sumar vectores grandes, mostrando cómo la memoria física y virtual trabajan juntas.

## Memoria Física

La **memoria física** es la RAM real del computador, donde se guardan datos y programas mientras están activos.

- **Características:**
  - Rápida (acceso en nanosegundos).
  - Limitada (ej. 4 GB, 8 GB).
  - Dividida en **páginas** de 4 KB por el kernel de Linux.
  - Visible con comandos como `free -h`.

- **Importancia:**
  - Permite ejecutar programas rápidamente.
  - Si se agota, Linux usa memoria virtual (swap), que es más lenta.

## Memoria Virtual

La **memoria virtual** es una abstracción que hace creer a cada programa que tiene su propia memoria exclusiva, aunque comparta la RAM física.

- **Características:**
  - Cada programa tiene su propio **espacio de direcciones virtuales**.
  - Mapea direcciones virtuales a físicas (RAM) o al disco (swap).
  - Usa la **Unidad de Gestión de Memoria (MMU)** para traducir direcciones.
  - El **swap** (espacio en disco) actúa como memoria extra cuando la RAM no alcanza.

- **Importancia:**
  - Permite ejecutar programas grandes aunque la RAM sea limitada.
  - Asegura que los programas no se "choquen" al acceder a la memoria.
  - Evita que el sistema colapse si la RAM se llena, aunque usar swap ralentiza.

## Allocator y Buddy System

### Allocator

El **allocator** es el mecanismo que asigna memoria a los programas.

- **En el espacio de usuario:** Los programas en C++ usan `new`, `std::vector` o allocators personalizados para pedir memoria. Estas funciones piden memoria virtual al kernel.
- **En el kernel:** El kernel asigna páginas físicas de RAM a las solicitudes virtuales.

**Importancia:** Simplifica la gestión de memoria para los programadores y optimiza su uso.

### Buddy System

El **Buddy System** es un algoritmo del kernel de Linux para gestionar la memoria física.

- **Cómo funciona:**
  - Divide la RAM en bloques de tamaños en potencias de 2 (ej. 4 KB, 8 KB, 16 KB).
  - Cada bloque tiene un "compañero" (buddy). Si se necesita un bloque pequeño, se divide uno grande en dos buddies.
  - Al liberar memoria, los buddies se combinan para reducir **fragmentación** (huecos inutilizables).

- **Importancia:**
  - Asigna memoria física rápido y eficiente.
  - Reduce fragmentación, asegurando que la RAM se use bien.

### ¿Por qué no dividir Allocator y Buddy System?

- **Allocator** es la interfaz (alto nivel) que los programas usan para pedir memoria.
- **Buddy System** es la estrategia (bajo nivel) del kernel para asignar páginas físicas.
- En este ejemplo, combinamos ambos en un `BuddyAllocator` porque:
  - En user-space, no accedemos al Buddy System real del kernel, así que lo emulamos.
  - Un allocator personalizado puede usar la lógica de Buddy para mostrar cómo funciona.
  - No es necesario separarlos aquí: el allocator usa la estrategia de Buddy directamente, simplificando el código.

## Ejemplo en C++: Suma de Vectores Grandes con BuddyAllocator

Este ejemplo crea un allocator que emula el Buddy System para asignar memoria a dos vectores grandes (10 millones de enteros, ~40 MB cada uno), suma sus elementos y mide el tiempo. Muestra:

- **Memoria física:** Si los vectores caben en RAM, la suma es rápida.
- **Memoria virtual:** Si la RAM se agota, Linux usa swap, la suma se ralentiza, pero no falla, mostrando la importancia de ambos tipos de memoria.

**Instrucciones para Linux:**
- Guarda el código en `suma_vectores.cpp`.
- Compila: `g++ suma_vectores.cpp -o suma_vectores`.
- Ejecuta: `./suma_vectores`.
- Monitorea memoria con `free -h` o `top` para ver si usa swap.

```cpp
#include <iostream>      // Para entrada/salida
#include <vector>        // Para listas dinámicas
#include <ctime>         // Para medir tiempo
#include <cstddef>       // Para size_t
#include <new>           // Para bad_alloc

// Constantes para el BuddyAllocator
constexpr int MAX_ORDER = 20;                // Máximo orden: 2^20 ≈ 1 MB por bloque
constexpr size_t MEMORY_POOL_SIZE = (1ULL << (MAX_ORDER + 10));  // Pool: ~1 GB
constexpr size_t MIN_BLOCK_SIZE = (1 << 12); // Mínimo bloque: 4 KB (como páginas)

// Clase BuddyAllocator: Combina allocator y Buddy System
// - Actúa como interfaz para asignar/liberar memoria (allocator).
// - Usa lógica de bloques en potencias de 2 (Buddy System).
// Justificación: Emula cómo el kernel asigna páginas físicas a solicitudes virtuales.
class BuddyAllocator {
private:
    std::vector<void*> free_lists[MAX_ORDER + 1];  // Bloques libres por orden
    void* memory_pool;                             // Pool de memoria

    // Calcula el orden (potencia de 2 superior) para un tamaño
    // Ej: size=5000 -> order=13 (8192 bytes)
    int get_order(size_t size) const {
        int order = 0;
        size_t power = 1;
        while (power < size) {
            power <<= 1;  // Multiplica por 2
            ++order;
        }
        return order;
    }

    // Divide un bloque mayor en dos buddies
    // Justificación: Reduce fragmentación al crear bloques del tamaño exacto
    void split_block(int order) {
        if (free_lists[order].empty()) {
            if (order + 1 > MAX_ORDER) {
                throw std::bad_alloc();  // Sin memoria
            }
            split_block(order + 1);  // Divide bloque mayor
        }
        void* block = free_lists[order].back();
        free_lists[order].pop_back();
        size_t block_size = 1ULL << order;

        // Crea dos buddies
        void* buddy1 = block;
        void* buddy2 = static_cast<char*>(block) + (block_size / 2);

        int lower_order = order - 1;
        free_lists[lower_order].push_back(buddy1);
        free_lists[lower_order].push_back(buddy2);
    }

public:
    // Constructor: Inicializa el pool
    // Justificación: Simula la memoria física inicial del sistema
    BuddyAllocator() {
        memory_pool = new (std::nothrow) char[MEMORY_POOL_SIZE];
        if (!memory_pool) throw std::bad_alloc();
        int pool_order = get_order(MEMORY_POOL_SIZE);
        if (pool_order > MAX_ORDER) pool_order = MAX_ORDER;
        free_lists[pool_order].push_back(memory_pool);
    }

    // Destructor: Libera el pool (RAII)
    ~BuddyAllocator() {
        delete[] static_cast<char*>(memory_pool);
    }

    // Asigna memoria
    // Justificación: Emula malloc/new con lógica de Buddy
    void* alloc(size_t size) {
        if (size == 0) return nullptr;
        int order = get_order(size);
        if (order > MAX_ORDER) throw std::bad_alloc();

        if (free_lists[order].empty()) {
            split_block(order);  // Divide si necesario
        }

        if (free_lists[order].empty()) throw std::bad_alloc();
        void* block = free_lists[order].back();
        free_lists[order].pop_back();
        return block;
    }

    // Libera memoria
    // Justificación: Simplificado, no combina buddies (en kernel real sí)
    void free_block(void* ptr, size_t size) {
        int order = get_order(size);
        free_lists[order].push_back(ptr);
    }
};

// Programa principal: Suma vectores grandes
// Justificación: Muestra cómo la memoria física (rápida) y virtual (swap, lenta) afectan el rendimiento
int main() {
    try {
        BuddyAllocator allocator;
        constexpr size_t VECTOR_SIZE = 10000000;  // 10M elementos (~40 MB por vector)

        // Asigna memoria para vectores
        // Justificación: Usa BuddyAllocator para simular asignación física/virtual
        int* vec1 = static_cast<int*>(allocator.alloc(VECTOR_SIZE * sizeof(int)));
        int* vec2 = static_cast<int*>(allocator.alloc(VECTOR_SIZE * sizeof(int)));
        int* sum_vec = static_cast<int*>(allocator.alloc(VECTOR_SIZE * sizeof(int)));

        if (!vec1 || !vec2 || !sum_vec) {
            std::cerr << "Error: Fallo en asignación." << std::endl;
            return 1;
        }

        // Inicializa vectores
        for (size_t i = 0; i < VECTOR_SIZE; ++i) {
            vec1[i] = static_cast<int>(i % 100);
            vec2[i] = static_cast<int>((i + 50) % 100);
        }

        // Suma y mide tiempo
        // Justificación: Tiempo bajo = usa RAM física; tiempo alto = usa swap virtual
        clock_t start = clock();
        for (size_t i = 0; i < VECTOR_SIZE; ++i) {
            sum_vec[i] = vec1[i] + vec2[i];
        }
        clock_t end = clock();
        double time_spent = static_cast<double>(end - start) / CLOCKS_PER_SEC;

        // Resultados
        std::cout << "Suma de primeros 5 elementos: ";
        for (size_t i = 0; i < 5; ++i) {
            std::cout << sum_vec[i] << (i < 4 ? ", " : "\n");
        }
        std::cout << "Tiempo de suma: " << time_spent << " segundos." << std::endl;
        std::cout << "Si el tiempo es alto, usó swap (memoria virtual). Si es bajo, usó RAM (memoria física)." << std::endl;

        // Libera memoria
        allocator.free_block(vec1, VECTOR_SIZE * sizeof(int));
        allocator.free_block(vec2, VECTOR_SIZE * sizeof(int));
        allocator.free_block(sum_vec, VECTOR_SIZE * sizeof(int));

    } catch (const std::bad_alloc& e) {
        std::cerr << "Error: Memoria insuficiente." << std::endl;
        return 1;
    }

    return 0;
}
```

## Conclusión

La **memoria física** (RAM) es rápida pero limitada, mientras la **memoria virtual** permite usar más memoria (con swap) y protege procesos. El **allocator** (como `BuddyAllocator`) simplifica la asignación, y el **Buddy System** organiza la RAM en bloques eficientes. No es necesario dividirlos en este ejemplo porque el allocator usa la lógica de Buddy directamente, emulando cómo Linux gestiona memoria. El ejemplo de suma de vectores muestra que la memoria física es clave para velocidad, pero la virtual evita fallos cuando la RAM no alcanza.
