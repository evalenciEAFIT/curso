// Video: https://youtu.be/8KFvrgbuWxY?si=KXcRgUb8Ie4OAb91

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <random>
#include <atomic>
#include <vector>
#include <string>

// Definición de códigos de color ANSI para terminal
#define RESET       "\033[0m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define BLACK       "\033[30m"
#define BG_GREEN    "\033[42m"
#define BG_RED      "\033[41m"

// Constantes del problema
const int NUM_FILOSOFOS = 5;
const int TIEMPO_PENSAR_MIN = 1000;  // ms
const int TIEMPO_PENSAR_MAX = 3000;  // ms
const int TIEMPO_COMER_MIN = 1000;   // ms
const int TIEMPO_COMER_MAX = 3000;   // ms

// Enumeración para los estados de los filósofos
enum Estado { PENSANDO, HAMBRIENTO, COMIENDO };

// Estructura para almacenar los contadores de estados de cada filósofo
struct Contadores {
    int pensando = 0;
    int hambriento = 0;
    int comiendo = 0;
};

/**
 * Mesa
 * Representa la mesa donde se sientan los filósofos y gestiona la concurrencia
 * 
 * Esta clase implementa la solución al problema de los filósofos comensales utilizando:
 * - Mutex para exclusión mutua
 * - Variables de condición para sincronización
 * - Un diseño que previene deadlocks e inanición
 * - Contadores de estados para estadísticas
 */
class Mesa {
private:
    std::mutex mtx;  // Mutex para proteger el estado de los filósofos (exclusión mutua)
    std::condition_variable cv[NUM_FILOSOFOS];  // Variables de condición por filósofo
    Estado estados[NUM_FILOSOFOS];  // Estados actuales de los filósofos
    std::atomic<bool>& running;  // Referencia a la variable de control de ejecución
    Contadores contadores[NUM_FILOSOFOS];  // Contadores de estados para cada filósofo

    /**
     * Verifica si un filósofo puede comer (previene deadlock)
     * i: Índice del filósofo a verificar
     * 
     * Esta función implementa la lógica para evitar deadlocks:
     * 1. Un filósofo solo puede comer si está hambriento
     * 2. Sus vecinos inmediatos no deben estar comiendo
     * 
     * Esto evita la situación clásica de deadlock donde todos los filósofos
     * toman su tenedor izquierdo y esperan indefinidamente por el derecho.
     */
    void probar(int i) {
        // Un filósofo puede comer si:
        // 1. Está hambriento
        // 2. Sus vecinos no están comiendo
        if (estados[i] == HAMBRIENTO && 
            estados[(i + NUM_FILOSOFOS - 1) % NUM_FILOSOFOS] != COMIENDO && 
            estados[(i + 1) % NUM_FILOSOFOS] != COMIENDO) {
            
            estados[i] = COMIENDO;  // Cambia estado a comiendo
            contadores[i].comiendo++;  // Incrementa contador de comiendo
            cv[i].notify_one();     // Notifica al filósofo que puede comer
        }
    }

public:
    /**
     * onstructor de la mesa
     * r: Referencia a la variable atómica que controla la ejecución
     * 
     * Inicializa todos los filósofos en estado PENSANDO, inicializa los contadores
     * y guarda la referencia a la variable de control de ejecución.
     */
    Mesa(std::atomic<bool>& r) : running(r) {
        // Inicializa todos los filósofos en estado PENSANDO
        for (int i = 0; i < NUM_FILOSOFOS; ++i) {
            estados[i] = PENSANDO;
            contadores[i].pensando = 1;  // Inicia en estado PENSANDO
            contadores[i].hambriento = 0;
            contadores[i].comiendo = 0;
        }
    }

    /**
     * Método público para verificar si la simulación está en ejecución
     * return true si la simulación está activa, false en caso contrario
     * 
     * Proporciona acceso controlado al miembro privado 'running',
     * manteniendo el principio de encapsulamiento.
     */
    bool esta_ejecutando() const {
        return running;
    }

