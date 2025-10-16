### **Introducción: ¿Qué es la Compresión de Datos?**

La compresión de datos es el proceso de codificar información utilizando menos bits que su representación original. El objetivo principal es reducir el tamaño de los archivos para ahorrar espacio en disco o disminuir el tiempo de transmisión.

Existen dos tipos principales de compresión:

*   **Sin Pérdida (Lossless):** El archivo original puede reconstruirse perfectamente. Esencial para texto, código, ejecutables y cualquier dato donde la pérdida de información es inaceptable.
*   **Con Pérdida (Lossy):** Se elimina información "menos importante" para lograr una compresión mayor. Usado en multimedia (imágenes JPEG, audio MP3).

En este documento, nos centraremos en la **compresión sin pérdida** y diseñaremos dos modelos para archivos binarios.

---

### **Modelo 1: Run-Length Encoding (RLE)**

#### **Concepto**

RLE es uno de los algoritmos más simples. Su idea es reemplazar secuencias de datos idénticos (corridas) con un único dato y un contador.

*   **Ejemplo:** La secuencia de bytes `AAAAABBBCCAAAAAA` se puede comprimir como `5A3B2C6A`.

#### **Diseño del Formato Comprimido**

Un diseño simple es usar pares `[contador][byte]`. El problema: una corrida de longitud 1 (`A`) se convertiría en `1A`, ocupando el doble de espacio.

**Solución de diseño:** Solo comprimiremos corridas con una longitud mayor a un umbral (ej. 3). Si la corrida es más corta, escribimos los bytes tal cual. Para que el descompresor sepa interpretar esto, usamos un byte especial de "escape".

**Formato Propuesto:**
1.  **Byte Literal:** Si el byte no forma parte de una corrida larga, se escribe tal cual.
2.  **Secuencia Comprimida:** Si encontramos una corrida de `N` bytes idénticos (donde `N > 3`), escribimos un byte de "escape" (ej. `0xFF`), seguido del contador `N` y luego del byte que se repite.
    *   Ejemplo: `AAAAA` -> `0xFF 0x05 0x41` (asumiendo `0xFF` como escape y 'A' como `0x41`).

#### **Implementación en C**

**`rle_compress.c`**
```c
#include <stdio.h>
#include <stdlib.h>

/* --- Constantes de Diseño --- */
// Byte especial que marca el inicio de una secuencia comprimida.
// Permite al descompresor distinguir entre un literal y un puntero.
#define ESCAPE_BYTE 0xFF
// Longitud mínima para que una corrida sea "compresible".
// Corridas más cortas no se benefician (ej. "AA" -> "1A2A" es más grande).
#define MIN_RUN_LENGTH 4

/*
 * Función para comprimir un archivo usando el algoritmo Run-Length Encoding (RLE).
 *
 * CÓMO FUNCIONA:
 * 1. Abre los archivos de entrada y salida en modo binario ("rb", "wb").
 *    - El modo binario es crucial para evitar que el SO altere los bytes.
 * 2. Lee el archivo de entrada byte por byte.
 * 3. Para cada byte, cuenta cuántas veces se repite consecutivamente (la "corrida").
 * 4. Si la corrida es lo suficientemente larga, la comprime escribiendo
 *    [escape][longitud][byte].
 * 5. Si la corrida es muy corta, escribe los bytes sin comprimir.
 * 6. Cierra ambos archivos para liberar los recursos del SO.
 */
void compress_rle(const char* input_filename, const char* output_filename) {
    // Abrir archivo de entrada en modo lectura binaria para leer bytes crudos.
    FILE *fin = fopen(input_filename, "rb");
    // Abrir archivo de salida en modo escritura binaria para escribir bytes crudos.
    FILE *fout = fopen(output_filename, "wb");
    if (!fin || !fout) {
        perror("Error al abrir archivos");
        if (fin) fclose(fin);
        if (fout) fclose(fout);
        return;
    }

    int current_byte, next_byte;
    int count = 0;

    // Leemos el primer byte para iniciar el proceso. Si el archivo está vacío, salimos.
    current_byte = fgetc(fin);
    if (current_byte == EOF) {
        fclose(fin);
        fclose(fout);
        return; // Archivo vacío, nada que hacer.
    }

    // Bucle principal de procesamiento. Se ejecuta hasta llegar al final del archivo.
    while (1) {
        count = 1; // Siempre hay al menos una ocurrencia del byte actual.
        // Bucle interno para contar la longitud de la corrida.
        // Comparamos el byte actual con el siguiente. Mientras sean iguales, seguimos contando.
        while ((next_byte = fgetc(fin)) == current_byte && next_byte != EOF) {
            count++;
        }

        // Decisión clave: ¿Comprimir o no?
        if (count >= MIN_RUN_LENGTH) {
            // La corrida es lo suficientemente larga como para ser útil.
            // Escribimos la secuencia comprimida: [escape][contador][byte].
            fputc(ESCAPE_BYTE, fout);
            fputc(count, fout);
            fputc(current_byte, fout);
        } else {
            // La corrida es muy corta. Comprimirla sería ineficiente.
            // Escribimos el byte 'count' veces sin comprimir, tal cual.
            for (int i = 0; i < count; i++) {
                fputc(current_byte, fout);
            }
        }

        // Preparamos para la siguiente iteración.
        // 'next_byte' ya es el primer byte de la siguiente corrida (o EOF).
        current_byte = next_byte;
        if (current_byte == EOF) {
            break; // Si llegamos al final del archivo, salimos del bucle principal.
        }
    }

    // Liberar los descriptores de archivo. Es fundamental para que el SO
    // guarde los cambios en el disco (flush) y libere los recursos.
    fclose(fin);
    fclose(fout);
    printf("Archivo '%s' comprimido como '%s' usando RLE.\n", input_filename, output_filename);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <archivo_entrada> <archivo_salida>\n", argv[0]);
        return 1;
    }
    compress_rle(argv[1], argv[2]);
    return 0;
}
```

