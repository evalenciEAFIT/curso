### **Módulo 3: Gestión de Memoria y Almacenamiento (Semanas 10-14)**

### **1. Perspectiva del Sistema Operativo (Linux): "Todo es un Archivo"**

En Linux (y en sistemas tipo Unix en general), una de las filosofías fundamentales es **"todo es un archivo"**. Esto significa que el SO interactúa con una gran cantidad de recursos (discos duros, puertos USB, la memoria, incluso la información de los procesos) a través de una interfaz unificada: la interfaz de archivo.

Para el Sistema Operativo, un archivo no es inherentemente "de texto" o "binario". Desde el punto de vista del kernel, un archivo es simplemente una **secuencia de bytes**. No le importa si esos bytes representan los caracteres de un poema, los números de una imagen o la estructura de un programa compilado.

La **interpretación** de esa secuencia de bytes es responsabilidad de la **aplicación** que abre el archivo.

*   Un editor de texto (como `vim` o `nano`) interpretará los bytes como caracteres codificados (ej. UTF-8, ASCII).
*   Un reproductor de música interpretará los bytes como datos de audio codificados (ej. MP3, FLAC).
*   Nuestro programa en C decidirá si tratar esos bytes como texto formateado o como una copia exacta de una estructura de datos en memoria.

El SO gestiona estos archivos a través de **descriptores de archivo** (file descriptors), que son simples números enteros que actúan como identificadores para un archivo abierto.

---

### **2. Archivos de Texto Plano (`.txt`, `.c`, `.csv`)**

#### **Concepto**

Un archivo de texto plano es una secuencia de caracteres legibles por humanos, organizados en líneas. Cada carácter está codificado utilizando un estándar como **ASCII** o **UTF-8**. Las líneas están separadas por caracteres especiales de nueva línea, que en Linux es el carácter `\n` (salto de línea).

**Ejemplo:** El texto `Hola, 123` se guardaría como una secuencia de bytes: los bytes para 'H', 'o', 'l', 'a', ',', ' ', '1', '2', '3'.

#### **Ventajas**
*   **Legibilidad:** Puedes abrirlos y leerlos directamente con cualquier editor de texto.
*   **Portabilidad:** Son muy portables entre diferentes sistemas operativos y arquitecturas.
*   **Simplicidad:** Fáciles de crear y depurar.

#### **Desventajas**
*   **Ineficiencia:** Almacenar números o estructuras complejas requiere convertirlos a texto (formateo), lo que es más lento y puede ocupar más espacio.
*   **Procesamiento:** Leer datos numéricos requiere un paso de "parseo" o conversión de texto a número, lo que añade sobrecarga.

#### **Código en C: Escribir y Leer un Archivo de Texto**

Vamos a crear un programa que escribe una lista de personas (nombre y edad) en un archivo de texto y otro que los lee.

**`escribir_texto.c`**
```c
#include <stdio.h>
#include <stdlib.h> // Para exit()

int main() {
    // 1. Abrir el archivo en modo de escritura de texto ("w").
    // "w" crea el archivo si no existe o lo trunca (vacía) si ya existe.
    FILE *archivo = fopen("personas.txt", "w");

    // 2. Buena Práctica: Siempre verificar si el archivo se abrió correctamente.
    if (archivo == NULL) {
        perror("Error al abrir el archivo de texto"); // perror imprime el error del sistema
        exit(1); // Termina el programa con un código de error
    }

    // 3. Escribir datos en el archivo usando fprintf.
    // fprintf funciona como printf, pero envía la salida a un archivo.
    // Formateamos los datos como texto: "Nombre,Edad\n"
    fprintf(archivo, "Ana, 28\n");
    fprintf(archivo, "Luis, 34\n");
    fprintf(archivo, "Marta, 22\n");

    // 4. Buena Práctica: Cerrar siempre el archivo para liberar recursos.
    fclose(archivo);

    printf("Datos escritos correctamente en 'personas.txt'\n");

    return 0;
}
```

**`leer_texto.c`**
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Para strncpy

#define MAX_NOMBRE 50

// Definimos una estructura para guardar los datos leídos
typedef struct {
    char nombre[MAX_NOMBRE];
    int edad;
} Persona;

