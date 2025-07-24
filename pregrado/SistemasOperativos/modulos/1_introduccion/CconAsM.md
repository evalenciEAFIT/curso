# Ejemplo de Integración entre C y Ensamblador

## 📋 Descripción General

Este tutorial muestra cómo integrar código C y ensamblador en un mismo programa, una práctica común en programación de sistemas donde se necesita el control fino del ensamblador junto con la comodidad de C.

## 🎯 Objetivos del Ejemplo

1. **Llamar funciones en ensamblador desde C**
2. **Llamar funciones de C desde ensamblador**
3. **Compartir variables entre ambos lenguajes**
4. **Manejar parámetros y valores de retorno**
5. **Demostrar casos de uso típicos**

---

## 📁 Estructura del Proyecto

```
proyecto-c-asm/
├── main.c              # Programa principal en C
├── funciones_asm.asm   # Funciones en ensamblador
├── funciones_c.c       # Funciones auxiliares en C
├── Makefile           # Archivo de construcción
└── README.md          # Documentación
```

---

## 📋 Ejemplo 1: Funciones Básicas en Ensamblador

### `funciones_asm.asm` - Funciones en Ensamblador

```nasm
; ================================================
; FUNCIONES EN ENSAMBLADOR
; Ejemplo de funciones típicas que se implementan en asm
; para mejor performance o acceso directo al hardware
; ================================================

section .data
    ; Mensajes para debugging
    msg_add_asm     db 'Funcion add_numbers_asm ejecutada', 0xA, 0
    msg_factorial   db 'Funcion factorial_asm ejecutada', 0xA, 0
    msg_string_ops  db 'Funcion string_operations_asm ejecutada', 0xA, 0

section .text
    ; ====================================================
    ; DECLARAR FUNCIONES GLOBALES
    ; Estas funciones serán llamadas desde C
    ; ====================================================
    global add_numbers_asm              ; Suma dos números
    global factorial_asm                ; Calcula factorial
    global string_operations_asm        ; Operaciones con strings
    global fast_memcpy                  ; Copia de memoria optimizada
    global bit_operations               ; Operaciones a nivel de bits
    global performance_test             ; Prueba de performance
    
    ; ====================================================
    ; DECLARAR FUNCIONES EXTERNAS DE C
    ; Estas funciones están definidas en archivos C
    ; ====================================================
    extern print_debug_message          ; Función de C para mensajes
    extern log_operation                ; Función de C para logging

; ================================================
; SUMA DE DOS NÚMEROS - Versión optimizada en ensamblador
; Esta función demuestra operaciones aritméticas básicas
; ================================================
add_numbers_asm:
    ; Convención System V AMD64 ABI:
    ; - Primer parámetro: RDI
    ; - Segundo parámetro: RSI
    ; - Valor de retorno: RAX
    
    push rbp                    ; Guardar frame pointer
    mov rbp, rsp                ; Establecer nuevo frame pointer
    
    ; Mostrar mensaje de debugging
    mov rdi, msg_add_asm        ; Cargar mensaje
    call print_debug_message    ; Llamar a función C
    
    ; Realizar suma
    mov rax, rdi                ; Cargar primer parámetro en RAX
    add rax, rsi                ; Sumar segundo parámetro
    
    ; Loggear la operación
    mov rdi, rax                ; Resultado para logging
    call log_operation          ; Llamar a función C
    
    pop rbp                     ; Restaurar frame pointer
    ret                         ; Retornar (resultado en RAX)

; ================================================
; CÁLCULO DE FACTORIAL - Ejemplo de bucle en ensamblador
; Demuestra estructuras de control y operaciones repetitivas
; ================================================
factorial_asm:
    push rbp                    ; Guardar frame pointer
    mov rbp, rsp                ; Establecer nuevo frame pointer
    push rbx                    ; Guardar registros usados
    push rcx
    
    ; Mostrar mensaje de debugging
    mov rdi, msg_factorial      ; Cargar mensaje
    call print_debug_message    ; Llamar a función C
    
    ; Validar entrada
    cmp rdi, 0                  ; Comparar con 0
    je .return_one              ; Si es 0, retornar 1
    cmp rdi, 20                 ; Comparar con límite (evitar overflow)
    ja .return_zero             ; Si es muy grande, retornar 0
    
    ; Inicializar variables para el cálculo
    mov rbx, rdi                ; Copiar número a RBX
    mov rax, 1                  ; Inicializar resultado = 1
    mov rcx, 1                  ; Inicializar contador = 1
    
.factorial_loop:
    cmp rcx, rbx                ; Comparar contador con número
    jg .done                    ; Si contador > número, terminar
    
    mul rcx                     ; Multiplicar RAX por RCX (RAX = RAX * RCX)
    inc rcx                     ; Incrementar contador
    jmp .factorial_loop         ; Continuar bucle
    
.return_one:
    mov rax, 1                  ; Retornar 1 para 0! = 1
    jmp .cleanup
    
.return_zero:
    xor rax, rax                ; Retornar 0 para valores inválidos
    jmp .cleanup
    
.done:
    ; Loggear la operación
    mov rdi, rax                ; Resultado para logging
    call log_operation          ; Llamar a función C
    
.cleanup:
    pop rcx                     ; Restaurar registros
    pop rbx
    pop rbp                     ; Restaurar frame pointer
    ret                         ; Retornar resultado en RAX

; ================================================
; OPERACIONES CON STRINGS - Manipulación de cadenas
; Demuestra acceso a memoria y operaciones con strings
; ================================================
string_operations_asm:
    push rbp                    ; Guardar frame pointer
    mov rbp, rsp                ; Establecer nuevo frame pointer
    push rbx                    ; Guardar registros usados
    push rcx
    push rdx
    
    ; Mostrar mensaje de debugging
    mov rdi, msg_string_ops     ; Cargar mensaje
    call print_debug_message    ; Llamar a función C
    
    ; Parámetros:
    ; RDI = puntero a string de entrada
    ; RSI = puntero a buffer de salida
    ; RDX = operación a realizar (1=mayúsculas, 2=minúsculas, 3=longitud)
    
    cmp rdx, 1                  ; Verificar tipo de operación
    je .to_uppercase
    cmp rdx, 2
    je .to_lowercase
    cmp rdx, 3
    je .get_length
    jmp .invalid_operation
    
.to_uppercase:
    call convert_to_uppercase
    jmp .cleanup
    
.to_lowercase:
    call convert_to_lowercase
    jmp .cleanup
    
.get_length:
    call calculate_length
    jmp .cleanup
    
.invalid_operation:
    xor rax, rax                ; Retornar 0 para operación inválida
    
.cleanup:
    pop rdx                     ; Restaurar registros
    pop rcx
    pop rbx
    pop rbp                     ; Restaurar frame pointer
    ret

; ================================================
; CONVERTIR STRING A MAYÚSCULAS
; Subrutina auxiliar para string_operations_asm
; ================================================
convert_to_uppercase:
    push rbp
    mov rbp, rsp
    push rsi                    ; Guardar buffer de salida
    push rdi                    ; Guardar string de entrada
    
.copy_and_convert_loop:
    movzx rax, byte [rdi]       ; Cargar carácter con extensión a 64-bit
    test rax, rax               ; Verificar fin de string
    jz .conversion_done         ; Si es 0, terminar
    
    ; Convertir a mayúscula si es letra minúscula
    cmp al, 'a'                 ; Comparar con 'a'
    jl .not_lowercase           ; Si es menor, no es minúscula
    cmp al, 'z'                 ; Comparar con 'z'
    jg .not_lowercase           ; Si es mayor, no es minúscula
    
    sub al, 32                  ; Convertir a mayúscula (diferencia ASCII)
    
.not_lowercase:
    mov [rsi], al               ; Guardar carácter convertido
    inc rdi                     ; Siguiente carácter de entrada
    inc rsi                     ; Siguiente posición de salida
    jmp .copy_and_convert_loop  ; Continuar bucle
    
.conversion_done:
    mov byte [rsi], 0           ; Terminar string con null
    mov rax, 1                  ; Retornar éxito
    
    pop rdi                     ; Restaurar registros
    pop rsi
    pop rbp
    ret

; ================================================
; CONVERTIR STRING A MINÚSCULAS
; Subrutina auxiliar para string_operations_asm
; ================================================
convert_to_lowercase:
    push rbp
    mov rbp, rsp
    push rsi
    push rdi
    
.copy_and_convert_lower_loop:
    movzx rax, byte [rdi]       ; Cargar carácter
    test rax, rax               ; Verificar fin de string
    jz .lower_conversion_done   ; Si es 0, terminar
    
    ; Convertir a minúscula si es letra mayúscula
    cmp al, 'A'                 ; Comparar con 'A'
    jl .not_uppercase           ; Si es menor, no es mayúscula
    cmp al, 'Z'                 ; Comparar con 'Z'
    jg .not_uppercase           ; Si es mayor, no es mayúscula
    
    add al, 32                  ; Convertir a minúscula
    
.not_uppercase:
    mov [rsi], al               ; Guardar carácter convertido
    inc rdi                     ; Siguiente carácter de entrada
    inc rsi                     ; Siguiente posición de salida
    jmp .copy_and_convert_lower_loop
    
.lower_conversion_done:
    mov byte [rsi], 0           ; Terminar string con null
    mov rax, 1                  ; Retornar éxito
    
    pop rdi
    pop rsi
    pop rbp
    ret

; ================================================
; CALCULAR LONGITUD DE STRING
; Subrutina auxiliar para string_operations_asm
; ================================================
calculate_length:
    push rbp
    mov rbp, rsp
    push rdi                    ; Guardar string de entrada
    
    xor rax, rax                ; Inicializar contador = 0
    
.length_loop:
    cmp byte [rdi + rax], 0     ; Comparar carácter actual con null
    je .length_done             ; Si es null, terminar
    inc rax                     ; Incrementar contador
    jmp .length_loop            ; Continuar bucle
    
.length_done:
    ; Loggear la operación
    mov rdi, rax                ; Longitud para logging
    call log_operation          ; Llamar a función C
    
    pop rdi                     ; Restaurar registros
    pop rbp
    ret

; ================================================
; COPIA DE MEMORIA OPTIMIZADA - Ejemplo de performance
; Demuestra técnicas de optimización a bajo nivel
; ================================================
fast_memcpy:
    push rbp
    mov rbp, rsp
    push rsi                    ; Guardar registros
    push rdi
    push rcx
    
    ; Parámetros:
    ; RDI = destino
    ; RSI = fuente
    ; RDX = número de bytes
    
    ; Optimización: usar movsq para copiar 8 bytes a la vez
    mov rcx, rdx                ; Cargar número de bytes
    shr rcx, 3                  ; Dividir por 8 (copiar 8 bytes a la vez)
    rep movsq                   ; Copiar quadwords (8 bytes cada uno)
    
    ; Copiar bytes restantes
    mov rcx, rdx                ; Restaurar número original
    and rcx, 7                  ; Obtener resto de división por 8
    rep movsb                   ; Copiar bytes individuales
    
    ; Retornar puntero al destino
    mov rax, rdi                ; Retornar destino en RAX
    
    pop rcx                     ; Restaurar registros
    pop rdi
    pop rsi
    pop rbp
    ret

; ================================================
; OPERACIONES A NIVEL DE BITS - Manipulación de bits
; Demuestra operaciones bit a bit típicas en sistemas
; ================================================
bit_operations:
    push rbp
    mov rbp, rsp
    
    ; Parámetros:
    ; RDI = valor
    ; RSI = operación (1=contar bits, 2=invertir, 3=encontrar LSB)
    
    cmp rsi, 1
    je .count_bits
    cmp rsi, 2
    je .reverse_bits
    cmp rsi, 3
    je .find_lsb
    jmp .invalid_bit_op
    
.count_bits:
    popcnt rax, rdi             ; Contar bits establecidos (instrucción POPCNT)
    jmp .bit_done
    
.reverse_bits:
    ; Invertir bits usando tabla de búsqueda o algoritmo bit-reversal
    mov rax, rdi
    ; Algoritmo simple de inversión bit a bit
    xor rcx, rcx                ; Contador
    xor rdx, rdx                ; Resultado
    
.reverse_loop:
    bt rax, rcx                 ; Test bit en posición RCX
    cmovc r8, rdx               ; Si carry, preparar para setear bit
    bts rdx, rcx                ; Setear bit si carry
    inc rcx
    cmp rcx, 64                 ; Para 64-bit
    jl .reverse_loop
    mov rax, rdx
    jmp .bit_done
    
.find_lsb:
    bsf rax, rdi                ; Buscar primer bit establecido (LSB)
    jmp .bit_done
    
.invalid_bit_op:
    xor rax, rax                ; Retornar 0 para operación inválida
    
.bit_done:
    pop rbp
    ret

; ================================================
; PRUEBA DE PERFORMANCE - Comparación de métodos
; Demuestra cómo medir performance en ensamblador
; ================================================
performance_test:
    push rbp
    mov rbp, rsp
    push rbx
    push rcx
    push rdx
    
    ; Guardar timestamp inicial
    rdtsc                       ; Leer contador de time-stamp
    shl rdx, 32                 ; Desplazar parte alta
    or rax, rdx                 ; Combinar en RAX
    mov rbx, rax                ; Guardar timestamp inicial
    
    ; Realizar operación de prueba (ejemplo: suma repetida)
    mov rcx, 1000000            ; Número de iteraciones
    xor rax, rax                ; Inicializar acumulador
    
.performance_loop:
    add rax, rcx                ; Sumar contador al acumulador
    loop .performance_loop      ; Decrementar RCX y continuar si no es 0
    
    ; Guardar timestamp final
    rdtsc                       ; Leer contador de time-stamp
    shl rdx, 32                 ; Desplazar parte alta
    or rax, rdx                 ; Combinar en RAX
    sub rax, rbx                ; Calcular diferencia de tiempo
    
    ; RAX ahora contiene ciclos de CPU usados
    pop rdx
    pop rcx
    pop rbx
    pop rbp
    ret
```