    /**
     * Intenta tomar los tenedores para comer
     * i: Índice del filósofo que intenta comer
     * 
     * Implementa el protocolo para adquirir recursos (tenedores):
     * 1. Cambia el estado a HAMBRIENTO
     * 2. Verifica si puede comer inmediatamente
     * 3. Si no puede, espera en una variable de condición
     * 
     * Este método previene condiciones de carrera mediante el uso de mutex
     * y evita deadlock mediante la función probar().
     */
    void tomar_tenedores(int i) {
        std::unique_lock<std::mutex> lock(mtx);  // Adquiere el mutex (exclusión mutua)
        
        // Cambia estado a hambriento
        estados[i] = HAMBRIENTO;
        contadores[i].hambriento++;  // Incrementa contador de hambriento
        
        // Verifica si puede comer inmediatamente
        probar(i);
        
        // Si no puede comer, espera hasta que se le notifique
        // La condición 'running' permite terminar la espera si la simulación se detiene
        while (estados[i] != COMIENDO && running) {
            cv[i].wait(lock);  // Libera el mutex y espera a ser notificado
        }
    }

    /**
     * Libera los tenedores después de comer
     * i: Índice del filósofo que termina de comer
     * 
     * Implementa el protocolo para liberar recursos:
     * 1. Cambia el estado a PENSANDO
     * 2. Verifica si los vecinos pueden comer ahora
     * 
     * Esto previene inanición (starvation) al dar oportunidad a los vecinos
     * de adquirir los tenedores liberados.
     */
    void liberar_tenedores(int i) {
        std::unique_lock<std::mutex> lock(mtx);  // Adquiere el mutex (exclusión mutua)
        
        // Cambia estado a pensando
        estados[i] = PENSANDO;
        contadores[i].pensando++;  // Incrementa contador de pensando
        
        // Verifica si los vecinos pueden comer ahora
        probar((i + NUM_FILOSOFOS - 1) % NUM_FILOSOFOS);  // Vecino izquierdo
        probar((i + 1) % NUM_FILOSOFOS);                  // Vecino derecho
    }

    /**
     * Despierta a todos los filósofos (para terminar la simulación)
     * 
     * Notifica a todas las variables de condición para despertar a los filósofos
     * que estén esperando. Esto es necesario para terminar la simulación de manera
     * controlada, evitando que los hilos queden bloqueados indefinidamente.
     */
    void despertar_todos() {
        for (int i = 0; i < NUM_FILOSOFOS; ++i) {
            cv[i].notify_all();  // Despierta a todos los hilos esperando en esta variable de condición
        }
    }

    /**
     * Imprime el estado actual de la mesa en la terminal
     * 
     * Muestra una representación visual del estado de cada filósofo, contadores de estados
     * y la disponibilidad de los tenedores usando colores para facilitar la comprensión.
     * Esta función es thread-safe gracias al uso del mutex.
     */
    void imprimir_estado() {
        std::unique_lock<std::mutex> lock(mtx);  // Adquiere el mutex para acceso exclusivo
        
        // Limpia la terminal
        std::cout << "\033[2J\033[H";
        
        // Imprime título
        std::cout << CYAN << "Problema de los Filósofos Comensales\n" << RESET;
        std::cout << "=========================================\n\n";
        
        // Imprime estados de los filósofos
        for (int i = 0; i < NUM_FILOSOFOS; ++i) {
            std::cout << "Filósofo " << i << ": ";
            
            switch (estados[i]) {
                case PENSANDO:
                    std::cout << BLUE << " PENSANDO " << RESET;
                    break;
                case HAMBRIENTO:
                    std::cout << RED << "HAMBRIENTO" << RESET;
                    break;
                case COMIENDO:
                    std::cout << GREEN << " COMIENDO " << RESET;
                    break;
            }
            
            // Muestra contadores de estados
            std::cout << " | Contadores: ";
            std::cout << BLUE << "P:" << contadores[i].pensando << RESET << " ";
            std::cout << RED << "H:" << contadores[i].hambriento << RESET << " ";
            std::cout << GREEN << "C:" << contadores[i].comiendo << RESET;
            
            // Muestra tenedores disponibles
            bool tenedor_izq = estados[(i + NUM_FILOSOFOS - 1) % NUM_FILOSOFOS] != COMIENDO;
            bool tenedor_der = estados[(i + 1) % NUM_FILOSOFOS] != COMIENDO;
            
            std::cout << " | Tenedores: ";
            
            // Tenedor izquierdo con fondo de color
            if (tenedor_izq) {
                std::cout << BG_GREEN << BLACK << "Izq" << RESET;
            } else {
                std::cout << BG_RED << BLACK << "Izq" << RESET;
            }
            
            std::cout << " ";
            
            // Tenedor derecho con fondo de color
            if (tenedor_der) {
                std::cout << BG_GREEN << BLACK << "Der" << RESET;
            } else {
                std::cout << BG_RED << BLACK << "Der" << RESET;
            }
            
            std::cout << "\n";
        }
        
        // Explicación de colores
        std::cout << "\nLeyenda:\n";
        std::cout << BLUE << "PENSANDO" << RESET << ": Filósofo meditando\n";
        std::cout << RED << "HAMBRIENTO" << RESET << ": Filósofo esperando tenedores\n";
        std::cout << GREEN << "COMIENDO" << RESET << ": Filósofo alimentándose\n";
        std::cout << "Contadores: " << BLUE << "P" << RESET << "=Pensando, " << RED << "H" << RESET << "=Hambriento, " << GREEN << "C" << RESET << "=Comiendo\n";
        std::cout << BG_GREEN << BLACK << "Tenedor" << RESET << ": Disponible | " << BG_RED << BLACK << "Tenedor" << RESET << ": Ocupado\n";
    }
};

