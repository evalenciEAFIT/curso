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



------------------------------------------------------------



## Contenido del Módulo

A continuación se presenta un menú de navegación para acceder a los archivos disponibles en este directorio:

### 0. **Comando generales de Linux**
- [**guiaLinux.md**](./guiaLinux.md)
  - Documento introductorio general sobre el uso de Linux
 

### 1. **Introducción General a Ensamblador**
- [**intro.md**](./intro.md)
  - Documento introductorio general sobre el módulo y su propósito. - ASSEMBLER
 
### 2. **Introducción a C++**
- [**introC_CPP.md**](./introC_CPP.md)
  - Guía introductoria a C++, comparaciones con C y conceptos básicos. (c/C++)

### 3. **C y Ensamblador**
- [**assembler.md**](./assembler.md)
  - Introducción al lenguaje de ensamblador, ejemplos básicos y conceptos clave.
- [**CconAsM.md**](./CconAsM.md)
  - Exploración de la relación entre C y ensamblador, incluyendo cómo interactúan ambos lenguajes.

### 4. **Introducción al Kernel**
- [**intro_kernel.md**](./intro_kernel.md)
  - Conceptos fundamentales sobre kernels de sistemas operativos, estructura básica y componentes clave. (ASSEMBLE)