---

## 📋 Ejemplo 2: Funciones Complementarias en C

### `funciones_c.c` - Funciones auxiliares en C

```c
// ================================================
// FUNCIONES AUXILIARES EN C
// Estas funciones son llamadas desde el ensamblador
// ================================================

#include <stdio.h>
#include <time.h>

// Variable global para logging
static int operation_count = 0;

// ================================================
// IMPRIMIR MENSAJES DE DEBUGGING
// Función llamada desde ensamblador para mostrar mensajes
// ================================================
void print_debug_message(const char* message) {
    printf("[DEBUG ASM] %s", message);
    fflush(stdout);  // Forzar salida inmediata
}

// ================================================
// LOGUEAR OPERACIONES
// Función llamada desde ensamblador para registrar operaciones
// ================================================
void log_operation(long long result) {
    operation_count++;
    printf("[LOG] Operacion #%d - Resultado: %lld\n", 
           operation_count, result);
}

// ================================================
// OBTENER TIMESTAMP
// Función para obtener tiempo actual
// ================================================
long long get_timestamp() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000000000LL + ts.tv_nsec;
}

// ================================================
// IMPRIMIR INFORMACIÓN DEL SISTEMA
// Función para mostrar información del entorno
// ================================================
void print_system_info() {
    printf("=== INTEGRACION C - ENSAMBLADOR ===\n");
    printf("Sistema compilado y listo para pruebas\n");
    printf("===================================\n\n");
}
```