**`rle_decompress.c`**
```c
#include <stdio.h>
#include <stdlib.h>

// Debe ser el mismo byte de escape que se usó en la compresión.
#define ESCAPE_BYTE 0xFF

/*
 * Función para descomprimir un archivo comprimido con el algoritmo RLE de este programa.
 *
 * CÓMO FUNCIONA:
 * 1. Abre los archivos en modo binario.
 * 2. Lee el archivo comprimido byte por byte.
 * 3. Actúa como una máquina de estados:
 *    - Si el byte leído es el byte de escape (0xFF), entra en "modo puntero".
 *      - Lee los dos bytes siguientes: el contador y el dato.
 *      - Escribe el dato 'contador' veces en el archivo de salida.
 *    - Si el byte leído no es el de escape, entra en "modo literal".
 *      - Simplemente copia ese byte al archivo de salida.
 * 4. Continúa hasta llegar al final del archivo comprimido.
 */
void decompress_rle(const char* input_filename, const char* output_filename) {
    FILE *fin = fopen(input_filename, "rb");
    FILE *fout = fopen(output_filename, "wb");
    if (!fin || !fout) {
        perror("Error al abrir archivos");
        if (fin) fclose(fin);
        if (fout) fclose(fout);
        return;
    }

    int byte;
    // Bucle principal que lee el archivo comprimido hasta el final.
    while ((byte = fgetc(fin)) != EOF) {
        if (byte == ESCAPE_BYTE) {
            // ESTADO: Encontramos una secuencia comprimida.
            // Leemos el contador y el byte de datos que le siguen.
            int count = fgetc(fin);
            int data_byte = fgetc(fin);
            if (count == EOF || data_byte == EOF) {
                fprintf(stderr, "Error: Formato de archivo comprimido inválido.\n");
                break; // El archivo está corrupto o incompleto.
            }
            // Escribimos el byte de datos 'count' veces para reconstruir la corrida original.
            for (int i = 0; i < count; i++) {
                fputc(data_byte, fout);
            }
        } else {
            // ESTADO: Es un byte literal.
            // Lo escribimos tal cual en el archivo de salida.
            fputc(byte, fout);
        }
    }

    fclose(fin);
    fclose(fout);
    printf("Archivo '%s' descomprimido como '%s'.\n", input_filename, output_filename);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <archivo_entrada> <archivo_salida>\n", argv[0]);
        return 1;
    }
    decompress_rle(argv[1], argv[2]);
    return 0;
}
```

---

### **Modelo 2: Lempel-Ziv (Variante de LZ77)**

