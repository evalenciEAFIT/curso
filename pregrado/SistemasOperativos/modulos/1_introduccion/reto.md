# **Documento Técnico: Comparativa de Paso por Valor vs. Referencia en C++**

## **1. Introducción y Descripción del Reto**

### **1.1 Objetivo**
El objetivo principal de este documento es comparar dos métodos de paso de parámetros en C++ (**por valor** y **por referencia**) para evaluar su impacto en:

- **Consumo de memoria**
- **Tiempo de ejecución**
- **Eficiencia computacional**

Se busca demostrar empíricamente por qué el paso por referencia es más eficiente en escenarios con estructuras de datos grandes.

### **1.2 Contexto del Problema**
En programación, especialmente en C++, el manejo de grandes volúmenes de datos (como arreglos, vectores o matrices) puede afectar significativamente el rendimiento si no se optimiza el paso de parámetros.  

- **Paso por valor**: Crea una copia completa del dato, consumiendo más memoria y tiempo.
- **Paso por referencia**: Trabaja directamente con la dirección de memoria del dato original, evitando copias innecesarias.

### **1.3 Metodología**
Para comparar ambos métodos, se implementa un programa que:
1. Genera un vector de **100 millones de enteros aleatorios** (≈380MB en memoria).
2. Realiza una operación de suma usando **paso por valor** y **paso por referencia**.
3. Mide el **tiempo de ejecución** y el **uso de memoria** en cada caso.

---

## **2. Solución Propuesta**

### **2.1 Descripción de la Solución**
Se desarrolla un programa en C++ que:
- Genera un dataset grande con números aleatorios.
- Compara dos funciones:
  - `sumarPorValor()`: Recibe el vector **por valor** (hace una copia).
  - `sumarPorReferencia()`: Recibe el vector **por referencia** (sin copia).
- Mide el tiempo y la memoria consumida en cada caso.

### **2.2 Beneficios de la Comparación**
- **Reducción de memoria**: Evita duplicar datos innecesariamente.
- **Mayor velocidad**: Elimina el tiempo de copia de datos.
- **Mismos resultados**: Ambas funciones producen la misma suma, pero con diferente eficiencia.

---

## **3. Código Implementado**

### **3.1 Estructura del Programa**
```cpp
#include <iostream>   // Entrada/salida estándar
#include <vector>     // Almacenamiento dinámico de datos
#include <random>     // Generación de números aleatorios
#include <chrono>     // Medición de tiempos de ejecución
#include <algorithm>  // Funciones útiles (abs, max)

using namespace std;
using namespace std::chrono;

/**
 * Genera un vector de números aleatorios entre 1 y 100.
 * @param n Tamaño del vector.
 * @return Vector con datos aleatorios.
 */
vector<int> generarDatos(size_t n) {
    random_device rd;       // Generador hardware-based
    mt19937 gen(rd());      // Motor Mersenne Twister
    uniform_int_distribution<int> dist(1, 100); // Distribución uniforme
    
    vector<int> datos(n);
    for (auto& num : datos) {
        num = dist(gen);    // Llena el vector con valores aleatorios
    }
    return datos;
}

/**
 * Suma los elementos de un vector recibido por valor (copia).
 * @param datos Copia del vector original.
 * @return Suma de los elementos.
 */
long long sumarPorValor(vector<int> datos) {
    long long suma = 0;
    for (int num : datos) {
        suma += num;
    }
    return suma;
}

/**
 * Suma los elementos de un vector recibido por referencia (sin copia).
 * @param datos Referencia al vector original.
 * @param sumaReferencia Variable donde se almacena el resultado.
 */
void sumarPorReferencia(const vector<int>& datos, long long &sumaReferencia) {
    long long suma = 0;
    for (int num : datos) {
        suma += num;
    }
    sumaReferencia = suma;
}

/**
 * Muestra una muestra de los datos generados (primeros y últimos 5 elementos).
 * @param datos Vector a analizar.
 */
void mostrarEstadisticas(const vector<int>& datos) {
    cout << "Muestra de datos - Primeros 5: ";
    for (int i = 0; i < 5 && i < datos.size(); ++i) {
        cout << datos[i] << " ";
    }
    cout << "\nMuestra de datos - Últimos 5: ";
    for (int i = max(0, (int)datos.size() - 5); i < datos.size(); ++i) {
        cout << datos[i] << " ";
    }
    cout << endl;
}

int main() {
    const size_t N = 100'000'000; // 100 millones de elementos (~380MB)
    cout << "Generando " << N << " números aleatorios...\n";
    
    // Generación del dataset
    vector<int> datos = generarDatos(N);
    mostrarEstadisticas(datos);
    
    // Variables para almacenar resultados
    long long sumaValor, sumaReferencia;
    
    // Benchmark: Paso por valor
    cout << "\nEjecutando suma por valor (con copia)...\n";
    auto inicioValor = high_resolution_clock::now();
    sumaValor = sumarPorValor(datos);
    auto finValor = high_resolution_clock::now();
    auto duracionValor = duration_cast<milliseconds>(finValor - inicioValor);
    
    // Benchmark: Paso por referencia
    cout << "Ejecutando suma por referencia (sin copia)...\n";
    auto inicioRef = high_resolution_clock::now();
    sumarPorReferencia(datos, sumaReferencia);
    auto finRef = high_resolution_clock::now();
    auto duracionRef = duration_cast<milliseconds>(finRef - inicioRef);
    
    // Resultados comparativos
    cout << "\nResultados:\n";
    cout << "• Suma por valor: " << sumaValor << " (" << duracionValor.count() << " ms)\n";
    cout << "• Suma por referencia: " << sumaReferencia << " (" << duracionRef.count() << " ms)\n";
    
    // Análisis de memoria
    cout << "\nConsumo de memoria:\n";
    const size_t mb_used = datos.size() * sizeof(int) / (1024 * 1024);
    cout << "• Memoria dataset original: " << mb_used << " MB\n";
    cout << "• Memoria adicional por valor: +" << mb_used << " MB\n";
    cout << "• Memoria adicional por referencia: +0 MB\n";
    
    // Validación de resultados
    cout << "\nControl de calidad:\n";
    cout << "• Diferencia entre sumas: " << abs(sumaValor - sumaReferencia) << "\n";
    cout << "• Resultados " << (sumaValor == sumaReferencia ? "consistentes" : "inconsistentes") << endl;
    
    return 0;
}
```