---

## 📋 Ejemplo 3: Programa Principal en C

### `main.c` - Programa principal

```c
// ================================================
// PROGRAMA PRINCIPAL EN C
// Este programa demuestra cómo llamar funciones en ensamblador
// ================================================

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ================================================
// DECLARACIONES DE FUNCIONES EN ENSAMBLADOR
// Estas funciones están implementadas en funciones_asm.asm
// ================================================
extern long long add_numbers_asm(long long a, long long b);
extern long long factorial_asm(long long n);
extern int string_operations_asm(char* input, char* output, long long operation);
extern void* fast_memcpy(void* dest, const void* src, size_t n);
extern long long bit_operations(long long value, long long operation);
extern long long performance_test();

// ================================================
// DECLARACIONES DE FUNCIONES EN C
// Estas funciones están implementadas en funciones_c.c
// ================================================
extern void print_debug_message(const char* message);
extern void log_operation(long long result);
extern void print_system_info();

// ================================================
// FUNCIONES AUXILIARES EN C
// ================================================
void test_arithmetic_operations();
void test_string_operations();
void test_bit_operations();
void test_performance();
void print_separator();

// ================================================
// PUNTO DE ENTRADA DEL PROGRAMA
// ================================================
int main() {
    printf("=== DEMO: Integracion C - Ensamblador ===\n\n");
    
    // Inicializar sistema
    print_system_info();
    
    // Ejecutar pruebas
    test_arithmetic_operations();
    test_string_operations();
    test_bit_operations();
    test_performance();
    
    printf("\n=== FIN DE LA DEMO ===\n");
    return 0;
}

// ================================================
// PRUEBA DE OPERACIONES ARITMÉTICAS
// ================================================
void test_arithmetic_operations() {
    printf("1. PRUEBAS DE OPERACIONES ARITMETICAS\n");
    print_separator();
    
    // Prueba de suma
    long long result1 = add_numbers_asm(15, 25);
    printf("Suma 15 + 25 = %lld\n", result1);
    
    // Prueba de factorial
    long long result2 = factorial_asm(5);
    printf("Factorial de 5 = %lld\n", result2);
    
    long long result3 = factorial_asm(0);
    printf("Factorial de 0 = %lld\n", result3);
    
    long long result4 = factorial_asm(25);  // Debería retornar 0 (demasiado grande)
    printf("Factorial de 25 = %lld (demasiado grande)\n", result4);
    
    printf("\n");
}

// ================================================
// PRUEBA DE OPERACIONES CON STRINGS
// ================================================
void test_string_operations() {
    printf("2. PRUEBAS DE OPERACIONES CON STRINGS\n");
    print_separator();
    
    char input[] = "Hola Mundo Ensamblador";
    char output[256];
    
    // Convertir a mayúsculas
    int success1 = string_operations_asm(input, output, 1);
    if (success1) {
        printf("Original: %s\n", input);
        printf("Mayusculas: %s\n", output);
    }
    
    // Convertir a minúsculas
    int success2 = string_operations_asm(input, output, 2);
    if (success2) {
        printf("Minusculas: %s\n", output);
    }
    
    // Calcular longitud
    int success3 = string_operations_asm(input, output, 3);
    if (success3) {
        printf("Longitud calculada en ensamblador\n");
    }
    
    printf("\n");
}

// ================================================
// PRUEBA DE OPERACIONES A NIVEL DE BITS
// ================================================
void test_bit_operations() {
    printf("3. PRUEBAS DE OPERACIONES A NIVEL DE BITS\n");
    print_separator();
    
    long long value = 0b1010101010101010;  // Valor binario de prueba
    
    // Contar bits establecidos
    long long bit_count = bit_operations(value, 1);
    printf("Valor: 0x%llX\n", value);
    printf("Bits establecidos: %lld\n", bit_count);
    
    // Encontrar LSB (Least Significant Bit)
    long long lsb = bit_operations(value, 3);
    printf("Posicion del LSB: %lld\n", lsb);
    
    printf("\n");
}

// ================================================
// PRUEBA DE PERFORMANCE
// ================================================
void test_performance() {
    printf("4. PRUEBA DE PERFORMANCE\n");
    print_separator();
    
    printf("Ejecutando prueba de performance...\n");
    long long cycles = performance_test();
    printf("Ciclos de CPU usados: %lld\n", cycles);
    printf("Tiempo aproximado: %.2f microsegundos\n", 
           (double)cycles / 2.5);  // Asumiendo 2.5 GHz
    
    printf("\n");
}

// ================================================
// IMPRIMIR SEPARADOR
// ================================================
void print_separator() {
    printf("----------------------------------------\n");
}
```

