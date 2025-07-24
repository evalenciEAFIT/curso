# Comandos Básicos de Linux

## 📋 Descripción General

Este documento presenta los comandos más fundamentales del sistema operativo Linux, esenciales para la navegación, gestión de archivos y operaciones básicas en la terminal.

## 🎯 Objetivo

Proporcionar una guía completa de los comandos básicos de Linux para usuarios nuevos y principiantes en sistemas Unix-like.

---

## 📁 1. Navegación y Directorios

### **pwd - Print Working Directory**
```bash
pwd
```
- **Función**: Muestra el directorio actual de trabajo
- **Uso**: Verificar en qué directorio te encuentras
- **Ejemplo**: 
  ```bash
  $ pwd
  /home/usuario/documentos
  ```

### **ls - List**
```bash
ls [opciones] [directorio]
```
- **Función**: Listar archivos y directorios
- **Opciones comunes**:
  - `-l`: Formato largo (detallado)
  - `-a`: Mostrar archivos ocultos
  - `-h`: Tamaños legibles (KB, MB)
  - `-t`: Ordenar por fecha
  - `-R`: Listar recursivamente

- **Ejemplos**:
  ```bash
  ls                    # Listar archivos en directorio actual
  ls -l                 # Listar con detalles
  ls -la                # Listar todo incluyendo ocultos
  ls -lh /home          # Listar directorio home con tamaños legibles
  ```

### **cd - Change Directory**
```bash
cd [directorio]
```
- **Función**: Cambiar de directorio
- **Usos especiales**:
  - `cd`: Ir al directorio home
  - `cd ..`: Ir al directorio padre
  - `cd -`: Volver al directorio anterior
  - `cd ~`: Ir al directorio home
  - `cd /`: Ir al directorio raíz

- **Ejemplos**:
  ```bash
  cd /home/usuario      # Cambiar a directorio específico
  cd ..                 # Subir un nivel
  cd documentos         # Entrar a subdirectorio
  cd -                  # Volver al directorio anterior
  ```

---

## 📝 2. Gestión de Archivos y Directorios

### **mkdir - Make Directory**
```bash
mkdir [opciones] directorio
```
- **Función**: Crear nuevos directorios
- **Opciones**:
  - `-p`: Crear directorios padres si no existen
  - `-v`: Mostrar información detallada

- **Ejemplos**:
  ```bash
  mkdir nuevo_directorio           # Crear un directorio
  mkdir -p ruta/a/nuevo/directorio # Crear estructura completa
  mkdir dir1 dir2 dir3             # Crear múltiples directorios
  ```

### **touch**
```bash
touch [opciones] archivo
```
- **Función**: Crear archivos vacíos o actualizar timestamps
- **Usos**:
  - Crear archivos nuevos
  - Actualizar fecha de modificación

- **Ejemplos**:
  ```bash
  touch archivo.txt                # Crear archivo vacío
  touch archivo1.txt archivo2.txt  # Crear múltiples archivos
  touch -t 202312251200 archivo.txt # Establecer fecha específica
  ```

### **cp - Copy**
```bash
cp [opciones] origen destino
```
- **Función**: Copiar archivos y directorios
- **Opciones importantes**:
  - `-r`: Copiar recursivamente (directorios)
  - `-i`: Preguntar antes de sobreescribir
  - `-v`: Modo verbose (mostrar proceso)
  - `-a`: Copia archivada (preservar todo)

- **Ejemplos**:
  ```bash
  cp archivo.txt copia.txt         # Copiar archivo
  cp -r directorio copia_dir       # Copiar directorio
  cp *.txt documentos/             # Copiar todos los .txt
  cp -i archivo.txt existente.txt  # Copiar con confirmación
  ```

### **mv - Move/Rename**
```bash
mv [opciones] origen destino
```
- **Función**: Mover archivos o renombrarlos
- **Opciones**:
  - `-i`: Preguntar antes de sobreescribir
  - `-v`: Modo verbose
  - `-n`: No sobreescribir archivos existentes

