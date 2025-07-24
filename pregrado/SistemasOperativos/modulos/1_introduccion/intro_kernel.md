# Ejemplo de Código en Ensamblador Típico del Kernel de un Sistema Operativo

## 📋 Descripción General

Este documento presenta ejemplos representativos de código ensamblador que se encuentra típicamente en kernels de sistemas operativos modernos. Los ejemplos están diseñados para ilustrar conceptos fundamentales de programación a nivel de kernel, incluyendo gestión de interrupciones, llamadas al sistema, manejo de memoria y contexto de procesos.

## 🎯 Conceptos del Kernel que se Implementan

1. **Sistema de Llamadas al Sistema (System Calls)**
2. **Manejo de Interrupciones (Interrupt Handling)**
3. **Gestión de Memoria del Kernel (Memory Management)**
4. **Gestión de Contexto de Procesos (Process Context)**
5. **Inicialización del Sistema (System Initialization)**

---

## 📋 Ejemplo 1: Sistema de Llamadas al Sistema Básico

```nasm
; ================================================
; KERNEL BASICO - SISTEMA DE LLAMADAS AL SISTEMA
; Este ejemplo demuestra cómo un kernel maneja las llamadas
; al sistema desde aplicaciones en modo usuario
; ================================================

section .data
    ; ====================================================
    ; MENSAJES DEL KERNEL - Cadenas de texto para mensajes
    ; ====================================================
    welcome_msg     db 'Kernel cargado correctamente', 0xA, 0
    syscall_msg     db 'Llamada al sistema ejecutada', 0xA, 0
    error_msg       db 'Error: Llamada no valida', 0xA, 0
    prompt_msg      db 'kernel> ', 0
    
    ; ====================================================
    ; TABLA DE LLAMADAS AL SISTEMA (System Call Table)
    ; Esta tabla permite al kernel enrutar llamadas al sistema
    ; a las funciones apropiadas usando índices
    ; ====================================================
    syscall_table:
        dq sys_print_string     ; syscall 0: Imprimir string
        dq sys_exit             ; syscall 1: Salir del sistema
        dq sys_get_version      ; syscall 2: Obtener versión del kernel
        dq sys_memory_info      ; syscall 3: Información de memoria
    
    ; ====================================================
    ; DATOS DEL SISTEMA - Información básica del kernel
    ; ====================================================
    kernel_version  db 'Kernel v1.0', 0
    memory_info     db 'Memoria: 4GB disponible', 0xA, 0

section .bss
    ; ====================================================
    ; BUFFER DE ENTRADA - Espacio reservado para entrada del usuario
    ; resb 256 reserva 256 bytes sin inicializar
    ; ====================================================
    input_buffer    resb 256

section .text
    ; ====================================================
    ; DIRECTIVAS GLOBALES Y EXTERNAS
    ; global: Hace visible el símbolo para el enlazador
    ; extern: Indica que la función está definida en otro archivo
    ; ====================================================
    global _start_kernel        ; Punto de entrada del kernel
    extern print_string         ; Función externa de impresión
    extern read_input           ; Función externa de lectura

; ================================================
; PUNTO DE ENTRADA DEL KERNEL
; Esta es la primera función que se ejecuta cuando el kernel arranca
; ================================================
_start_kernel:
    ; Inicializar el kernel - configurar hardware y estructuras básicas
    call initialize_kernel
    
    ; BUCLE PRINCIPAL DEL KERNEL - El kernel corre continuamente
    ; esperando y procesando comandos del usuario
main_loop:
    ; Mostrar prompt al usuario (como una shell)
    mov rdi, prompt_msg         ; Cargar dirección del mensaje en RDI
    call print_string           ; Llamar a función externa de impresión
    
    ; Leer comando del usuario
    mov rdi, input_buffer       ; Dirección del buffer de entrada
    mov rsi, 256                ; Tamaño máximo a leer
    call read_input             ; Llamar a función externa de lectura
    
    ; Procesar el comando ingresado por el usuario
    call process_command
    
    ; Continuar con el bucle principal (jmp = jump incondicional)
    jmp main_loop

; ================================================
; INICIALIZACION DEL KERNEL
; Configura el entorno básico del kernel cuando arranca
; ================================================
initialize_kernel:
    ; Mostrar mensaje de bienvenida al iniciar
    mov rdi, welcome_msg        ; Cargar mensaje en RDI (primer argumento)
    call print_string           ; Imprimir mensaje
    
    ; Inicializar hardware del sistema (funciones simuladas)
    call init_hardware          ; Configurar dispositivos
    
    ; Configurar tabla de interrupciones del sistema
    call setup_interrupts       ; Preparar manejo de interrupciones
    
    ret                         ; Retornar al llamador

; ================================================
; PROCESAR COMANDOS DEL USUARIO
; Convierte entrada de texto en llamadas al sistema válidas
; ================================================
process_command:
    ; Convertir string a número para identificar syscall
    call string_to_number       ; Convertir input_buffer a número
    mov rbx, rax                ; Guardar número convertido en RBX
    
    ; Validar que el número de syscall esté en rango válido
    cmp rbx, 3                  ; Comparar con máximo syscall válido (3)
    ja invalid_syscall          ; Saltar si está fuera de rango
    
    ; Llamar a la función correspondiente usando la tabla
    ; [syscall_table + rax * 8] accede a la entrada correcta
    ; (8 bytes por entrada en sistemas 64-bit)
    mov rax, rbx                ; Cargar índice del syscall
    call [syscall_table + rax * 8]  ; Llamar indirectamente
    ret

invalid_syscall:
    ; Manejar error de syscall inválido
    mov rdi, error_msg          ; Cargar mensaje de error
    call print_string           ; Moprimir mensaje
    ret

; ================================================
; LLAMADAS AL SISTEMA IMPLEMENTADAS
; Estas funciones son las que el kernel proporciona a las aplicaciones
; ================================================

; SYSCALL 0: Imprimir string
; Función que permite a aplicaciones imprimir texto
sys_print_string:
    push rbp                    ; Guardar frame pointer actual
    mov rbp, rsp                ; Establecer nuevo frame pointer
    
    ; Obtener string del registro rdi
    ; En un kernel real, esto vendría del contexto del proceso
    call print_string           ; Llamar a función de impresión
    
    ; Retornar éxito al proceso llamador
    mov rax, 0                  ; Código de éxito (0 = OK)
    pop rbp                     ; Restaurar frame pointer
    ret                         ; Retornar

; SYSCALL 1: Salir del sistema
; Función que permite a aplicaciones terminar el sistema
sys_exit:
    push rbp
    mov rbp, rsp
    
    ; Limpiar recursos del sistema antes de salir
    call cleanup_system         ; Liberar memoria, cerrar dispositivos
    
    ; En un kernel real, aquí se llamaría a apagar o reiniciar
    mov rdi, 0                  ; Código de salida del sistema
    mov rax, 60                 ; Número de syscall sys_exit en Linux
    syscall                     ; Llamar al sistema para terminar
    
    pop rbp
    ret

; SYSCALL 2: Obtener versión del kernel
; Función que permite a aplicaciones consultar la versión del kernel
sys_get_version:
    push rbp
    mov rbp, rsp
    
    mov rdi, kernel_version     ; Cargar dirección de la versión
    call print_string           ; Imprimir versión
    
    mov rax, 0                  ; Retornar éxito
    pop rbp
    ret

; SYSCALL 3: Información de memoria
; Función que proporciona información sobre la memoria del sistema
sys_memory_info:
    push rbp
    mov rbp, rsp
    
    mov rdi, memory_info        ; Cargar mensaje de información de memoria
    call print_string           ; Imprimir información
    
    mov rax, 0                  ; Retornar éxito
    pop rbp
    ret

; ================================================
; FUNCIONES DE SOPORTE DEL KERNEL
; Funciones auxiliares que ayudan al funcionamiento del kernel
; ================================================

; Inicialización de hardware (simulada)
init_hardware:
    ; En un kernel real, esta función realizaría:
    ; - Configurar controladores de interrupción (PIC/APIC)
    ; - Inicializar dispositivos (discos, red, teclado, etc.)
    ; - Configurar memoria física
    ; - Configurar temporizador del sistema
    ret

; Configuración de interrupciones
setup_interrupts:
    ; En un kernel real, esta función:
    ; - Configura la IDT (Interrupt Descriptor Table)
    ; - Habilita las interrupciones en el procesador
    ; - Configura manejadores específicos para cada tipo de interrupción
    ret

; Limpieza del sistema
cleanup_system:
    ; En un kernel real, esta función:
    ; - Libera toda la memoria asignada
    ; - Cierra todos los dispositivos abiertos
    ; - Guarda el estado del sistema si es necesario
    ret

; Convertir string a número
string_to_number:
    push rbp                    ; Guardar registros usados
    mov rbp, rsp
    push rbx
    push rcx
    push rdx
    
    mov rsi, input_buffer       ; String de entrada a convertir
    xor rax, rax                ; Resultado inicial = 0
    xor rbx, rbx                ; Base = 10 (decimal)
    
.convert_loop:
    movzx rcx, byte [rsi]       ; Cargar carácter actual (con extensión a 64-bit)
    test rcx, rcx               ; Verificar si es fin de string (0)
    jz .done                    ; Si es 0, terminar conversión
    
    cmp cl, 0xA                 ; Comparar con nueva línea
    je .done                    ; Saltar nueva línea
    cmp cl, ' '                 ; Comparar con espacio
    je .done                    ; Saltar espacios
    
    sub cl, '0'                 ; Convertir ASCII a valor numérico ('0'-'9' → 0-9)
    imul rax, 10                ; Multiplicar resultado acumulado por 10
    add rax, rcx                ; Añadir nuevo dígito al resultado
    
    inc rsi                     ; Mover al siguiente carácter
    jmp .convert_loop           ; Continuar bucle
    
.done:
    pop rdx                     ; Restaurar registros
    pop rcx
    pop rbx
    pop rbp
    ret
```