#### **Concepto**

LZ77 es mucho más potente. Su idea es reemplazar secuencias de bytes que ya han aparecido por una referencia (puntero) a la aparición anterior. Este puntero tiene dos valores:
*   **Distancia (Offset):** Cuántos bytes ir hacia atrás en el archivo ya descomprimido.
*   **Longitud (Length):** Cuántos bytes de esa secuencia anterior copiar.

*   **Ejemplo:** En `the cat in the hat`, la segunda `the ` puede ser un puntero que dice "ve 12 bytes hacia atrás y copia 4 bytes".

#### **Diseño del Formato Comprimido**

Necesitamos distinguir entre un byte literal y un puntero (distancia, longitud).

**Formato Propuesto (basado en tokens):**
1.  **Token Literal:** Un byte normal. Si no es el flag de puntero, es un literal.
2.  **Token de Puntero:** Un byte flag (ej. `0xFF`), seguido de dos bytes para la distancia y uno para la longitud.
    *   Formato: `0xFF` `[Byte Distancia Alta]` `[Byte Distancia Baja]` `[Byte Longitud]`
    *   Nota: Simplificamos la longitud a 1 byte (máx 255) y la distancia a 2 bytes (máx 65535).

#### **Implementación en C**

**`lz77_compress.c`**
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- Constantes de Diseño --- */
// Tamaño de la ventana de búsqueda (datos ya procesados).
// 4 KB es un buen balance entre memoria y probabilidad de encontrar coincidencias.
#define WINDOW_SIZE 4096
// Longitud máxima de una coincidencia que buscaremos.
// Simplifica el formato (longitud en 1 byte) y evita coincidencias excesivamente largas.
#define LOOKAHEAD_SIZE 18

// Estructura para almacenar el resultado de la búsqueda de una coincidencia.
typedef struct {
    int offset; // Distancia hacia atrás en la ventana.
    int length; // Longitud de la coincidencia.
} Match;

/*
 * Busca la coincidencia más larga entre el buffer de lookahead y la ventana.
 *
 * CÓMO FUNCIONA:
 * 1. Itera sobre cada posible posición de inicio en la ventana.
 * 2. Desde esa posición, compara byte a byte con el buffer de lookahead.
 * 3. Mantiene un registro de la coincidencia más larga encontrada.
 * 4. Solo considera coincidencias útiles (longitud > 2).
 */
Match find_longest_match(const char* window, const char* lookahead, int window_pos, int lookahead_size) {
    Match best_match = {0, 0};
    // Iteramos por cada posible punto de inicio en la ventana.
    for (int i = 0; i < window_pos; i++) {
        int current_match_len = 0;
        // Comparamos la ventana con el buffer de lookahead carácter por carácter.
        while (current_match_len < lookahead_size &&
               window[i + current_match_len] == lookahead[current_match_len]) {
            current_match_len++;
        }
        // Si encontramos una coincidencia mejor y es útil (longitud > 2), la guardamos.
        if (current_match_len > best_match.length && current_match_len > 2) {
            best_match.length = current_match_len;
            // La distancia es la posición actual en la ventana menos el inicio de la coincidencia.
            best_match.offset = window_pos - i;
        }
    }
    return best_match;
}

/*
 * Función para comprimir un archivo usando una variante del algoritmo LZ77.
 *
 * CÓMO FUNCIONA:
 * 1. Inicializa una "ventana deslizante" y un buffer de "lookahead".
 * 2. Llena la ventana con los primeros bytes del archivo.
 * 3. En un bucle:
 *    a. Llena el buffer de lookahead con los siguientes bytes del archivo.
 *    b. Busca la mejor coincidencia del buffer de lookahead dentro de la ventana.
 *    c. Si se encuentra una coincidencia buena, escribe un token de puntero
 *       (offset, length) y desliza la ventana hacia adelante por la longitud de la coincidencia.
 *    d. Si no, escribe el primer byte del lookahead como un token literal
 *       y desliza la ventana hacia adelante por un byte.
 * 4. Repite hasta que se procese todo el archivo.
 */
