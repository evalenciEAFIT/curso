# Guía Paso a Paso de Dask en Python (con Ejemplo de Multiplicación de Matrices)

Dask es una biblioteca de Python para computación paralela y distribuida, ideal para manejar grandes conjuntos de datos que no caben en la memoria RAM. Esta guía amplía la explicación anterior, incluyendo un ejemplo específico de **multiplicación de matrices** al final, como solicitaste. La estructura sigue siendo paso a paso, con explicaciones claras, ejemplos prácticos y el nuevo ejemplo integrado.

Los ejemplos son código Python ejecutable. Asumimos que tienes Dask instalado (`pip install "dask[complete]"`) y usas un entorno como Jupyter Notebook o un script. Los ejemplos previos (arrays, DataFrames, bags, delayed) se mantienen, y al final se añade el ejemplo de multiplicación de matrices usando `dask.array`.

## Paso 1: Instalación y Configuración Inicial
- **Requisitos**: Python 3.8+, Dask instalado (`pip install "dask[complete]"` para arrays, DataFrames, etc.).
- **Importación**: Usa `import dask.array as da` para arrays, `dask.dataframe as dd` para DataFrames, etc.
- **Conceptos básicos**:
  - Dask divide datos en **chunks** para procesarlos en paralelo.
  - Operaciones son **lazy**: se planifican en un gráfico de tareas y se ejecutan con `.compute()` o `.persist()`.
  - Usa un scheduler local (multicore) por defecto; para clústers, usa `dask.distributed`.
- **Ejemplo inicial** (suma paralela, repetido para contexto):
```python
import dask.array as da

# Array grande dividido en chunks
x = da.random.random((10000, 10000), chunks=(1000, 1000))
suma = x.sum()
resultado = suma.compute()
print(resultado)  # Aprox. 50,000,000
```
- **Explicación**: Crea un array grande, planifica la suma y la ejecuta en paralelo.

## Paso 2: Componentes Principales de Dask
Repetimos los módulos clave con ejemplos breves para mantener la guía completa. Cada uno incluye sintaxis, propósito y ejemplo.

### 2.1 Dask Arrays (Similar a NumPy)
- **Sintaxis**: `import dask.array as da`. Usa `da.ones()`, `da.dot()`, etc.
- **Explicación**: Maneja arrays multidimensionales grandes en chunks, ideal para operaciones matriciales.
- **Opciones**: `chunks`, `.compute()`, `.persist()`.

Ejemplo (operaciones matriciales básicas):
```python
import dask.array as da
import numpy as np

a = da.ones((5000, 5000), chunks=(1000, 1000))
b = da.random.normal(0, 1, (5000, 5000), chunks=(1000, 1000))
c = a + b * 2
media = c.mean(axis=0).compute()
print(np.mean(media))  # Aprox. 1
```

### 2.2 Dask DataFrames (Similar a Pandas)
- **Sintaxis**: `import dask.dataframe as dd`. Usa `dd.read_csv()`, `df.groupby()`, etc.
- **Explicación**: Maneja DataFrames grandes en particiones.

Ejemplo (procesamiento CSV):
```python
import dask.dataframe as dd
import pandas as pd

# Genera CSV de prueba si no tienes uno
pd.DataFrame({'edad': [20, 40, 30], 'ciudad': ['A', 'A', 'B'], 'salario': [100, 200, 150]}).to_csv('datos.csv', index=False)
df = dd.read_csv('datos.csv', blocksize='64MB')
resultado = df[df['edad'] > 30].groupby('ciudad')['salario'].mean().compute()
print(resultado)  # ciudad A: 200
```

### 2.3 Dask Bags (Datos No Estructurados)
- **Sintaxis**: `import dask.bag as db`. Usa `db.from_sequence()`, `bag.map()`, etc.
- **Explicación**: Para colecciones arbitrarias (e.g., texto, JSON).

Ejemplo (frecuencia de palabras):
```python
import dask.bag as db

bag = db.from_sequence(['Hola mundo', 'Dask es genial', 'Python paralelo'], npartitions=2)
frecuencias = bag.map(lambda x: x.split()).flatten().frequencies().compute()
print(frecuencias)  # [('Hola',1), ('mundo',1), ...]
```

### 2.4 Dask Delayed (Código Personalizado)
- **Sintaxis**: `from dask import delayed`. Usa `@delayed` o `delayed(func)`.
- **Explicación**: Paraleliza funciones arbitrarias.