int main() {
    // 1. Abrir el archivo en modo de lectura de texto ("r").
    FILE *archivo = fopen("personas.txt", "r");

    if (archivo == NULL) {
        perror("Error al abrir el archivo de texto");
        exit(1);
    }

    Persona p;

    printf("--- Leyendo datos de 'personas.txt' ---\n");

    // 2. Leer línea por línea usando fscanf.
    // fscanf intenta leer datos formateados desde el archivo.
    // Devuelve el número de elementos que leyó correctamente.
    // El bucle se detiene cuando fscanf no pueda leer 2 elementos (llega a EOF).
    while (fscanf(archivo, "%49[^,], %d\n", p.nombre, &p.edad) == 2) {
        // %49[^,] es un truco: lee hasta 49 caracteres que NO sean una coma.
        // Esto evita desbordamientos de búfer y lee el nombre correctamente.
        printf("Nombre: %s, Edad: %d\n", p.nombre, p.edad);
    }

    // 3. Cerrar el archivo.
    fclose(archivo);

    return 0;
}
```

---

### **3. Archivos Binarios (`.bin`, `.dat`, ejecutables)**

#### **Concepto**

Un archivo binario contiene una **copia exacta de los bytes** que representan los datos en la memoria RAM del programa. No hay formateo ni conversión a caracteres legibles. Si guardas una variable `int` (que ocupa 4 bytes), se escriben esos 4 bytes directamente en el archivo.

**Ejemplo:** El número entero `123` (en representación binaria) se guardaría como los 4 bytes que lo componen en memoria, no como los caracteres '1', '2', '3'.

#### **Ventajas**
*   **Rendimiento:** La lectura y escritura son extremadamente rápidas porque no hay conversión (formateo/parseo).
*   **Compacidad:** Ocupan el espacio mínimo necesario, ideal para números y estructuras complejas.
*   **Integridad:** Se pueden guardar estructuras de datos completas (`structs`) de una sola vez.

#### **Desventajas**
*   **No legibles:** No se pueden entender con un editor de texto.
*   **Poca Portabilidad:** Un archivo binario creado en una arquitectura (ej. Intel de 64 bits) puede no ser legible en otra (ej. ARM de 32 bits) debido a diferencias en la representación de números (endianness) o el alineamiento de bytes en las estructuras.
*   **Difícil de depurar:** Ver su contenido requiere herramientas especiales como `hexdump`.

#### **Código en C: Escribir y Leer un Archivo Binario**

Usaremos la misma estructura `Persona` para demostrar la diferencia.

**`escribir_binario.c`**
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOMBRE 50

typedef struct {
    char nombre[MAX_NOMBRE];
    int edad;
} Persona;

int main() {
    // 1. Abrir el archivo en modo de escritura binaria ("wb").
    // La 'b' es crucial. Le indica a la librería estándar de C que no
    // realice traducciones de caracteres (como \r\n a \n en Windows).
    FILE *archivo = fopen("personas.bin", "wb");

    if (archivo == NULL) {
        perror("Error al abrir el archivo binario");
        exit(1);
    }

    // 2. Crear un array de personas para escribir.
    Persona personas[3] = {
        {"Ana", 28},
        {"Luis", 34},
        {"Marta", 22}
    };

    // 3. Escribir los datos en el archivo usando fwrite.
    // fwrite escribe bloques de bytes directamente desde memoria.
    // Parámetros:
    //  - Puntero a los datos a escribir (&personas[0] o simplemente 'personas')
    //  - Tamaño de un solo elemento (sizeof(Persona))
    //  - Número de elementos a escribir (3)
    //  - Puntero al archivo
    size_t elementos_escritos = fwrite(personas, sizeof(Persona), 3, archivo);

    if (elementos_escritos != 3) {
        perror("Error al escribir en el archivo binario");
        exit(1);
    }

    // 4. Cerrar el archivo.
    fclose(archivo);

    printf("Datos escritos correctamente en 'personas.bin'\n");

    return 0;
}
```

**`leer_binario.c`**
```c
#include <stdio.h>
#include <stdlib.h>

#define MAX_NOMBRE 50

typedef struct {
    char nombre[MAX_NOMBRE];
    int edad;
} Persona;

int main() {
    // 1. Abrir el archivo en modo de lectura binaria ("rb").
    FILE *archivo = fopen("personas.bin", "rb");

    if (archivo == NULL) {
        perror("Error al abrir el archivo binario");
        exit(1);
    }

    Persona p;

    printf("--- Leyendo datos de 'personas.bin' ---\n");

    // 2. Leer los datos usando fread.
    // fread lee un bloque de bytes del archivo y los coloca directamente en una variable.
    // Devuelve el número de elementos leídos.
    // El bucle continúa mientras se pueda leer 1 elemento completo (sizeof(Persona)).
    while (fread(&p, sizeof(Persona), 1, archivo) == 1) {
        // No hay necesidad de parsear. Los datos ya están en la estructura 'p'.
        printf("Nombre: %s, Edad: %d\n", p.nombre, p.edad);
    }

    // 3. Cerrar el archivo.
    fclose(archivo);

    return 0;
}
```

---

### **4. Verificación con Herramientas de Linux**

Esta es la parte clave para entender la diferencia desde la perspectiva del SO. Abre una terminal y ejecuta los programas:

```bash
gcc escribir_texto.c -o escribir_texto
./escribir_texto

gcc escribir_binario.c -o escribir_binario
./escribir_binario
```

Ahora tendrás dos archivos: `personas.txt` y `personas.bin`. Usemos herramientas de Linux para "ver" su contenido.

#### **Comando `cat`**
`cat` intenta mostrar el contenido de un archivo como texto.

```bash
$ cat personas.txt
Ana, 28
Luis, 34
Marta, 22
```
El resultado es perfectamente legible.

```bash
$ cat personas.bin
Ana, 28Luis, 34Marta, 22
```
El resultado es un galimatías. `cat` interpreta los bytes del número `28` (y otros) como caracteres, lo que produce basura legible y caracteres de control. Esto demuestra que no es un archivo de texto.

#### **Comando `hexdump -C`**
`hexdump` muestra el contenido de un archivo en formato hexadecimal y su equivalente en ASCII. Es la herramienta perfecta para inspeccionar archivos binarios.

```bash
$ hexdump -C personas.txt
00000000  41 6e 61 2c 20 32 38 0a  4c 75 69 73 2c 20 33 34  |Ana, 28.Luis, 34|
00000010  0a 4d 61 72 74 61 2c 20  32 32 0a                 |.Marta, 22.|
```
*   **Hexadecimal (izquierda):** `41` es 'A', `6e` es 'n', `2c` es ',', `20` es espacio, `32` es '2', `38` es '8', `0a` es el salto de línea `\n`.
*   **ASCII (derecha):** Se ve claramente el texto.

```bash
$ hexdump -C personas.bin
00000000  41 6e 61 00 00 00 00 00  00 00 00 00 00 00 00 00  |Ana.............|
00000010  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
00000030  00 00 00 00 1c 00 00 00  4c 75 69 73 00 00 00 00  |........Luis....|
00000040  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
00000060  00 00 00 00 22 00 00 00  4d 61 72 74 61 00 00 00  |...."...Marta...|
00000070  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
```
*   **Hexadecimal (izquierda):**
    *   `41 6e 61` es 'A', 'n', 'a'.
    *   `00 00 ...` son los bytes restantes del campo `nombre` (rellenados con ceros, ya que no se usaron todos).
    *   `1c 00 00 00` (en la dirección `0x30`) es la representación en memoria del entero `28` (en little-endian).
    *   `22 00 00 00` (en la dirección `0x60`) es el entero `34`.
*   **ASCII (derecha):** Solo se ven los nombres, los números no se corresponden con caracteres imprimibles.

#### **Comando `file`**
El comando `file` intenta adivinar el tipo de un archivo analizando su contenido.

```bash
$ file personas.txt
personas.txt: ASCII text
```
Lo identifica correctamente como texto.

```bash
$ file personas.bin
personas.bin: data
```
No puede identificarlo como un formato específico (como PNG o ELF), así que lo clasifica genéricamente como "datos", lo que es típico para archivos binarios personalizados.

---

### **5. Tabla Comparativa Resumen**

| Característica | Archivo de Texto Plano | Archivo Binario |
| :--- | :--- | :--- |
| **Contenido** | Caracteres legibles (ASCII, UTF-8). | Bytes crudos, copia exacta de la memoria. |
| **Representación** | Los números se convierten a su representación de texto (ej. `123` -> `'1' '2' '3'`). | Los números se guardan en su formato binario nativo (ej. `123` -> `0x7B 0x00 0x00 0x00`). |
| **Rendimiento** | Más lento (requiere formateo y parseo). | Muy rápido (lectura/escritura directa de bytes). |
| **Espacio** | Puede ser ineficiente. | Generalmente más compacto. |
| **Portabilidad** | Alta. | Baja (depende de la arquitectura). |
| **Legibilidad** | Alta (se puede leer con `cat`, `less`). | Nula (requiere `hexdump` o programas específicos). |
| **Funciones C** | `fprintf`, `fscanf`, `fputs`, `fgets`. | `fwrite`, `fread`. |
| **Modos Apertura** | `"r"`, `"w"`, `"a"`. | `"rb"`, `"wb"`, `"ab"`. |

### **Conclusión**

La elección entre un archivo de texto o binario depende completamente del caso de uso:

*   Usa **archivos de texto** para archivos de configuración, logs, datos que necesiten ser editados a mano o intercambiados entre sistemas muy diferentes.
*   Usa **archivos binarios** para almacenar grandes volúmenes de datos numéricos o estructuras complejas, cuando el rendimiento y el espacio son críticos, y cuando el archivo solo será leído por tu propia aplicación en un entorno controlado.

Entender esta diferencia es fundamental para un programador, ya que impacta directamente en el rendimiento, la portabilidad y la robustez de las aplicaciones que gestionan datos.