- **Ejemplos**:
  ```bash
  mv archivo.txt nuevo_nombre.txt  # Renombrar archivo
  mv archivo.txt documentos/       # Mover archivo a directorio
  mv *.jpg imagenes/               # Mover todos los .jpg
  ```

### **rm - Remove**
```bash
rm [opciones] archivo
```
- **Función**: Eliminar archivos y directorios
- **⚠️ Precaución**: ¡Esta acción es irreversible!
- **Opciones críticas**:
  - `-r`: Eliminar directorios recursivamente
  - `-f`: Forzar eliminación (sin preguntar)
  - `-i`: Preguntar antes de eliminar
  - `-v`: Modo verbose

- **Ejemplos**:
  ```bash
  rm archivo.txt                   # Eliminar archivo
  rm -i archivo.txt                # Eliminar con confirmación
  rm -r directorio                 # Eliminar directorio
  rm -rf directorio                # Forzar eliminación de directorio
  rm *.tmp                         # Eliminar todos los .tmp
  ```

---

## 🔍 3. Visualización y Búsqueda

### **cat - Concatenate**
```bash
cat [opciones] archivo
```
- **Función**: Mostrar contenido de archivos
- **Usos adicionales**: Concatenar archivos

- **Ejemplos**:
  ```bash
  cat archivo.txt                  # Mostrar contenido
  cat archivo1.txt archivo2.txt    # Concatenar y mostrar
  cat > nuevo.txt                  # Crear archivo (modo escritura)
  cat >> archivo.txt               # Añadir al final
  ```

### **less**
```bash
less [opciones] archivo
```
- **Función**: Visualizar archivos grandes por páginas
- **Ventajas**: No carga todo el archivo en memoria
- **Controles**:
  - `Espacio`: Página siguiente
  - `b`: Página anterior
  - `/texto`: Buscar texto
  - `q`: Salir

- **Ejemplos**:
  ```bash
  less archivo_grande.log          # Ver archivo grande
  less +F archivo.log              # Seguir archivo (como tail -f)
  ```

### **head**
```bash
head [opciones] archivo
```
- **Función**: Mostrar las primeras líneas de un archivo
- **Opciones**:
  - `-n número`: Número de líneas (por defecto 10)
  - `-c bytes`: Número de bytes

- **Ejemplos**:
  ```bash
  head archivo.txt                 # Primeras 10 líneas
  head -n 20 archivo.txt           # Primeras 20 líneas
  head -c 100 archivo.txt          # Primeros 100 bytes
  ```

### **tail**
```bash
tail [opciones] archivo
```
- **Función**: Mostrar las últimas líneas de un archivo
- **Opciones útiles**:
  - `-n número`: Número de líneas
  - `-f`: Seguir archivo (actualizaciones en tiempo real)
  - `-c bytes`: Número de bytes

- **Ejemplos**:
  ```bash
  tail archivo.log                 # Últimas 10 líneas
  tail -f archivo.log              # Seguir archivo en tiempo real
  tail -n 20 archivo.txt           # Últimas 20 líneas
  ```

### **grep - Global Regular Expression Print**
```bash
grep [opciones] patrón archivo
```
- **Función**: Buscar patrones en archivos
- **Opciones importantes**:
  - `-i`: Ignorar mayúsculas/minúsculas
  - `-r`: Búsqueda recursiva
  - `-n`: Mostrar números de línea
  - `-v`: Mostrar líneas que NO coinciden

- **Ejemplos**:
  ```bash
  grep "error" archivo.log         # Buscar "error"
  grep -i "ERROR" archivo.log      # Búsqueda case-insensitive
  grep -r "función" directorio/    # Búsqueda recursiva
  grep -n "importante" archivo.txt # Con números de línea
  ```

---

## 🔧 4. Información del Sistema

### **whoami**
```bash
whoami
```
- **Función**: Mostrar el nombre del usuario actual
- **Ejemplo**:
  ```bash
  $ whoami
  usuario
  ```

### **date**
```bash
date [opciones]
```
- **Función**: Mostrar o establecer fecha y hora del sistema
- **Ejemplos**:
  ```bash
  date                             # Fecha y hora actual
  date +"%Y-%m-%d"                 # Formato personalizado
  ```