---

## 📋 Ejemplo 2: Manejo de Interrupciones

```nasm
; ================================================
; MANEJADOR DE INTERRUPCIONES DEL KERNEL
; Este ejemplo muestra cómo un kernel maneja interrupciones
; del hardware, como el temporizador del sistema
; ================================================

section .data
    ; ====================================================
    ; MENSAJES DE INTERRUPCIÓN - Para debugging y monitoreo
    ; ====================================================
    interrupt_msg   db 'Interrupcion recibida', 0xA, 0
    timer_msg       db 'Timer tick', 0xA, 0

section .text
    ; ====================================================
    ; EXPORTAR MANEJADORES PARA QUE EL SISTEMA LOS PUEDA USAR
    ; ====================================================
    global interrupt_handler    ; Manejador general de interrupciones
    global timer_handler        ; Manejador específico del temporizador
    extern print_string         ; Función externa de impresión

; ================================================
; MANEJADOR GENERAL DE INTERRUPCIONES
; Esta función se ejecuta cuando ocurre cualquier interrupción
; ================================================
interrupt_handler:
    ; GUARDAR REGISTROS - Preservar estado del programa interrumpido
    ; push guarda registros en la pila para restaurarlos después
    push rax                    ; Guardar registro acumulador
    push rbx                    ; Guardar registro base
    push rcx                    ; Guardar registro contador
    push rdx                    ; Guardar registro de datos
    push rsi                    ; Guardar índice fuente
    push rdi                    ; Guardar índice destino
    push rbp                    ; Guardar frame pointer
    
    ; PROCESAR LA INTERRUPCIÓN - En este caso, solo mostrar mensaje
    mov rdi, interrupt_msg      ; Cargar mensaje de interrupción
    call print_string           ; Imprimir mensaje
    
    ; RESTAURAR REGISTROS - Recuperar estado antes de la interrupción
    pop rbp                     ; Restaurar frame pointer
    pop rdi                     ; Restaurar índice destino
    pop rsi                     ; Restaurar índice fuente
    pop rdx                     ; Restaurar registro de datos
    pop rcx                     ; Restaurar registro contador
    pop rbx                     ; Restaurar registro base
    pop rax                     ; Restaurar registro acumulador
    
    ; RETORNAR DE INTERRUPCIÓN - Reanudar ejecución interrumpida
    ; iretq es la instrucción para retornar de interrupciones en 64-bit
    iretq

; ================================================
; MANEJADOR ESPECÍFICO DEL TIMER
; Esta función maneja específicamente las interrupciones del temporizador
; ================================================
timer_handler:
    ; GUARDAR SOLO LOS REGISTROS QUE VAMOS A USAR
    ; Optimización: solo guardar lo que vamos a modificar
    push rax                    ; Guardar acumulador
    push rdi                    ; Guardar índice destino (para print_string)
    
    ; INCREMENTAR CONTADOR DE TICKS - Contar cuántas veces ha sonado el timer
    inc dword [timer_ticks]     ; Incrementar contador de 32 bits
    
    ; CADA 100 TICKS, MOSTRAR MENSAJE - Ejemplo de procesamiento periódico
    mov eax, [timer_ticks]      ; Cargar valor actual del contador
    mov edx, 0                  ; Limpiar parte alta para división
    mov ebx, 100                ; Divisor (cada 100 ticks)
    div ebx                     ; Dividir EAX entre EBX, resultado en EAX, resto en EDX
    test edx, edx               ; Verificar si hay resto (no es múltiplo de 100)
    jnz .skip_message           ; Si hay resto, saltar mensaje
    
    ; Mostrar mensaje cada 100 ticks
    mov rdi, timer_msg          ; Cargar mensaje de timer
    call print_string           ; Imprimir mensaje
    
.skip_message:
    ; ENVIAR EOI (END OF INTERRUPT) AL PIC
    ; Es crucial informar al controlador de interrupciones
    ; que hemos terminado de procesar la interrupción
    mov al, 0x20                ; Comando EOI (End of Interrupt)
    out 0x20, al                ; Enviar comando al puerto del PIC master
                                ; 0x20 es el puerto de comando del PIC
    
    ; RESTAURAR REGISTROS - Recuperar estado anterior
    pop rdi                     ; Restaurar índice destino
    pop rax                     ; Restaurar acumulador
    
    ; RETORNAR DE INTERRUPCIÓN
    iretq

section .bss
    ; ====================================================
    ; VARIABLES DE ESTADO - Contadores y estado del sistema
    ; ====================================================
    timer_ticks     resd 1      ; Reservar 1 double word (4 bytes) para contador
                                ; Contador de ticks del timer del sistema
```