Ejemplo (cálculos encadenados):
```python
from dask import delayed

@delayed
def suma(x, y): return x + y
@delayed
def multiplica(x, y): return x * y

a = suma(1, 2)
b = multiplica(a, 3)
c = suma(b, 4)
print(c.compute())  # (1+2)*3 + 4 = 13
```

### 2.5 Dask Distributed (Clústers)
- **Explicación**: Usa `dask.distributed` para clústers o monitoreo avanzado.
- Ejemplo:
```python
from dask.distributed import Client
import dask.array as da

client = Client()  # Dashboard en http://localhost:8787
x = da.random.random((10000, 10000), chunks=(1000, 1000))
print(x.sum().compute())
client.close()
```

## Paso 3: Ejemplo de Multiplicación de Matrices
Ahora, añadimos un ejemplo específico de **multiplicación de matrices** usando `dask.array`. Este ejemplo crea dos matrices grandes, las multiplica en paralelo y compara el rendimiento con NumPy.

<xaiArtifact artifact_id="43073944-4ef9-46c3-8efb-3d8feef782b0" artifact_version_id="37f313b6-7697-4723-9849-60a0cfd47726" title="matrix_multiply.py" contentType="text/python">
import dask.array as da
import numpy as np
import time

# Configura matrices grandes
n = 5000  # Tamaño de las matrices (5000x5000)
chunks = (1000, 1000)  # Tamaño de los chunks

# Crea matrices Dask
A = da.random.random((n, n), chunks=chunks)
B = da.random.random((n, n), chunks=chunks)

# Multiplicación de matrices (lazy)
start_time = time.time()
C_dask = da.dot(A, B)  # Planifica multiplicación
dask_time = time.time() - start_time
print(f"Tiempo de planificación Dask: {dask_time:.4f} segundos")

# Computa resultado
start_time = time.time()
result_dask = C_dask.compute()  # Ejecuta en paralelo
dask_compute_time = time.time() - start_time
print(f"Tiempo de cómputo Dask: {dask_compute_time:.4f} segundos")

# Comparación con NumPy (matrices más pequeñas para evitar out-of-memory)
n_small = 1000
A_np = np.random.random((n_small, n_small))
B_np = np.random.random((n_small, n_small))
start_time = time.time()
C_np = np.dot(A_np, B_np)
numpy_time = time.time() - start_time
print(f"Tiempo NumPy (matriz {n_small}x{n_small}): {numpy_time:.4f} segundos")

# Verifica dimensiones
print(f"Dimensiones resultado Dask: {result_dask.shape}")  # (5000, 5000)
</xaiArtifact>

- **Explicación**:
  - **Matrices Dask**: Creamos matrices `A` y `B` de 5000x5000, divididas en chunks de 1000x1000 para no saturar la RAM.
  - **Multiplicación**: `da.dot(A, B)` planifica la multiplicación (rápido, lazy). Al llamar a `.compute()`, Dask distribuye el cálculo entre núcleos.
  - **Comparación NumPy**: Para matrices pequeñas (1000x1000), NumPy puede ser más rápido por menor overhead, pero para matrices grandes, Dask es más eficiente y evita problemas de memoria.
  - **Salida típica** (varía según hardware):
    - Tiempo de planificación Dask: ~0.01 segundos.
    - Tiempo de cómputo Dask: ~5-10 segundos (8 núcleos, típico).
    - Tiempo NumPy (1000x1000): ~0.5 segundos.
    - Dimensiones resultado Dask: (5000, 5000).
  - **Por qué usar Dask**: Puede manejar matrices mucho más grandes que NumPy sin colapsar.

## Paso 4: Consejos y Consideraciones
- **Chunks**: Elige tamaños razonables (e.g., 100MB por chunk). Usa `x.dask` para inspeccionar el gráfico.
- **Rendimiento**: Monitorea con `Client()` y el dashboard (`http://localhost:8787`). Evita `.compute()` en resultados grandes.
- **Errores comunes**: Olvidar `.compute()`, chunks demasiado pequeños/grandes, o no cerrar `Client()`.
- **Multiplicación de matrices**:
  - Usa chunks cuadrados para `dot` (mejor distribución).
  - Verifica compatibilidad de dimensiones (`A.shape[1] == B.shape[0]`).
  - Para clústers, usa `Client()` para distribuir la carga.
- **Debugging**: Usa `C_dask.visualize()` para ver el gráfico de tareas (requiere `graphviz`).
- **Cuándo usar Dask**: Para datos > RAM o cálculos intensivos. NumPy/Pandas es mejor para datos pequeños.