/**
 * Función que simula el comportamiento de un filósofo
 * id: Identificador del filósofo
 * mesa: Referencia a la mesa compartida
 * 
 * Esta función representa el ciclo de vida de un filósofo:
 * 1. Piensa por un tiempo aleatorio
 * 2. Intenta tomar los tenedores para comer
 * 3. Si logra tomar los tenedores, come por un tiempo aleatorio
 * 4. Libera los tenedores
 * 5. Repite el ciclo
 * 
 * La función utiliza mecanismos de sincronización para coordinarse con otros filósofos.
 */
void filosofo(int id, Mesa& mesa) {
    // Generador de números aleatorios para tiempos de espera
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> pensar_dist(TIEMPO_PENSAR_MIN, TIEMPO_PENSAR_MAX);
    std::uniform_int_distribution<> comer_dist(TIEMPO_COMER_MIN, TIEMPO_COMER_MAX);
    
    // Ciclo principal del filósofo
    while (mesa.esta_ejecutando()) {
        // Fase de pensar: el filósofo no necesita recursos
        std::this_thread::sleep_for(std::chrono::milliseconds(pensar_dist(gen)));
        
        // Intenta tomar los tenedores (operación bloqueante si no puede comer)
        mesa.tomar_tenedores(id);
        
        // Si la simulación sigue corriendo, come
        if (mesa.esta_ejecutando()) {
            // Fase de comer: el filósofo tiene ambos tenedores
            std::this_thread::sleep_for(std::chrono::milliseconds(comer_dist(gen)));
            
            // Libera los tenedores para que otros puedan usarlos
            mesa.liberar_tenedores(id);
        }
    }
}

/**
 * Función principal del programa
 * 
 * Configura y ejecuta la simulación del problema de los filósofos comensales:
 * 1. Crea la variable de control de ejecución
 * 2. Inicializa la mesa compartida
 * 3. Crea e inicia los hilos de los filósofos
 * 4. Muestra el estado actualizado de la simulación
 * 5. Espera la interacción del usuario para terminar
 * 6. Limpia los recursos y termina
 * 
 * return 0 si la ejecución fue exitosa
 */

int main() {
    // Variable para controlar la ejecución de los hilos
    // std::atomic garantiza que las operaciones sean atómicas y thread-safe
    std::atomic<bool> running(true);
    
    // Crear la mesa compartida entre todos los filósofos
    Mesa mesa(running);
    
    // Vector para almacenar los hilos de los filósofos
    std::vector<std::thread> filosofos;
    
    // Crear e iniciar los hilos de los filósofos
    // Cada filósofo se ejecuta en su propio hilo
    for (int i = 0; i < NUM_FILOSOFOS; ++i) {
        filosofos.emplace_back(filosofo, i, std::ref(mesa));
    }
    
    // Bucle principal para la visualización
    // Se ejecuta en el hilo principal mientras la simulación esté activa
    while (running) {
        mesa.imprimir_estado();  // Muestra el estado actual
        std::this_thread::sleep_for(std::chrono::milliseconds(500));  // Actualiza cada 500ms
    }
    
    // Despertar a todos los filósofos para que terminen
    // Esto es necesario para que los hilos bloqueados en variables de condición puedan terminar
    mesa.despertar_todos();
    
    // Esperar a que todos los hilos terminen (join)
    // Evita que el programa termine mientras los hilos siguen ejecutándose
    for (auto& f : filosofos) {
        f.join();
    }
    
    return 0;
}