### **uptime**
```bash
uptime
```
- **Función**: Mostrar tiempo de actividad del sistema
- **Información**: Tiempo encendido, usuarios, carga promedio

### **df - Disk Free**
```bash
df [opciones]
```
- **Función**: Mostrar espacio en disco
- **Opciones**:
  - `-h`: Tamaños legibles
  - `-T`: Mostrar tipo de sistema de archivos

- **Ejemplos**:
  ```bash
  df -h                            # Espacio en disco legible
  ```

### **du - Disk Usage**
```bash
du [opciones] [directorio]
```
- **Función**: Mostrar uso de disco de archivos/directorios
- **Opciones**:
  - `-h`: Tamaños legibles
  - `-s`: Resumen total
  - `--max-depth=N`: Profundidad máxima

- **Ejemplos**:
  ```bash
  du -h                            # Uso de disco actual
  du -sh documentos/               # Tamaño total de documentos
  ```

---

## 🌐 5. Información de la Distribución de Linux

### **lsb_release - Linux Standard Base**
```bash
lsb_release [opciones]
```
- **Función**: Mostrar información de la distribución Linux
- **Opciones**:
  - `-a`: Mostrar toda la información
  - `-d`: Mostrar descripción
  - `-r`: Mostrar versión
  - `-i`: Mostrar ID de distribución

- **Ejemplos**:
  ```bash
  lsb_release -a                   # Toda la información
  lsb_release -d                   # Solo descripción
  lsb_release -r                   # Solo versión
  ```

### **uname - Unix Name**
```bash
uname [opciones]
```
- **Función**: Mostrar información del sistema
- **Opciones importantes**:
  - `-a`: Toda la información del sistema
  - `-r`: Versión del kernel
  - `-m`: Arquitectura del sistema
  - `-n`: Nombre del host
  - `-s`: Nombre del kernel
  - `-v`: Versión del kernel

- **Ejemplos**:
  ```bash
  uname -a                         # Información completa del sistema
  uname -r                         # Versión del kernel
  uname -m                         # Arquitectura (x86_64, arm, etc.)
  uname -n                         # Nombre del host
  ```

### **cat /etc/os-release**
```bash
cat /etc/os-release
```
- **Función**: Mostrar información detallada de la distribución
- **Contenido**: Nombre, versión, ID, URL, soporte, etc.
- **Ejemplo de salida**:
  ```bash
  NAME="Ubuntu"
  VERSION="22.04.3 LTS (Jammy Jellyfish)"
  ID=ubuntu
  ID_LIKE=debian
  PRETTY_NAME="Ubuntu 22.04.3 LTS"
  VERSION_ID="22.04"
  ```

### **hostnamectl**
```bash
hostnamectl
```
- **Función**: Mostrar y modificar información del host
- **Información**: Sistema operativo, kernel, arquitectura, hostname
- **Ejemplo de salida**:
  ```bash
  Static hostname: mi-equipo
  Icon name: computer-laptop
  Chassis: laptop
  Machine ID: xxxxxxxx
  Boot ID: xxxxxxxx
  Operating System: Ubuntu 22.04.3 LTS
  Kernel: Linux 5.15.0-88-generic
  Architecture: x86-64
  ```

### **cat /proc/version**
```bash
cat /proc/version
```
- **Función**: Mostrar información de versión del kernel y compilador
- **Contenido**: Versión del kernel, compilador usado, fecha de compilación

### **cat /etc/issue**
```bash
cat /etc/issue
```
- **Función**: Mostrar mensaje de bienvenida del sistema (pre-login)

### **neofetch** (si está instalado)
```bash
neofetch
```
- **Función**: Mostrar información del sistema con arte ASCII
- **Instalación**:
  ```bash
  sudo apt install neofetch        # Ubuntu/Debian
  sudo dnf install neofetch        # Fedora
  sudo pacman -S neofetch          # Arch Linux
  ```

---

## 🔧 6. Permisos y Propiedad