void compress_lz77(const char* input_filename, const char* output_filename) {
    FILE *fin = fopen(input_filename, "rb");
    FILE *fout = fopen(output_filename, "wb");
    if (!fin || !fout) {
        perror("Error al abrir archivos");
        if (fin) fclose(fin);
        if (fout) fclose(fout);
        return;
    }

    // Buffers para la ventana de búsqueda y el buffer de lookahead.
    char window[WINDOW_SIZE];
    char lookahead[LOOKAHEAD_SIZE];
    int window_pos = 0;
    int bytes_read;

    // 1. "Cebamos" la ventana con los primeros datos del archivo.
    window_pos = fread(window, 1, WINDOW_SIZE, fin);

    // Bucle principal que continúa mientras haya datos en la ventana.
    while (window_pos > 0) {
        // 2. Leemos los datos que vamos a procesar en esta iteración.
        bytes_read = fread(lookahead, 1, LOOKAHEAD_SIZE, fin);

        // 3. Buscamos la mejor coincidencia posible.
        Match match = find_longest_match(window, lookahead, window_pos, bytes_read);

        if (match.length > 0) {
            // 4a. Encontramos una coincidencia, escribimos un token de puntero.
            // Nuestro formato simplificado: un flag (0xFF), luego offset y length.
            fputc(0xFF, fout); // Flag para indicar que viene un puntero.
            // Descomponemos el offset (16 bits) en dos bytes (8 bits cada uno).
            fputc((match.offset >> 8) & 0xFF, fout); // Byte alto del offset.
            fputc(match.offset & 0xFF, fout);        // Byte bajo del offset.
            fputc(match.length, fout);               // Longitud de la coincidencia.

            // Deslizamos la ventana hacia adelante por la longitud de la coincidencia.
            // Usamos memmove porque las áreas de memoria pueden solaparse. Es más seguro.
            memmove(window, window + match.length, WINDOW_SIZE - match.length);
            // Rellenamos el espacio vacío en la ventana con nuevos datos del archivo.
            int new_bytes = fread(window + WINDOW_SIZE - match.length, 1, match.length, fin);
            window_pos = WINDOW_SIZE - match.length + new_bytes;

        } else {
            // 4b. No hay coincidencia útil, escribimos un token literal.
            // Escribimos el primer byte del lookahead.
            fputc(lookahead[0], fout);

            // Deslizamos la ventana hacia adelante por un solo byte.
            memmove(window, window + 1, WINDOW_SIZE - 1);
            // Leemos un solo byte nuevo para rellenar el hueco.
            int new_byte = fgetc(fin);
            if (new_byte != EOF) {
                window[WINDOW_SIZE - 1] = new_byte;
            } else {
                // Si no hay más bytes, la ventana se encoge.
                window_pos--;
            }
        }
    }

    fclose(fin);
    fclose(fout);
    printf("Archivo '%s' comprimido como '%s' usando LZ77.\n", input_filename, output_filename);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <archivo_entrada> <archivo_salida>\n", argv[0]);
        return 1;
    }
    compress_lz77(argv[1], argv[2]);
    return 0;
}
```

**`lz77_decompress.c`**
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// El buffer de salida debe ser lo suficientemente grande para contener
// la distancia máxima de búsqueda (4096) y los nuevos datos que se van añadiendo.
#define OUTPUT_BUFFER_SIZE 4096 * 2

/*
 * Función para descomprimir un archivo comprimido con nuestra variante de LZ77.
 *
 * CÓMO FUNCIONA:
 * 1. Lee el archivo comprimido byte por byte.
 * 2. Actúa como una máquina de estados inversa a la del compresor:
 *    - Si el byte es el flag de puntero (0xFF):
 *      a. Lee el offset y la longitud.
 *      b. Recombina los dos bytes del offset en un entero de 16 bits.
 *      c. Copia `length` bytes desde el buffer de salida (que actúa como diccionario)
 *         hacia atrás `offset` posiciones. Esta es la magia de LZ77.
 *    - Si el byte no es el flag:
 *      a. Es un byte literal. Lo escribe en el archivo de salida.
 *      b. También lo añade al buffer de salida para que pueda ser usado en futuras coincidencias.
 */
void decompress_lz77(const char* input_filename, const char* output_filename) {
    FILE *fin = fopen(input_filename, "rb");
    FILE *fout = fopen(output_filename, "wb");
    if (!fin || !fout) {
        perror("Error al abrir archivos");
        if (fin) fclose(fin);
        if (fout) fclose(fout);
        return;
    }

    // Este buffer actúa como nuestro "diccionario" dinámico.
    char output_buffer[OUTPUT_BUFFER_SIZE] = {0};
    int output_pos = 0; // Posición actual donde escribimos en el buffer.

    int byte;
    while ((byte = fgetc(fin)) != EOF) {
        if (byte == 0xFF) { // ESTADO: Encontramos un token de puntero.
            // Leemos los componentes del puntero.
            int offset_high = fgetc(fin);
            int offset_low = fgetc(fin);
            int length = fgetc(fin);
            if (offset_high == EOF || offset_low == EOF || length == EOF) break;

            // Recombinamos los dos bytes del offset en un entero de 16 bits.
            int offset = (offset_high << 8) | offset_low;

            // Copiamos desde el buffer de salida.
            // No leemos del archivo de entrada, sino de nuestro propio
            // buffer de salida, que ya contiene los datos descomprimidos.
            for (int i = 0; i < length; i++) {
                // Calculamos la posición de origen en el buffer.
                int source_pos = output_pos - offset;
                char byte_to_copy = output_buffer[source_pos];
                // Escribimos el byte copiado en el archivo de salida.
                fputc(byte_to_copy, fout);
                // Y también lo guardamos en el buffer para futuras referencias.
                // El operador módulo (%) hace que el buffer sea "circular",
                // permitiéndonos reutilizar la memoria una vez que nos pasamos del tamaño.
                output_buffer[output_pos % OUTPUT_BUFFER_SIZE] = byte_to_copy;
                output_pos++;
            }
        } else { // ESTADO: Es un token literal.
            // Escribimos el byte literal directamente en el archivo de salida.
            fputc(byte, fout);
            // Lo guardamos en nuestro buffer para que pueda ser parte de futuras coincidencias.
            output_buffer[output_pos % OUTPUT_BUFFER_SIZE] = byte;
            output_pos++;
        }
    }

    fclose(fin);
    fclose(fout);
    printf("Archivo '%s' descomprimido como '%s'.\n", input_filename, output_filename);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <archivo_entrada> <archivo_salida>\n", argv[0]);
        return 1;
    }
    decompress_lz77(argv[1], argv[2]);
    return 0;
}
```