---

## **4. Explicación del Código**

### **4.1 Generación de Datos Aleatorios**
- Se usa `random_device` y `mt19937` para generar números pseudoaleatorios con buena distribución.
- El vector `datos` almacena **100 millones de enteros** (≈380MB).

### **4.2 Funciones de Suma**
| Función | Mecanismo | Memoria Usada | Tiempo |
|---------|-----------|--------------|--------|
| `sumarPorValor()` | Copia el vector completo | +380MB | Más lento |
| `sumarPorReferencia()` | Usa referencia (sin copia) | +0MB | Más rápido |

### **4.3 Medición de Tiempo**
- Se emplea `high_resolution_clock` para medir el tiempo en milisegundos.
- Se comparan los resultados de ambas funciones.

### **4.4 Análisis de Memoria**
- El paso por valor **duplica** el consumo de memoria.
- El paso por referencia **no requiere memoria adicional**.

---

## **5. Resultados y Conclusiones**

### **5.1 Hallazgos Principales**
1. **El paso por referencia es más eficiente** en memoria y tiempo.
2. **El paso por valor es más lento** debido a la copia de datos.
3. **Ambos métodos producen el mismo resultado**, pero con diferente impacto en rendimiento.

### **5.2 Recomendaciones**
✔ **Usar paso por referencia** cuando:
   - Se trabaja con estructuras grandes (vectores, matrices).
   - No se necesita modificar el dato original (usar `const`).  

✔ **Usar paso por valor** cuando:
   - Los datos son pequeños (ej. `int`, `float`).
   - Se requiere una copia independiente del original.

### **5.3 Conclusión Final**
El paso por referencia **reduce significativamente el uso de memoria y mejora el rendimiento** en operaciones con grandes volúmenes de datos, siendo la opción preferida en aplicaciones de alto rendimiento.  

**¿Cuándo usar cada método?**  
| Criterio | Paso por Valor | Paso por Referencia |
|----------|---------------|---------------------|
| Memoria  | Ineficiente (copia) | Óptimo (solo referencia) |
| Velocidad | Más lento | Más rápido |
| Seguridad | Aísla datos originales | Requiere `const` para protección |

Este análisis proporciona una base técnica para tomar decisiones informadas en el desarrollo de software eficiente en C++.