### **chmod - Change Mode**
```bash
chmod [permisos] archivo
```
- **Función**: Cambiar permisos de archivos/directorios
- **Formatos**:
  - Numérico: `chmod 755 archivo`
  - Simbólico: `chmod u+rwx,g+rx,o+rx archivo`

- **Ejemplos**:
  ```bash
  chmod 755 script.sh              # Permisos rwxr-xr-x
  chmod +x ejecutable              # Añadir permiso de ejecución
  chmod -R 755 directorio/         # Recursivo
  ```

### **chown - Change Owner**
```bash
chown [usuario:grupo] archivo
```
- **Función**: Cambiar propietario de archivos
- **Ejemplos**:
  ```bash
  chown usuario archivo.txt        # Cambiar propietario
  chown usuario:grupo archivo.txt  # Cambiar propietario y grupo
  chown -R usuario directorio/     # Recursivo
  ```

### **chgrp - Change Group**
```bash
chgrp grupo archivo
```
- **Función**: Cambiar grupo de archivos
- **Ejemplo**:
  ```bash
  chgrp desarrolladores archivo.txt
  ```

---

## 🔧 7. Red y Procesos

### **ps - Process Status**
```bash
ps [opciones]
```
- **Función**: Mostrar procesos en ejecución
- **Opciones útiles**:
  - `aux`: Todos los procesos
  - `ef`: Formato extendido

- **Ejemplos**:
  ```bash
  ps                               # Procesos del usuario actual
  ps aux                           # Todos los procesos
  ps ef                            # Procesos con árbol
  ```

### **kill**
```bash
kill [opciones] PID
```
- **Función**: Terminar procesos
- **Señales comunes**:
  - `-9`: Forzar terminación (SIGKILL)
  - `-15`: Terminación normal (SIGTERM)

- **Ejemplos**:
  ```bash
  kill 1234                        # Terminar proceso 1234
  kill -9 1234                     # Forzar terminación
  killall firefox                  # Terminar todos los firefox
  ```

### **top**
```bash
top
```
- **Función**: Monitor de procesos en tiempo real
- **Controles**:
  - `q`: Salir
  - `k`: Matar proceso
  - `M`: Ordenar por memoria

### **ping**
```bash
ping [opciones] host
```
- **Función**: Probar conectividad de red
- **Ejemplos**:
  ```bash
  ping google.com                  # Ping continuo
  ping -c 4 google.com             # 4 pings
  ```

---

## 📚 8. Comodines y Expansión

### **Comodines (Wildcards)**
- `*`: Cualquier número de caracteres
- `?`: Un solo carácter
- `[abc]`: Cualquier carácter entre corchetes
- `[a-z]`: Rango de caracteres

### **Ejemplos de comodines**:
```bash
ls *.txt                         # Todos los archivos .txt
ls archivo?.txt                  # archivo1.txt, archivoA.txt
ls [abc]*.txt                    # Archivos que empiezan con a, b o c
rm *.tmp                         # Eliminar todos los .tmp
cp *.jpg imagenes/               # Copiar todos los .jpg
```

---

## 🔧 9. Redirecciones y Tuberías

### **Redirecciones**
```bash
# Salida estándar
comando > archivo                # Escribir salida a archivo
comando >> archivo               # Añadir salida a archivo

# Entrada estándar
comando < archivo                # Usar archivo como entrada

# Error estándar
comando 2> error.log             # Redirigir errores
comando 2>&1                     # Combinar salida y error
```

### **Tuberías (Pipes)**
```bash
comando1 | comando2              # Salida de comando1 a entrada de comando2
```

### **Ejemplos**:
```bash
ls -l | grep ".txt"              # Listar y filtrar .txt
ps aux | grep firefox            # Buscar proceso firefox
cat archivo.txt | head -n 10     # Ver primeras 10 líneas
sort archivo.txt | uniq          # Ordenar y eliminar duplicados
```

---

## ⚡ 10. Alias y Personalización