---

## 📋 Archivo Makefile

### `Makefile` - Archivo de construcción

```makefile
# ================================================
# MAKEFILE PARA INTEGRACION C - ENSAMBLADOR
# ================================================

# Variables
CC = gcc
ASM = nasm
CFLAGS = -Wall -Wextra -std=c99 -g
ASMFLAGS = -f elf64
TARGET = demo_c_asm
SOURCES_C = main.c funciones_c.c
SOURCES_ASM = funciones_asm.asm
OBJECTS_C = $(SOURCES_C:.c=.o)
OBJECTS_ASM = $(SOURCES_ASM:.asm=.o)

# Regla por defecto
all: $(TARGET)

# Enlazar el ejecutable final
$(TARGET): $(OBJECTS_C) $(OBJECTS_ASM)
	$(CC) $(CFLAGS) -o $@ $^

# Compilar archivos C
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Ensamblar archivos .asm
%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

# Limpiar archivos compilados
clean:
	rm -f $(OBJECTS_C) $(OBJECTS_ASM) $(TARGET)

# Instalar dependencias (Ubuntu/Debian)
install-deps:
	sudo apt update
	sudo apt install nasm build-essential

# Ejecutar el programa
run: $(TARGET)
	./$(TARGET)

# Depurar con GDB
debug: $(TARGET)
	gdb ./$(TARGET)

# Ver información del ejecutable
info: $(TARGET)
	file $(TARGET)
	size $(TARGET)

.PHONY: all clean install-deps run debug info
```

