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