### **alias**
```bash
alias [nombre='comando']
```
- **Función**: Crear comandos abreviados
- **Ejemplos**:
  ```bash
  alias ll='ls -la'                # Alias común
  alias ..='cd ..'                 # Atajo para subir directorio
  alias grep='grep --color=auto'   # Grep con colores
  ```

### **unalias**
```bash
unalias nombre
```
- **Función**: Eliminar alias
- **Ejemplo**:
  ```bash
  unalias ll                       # Eliminar alias ll
  ```

---

## 🎯 11. Comandos Útiles Adicionales

### **which**
```bash
which comando
```
- **Función**: Mostrar ruta del comando ejecutable
- **Ejemplo**:
  ```bash
  which ls                         # /bin/ls
  ```

### **history**
```bash
history [número]
```
- **Función**: Mostrar historial de comandos
- **Ejemplos**:
  ```bash
  history                          # Todo el historial
  history 10                       # Últimos 10 comandos
  ```

### **man - Manual**
```bash
man comando
```
- **Función**: Mostrar manual del comando
- **Ejemplo**:
  ```bash
  man ls                           # Manual de ls
  man 2 open                       # Manual sección 2 de open
  ```

### **clear**
```bash
clear
```
- **Función**: Limpiar la pantalla de la terminal

### **exit**
```bash
exit
```
- **Función**: Cerrar la sesión de terminal

---

## 📊 Tabla de Referencia Rápida

| Comando | Función Principal | Uso Básico |
|---------|------------------|------------|
| `pwd` | Mostrar directorio actual | `pwd` |
| `ls` | Listar archivos | `ls -la` |
| `cd` | Cambiar directorio | `cd /ruta` |
| `mkdir` | Crear directorio | `mkdir nombre` |
| `touch` | Crear archivo | `touch archivo.txt` |
| `cp` | Copiar archivos | `cp origen destino` |
| `mv` | Mover/Renombrar | `mv origen destino` |
| `rm` | Eliminar archivos | `rm archivo` |
| `cat` | Mostrar contenido | `cat archivo.txt` |
| `less` | Ver archivos grandes | `less archivo.log` |
| `grep` | Buscar patrones | `grep "texto" archivo` |
| `chmod` | Cambiar permisos | `chmod 755 archivo` |
| `ps` | Ver procesos | `ps aux` |
| `kill` | Terminar procesos | `kill PID` |
| `lsb_release -a` | Info distribución | `lsb_release -a` |
| `uname -a` | Info sistema | `uname -a` |
| `hostnamectl` | Info host | `hostnamectl` |

---

## 📋 Información Específica por Distribución

### **Ubuntu/Debian**
```bash
cat /etc/debian_version          # Versión de Debian base
cat /etc/lsb-release             # Información LSB específica
```

### **Red Hat/CentOS/Fedora**
```bash
cat /etc/redhat-release          # Información de Red Hat
rpm -q centos-release            # (CentOS)
rpm -q fedora-release            # (Fedora)
```

### **Arch Linux**
```bash
cat /etc/arch-release            # Confirmación de Arch Linux
```

### **SUSE/openSUSE**
```bash
cat /etc/SuSE-release            # Información de SUSE
```

---

## ⚠️ Precauciones Importantes

1. **rm -rf /**: ¡Nunca ejecutes este comando! Destruye todo el sistema.
2. **Permisos**: Verifica permisos antes de modificar archivos del sistema.
3. **Backup**: Haz copias de seguridad antes de operaciones críticas.
4. **Sudo**: Usa `sudo` solo cuando sea necesario y entiendas las consecuencias.

---

## 🎯 Conclusión

Dominar estos comandos básicos es fundamental para trabajar eficientemente en sistemas Linux. La práctica constante y la experimentación segura te ayudarán a volverte más cómodo con la terminal. Recuerda que siempre puedes usar `man comando` para obtener información detallada sobre cualquier comando.

---

**💡 Tip**: Combina comandos usando tuberías y redirecciones para crear flujos de trabajo potentes y automatizados. Los comandos de información del sistema (`uname`, `lsb_release`, `hostnamectl`) son especialmente útiles para diagnosticar problemas y verificar compatibilidad con software.