---

## 🛠️ Proceso de Compilación

### 1. **Instalar dependencias** (si no las tienes):
```bash
make install-deps
```

### 2. **Compilar el proyecto**:
```bash
make
```

### 3. **Ejecutar el programa**:
```bash
make run
```

### 4. **Limpiar archivos compilados**:
```bash
make clean
```

---

## 📚 Explicación Detallada de la Integración

### **Convención de Llamadas (Calling Convention)**
- **System V AMD64 ABI**: Estándar en Linux x86-64
- **Registros para parámetros**: RDI, RSI, RDX, RCX, R8, R9
- **Registro de retorno**: RAX
- **Registros preservados**: RBX, RBP, R12-R15

### **Comunicación entre C y Ensamblador**

#### **Desde C a Ensamblador**:
1. Declarar funciones con `extern`
2. Llamar normalmente como funciones C
3. Los parámetros se pasan según ABI

#### **Desde Ensamblador a C**:
1. Declarar funciones con `extern`
2. Llamar usando `call`
3. Los parámetros se pasan en registros

### **Casos de Uso Comunes**

1. **Operaciones matemáticas optimizadas**
2. **Manipulación de bits y bytes**
3. **Acceso directo al hardware**
4. **Rutinas de copia de memoria**
5. **Funciones de bajo nivel de performance**
6. **Manejo de interrupciones**
7. **Criptografía y hashing**