---

## 📋 Ejemplo 3: Gestión de Memoria Básica

```nasm
; ================================================
; GESTOR DE MEMORIA DEL KERNEL
; Este ejemplo muestra cómo un kernel administra la memoria
; asignada a procesos y estructuras del sistema
; ================================================

section .data
    ; ====================================================
    ; PARÁMETROS DE MEMORIA - Configuración del heap del kernel
    ; ====================================================
    heap_start      dq 0x100000     ; Dirección de inicio del heap (1MB)
                                    ; dq = define quad word (8 bytes)
    heap_end        dq 0x200000     ; Dirección final del heap (2MB)
    heap_current    dq 0x100000     ; Puntero actual de asignación

section .text
    ; ====================================================
    ; EXPORTAR FUNCIONES DE GESTIÓN DE MEMORIA
    ; ====================================================
    global kmalloc              ; Asignar memoria del kernel
    global kfree                ; Liberar memoria del kernel
    global get_memory_info      ; Obtener información de memoria

; ================================================
; ASIGNAR MEMORIA (kmalloc)
; Función que asigna bloques de memoria para el kernel
; Similar a malloc() pero para uso del kernel
; ================================================
kmalloc:
    ; ENTRADA: rdi = tamaño solicitado en bytes
    ; SALIDA: rax = puntero a memoria asignada o 0 si error
    
    ; GUARDAR REGISTROS QUE VAMOS A USAR
    push rbx                    ; Guardar registro base
    push rcx                    ; Guardar registro contador
    push rdx                    ; Guardar registro de datos
    
    ; VERIFICAR SI HAY SUFICIENTE ESPACIO EN EL HEAP
    mov rax, [heap_current]     ; Cargar dirección actual del heap
    add rax, rdi                ; Sumar tamaño solicitado
    cmp rax, [heap_end]         ; Comparar con límite del heap
    ja .out_of_memory           ; Saltar si no hay suficiente espacio
    
    ; ASIGNAR MEMORIA - Actualizar puntero y retornar dirección
    mov rax, [heap_current]     ; Dirección de inicio del bloque asignado
    add qword [heap_current], rdi  ; Actualizar puntero del heap
                                    ; qword especifica operación de 64 bits
    jmp .done                   ; Saltar al final
    
.out_of_memory:
    ; MANEJAR ERROR DE MEMORIA INSUFICIENTE
    xor rax, rax                ; Retornar NULL (0) indicando error
    
.done:
    ; RESTAURAR REGISTROS
    pop rdx                     ; Restaurar registro de datos
    pop rcx                     ; Restaurar registro contador
    pop rbx                     ; Restaurar registro base
    ret                         ; Retornar al llamador

; ================================================
; LIBERAR MEMORIA (kfree)
; Función que libera bloques de memoria previamente asignados
; ================================================
kfree:
    ; EN UN KERNEL REAL, AQUÍ SE IMPLEMENTARÍA:
    ; - Algoritmo de liberación de bloques (first-fit, best-fit, etc.)
    ; - Coalescing de bloques libres adyacentes
    ; - Metadata para tracking de bloques
    
    ; POR SIMPLICIDAD EN ESTE EJEMPLO:
    ; Solo mostramos el concepto - en la práctica se necesitaría
    ; información adicional sobre el bloque a liberar
    
    ; En este ejemplo básico, no hacemos nada
    ; Un kernel real necesitaría recibir el puntero a liberar
    ; y metadata para tracking de asignaciones
    
    ret

; ================================================
; INFORMACIÓN DE MEMORIA
; Función que proporciona estadísticas de uso de memoria
; ================================================
get_memory_info:
    push rbp                    ; Guardar frame pointer
    mov rbp, rsp                ; Establecer nuevo frame pointer
    
    ; CALCULAR MEMORIA DISPONIBLE
    mov rax, [heap_end]         ; Cargar dirección final del heap
    sub rax, [heap_current]     ; Restar posición actual = memoria disponible
    
    ; EN UN KERNEL REAL, ESTO DEVOLVERÍA UNA ESTRUCTURA CON:
    ; - Memoria total del sistema
    ; - Memoria usada por el kernel
    ; - Memoria disponible para asignación
    ; - Fragmentación de memoria
    ; - Estadísticas de asignaciones
    
    pop rbp                     ; Restaurar frame pointer
    ret                         ; Retornar información (en RAX)
```

