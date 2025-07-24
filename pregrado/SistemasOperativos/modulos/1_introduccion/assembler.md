# Guía Paso a Paso para Aprender Ensamblador en Windows 11 con WSL y Visual Studio Code

¡Bienvenido al fascinante mundo del ensamblador! Esta guía te llevará desde la configuración inicial de tu entorno de desarrollo hasta la escritura, compilación y ejecución de tus primeros programas en ensamblador. Nos apoyaremos en **WSL (Windows Subsystem for Linux)** para ejecutar un ambiente Linux directamente en Windows 11, y utilizaremos **NASM (Netwide Assembler)** para escribir el código. Además, **Visual Studio Code** será nuestro editor de texto principal, ofreciéndote una experiencia de desarrollo moderna y eficiente.

---

## 1. Comprendiendo las Arquitecturas de Procesadores y el Ensamblador

Cuando hablamos de programar en ensamblador, es crucial entender la **arquitectura del procesador** para la que estás escribiendo. Cada familia de procesadores tiene su propio conjunto de instrucciones de ensamblador, como si fueran dialectos diferentes.

En esta guía, nos centraremos en la arquitectura **x86-64**.

### 1.1. Arquitectura x86-64 (Intel y AMD)

* **¿Qué es?** Es la arquitectura predominante en las computadoras personales y servidores modernos. Es una extensión de la antigua arquitectura x86 de 32 bits, que añadió el soporte para registros y direcciones de memoria de 64 bits.
* **Compatibilidad:** La gran ventaja es que las instrucciones de ensamblador x86-64 que aprenderás y usarás en esta guía son **compatibles con la gran mayoría de los procesadores modernos, tanto de Intel como de AMD**. No hay diferencia en el código que escribirás si tu PC tiene un procesador Intel o AMD, ya que ambos implementan este mismo estándar de instrucciones.
* **¿Por qué es importante?** Programar directamente para x86-64 te da un control muy fino sobre el hardware de tu PC, permitiéndote entender cómo interactúa el software con el procesador a un nivel muy bajo.

### 1.2. Otras Arquitecturas de Procesadores (Mención General)

Aunque nos enfocaremos en x86-64, es bueno saber que existen otras arquitecturas importantes, cada una con su propio conjunto de instrucciones de ensamblador:

* **ARM (Advanced RISC Machine):**
    * **Diferencia:** Es una arquitectura de tipo RISC (Reduced Instruction Set Computer), lo que significa que tiende a tener un conjunto de instrucciones más pequeño y simple que x86-64 (que es CISC - Complex Instruction Set Computer). Esto a menudo la hace más eficiente energéticamente.
    * **¿Dónde se usa?** Predomina en dispositivos móviles (smartphones, tablets), sistemas embebidos, y ahora también en computadoras de escritorio y portátiles (como los chips Apple Silicon, o las versiones ARM de Windows/Linux). Tu programa x86-64 **no funcionaría** directamente en un dispositivo ARM sin ser reescrito o emulado.
* **MIPS (Microprocessor without Interlocked Pipeline Stages):**
    * **Diferencia:** También es RISC, con un diseño muy limpio y simple, a menudo usado en el ámbito académico para enseñar arquitectura de computadoras.
    * **¿Dónde se usa?** Históricamente en estaciones de trabajo, routers, y algunos sistemas embebidos. Es menos común en PCs de usuario final hoy en día.
* **RISC-V:**
    * **Diferencia:** Es una arquitectura de conjunto de instrucciones (ISA) abierta y de código abierto. Al ser modular, puedes elegir solo las extensiones de instrucciones que necesitas.
    * **¿Dónde se usa?** Está ganando tracción en investigación, educación, y aplicaciones personalizadas desde pequeños microcontroladores hasta procesadores de alto rendimiento.

**En resumen:** Lo que aprenderás aquí para **x86-64 es específico para computadoras de escritorio/portátiles y servidores con CPUs Intel o AMD modernas.** Para programar en ensamblador en otras arquitecturas, necesitarías aprender su conjunto de instrucciones particular.

---

## 2. Configuración del Entorno: WSL, Visual Studio Code y Herramientas

Asumo que ya tienes **WSL instalado en tu Windows 11** y que cuentas con una distribución de Linux como **Ubuntu**. Si no la tienes instalada aún, puedes hacerlo abriendo PowerShell como administrador y ejecutando `wsl --install`.