---

## 🎯 Resultado Esperado

Al ejecutar el programa, deberías ver una salida similar a:

```
=== DEMO: Integracion C - Ensamblador ===

=== INTEGRACION C - ENSAMBLADOR ===
Sistema compilado y listo para pruebas
===================================

1. PRUEBAS DE OPERACIONES ARITMETICAS
----------------------------------------
[DEBUG ASM] Funcion add_numbers_asm ejecutada
[LOG] Operacion #1 - Resultado: 40
Suma 15 + 25 = 40
[DEBUG ASM] Funcion factorial_asm ejecutada
[LOG] Operacion #2 - Resultado: 120
Factorial de 5 = 120
[DEBUG ASM] Funcion factorial_asm ejecutada
[LOG] Operacion #3 - Resultado: 1
Factorial de 0 = 1
[DEBUG ASM] Funcion factorial_asm ejecutada
Factorial de 25 = 0 (demasiado grande)

2. PRUEBAS DE OPERACIONES CON STRINGS
----------------------------------------
[DEBUG ASM] Funcion string_operations_asm ejecutada
Original: Hola Mundo Ensamblador
Mayusculas: HOLA MUNDO ENSAMBLADOR
[DEBUG ASM] Funcion string_operations_asm ejecutada
Minusculas: hola mundo ensamblador
[DEBUG ASM] Funcion string_operations_asm ejecutada
[LOG] Operacion #4 - Resultado: 22
Longitud calculada en ensamblador

3. PRUEBAS DE OPERACIONES A NIVEL DE BITS
----------------------------------------
Valor: 0xAAAA
Bits establecidos: 8
Posicion del LSB: 1

4. PRUEBA DE PERFORMANCE
----------------------------------------
Ejecutando prueba de performance...
Ciclos de CPU usados: 123456
Tiempo aproximado: 49382.40 microsegundos

=== FIN DE LA DEMO ===
```

---

## ⚠️ Notas Importantes

1. **Compatibilidad**: Este ejemplo es para Linux x86-64
2. **Instrucciones específicas**: Algunas instrucciones como `popcnt` requieren CPU moderna
3. **Optimización**: El ensamblador puede ser significativamente más rápido para ciertas operaciones
4. **Debugging**: Usar `gdb` para depurar código mixto
5. **Portabilidad**: El código ensamblador es específico de la arquitectura

---

## 🎯 Mensaje:

Esta integración demuestra cómo combinar lo mejor de ambos mundos:
- **C**: Facilidad de programación, portabilidad, bibliotecas
- **Ensamblador**: Control total del hardware, máxima performance

Es una técnica esencial en programación de sistemas, drivers, y aplicaciones de alto rendimiento.