---

## 📋 Ejemplo 4: Contexto de Procesos

```nasm
; ================================================
; GESTIÓN DE CONTEXTOS DE PROCESOS
; Este ejemplo muestra cómo un kernel guarda y restaura
; el estado de ejecución de diferentes procesos
; ================================================

; ====================================================
; DEFINICIÓN DE ESTRUCTURA DE CONTEXTO DE PROCESO
; struc define una estructura de datos
; resq reserva quad words (8 bytes cada uno)
; endstruc finaliza la definición de la estructura
; ====================================================
struc process_context           ; Estructura para guardar contexto de proceso
    .rax    resq 1              ; Registro acumulador (8 bytes)
    .rbx    resq 1              ; Registro base (8 bytes)
    .rcx    resq 1              ; Registro contador (8 bytes)
    .rdx    resq 1              ; Registro de datos (8 bytes)
    .rsi    resq 1              ; Índice fuente (8 bytes)
    .rdi    resq 1              ; Índice destino (8 bytes)
    .rbp    resq 1              ; Frame pointer (8 bytes)
    .rsp    resq 1              ; Stack pointer (8 bytes)
    .r8     resq 1              ; Registro extendido 8 (8 bytes)
    .r9     resq 1              ; Registro extendido 9 (8 bytes)
    .r10    resq 1              ; Registro extendido 10 (8 bytes)
    .r11    resq 1              ; Registro extendido 11 (8 bytes)
    .r12    resq 1              ; Registro extendido 12 (8 bytes)
    .r13    resq 1              ; Registro extendido 13 (8 bytes)
    .r14    resq 1              ; Registro extendido 14 (8 bytes)
    .r15    resq 1              ; Registro extendido 15 (8 bytes)
    .rip    resq 1              ; Instruction pointer (8 bytes)
    .rflags resq 1              ; Flags del procesador (8 bytes)
    .cs     resw 1              ; Code segment (2 bytes)
    .ss     resw 1              ; Stack segment (2 bytes)
endstruc                        ; Fin de la estructura

section .data
    ; ====================================================
    ; VARIABLES DE CONTROL DE PROCESOS
    ; ====================================================
    current_process dq 0        ; Puntero al contexto del proceso actual
                                ; dq = define quad word (8 bytes)

section .text
    ; ====================================================
    ; EXPORTAR FUNCIONES DE GESTIÓN DE CONTEXTOS
    ; ====================================================
    global save_context         ; Guardar contexto de proceso
    global restore_context      ; Restaurar contexto de proceso
    global switch_to_process    ; Cambiar entre procesos

; ================================================
; GUARDAR CONTEXTO DE PROCESO
; Función que guarda el estado completo de un proceso
; Se llama cuando se va a interrumpir o cambiar de proceso
; ================================================
save_context:
    ; ENTRADA: rdi = puntero a estructura process_context
    
    ; GUARDAR REGISTROS GENERALES DEL PROCESADOR
    mov [rdi + process_context.rax], rax    ; Guardar RAX en estructura
    mov [rdi + process_context.rbx], rbx    ; Guardar RBX en estructura
    mov [rdi + process_context.rcx], rcx    ; Guardar RCX en estructura
    mov [rdi + process_context.rdx], rdx    ; Guardar RDX en estructura
    mov [rdi + process_context.rsi], rsi    ; Guardar RSI en estructura
    mov [rdi + process_context.rdi], rdi    ; Guardar RDI en estructura
    mov [rdi + process_context.rbp], rbp    ; Guardar RBP en estructura
    
    ; GUARDAR STACK POINTER - Dirección actual de la pila
    mov rax, rsp                            ; Cargar stack pointer actual
    mov [rdi + process_context.rsp], rax    ; Guardar en estructura
    
    ; GUARDAR REGISTROS EXTENDIDOS (r8-r15)
    mov [rdi + process_context.r8], r8      ; Guardar R8
    mov [rdi + process_context.r9], r9      ; Guardar R9
    mov [rdi + process_context.r10], r10    ; Guardar R10
    mov [rdi + process_context.r11], r11    ; Guardar R11
    mov [rdi + process_context.r12], r12    ; Guardar R12
    mov [rdi + process_context.r13], r13    ; Guardar R13
    mov [rdi + process_context.r14], r14    ; Guardar R14
    mov [rdi + process_context.r15], r15    ; Guardar R15
    
    ; GUARDAR FLAGS DEL PROCESADOR - Estado de ejecución
    pushfq                                  ; Empujar flags a la pila
    pop rax                                 ; Sacar flags a RAX
    mov [rdi + process_context.rflags], rax ; Guardar flags en estructura
    
    ret                                     ; Retornar

; ================================================
; RESTAURAR CONTEXTO DE PROCESO
; Función que restaura el estado de un proceso guardado
; Se llama cuando se va a reanudar un proceso
; ================================================
restore_context:
    ; ENTRADA: rdi = puntero a estructura process_context
    
    ; RESTAURAR REGISTROS GENERALES
    mov rax, [rdi + process_context.rax]    ; Restaurar RAX
    mov rbx, [rdi + process_context.rbx]    ; Restaurar RBX
    mov rcx, [rdi + process_context.rcx]    ; Restaurar RCX
    mov rdx, [rdi + process_context.rdx]    ; Restaurar RDX
    mov rsi, [rdi + process_context.rsi]    ; Restaurar RSI
    mov rbp, [rdi + process_context.rbp]    ; Restaurar RBP
    
    ; RESTAURAR REGISTROS EXTENDIDOS
    mov r8, [rdi + process_context.r8]      ; Restaurar R8
    mov r9, [rdi + process_context.r9]      ; Restaurar R9
    mov r10, [rdi + process_context.r10]    ; Restaurar R10
    mov r11, [rdi + process_context.r11]    ; Restaurar R11
    mov r12, [rdi + process_context.r12]    ; Restaurar R12
    mov r13, [rdi + process_context.r13]    ; Restaurar R13
    mov r14, [rdi + process_context.r14]    ; Restaurar R14
    mov r15, [rdi + process_context.r15]    ; Restaurar R15
    
    ; RESTAURAR STACK POINTER - Cambiar contexto de pila
    mov rsp, [rdi + process_context.rsp]    ; Establecer nueva pila
    
    ; RESTAURAR FLAGS DEL PROCESADOR
    push qword [rdi + process_context.rflags]  ; Empujar flags guardados
    popfq                                   ; Restaurar flags del procesador
    
    ; SALTAR A LA DIRECCIÓN DE EJECUCIÓN GUARDADA
    mov rax, [rdi + process_context.rip]    ; Cargar instruction pointer
    jmp rax                                 ; Saltar a esa dirección

; ================================================
; CAMBIAR ENTRE PROCESOS (Context Switch)
; Función que cambia de un proceso a otro
; Esta es la base del multitasking en los sistemas operativos
; ================================================
switch_to_process:
    ; ENTRADA: rdi = nuevo proceso (contexto a restaurar)
    ;          rsi = proceso actual (contexto a guardar)
    
    ; GUARDAR CONTEXTO ACTUAL - Solo si hay proceso actual
    test rsi, rsi                           ; Verificar si rsi no es 0
    jz .no_save                             ; Saltar si no hay proceso actual
    call save_context                       ; Guardar contexto del proceso actual
    
.no_save:
    ; ESTABLECER NUEVO PROCESO ACTUAL
    mov [current_process], rdi              ; Actualizar puntero al proceso actual
    
    ; RESTAURAR CONTEXTO DEL NUEVO PROCESO
    mov rdi, rdi                            ; Cargar contexto del nuevo proceso
    call restore_context                    ; Restaurar y saltar a ese proceso
```