Una vez tengas WSL y Ubuntu listos, vamos a configurar nuestro entorno de desarrollo.

### 2.1. Instalar Visual Studio Code y su Extensión WSL

Visual Studio Code (VS Code) es un editor de código muy popular y potente, con una excelente integración con WSL.

1.  **Instala Visual Studio Code en Windows:**
    * Si no lo tienes, descárgalo e instálalo desde la página oficial: [code.visualstudio.com](https://code.visualstudio.com/).

2.  **Instala la Extensión "WSL" en VS Code:**
    * Abre VS Code.
    * Ve a la vista de **Extensiones** (icono de cubos en la barra lateral izquierda, o usa `Ctrl+Shift+X`).
    * Busca "WSL" e instala la extensión "WSL" de Microsoft.

    Esta extensión es mágica: te permitirá abrir carpetas de tu sistema de archivos de WSL directamente en VS Code, como si fueran carpetas locales de Windows. Esto es mucho más cómodo que usar un editor básico en la terminal.

### 2.2. Instalar NASM (El Ensamblador) en WSL

**NASM** es la herramienta clave que traduce tu código escrito en ensamblador (legible por humanos) a un formato binario que la computadora puede procesar.

1.  Abre tu **terminal de WSL (Ubuntu)**.
2.  Primero, actualiza la lista de paquetes disponibles en tu sistema Ubuntu y luego instala NASM. Esto asegura que obtengas la versión más reciente y compatible:
    ```bash
    sudo apt update        # Actualiza la lista de paquetes en tu Ubuntu de WSL
    sudo apt install nasm  # Instala el ensamblador NASM
    ```

### 2.3. Instalar Build-Essential (El Enlazador y Compilador) en WSL

Necesitarás el paquete `build-essential`. Este incluye **GCC** (un compilador de C/C++) y, crucialmente para nuestros programas en ensamblador, el **enlazador (`ld`)**. El enlazador es el programa que toma el código ya ensamblado (`.o`) y lo convierte en un programa ejecutable final que el sistema operativo puede cargar y ejecutar.

1.  En tu terminal de WSL, ejecuta:
    ```bash
    sudo apt install build-essential # Instala las herramientas de compilación esenciales
    ```

---

## 3. Operaciones Básicas: El Ciclo de Vida de un Programa en Ensamblador

Para transformar tu código fuente en ensamblador en un programa ejecutable funcional para arquitecturas x86-64, seguirás estos tres pasos principales:

1.  **Ensamblar:** Tu código `.asm` se convierte en un **archivo objeto (`.o`)**.
2.  **Enlazar:** El archivo objeto se convierte en un **ejecutable** listo para ser lanzado.
3.  **Ejecutar:** Pones en marcha tu programa.

Vamos a ver este proceso con el clásico ejemplo de "Hola Mundo".

### 3.1. Preparando tu Carpeta de Trabajo y el Código "Hola Mundo"

1.  **Crea una carpeta de trabajo en WSL:**
    * Abre tu **terminal de WSL (Ubuntu)**.
    * Crea una carpeta donde guardarás todos tus proyectos de ensamblador. Es buena práctica mantenerlos organizados. Por ejemplo:
        ```bash
        mkdir mis_proyectos_asm
        cd mis_proyectos_asm # Entra a la nueva carpeta
        ```

2.  **Abre la carpeta en Visual Studio Code:**
    * Desde tu terminal de WSL, y asegurándote de estar dentro de la carpeta `mis_proyectos_asm`, escribe y presiona Enter:
        ```bash
        code .
        ```
    * Esto lanzará Visual Studio Code. La ventana de VS Code se conectará automáticamente a tu entorno WSL y abrirá la carpeta `mis_proyectos_asm`. Ahora podrás editar archivos en esta carpeta con toda la comodidad de VS Code.

3.  **Crea el archivo `hola_mundo.asm` en VS Code:**
    * En VS Code, en el explorador de archivos (el primer icono a la izquierda), haz clic en "New File" (el icono de hoja con un `+`).
    * Nombra el archivo `hola_mundo.asm` y presiona Enter.
    * Pega el siguiente código en `hola_mundo.asm` y **guárdalo** (`Ctrl+S`).

    ```assembly
    section .data                 ; Sección para datos inicializados (variables, cadenas de texto, etc.)
        msg db 'Hola Mundo!', 0xA  ; Define 'msg' como una cadena de bytes.
                                  ; 'Hola Mundo!' es el texto que queremos imprimir.
                                  ; '0xA' es el valor ASCII para el carácter de nueva línea (Enter).

        len equ $ - msg           ; Define 'len' (longitud de la cadena 'msg').
                                  ; '$' representa la posición actual del ensamblador en el código.
                                  ; '$ - msg' calcula cuántos bytes hay desde el inicio de 'msg' hasta la posición actual,
                                  ; dándonos la longitud exacta del mensaje.

    section .text                 ; Sección para el código ejecutable del programa (donde van las instrucciones)
        global _start             ; Declara '_start' como el punto de entrada principal del programa.
                                  ; El sistema operativo busca esta etiqueta para saber dónde debe empezar a ejecutar tu código.

    _start:                       ; Esta etiqueta marca el inicio de la ejecución de nuestro programa

        ; --- Syscall para escribir en la salida estándar (sys_write) ---
        ; Para que nuestro programa imprima algo en la consola, necesitamos pedirle al sistema operativo (kernel)
        ; que lo haga por nosotros, a través de una "llamada al sistema" (syscall).
        ; En Linux para arquitecturas x86-64 (usadas por CPUs Intel y AMD), los argumentos para las syscalls
        ; se pasan en registros específicos:
        ; RAX: Contiene el número de la syscall (qué operación queremos realizar).
        ; RDI: Primer argumento de la syscall.
        ; RSI: Segundo argumento de la syscall.
        ; RDX: Tercer argumento de la syscall.

        mov rax, 1                ; Mueve el valor 1 al registro RAX.
                                  ; El número de la syscall para 'write' (escribir datos) es 1.
        mov rdi, 1                ; Mueve el valor 1 al registro RDI.
                                  ; El número 1 es el "descriptor de archivo" para la salida estándar (stdout),
                                  ; que es tu terminal o consola.
        mov rsi, msg              ; Mueve la dirección de memoria donde se encuentra nuestra cadena 'msg' al registro RSI.
                                  ; Esto le dice a 'sys_write' dónde encontrar el texto que queremos imprimir.
        mov rdx, len              ; Mueve la longitud de la cadena 'len' al registro RDX.
                                  ; Esto le dice a 'sys_write' cuántos bytes debe escribir desde la dirección dada.
        syscall                   ; ¡Ejecuta la llamada al sistema! El kernel de Linux toma el control por un momento,
                                  ; realiza la operación de escritura, y luego devuelve el control a tu programa.

        ; --- Syscall para salir del programa (sys_exit) ---
        ; Una vez que hemos terminado de imprimir nuestro mensaje, es importante indicarle al sistema operativo
        ; que nuestro programa ha finalizado y que puede liberar los recursos que estaba utilizando.

        mov rax, 60               ; Mueve el valor 60 al registro RAX.
                                  ; El número de la syscall para 'exit' (terminar el proceso) es 60.
        mov rdi, 0                ; Mueve el valor 0 al registro RDI.
                                  ; Este es el "código de salida" del programa. Un valor de 0 generalmente
                                  ; indica que el programa terminó con éxito (sin errores).
        syscall                   ; Ejecuta la llamada al sistema 'exit'. El programa se detiene completamente.
    ```

### 3.2. Ensamblar (Traducir tu Código `.asm` a `.o`)

Este paso convierte tu código fuente `.asm` en un **archivo objeto (`.o`)**. Piensa en el archivo `.o` como un "módulo" de código binario que la CPU puede entender, pero que aún no puede ejecutarse por sí mismo.

1.  **Abre la Terminal Integrada de VS Code:**
    * En VS Code, ve a **Terminal** > **New Terminal** (o usa `Ctrl+` `` ` ``).
    * Asegúrate de que la terminal esté en tu carpeta `mis_proyectos_asm` (debería ser el caso si abriste la carpeta con `code .`).

2.  **Ejecuta el comando `nasm`:**
    ```bash
    nasm -f elf64 hola_mundo.asm -o hola_mundo.o
    ```
    * `nasm`: Es el programa ensamblador que hemos instalado.
    * `-f elf64`: Este es un parámetro crucial que le dice a NASM que genere el archivo objeto en formato **ELF64**. Este es el formato estándar para programas de 64 bits en sistemas Linux (compatible con Intel y AMD).
    * `hola_mundo.asm`: Es el nombre de tu archivo de código fuente.
    * `-o hola_mundo.o`: Especifica el nombre del archivo de salida, que será `hola_mundo.o` (la extensión `.o` es estándar para archivos objeto).

3.  **Verifica:** Si el comando se ejecuta sin mostrar ningún mensaje de error, significa que el ensamblado fue exitoso. Puedes usar `ls -l` en la terminal de WSL para listar los archivos; deberías ver `hola_mundo.asm` y el nuevo `hola_mundo.o`.

### 3.3. Enlazar (Armar el Programa Final)

El archivo `.o` todavía no es un programa que puedas ejecutar. El **enlazador (`ld`)** toma ese archivo objeto y lo combina con la información necesaria (como la declaración de `_start`) para crear un **ejecutable** completo que el sistema operativo pueda cargar en memoria y ejecutar.

1.  Desde la **misma terminal de VS Code** (asegúrate de seguir en la carpeta `mis_proyectos_asm`), ejecuta:
    ```bash
    ld hola_mundo.o -o hola_mundo
    ```
    * `ld`: Es el enlazador. Este programa es parte de las herramientas de `build-essential` que instalamos.
    * `hola_mundo.o`: Es el archivo objeto que generaste en el paso anterior.
    * `-o hola_mundo`: Le indicas al enlazador cómo quieres que se llame tu programa ejecutable final. En Linux, los ejecutables no suelen llevar una extensión como `.exe`.

2.  **Verifica:** De nuevo, si no ves errores, ¡has creado tu programa ejecutable! Usa `ls -l` en la terminal. Verás un nuevo archivo llamado `hola_mundo`. Observa sus permisos: tendrá una `x` (de "ejecutable"), por ejemplo, `-rwxr-xr-x`.

### 3.4. Ejecutar (Poner en Marcha tu Programa)

¡Ahora sí! Es el momento de ver tu programa en acción y disfrutar del "Hola Mundo" desde tu código en ensamblador.

1.  Desde la **misma terminal de VS Code** (en la carpeta `mis_proyectos_asm`), ejecuta:
    ```bash
    ./hola_mundo
    ```
    * `./`: Este prefijo es **crucial** en Linux. Le dice al sistema que el programa que quieres ejecutar se encuentra en el "directorio actual". Sin él, el sistema buscaría el programa solo en las rutas estándar del sistema (como `/bin` o `/usr/bin`), y no lo encontraría en tu carpeta personal.
    * `hola_mundo`: El nombre de tu programa ejecutable.

2.  **Observa la salida:** Inmediatamente después de ejecutar el comando, deberías ver el mensaje:
    ```
    Hola Mundo!
    ```

¡Felicidades! Has completado con éxito el ciclo de desarrollo de un programa en ensamblador para arquitecturas x86-64. Este proceso fundamental se aplica a todos los ejemplos que veremos a continuación.

---

## 4. Ejemplos Prácticos con Código Comentado para x86-64

Para cada uno de los siguientes ejemplos, recuerda los pasos de compilación y ejecución que acabamos de practicar:

1.  **Crea el archivo `.asm`:** En VS Code, crea un nuevo archivo (`.asm`) en tu carpeta `mis_proyectos_asm` y pega el código. **¡Guarda el archivo!**
2.  **Ensambla:** En la terminal de VS Code, ejecuta:
    ```bash
    nasm -f elf64 <nombre_archivo>.asm -o <nombre_archivo>.o
    ```
3.  **Enlaza:** En la terminal de VS Code, ejecuta:
    ```bash
    ld <nombre_archivo>.o -o <nombre_ejecutable>
    ```
4.  **Ejecuta:** En la terminal de VS Code, ejecuta:
    ```bash
    ./<nombre_ejecutable>
    ```

### 4.1. Suma y Resta de Dos Números Fijos

Este programa realiza una suma y una resta con dos números predefinidos y muestra los resultados. Incluye funciones auxiliares para convertir números a texto y para imprimir cadenas.

```assembly
section .data                   ; Sección para datos inicializados (variables, cadenas de texto, etc.)
    num1 db 10                  ; Define un byte 'num1' con el valor 10.
    num2 db 5                   ; Define un byte 'num2' con el valor 5.

    msg_sum db 'Suma: ', 0      ; Cadena de texto para mostrar "Suma: ". '0' es el terminador nulo.
    msg_sub db 'Resta: ', 0     ; Cadena de texto para mostrar "Resta: ".
    newline db 0xA, 0           ; Carácter de nueva línea (Enter) y terminador nulo.

section .bss                    ; Sección para datos no inicializados (se reserva espacio, pero no un valor inicial)
    res_buffer resb 20          ; Reserva 20 bytes de memoria para 'res_buffer'.
                                ; Aquí almacenaremos el resultado numérico convertido a cadena de texto.

section .text                   ; Sección para el código ejecutable
    global _start               ; Punto de entrada del programa.

_start:                         ; Inicio del programa

    ; --- Realizar la Suma y mostrar el resultado ---
    mov al, byte [num1]         ; Carga el valor del byte en la dirección de 'num1' (que es 10) en el registro AL.
                                ; AL es la parte baja de 8 bits del registro RAX.
    add al, byte [num2]         ; Suma el valor del byte en 'num2' (que es 5) al contenido de AL.
                                ; AL ahora contendrá 15 (10 + 5).
    mov ah, 0                   ; Limpia la parte alta de 8 bits de AX (AH).
                                ; Esto es importante para la instrucción 'div' en la función 'itoa'.
    call itoa                   ; Llama a la subrutina 'itoa' (Integer to ASCII).
                                ; Esta función convierte el número en AX (15) a su representación ASCII ("15")
                                ; y la guarda en 'res_buffer'. También prepara RSI para 'print_string'.
    mov rsi, msg_sum            ; Carga la dirección de la cadena 'msg_sum' ("Suma: ") en RSI.
    call print_string           ; Llama a la subrutina 'print_string' para imprimir "Suma: ".
    mov rsi, res_buffer         ; Carga la dirección de 'res_buffer' (donde 'itoa' puso el "15") en RSI.
    call print_string           ; Llama a 'print_string' para imprimir el "15".
    mov rsi, newline            ; Carga la dirección del carácter de nueva línea en RSI.
    call print_string           ; Imprime un salto de línea.

    ; --- Realizar la Resta y mostrar el resultado ---
    mov al, byte [num1]         ; Carga el valor del byte en 'num1' (10) en AL.
    sub al, byte [num2]         ; Resta el valor del byte en 'num2' (5) del contenido de AL.
                                ; AL ahora contendrá 5 (10 - 5).
    mov ah, 0                   ; Limpia AH de nuevo para 'itoa'.
    call itoa                   ; Llama a 'itoa' para convertir el resultado (5) a ASCII ("5") en 'res_buffer'.
    mov rsi, msg_sub            ; Carga la dirección de la cadena 'msg_sub' ("Resta: ") en RSI.
    call print_string           ; Imprime "Resta: ".
    mov rsi, res_buffer         ; Carga la dirección de 'res_buffer' en RSI.
    call print_string           ; Imprime el "5".
    mov rsi, newline            ; Carga la dirección del carácter de nueva línea en RSI.
    call print_string           ; Imprime un salto de línea.

    ; --- Salir del programa ---
    mov rax, 60                 ; Número de syscall para 'exit' (terminar el programa).
    mov rdi, 0                  ; Código de salida 0 (indica éxito).
    syscall                     ; Invoca la syscall 'exit'.

; --- Subrutina: itoa (Integer to ASCII) ---
; Convierte un número entero (en AX) a una cadena de caracteres ASCII.
; La cadena resultante se almacena en 'res_buffer'.
; Entrada: AX (16 bits) contiene el número a convertir.
; Salida: La cadena ASCII en 'res_buffer'. También RSI se ajusta para apuntar al inicio de esta cadena.
itoa:
    mov rdi, res_buffer + 19    ; Apunta RDI al final del 'res_buffer' (posición 19 de 0-19).
                                ; Vamos a construir la cadena de derecha a izquierda (dígitos menos significativos primero).
    mov byte [rdi], 0           ; Coloca el terminador nulo (0) al final del buffer.
    dec rdi                     ; Decrementa RDI para apuntar al byte justo antes del terminador nulo.

.loop_itoa:                     ; Bucle principal para la conversión
    mov rdx, 0                  ; Limpia el registro RDX. Para la instrucción 'div' (división),
                                ; RDX:RAX es el dividendo. Como estamos dividiendo un número de 16 bits (AX),
                                ; RDX debe ser 0 para que la operación sea correcta.
    mov rbx, 10                 ; Mueve el valor 10 al registro RBX (nuestro divisor para conversión a base 10).
    div rbx                     ; Divide AX (dividendo) por RBX (divisor).
                                ; El cociente (resultado de la división entera) se guarda en AL (parte baja de AX).
                                ; El residuo (lo que sobra) se guarda en AH (parte alta de AX).
    add ah, '0'                 ; Convierte el residuo (en AH), que es un dígito numérico (0-9), a su carácter ASCII
                                ; correspondiente sumándole el valor ASCII del carácter '0' (que es 48).
    mov byte [rdi], ah          ; Almacena el dígito ASCII recién convertido en la posición de memoria apuntada por RDI.
    dec rdi                     ; Decrementa RDI para moverse a la siguiente posición a la izquierda en el buffer,
                                ; donde se colocará el próximo dígito (más significativo).
    mov ah, 0                   ; Limpia AH para la próxima iteración. (AL ya contiene el nuevo cociente para la siguiente división).
    cmp al, 0                   ; Compara el cociente (en AL) con 0.
    jnz .loop_itoa              ; Si el cociente no es cero (Jump if Not Zero), salta de nuevo a '.loop_itoa'
                                ; para seguir dividiendo y extrayendo más dígitos.

    inc rdi                     ; Cuando el bucle termina (AL es 0, no quedan más dígitos), RDI apunta un byte antes del primer dígito real.
                                ; Lo incrementamos para que apunte al inicio de la cadena de resultado en el buffer.
    mov rsi, rdi                ; Mueve la dirección del inicio de la cadena de resultado a RSI.
                                ; Esto es para que la subrutina 'print_string' sepa dónde está la cadena a imprimir.
    ret                         ; Regresa de la subrutina 'itoa'.

; --- Subrutina: print_string ---
; Imprime una cadena de caracteres en la salida estándar (consola).
; Entrada: RSI debe contener la dirección de la cadena (la cadena debe terminar con un byte nulo (0)).
print_string:
    ; Guardamos el estado de los registros que vamos a modificar en la pila. Esto es una buena práctica
    ; para asegurar que la función que nos llamó no vea sus registros cambiados.
    push rax                    ; Guarda el valor actual de RAX en la pila.
    push rdi                    ; Guarda el valor actual de RDI.
    push rdx                    ; Guarda el valor actual de RDX.
    push rsi                    ; Guarda el valor actual de RSI.

    mov rdx, 0                  ; Inicializa RDX a 0. Usaremos RDX como contador para calcular la longitud de la cadena.
    .loop_len:                  ; Bucle para calcular la longitud de la cadena (hasta encontrar el terminador nulo)
        cmp byte [rsi + rdx], 0 ; Compara el byte en la dirección (RSI + RDX) con 0 (el terminador nulo).
        je .end_loop_len        ; Si es igual a 0, significa que hemos encontrado el final de la cadena, salta al final del bucle.
        inc rdx                 ; Si no es 0, incrementa RDX (contando un carácter más).
        jmp .loop_len           ; Salta de nuevo al inicio del bucle.
    .end_loop_len:              ; Fin del bucle de longitud. RDX ahora contiene la longitud exacta de la cadena.

    mov rax, 1                  ; Mueve el número de la syscall 'write' (1) a RAX.
    mov rdi, 1                  ; Mueve el descriptor de archivo para la salida estándar (1 - stdout) a RDI.
    ; RSI ya contiene la dirección de la cadena (pasada como argumento).
    ; RDX ya contiene la longitud de la cadena (calculada en el bucle anterior).

    syscall                     ; Invoca la syscall 'write' para imprimir la cadena en la consola.

    ; Restauramos los registros guardados de la pila, en el orden inverso en que se guardaron.
    pop rsi
    pop rdx
    pop rdi
    pop rax
    ret                         ; Regresa de la subrutina 'print_string' a la función que la llamó.
