### **Módulo 1: Fundamentos y Nivelación (Semanas 1-4)**

Este módulo establece las bases necesarias en programación y arquitectura para poder abordar los temas complejos del curso. (Contenido módulo 1\)


**Sistemas Operativos Virtualizados y Tipos de Virtualización**

Un sistema operativo virtualizado es un **\*\*entorno que simula un hardware\*\*** y permite ejecutar un sistema operativo invitado dentro de otro sistema operativo anfitrión. Esto significa que puedes tener, por ejemplo, una versión de Linux ejecutándose dentro de tu sistema Windows, o viceversa, sin necesidad de particionar tu disco duro o reiniciar tu computadora.

# Tipos de Virtualización

Existen varios tipos de virtualización, cada uno con sus propias características y casos de uso:

* Virtualización Completa (Full Virtualization)  
  * **Descripción:** En este tipo, el **hypervisor** (una capa de software que gestiona la virtualización) simula completamente el hardware subyacente, lo que permite que un sistema operativo invitado sin modificar se ejecute como si estuviera en una máquina física. Los sistemas operativos invitados no necesitan ser conscientes de que están virtualizados.  
  * **Ejemplos:**VMware Workstation, Oracle VirtualBox.  
  * **Ventajas:** Alta compatibilidad con una amplia gama de sistemas operativos invitados.  
  * **Desventajas:** Puede tener un mayor *\*overhead\** de rendimiento debido a la emulación completa del hardware.

*  Paravirtualización  
  * **Descripción:** A diferencia de la virtualización completa, en la paravirtualización el sistema operativo invitado es **modificado (o "porteado")** para colaborar directamente con el hypervisor. Esto permite una comunicación más eficiente y un mejor rendimiento, ya que el hypervisor no necesita emular todo el hardware.  
  * **Ejemplos:** Xen, KVM (con ciertos controladores).  
  * **Ventajas:** Mayor rendimiento y menor *overhead* que la virtualización completa.  
  * **Desventajas:** Requiere la modificación del sistema operativo invitado, lo que puede limitar la compatibilidad.

* Virtualización a Nivel de Sistema Operativo (OS-level Virtualization / Contenedores)  
  * **Descripción:** En este enfoque, el **kernel del sistema operativo anfitrión es compartido** entre múltiples entornos aislados (contenedores). Cada contenedor se ejecuta de forma aislada, pero comparte los recursos del kernel del anfitrión. Esto es más ligero que las máquinas virtuales completas porque no hay un sistema operativo invitado completo dentro de cada contenedor.  
  *  **Ejemplos:** Docker, LXC (Linux Containers), Podman.  
  * **Ventajas:** Muy ligero, rápido de iniciar, eficiente en el uso de recursos. Ideal para microservicios y despliegue de aplicaciones.  
  * **Desventajas:** Todos los contenedores deben usar el mismo kernel del sistema operativo anfitrión. Menos aislamiento que la virtualización completa.

* Virtualización Asistida por Hardware  
  * **Descripción:** Muchos CPUs modernos incluyen **extensiones de virtualización** (como Intel VT-x o AMD-V) que mejoran significativamente el rendimiento de la virtualización completa. Estas extensiones permiten que el hypervisor ejecute ciertas instrucciones del sistema operativo invitado directamente en el hardware, reduciendo la necesidad de emulación por software.  
  * **Ejemplos:** La mayoría de los hypervisores modernos (incluidos VMware, VirtualBox, Hyper-V, KVM) utilizan estas extensiones cuando están disponibles.  
  * **Ventajas:** Mejora drásticamente el rendimiento de la virtualización completa.  
  * **Desventajas:** Requiere soporte de hardware en el CPU y a menudo debe ser habilitado en la BIOS/UEFI.

# Cómo Instalar WSL (Windows Subsystem for Linux)

WSL es una capa de compatibilidad para ejecutar ejecutables de Linux directamente en Windows. No es una máquina virtual tradicional, sino un tipo de virtualización a nivel de sistema operativo que permite una integración muy fluida.

**Requisitos Previos:**

* Windows 10, versión 2004 o posterior (Build 19041 o superior) para WSL2.  
* Virtualización habilitada en la BIOS/UEFI de tu computadora (generalmente "Intel VT-x" o "AMD-V").

**Pasos de Instalación:**

* **Abrir PowerShell como Administrador:**  
  Haz clic derecho en el botón de Inicio de Windows y selecciona "Windows PowerShell (Administrador)" o "Terminal de Windows (Administrador)".  
* **Habilitar las características de la Plataforma de Máquina Virtual y el Subsistema de Windows para Linux:**  
   Ejecuta el siguiente comando:

| dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart |
| :---- |

* **Reiniciar tu Computadora:**  
  Es necesario reiniciar para que los cambios surtan efecto.  
* **Establecer WSL 2 como tu versión predeterminada (Recomendado):**  
  Después del reinicio, abre PowerShell o Símbolo de sistema y ejecuta:

| wsl \--set-default-version 2 Si ves un mensaje sobre "Actualización del componente del kernel de WSL 2", visita \[https://aka.ms/wsl2kernel\](https://aka.ms/wsl2kernel) y descarga el paquete de actualización. Ejecútalo y luego vuelve a intentar el comando \`wsl \--set-default-version 2\. |
| :---- |

* **Instalar una Distribución de Linux desde la Microsoft Store:**  
  Abre la **Microsoft Store** y busca tu distribución de Linux preferida, como "Ubuntu", "Debian", "Kali Linux", etc.  
  Haz clic en la distribución que deseas y luego en "Obtener" o "Instalar".  
  Una vez instalada, haz clic en "Iniciar".  
* **Configurar tu Nueva Distribución de Linux:**  
  La primera vez que inicies la distribución, se te pedirá que crees un **nombre de usuario y una contraseña** para tu entorno Linux. Recuerda estas credenciales.

¡Listo\! Ya tienes WSL instalado con tu distribución de Linux. Puedes acceder a ella abriendo el menú de inicio y buscando la distribución que instalaste (por ejemplo, "Ubuntu").

## Cómo Instalar C y C++ en WSL y Linux

Una vez que tienes tu entorno Linux (ya sea a través de WSL o una instalación nativa), la instalación de los compiladores de C y C++ es sencilla. Utilizaremos \`build-essential\`, un paquete que incluye el compilador GCC (GNU Compiler Collection), G++ (el compilador de C++), make, y otras herramientas esenciales para la compilación.

**Pasos de Instalación:**

* **Abrir la Terminal:**  
  * Si estás en WSL, abre tu distribución de Linux (por ejemplo, "Ubuntu").  
  * Si estás en una instalación nativa de Linux, abre la aplicación de Terminal.  
* **Actualizar la Lista de Paquetes:**  
  Es una buena práctica actualizar primero la lista de paquetes para asegurarte de que estás obteniendo las últimas versiones:

|  sudo apt update  Se te pedirá tu contraseña de usuario de Linux. Escríbela (no verás los caracteres mientras escribes) y presiona Enter.  |
| :---- |

* **Instalar \`build-essential\`:**  
  Ahora, instala el paquete \`build-essential\`:

| sudo apt install build-essential Cuando se te pregunte si deseas continuar (Y/n), escribe \`Y\` y presiona Enter. |
| :---- |

* **Verificar la Instalación:**  
  Puedes verificar que GCC y G++ se han instalado correctamente ejecutando los siguientes comandos para ver sus versiones:

|  gcc \--version  g++ \--version          Si ves la información de la versión, ¡los compiladores están listos para usar\! |
| :---- |


# Iniciando en C y C++ y Ejemplos de Código

### Conceptos Básicos de C

C es un lenguaje de programación de propósito general, imperativo y estructurado, muy eficiente y ampliamente utilizado para sistemas operativos, desarrollo de juegos y sistemas embebidos.

## Estructura Básica de un Programa en C:

\`\`\`c  
\#include \<stdio.h\> // Incluye la biblioteca estándar de entrada/salida

int main() { // Función principal donde comienza la ejecución del programa  
    printf("¡Hola, Mundo\!\\n"); // Imprime texto en la consola  
    return 0; // Indica que el programa finalizó exitosamente  
}  
\`\`\`

* \#include \<stdio.h\>: Incluye la cabecera de la biblioteca estándar de entrada/salida. Esto nos da acceso a funciones como printf().  
* int main(): Es la función principal donde comienza la ejecución de cualquier programa en C. int indica que la función devuelve un entero (generalmente 0 para éxito).  
* printf("¡Hola, Mundo\!\\n");: La función printf() se utiliza para imprimir texto en la consola. \\n es un carácter de nueva línea.  
* return 0;: Termina la función main y devuelve un valor de 0 al sistema operativo, indicando una ejecución exitosa.

## Conceptos Básicos de C++

C++ es una extensión de C, añadiendo características de programación orientada a objetos (POO), manejo de excepciones, plantillas y la Standard Template Library (STL).

### Estructura Básica de un Programa en C++:

C++

\#**include** \<iostream\> // Incluye la biblioteca de entrada/salida de C++

int main() { // Función principal  
    std::cout \<\< "¡Hola, C++ Mundo\!" \<\< std::endl; // Imprime texto  
    return 0; // Indica que el programa finalizó exitosamente  
}

* \#include \<iostream\>: Incluye la cabecera para las operaciones de entrada/salida en C++.  
* std::cout: Es el objeto de flujo de salida estándar, usado para imprimir en la consola.  
* \<\<: Es el operador de inserción, usado para enviar datos al flujo de salida.  
* "¡Hola, C++ Mundo\!": La cadena de texto a imprimir.  
* std::endl: Inserta una nueva línea y vacía el búfer de salida.  
* return 0;: Igual que en C.

---

## **Códigos de Ejemplo y Compilación**

### **Ejemplo 1: "Hola, Mundo" en C**

**Código (hello.c):**

C

\#**include** \<stdio.h\>

int main() {  
    printf("¡Hola, Mundo desde C\!\\n");  
    return 0;  
}

**Instrucciones para Compilar y Ejecutar:**

1. **Guarda el código:** Abre un editor de texto (Nano, Vim, VS Code, Gedit, etc.) en tu entorno Linux (WSL o nativo) y guarda el código anterior como hello.c.  
   * Por ejemplo, en la terminal: nano hello.c (pega el código y guarda con Ctrl+X, Y, Enter).  
2. **Compila:** En la terminal, usa gcc para compilar:  
3. *gcc hello.c \-o hello\_c*

   \-o hello\_c especifica que el ejecutable de salida se llamará hello\_c. Si no usas \-o, el ejecutable se llamará a.out por defecto.  
4. **Ejecuta:**  
   ./hello\_c  
5. Verás la salida: ¡Hola, Mundo desde C\!

---

### **Ejemplo 2: Suma de Dos Números en C++**

**Código (sum.cpp):**

C++

\#**include** \<iostream\>

int main() {  
    int num1;  
    int num2;  
    int sum;

    std::cout \<\< "Introduce el primer numero: ";  
    std::cin \>\> num1; // Lee el primer número

    std::cout \<\< "Introduce el segundo numero: ";  
    std::cin \>\> num2; // Lee el segundo número

    sum \= num1 \+ num2; // Calcula la suma

    std::cout \<\< "La suma es: " \<\< sum \<\< std::endl; // Muestra el resultado

    return 0;  
}

**Instrucciones para Compilar y Ejecutar:**

1. **Guarda el código:** Guarda el código anterior como sum.cpp.  
   * Por ejemplo: nano sum.cpp  
2. **Compila:** En la terminal, usa g++ para compilar:  
3. g++ sum.cpp \-o sum\_cpp  
      \-o sum\_cpp especifica el nombre del ejecutable.  
4. **Ejecuta:**  
   ./sum\_cpp  
5. El programa te pedirá que introduzcas dos números y luego mostrará su suma.

---

### **Ejemplo 3: Uso de Condicionales (if/else) en C**

**Código (conditional.c):**

C

\#**include** \<stdio.h\>

int main() {  
    int edad;

    printf("Por favor, introduce tu edad: ");  
    scanf("%d", \&edad); // Lee un entero desde el teclado

    if (edad \>= 18) {  
        printf("Eres mayor de edad.\\n");  
    } else {  
        printf("Eres menor de edad.\\n");  
    }

    return 0;  
}

**Instrucciones para Compilar y Ejecutar:**

1. **Guarda el código:** Guarda el código como conditional.c.  
   * Por ejemplo: nano conditional.c  
2. **Compila:**  
   gcc conditional.c \-o conditional\_c  
3. **Ejecuta:**  
4.     ./conditional\_c  
5. El programa te pedirá tu edad y te dirá si eres mayor o menor de edad.

---

### **Ejemplo 4: Bucle (for) en C++**

**Código (loop.cpp):**

C++

\#**include** \<iostream\>

int main() {  
    std::cout \<\< "Contando del 1 al 5:" \<\< std::endl;  
    for (int i \= 1; i \<= 5; \++i) { // Bucle for: inicialización, condición, incremento  
        std::cout \<\< i \<\< std::endl;  
    }  
    return 0;  
}

**Instrucciones para Compilar y Ejecutar:**

1. **Guarda el código:** Guarda el código como loop.cpp.  
   * Por ejemplo: nano loop.cpp  
2. **Compila:**  
   g++ loop.cpp \-o loop\_cpp  
3. **Ejecuta:**  
        ./loop\_cpp  
4. Verás la salida:  
   Contando del 1 al 5:  
   1  
   2  
   3  
   4  
   5  
   ……..