---

## 📚 Explicación Detallada de los Conceptos del Kernel

### 1. **Sistema de Llamadas al Sistema (System Calls)**
Los kernels proporcionan una interfaz controlada entre aplicaciones en modo usuario y el sistema operativo en modo kernel. Las aplicaciones usan números de syscall para solicitar servicios del kernel.

### 2. **Manejo de Interrupciones (Interrupt Handling)**
Las interrupciones permiten al hardware notificar eventos al kernel (temporizador, teclado, disco). Los manejadores deben ser rápidos y restaurar el contexto exacto del programa interrumpido.

### 3. **Gestión de Memoria del Kernel (Memory Management)**
El kernel necesita gestionar su propia memoria (heap) y la memoria de los procesos. Esto incluye asignación, liberación y protección de memoria.

### 4. **Gestión de Contexto de Procesos (Process Context)**
Para multitasking, el kernel debe guardar y restaurar el estado completo de cada proceso, incluyendo registros, pila y dirección de ejecución.

### 5. **Inicialización del Sistema (System Initialization)**
Al arrancar, el kernel debe configurar hardware, estructuras de datos y servicios básicos antes de permitir la ejecución de aplicaciones.

---

## 🎯 Componentes Típicos en Kernels Reales

Estos ejemplos muestran componentes que encontrarías en kernels como:
- **Linux Kernel**: Sistema de syscalls, gestión de memoria
- **Windows NT**: Manejo de contexto, interrupciones
- **FreeBSD**: Arquitectura de llamadas al sistema

Los kernels reales incluyen muchas más funcionalidades como:
- **Scheduling**: Planificación de procesos
- **Virtual Memory**: Memoria virtual y paginación
- **Device Drivers**: Controladores de dispositivos
- **File Systems**: Sistemas de archivos
- **Security**: Protección y permisos
- **Networking**: Stack de red

---

## ⚠️ Notas Importantes

1. **Este es código educativo**: Los kernels reales son mucho más complejos
2. **Manejo de errores**: En producción se requiere manejo exhaustivo de errores
3. **Seguridad**: Se necesitan mecanismos de protección contra código malicioso
4. **Concurrencia**: Los kernels reales manejan múltiples CPUs y concurrencia
5. **Performance**: El código del kernel debe ser altamente optimizado
