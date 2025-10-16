### **1. Introducción: ¿Qué es la Compresión de Datos?**

La compresión de datos es el proceso de codificar información utilizando menos bits que la representación original. El objetivo principal es reducir el tamaño de los archivos para ahorrar espacio en disco o disminuir el tiempo de transmisión.

Existen dos tipos principales de compresión:

*   **Sin Pérdida (Lossless):** El archivo original puede reconstruirse perfectamente a partir del comprimido. Esencial para archivos de texto, código fuente, ejecutables y, en general, cualquier dato donde la pérdida de información es inaceptable.
*   **Con Pérdida (Lossy):** Se elimina información "menos importante" para lograr una compresión mucho mayor. Usado comúnmente en archivos multimedia (imágenes JPEG, audio MP3, video MPEG).

En este documento, nos centraremos en la **compresión sin pérdida** y diseñaremos dos modelos para archivos binarios.

---

### **Modelo 1: Run-Length Encoding (RLE)**

#### **Concepto**

RLE es uno de los algoritmos de compresión más simples. Su idea es reemplazar secuencias de datos idénticos (corridas) con un único dato y un contador.

*   **Ejemplo:** La secuencia de bytes `AAAAABBBCCAAAAAA` se puede comprimir como `5A3B2C6A`.

#### **Diseño del Formato Comprimido**

Un diseño simple pero efectivo es usar pares de `[contador][byte]`. Sin embargo, esto tiene un problema: ¿qué pasa si la corrida es de longitud 1? `A` se convertiría en `1A`, ocupando el doble de espacio.

**Solución de diseño:** Solo comprimiremos las corridas que tengan una longitud mayor a un umbral (por ejemplo, 3). Si una corrida es más corta, escribimos los bytes tal cual. Para que el descompresor sepa interpretar esto, necesitamos una forma de marcar el inicio de una secuencia comprimida.

**Formato Propuesto:**
1.  **Byte Literal:** Si el byte leído del archivo original no forma parte de una corrida larga, se escribe tal cual en el archivo comprimido.
2.  **Secuencia Comprimida:** Si encontramos una corrida de `N` bytes idénticos (donde `N > 3`), escribimos un byte especial de "escape" (ej. `0xFF`), seguido del contador `N` y luego del byte que se repite.
    *   Ejemplo: `AAAAA` -> `0xFF 0x05 0x41` (asumiendo `0xFF` como escape y que 'A' es `0x41`).

#### **Implementación en C**

