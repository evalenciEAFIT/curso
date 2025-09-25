

# Guía Completa para Compilar y Ejecutar Programas de Multiplicación de Matrices

Aquí te presento una guía paso a paso para compilar y ejecutar los tres programas de multiplicación de matrices:

## Requisitos Previos
Antes de comenzar, asegúrate de tener instalados:
- **GCC** (compilador de C)
- **OpenMP** (para la versión paralela)
- **Librerías de desarrollo POSIX** (para la versión con llamadas al sistema)

En Ubuntu/Debian, puedes instalar todo con:
```bash
sudo apt update && sudo apt install build-essential libopenmpi-dev
```

---

## 1. Programa Secuencial (`sequential.c`)
### Compilación:
```bash
gcc sequential.c -o matrix_sequential -std=c11
```
- `-o matrix_sequential`: Nombre del archivo ejecutable resultante
- `-std=c11`: Usa el estándar C11 (opcional pero recomendado)

### Ejecución:
```bash
./matrix_sequential
```

---

## 2. Programa con Llamadas al Sistema (POSIX) (`posix.c`)
### Compilación:
```bash
gcc posix.c -o matrix_posix -std=c11
```

### Ejecución:
```bash
./matrix_posix
```

---

## 3. Programa con OpenMP (`openmp.c`)
### Compilación:
```bash
gcc openmp.c -o matrix_openmp -fopenmp -std=c11
```
- `-fopenmp`: Habilita el soporte para OpenMP
- `-std=c11`: Usa el estándar C11

### Ejecución:
```bash
./matrix_openmp
```

---

## Consideraciones Importantes

### 1. Permisos de Ejecución
Si obtienes errores de permisos al ejecutar:
```bash
chmod +x matrix_sequential matrix_posix matrix_openmp
```

### 2. Medición de Tiempos Precisa
Para obtener mediciones de tiempo más precisas, usa el comando `time`:
```bash
time ./matrix_sequential
time ./matrix_posix
time ./matrix_openmp
```

### 3. Ajuste del Tamaño de Matriz
Por defecto, el tamaño de la matriz es 1000x1000. Para cambiarlo:
1. Modifica la definición de `N` en cada archivo fuente (ej: `#define N 500`)
2. Vuelve a compilar el programa correspondiente

### 4. Entorno de Ejecución
- Los programas pueden requerir varios GB de RAM dependiendo del tamaño de la matriz
- Asegúrate de tener suficiente memoria disponible

### 5. Comparación de Resultados
Los tiempos típicos de ejecución (para matrices 1000x1000 en un sistema moderno):
- **Secuencial**: ~15-20 segundos
- **POSIX**: ~5-7 segundos
- **OpenMP**: ~3-5 segundos

---

## Resumen de Comandos

| Programa          | Comando de Compilación                     | Comando de Ejecución       |
|-------------------|-------------------------------------------|----------------------------|
| Secuencial        | `gcc sequential.c -o matrix_sequential`   | `./matrix_sequential`      |
| POSIX             | `gcc posix.c -o matrix_posix`             | `./matrix_posix`           |
| OpenMP            | `gcc openmp.c -o matrix_openmp -fopenmp`  | `./matrix_openmp`          |

Con estos pasos podrás compilar y ejecutar todos los programas para comparar su rendimiento en la multiplicación de matrices grandes.