---

### **3. Comparación de Modelos y Conclusión**

| Característica | Run-Length Encoding (RLE) | Lempel-Ziv (LZ77) |
| :--- | :--- | :--- |
| **Tipo de Datos Ideal** | Datos con corridas muy largas (ej. imágenes con áreas de color uniforme). | Casi cualquier tipo de datos con patrones repetitivos (texto, código, datos mixtos). |
| **Complejidad** | Muy baja. Fácil de implementar y entender. | Alta. Requiere gestión de buffers, búsqueda de coincidencias y punteros. |
| **Ratio de Compresión** | Generalmente bajo. Puede incluso aumentar el tamaño de archivos sin corridas largas. | Generalmente alto y consistente. |
| **Rendimiento** | Muy rápido tanto en compresión como descompresión. | Más lento en compresión (búsqueda), pero rápido en descompresión. |
| **Uso en el Mundo Real** | Como parte de algoritmos más complejos (ej. en el formato BMP, PCX). | Base para formatos omnipresentes como **ZIP**, **GZIP**, **PNG** (que usan una variante llamada DEFLATE, que combina LZ77 y codificación Huffman). |

### **Conclusión**

Hemos diseñado e implementado dos modelos de compresión fundamentales.

*   **RLE** nos enseña el concepto básico de eliminar redundancia local y es una excelente introducción a la manipulación de flujos de bytes.
*   **LZ77** nos introduce a un paradigma mucho más poderoso: el uso de un diccionario dinámico (la ventana deslizante) para eliminar redundancias a distancia, que es la base de la compresión moderna.

Estas implementaciones, aunque educativas y no optimizadas para producción, revelan la lógica interna que hace posible que herramientas como `gzip` o `zip` reduzcan drásticamente el tamaño de nuestros archivos. Para aplicaciones del mundo real, se recomienda usar librerías robustas y altamente optimizadas como **zlib**, que implementan el algoritmo DEFLATE de manera estándar y eficiente.