**`rle_compress.c`**
```c
#include <stdio.h>
#include <stdlib.h>

#define ESCAPE_BYTE 0xFF
#define MIN_RUN_LENGTH 4

// Función para comprimir un archivo usando RLE
void compress_rle(const char* input_filename, const char* output_filename) {
    FILE *fin = fopen(input_filename, "rb");
    FILE *fout = fopen(output_filename, "wb");
    if (!fin || !fout) {
        perror("Error al abrir archivos");
        if (fin) fclose(fin);
        if (fout) fclose(fout);
        return;
    }

    int current_byte, next_byte;
    int count = 0;

    // Leemos el primer byte para iniciar el proceso
    current_byte = fgetc(fin);
    if (current_byte == EOF) {
        fclose(fin);
        fclose(fout);
        return; // Archivo vacío
    }

    while (1) {
        count = 1;
        // Contamos la longitud de la corrida del byte actual
        while ((next_byte = fgetc(fin)) == current_byte && next_byte != EOF) {
            count++;
        }

        // Decidimos si comprimir o escribir literalmente
        if (count >= MIN_RUN_LENGTH) {
            // Escribimos la secuencia comprimida: [escape][contador][byte]
            fputc(ESCAPE_BYTE, fout);
            fputc(count, fout);
            fputc(current_byte, fout);
        } else {
            // Escribimos el byte 'count' veces sin comprimir
            for (int i = 0; i < count; i++) {
                fputc(current_byte, fout);
            }
        }

        // Preparamos para la siguiente iteración
        current_byte = next_byte;
        if (current_byte == EOF) {
            break; // Fin del archivo
        }
    }

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

#define ESCAPE_BYTE 0xFF

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
    while ((byte = fgetc(fin)) != EOF) {
        if (byte == ESCAPE_BYTE) {
            // Es una secuencia comprimida. Leemos el contador y el byte.
            int count = fgetc(fin);
            int data_byte = fgetc(fin);
            if (count == EOF || data_byte == EOF) {
                fprintf(stderr, "Error: Formato de archivo comprimido inválido.\n");
                break;
            }
            // Escribimos el byte 'count' veces
            for (int i = 0; i < count; i++) {
                fputc(data_byte, fout);
            }
        } else {
            // Es un byte literal, lo escribimos tal cual.
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

**Prueba:** Crea un archivo con muchas repeticiones (ej. `AAAAA...BBBB...CCCC`) y compruébalo.

---

### **Modelo 2: Lempel-Ziv (Variante de LZ77)**

#### **Concepto**

LZ77 es un algoritmo mucho más potente que RLE. Su idea es reemplazar secuencias de bytes que ya han aparecido anteriormente en el archivo por una referencia (un puntero) a la aparición anterior. Este puntero se compone de dos valores:
*   **Distancia (Offset):** Cuántos bytes hay que ir hacia atrás en el archivo ya descomprimido para encontrar la secuencia.
*   **Longitud (Length):** Cuántos bytes de esa secuencia anterior hay que copiar.

*   **Ejemplo:** En el texto `the cat in the hat`, la segunda aparición de `the ` puede ser reemplazada por un puntero que dice "ve 12 bytes hacia atrás y copia 4 bytes".

#### **Diseño del Formato Comprimido**

Necesitamos una forma de distinguir entre un byte literal y un puntero (distancia, longitud).

**Formato Propuesto (basado en tokens):**
1.  **Token Literal:** Un byte que comienza con el bit `0`. El resto del byte es el dato literal.
    *   Formato: `0XXXXXXX` (donde `X` es el dato de 7 bits).
2.  **Token de Puntero (Distancia/Longitud):** Un byte que comienza con el bit `1`, seguido de dos bytes más para la distancia y la longitud.
    *   Formato: `1XXXXXXX` `[Byte Distancia Alta]` `[Byte Distancia Baja]` `[Byte Longitud]`
    *   Nota: Simplificaremos la longitud a un solo byte (máx 255) y la distancia a dos bytes (máx 65535).

#### **Implementación en C**

Esta implementación es más compleja y requiere una "ventana deslizante" para buscar coincidencias.

**`lz77_compress.c`**
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WINDOW_SIZE 4096   // Tamaño de la ventana de búsqueda (buffer deslizante)
#define LOOKAHEAD_SIZE 18  // Tamaño máximo de la coincidencia a buscar

typedef struct {
    int offset; // Distancia
    int length; // Longitud
} Match;

// Función para encontrar la mejor coincidencia en la ventana
Match find_longest_match(const char* window, const char* lookahead, int window_pos, int lookahead_size) {
    Match best_match = {0, 0};
    for (int i = 0; i < window_pos; i++) {
        int current_match_len = 0;
        // Comparamos la ventana con el buffer de lookahead
        while (current_match_len < lookahead_size &&
               window[i + current_match_len] == lookahead[current_match_len]) {
            current_match_len++;
        }
        // Si encontramos una coincidencia mejor y es útil (longitud > 2)
        if (current_match_len > best_match.length && current_match_len > 2) {
            best_match.length = current_match_len;
            best_match.offset = window_pos - i;
        }
    }
    return best_match;
}

void compress_lz77(const char* input_filename, const char* output_filename) {
    FILE *fin = fopen(input_filename, "rb");
    FILE *fout = fopen(output_filename, "wb");
    if (!fin || !fout) {
        perror("Error al abrir archivos");
        if (fin) fclose(fin);
        if (fout) fclose(fout);
        return;
    }

    char window[WINDOW_SIZE];
    char lookahead[LOOKAHEAD_SIZE];
    int window_pos = 0;
    int bytes_read;

    // Inicializamos la ventana con datos del archivo
    window_pos = fread(window, 1, WINDOW_SIZE, fin);

    while (window_pos > 0) {
        // Leemos el buffer de lookahead
        bytes_read = fread(lookahead, 1, LOOKAHEAD_SIZE, fin);

        // Buscamos la mejor coincidencia
        Match match = find_longest_match(window, lookahead, window_pos, bytes_read);

        if (match.length > 0) {
            // Escribimos un token de puntero (1xxxxxxx)
            // Simplificado: escribimos un flag, luego offset y length
            fputc(0xFF, fout); // Flag para indicar puntero
            fputc((match.offset >> 8) & 0xFF, fout); // Offset alto
            fputc(match.offset & 0xFF, fout);        // Offset bajo
            fputc(match.length, fout);               // Length

            // Movemos la ventana
            memmove(window, window + match.length, WINDOW_SIZE - match.length);
            int new_bytes = fread(window + WINDOW_SIZE - match.length, 1, match.length, fin);
            window_pos = WINDOW_SIZE - match.length + new_bytes;

        } else {
            // Escribimos un token literal (0xxxxxxx)
            fputc(lookahead[0] & 0x7F, fout);

            // Movemos la ventana un byte
            memmove(window, window + 1, WINDOW_SIZE - 1);
            int new_byte = fgetc(fin);
            if (new_byte != EOF) {
                window[WINDOW_SIZE - 1] = new_byte;
            } else {
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

#define OUTPUT_BUFFER_SIZE 4096 * 2

void decompress_lz77(const char* input_filename, const char* output_filename) {
    FILE *fin = fopen(input_filename, "rb");
    FILE *fout = fopen(output_filename, "wb");
    if (!fin || !fout) {
        perror("Error al abrir archivos");
        if (fin) fclose(fin);
        if (fout) fclose(fout);
        return;
    }

    char output_buffer[OUTPUT_BUFFER_SIZE] = {0};
    int output_pos = 0;

    int byte;
    while ((byte = fgetc(fin)) != EOF) {
        if (byte == 0xFF) { // Nuestro flag de puntero
            int offset_high = fgetc(fin);
            int offset_low = fgetc(fin);
            int length = fgetc(fin);
            if (offset_high == EOF || offset_low == EOF || length == EOF) break;

            int offset = (offset_high << 8) | offset_low;

            // Copiamos desde el buffer de salida
            for (int i = 0; i < length; i++) {
                int source_pos = output_pos - offset;
                char byte_to_copy = output_buffer[source_pos];
                fputc(byte_to_copy, fout);
                output_buffer[output_pos % OUTPUT_BUFFER_SIZE] = byte_to_copy;
                output_pos++;
            }
        } else { // Es un literal
            fputc(byte, fout);
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
| **Tipo de Datos Ideal** | Datos con corridas muy largas (ej. imágenes con áreas de color uniforme, archivos repetitivos). | Casi cualquier tipo de datos con patrones repetitivos (texto, código, datos mixtos). |
| **Complejidad** | Muy baja. Fácil de implementar y entender. | Alta. Requiere gestión de buffers, búsqueda de coincidencias y punteros. |
| **Ratio de Compresión** | Generalmente bajo. Puede incluso aumentar el tamaño de archivos sin corridas largas. | Generalmente alto y consistente. |
| **Rendimiento** | Muy rápido tanto en compresión como descompresión. | Más lento en compresión (búsqueda), pero rápido en descompresión. |
| **Uso en el Mundo Real** | Como parte de algoritmos más complejos (ej. en el formato BMP, PCX). | Base para formatos omnipresentes como **ZIP**, **GZIP**, **PNG** (que usan una variante llamada DEFLATE, que combina LZ77 y codificación Huffman). |

### **Conclusión**

Hemos diseñado e implementado dos modelos de compresión fundamentales.

*   **RLE** nos enseña el concepto básico de eliminar redundancia local y es una excelente introducción a la manipulación de flujos de bytes.
*   **LZ77** nos introduce a un paradigma mucho más poderoso: el uso de un diccionario dinámico (la ventana deslizante) para eliminar redundancias a distancia, que es la base de la compresión moderna.
