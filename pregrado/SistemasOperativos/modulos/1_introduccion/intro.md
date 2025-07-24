# Hola Mundo en Ensamblador x86-64 para Windows 11 con WSL

## 📋 Descripción

Este tutorial te guiará paso a paso para crear, compilar y ejecutar un programa "Hola Mundo" en lenguaje ensamblador para procesadores Intel/AMD de 64 bits, utilizando Windows Subsystem for Linux (WSL) y Visual Studio Code en Windows 11.

## 🛠️ Requisitos Previos

Antes de comenzar, asegúrate de tener instalados los siguientes componentes:

### 1. Windows Subsystem for Linux (WSL2)
- Windows 11 con WSL2 habilitado
- Distribución Linux instalada (preferentemente Ubuntu)

### 2. Visual Studio Code
- Editor de código con extensión WSL instalada

### 3. Herramientas de desarrollo en WSL
- NASM (Netwide Assembler)
- GNU Linker (ld)
- Build-essential

## 🔧 Instalación de Herramientas

Abre tu terminal de WSL y ejecuta los siguientes comandos:

```bash
# Actualizar lista de paquetes
sudo apt update

# Instalar NASM y herramientas de compilación
sudo apt install nasm build-essential
```

### Verificar instalación:
```bash
nasm --version
ld --version
```

## 💻 Código Fuente

Crea un archivo llamado `hola.asm` con el siguiente contenido:

```nasm
; ================================================
; HOLA MUNDO EN ENSAMBLADOR x86-64 PARA LINUX
; ================================================

section .data                   ; Sección de datos inicializados
    msg db 'Hola Mundo', 0xA    ; Declaramos una variable 'msg' que contiene:
                                ; - 'Hola Mundo': cadena de caracteres
                                ; - 0xA: carácter de nueva línea (ASCII 10)
    len equ $ - msg             ; Calculamos la longitud del mensaje:
                                ; - $ = dirección actual
                                ; - msg = dirección inicial
                                ; - equ define una constante en tiempo de ensamblado

section .text                   ; Sección de código ejecutable
    global _start               ; Hacemos visible '_start' para el enlazador
                                ; (es el punto de entrada del programa)

_start:                         ; Punto de entrada del programa
    ; ---- ESCRIBIR MENSAJE EN PANTALLA ----
    mov rax, 1                  ; Cargamos en RAX el número de la llamada al sistema:
                                ; 1 = sys_write (escribir en un descriptor de archivo)
    
    mov rdi, 1                  ; Cargamos en RDI el descriptor de archivo:
                                ; 1 = stdout (salida estándar - pantalla)
    
    mov rsi, msg                ; Cargamos en RSI la dirección del buffer a escribir:
                                ; 'msg' apunta al inicio de nuestra cadena
    
    mov rdx, len                ; Cargamos en RDX la cantidad de bytes a escribir:
                                ; 'len' contiene la longitud calculada
    
    syscall                     ; Llamamos al sistema operativo para ejecutar sys_write

    ; ---- SALIR DEL PROGRAMA ----
    mov rax, 60                 ; Cargamos en RAX el número de llamada al sistema:
                                ; 60 = sys_exit (terminar el proceso)
    
    mov rdi, 0                  ; Cargamos en RDI el código de salida:
                                ; 0 = éxito, cualquier otro valor indica error
    
    syscall                     ; Llamamos al sistema operativo para ejecutar sys_exit
```

## ⚙️ Proceso de Compilación

El proceso de compilación en ensamblador consta de dos etapas principales:

### 1. Ensamblado (Assembly)
Convierte el código fuente en código objeto:

```bash
nasm -f elf64 hola.asm -o hola.o
```

**Parámetros explicados:**
- `-f elf64`: Especifica el formato de salida como ELF64 (formato ejecutable de Linux 64-bit)
- `hola.asm`: Archivo fuente de entrada
- `-o hola.o`: Archivo objeto de salida

### 2. Enlazado (Linking)
Combina el código objeto con las bibliotecas necesarias para crear el ejecutable final:

```bash
ld hola.o -o hola
```

**Parámetros explicados:**
- `hola.o`: Archivo objeto de entrada
- `-o hola`: Ejecutable de salida

## ▶️ Ejecución del Programa

Una vez compilado, ejecuta el programa:

```bash
./hola
```

**Salida esperada:**
```
Hola Mundo
```

## 📚 Explicación Detallada del Código

### Registros del procesador x86-64 utilizados:
- **RAX**: Almacena el número de la llamada al sistema
- **RDI**: Primer argumento de la llamada al sistema
- **RSI**: Segundo argumento
- **RDX**: Tercer argumento

### Llamadas al sistema en Linux x86-64:
| Número | Nombre     | Descripción                    |
|--------|------------|--------------------------------|
| 1      | sys_write  | Escribe datos a un fd          |
| 60     | sys_exit   | Termina el proceso actual      |

### Descriptores de archivo estándar:
- **0**: stdin (entrada estándar)
- **1**: stdout (salida estándar)
- **2**: stderr (error estándar)

### Directivas del ensamblador:
- `section .data`: Define la sección de datos
- `section .text`: Define la sección de código
- `db`: Define bytes (almacena datos)
- `equ`: Define una constante
- `global`: Hace visible un símbolo para el enlazador
- `$`: Dirección actual en memoria

## 🧪 Opciones de Depuración

Para compilar con información de depuración:

```bash
nasm -g -f elf64 hola.asm -o hola.o
ld hola.o -o hola
```

Luego puedes usar GDB para depurar:

```bash
gdb ./hola
```

## 📁 Estructura del Proyecto

```
proyecto-ensamblador/
├── hola.asm        # Código fuente
├── hola.o          # Archivo objeto (generado)
└── hola            # Ejecutable final (generado)
```

## ⚠️ Notas Importantes

1. **Compatibilidad**: Este código está diseñado para Linux x86-64 y funciona en WSL, pero no en Windows nativo.

2. **Sintaxis**: Utiliza la sintaxis Intel con NASM como ensamblador.

3. **Llamadas al sistema**: Utiliza las llamadas al sistema de Linux directamente.

4. **Arquitectura**: Específico para procesadores de 64 bits (x86-64).

## 🎯 Conclusión

Este tutorial demuestra cómo crear un programa básico en ensamblador utilizando herramientas modernas en Windows 11. El uso de WSL permite acceder a un entorno Linux completo para el desarrollo en bajo nivel, mientras que VS Code proporciona una experiencia de desarrollo moderna y eficiente.

¡Felicitaciones! Has creado tu primer programa en ensamblador x86-64.
