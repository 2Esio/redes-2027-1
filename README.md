# Redes de Computadoras · 2027-1

**Marco Flores Cid · Facultad de Ciencias, UNAM**

Práctica 1: capa física, emulación de un bit-flip en **C** y probador de cable RJ-45 en protoboard digital.

## Acceso para el ayudante

Repositorio público: **https://github.com/2Esio/redes-2027-1**

Se puede consultar y clonar sin cuenta de GitHub ni contraseñas. El programa de consola usa únicamente la biblioteca estándar de C.

### 1. Instalar las herramientas

En Kali, Debian o Ubuntu, si todavía no están instaladas:

```bash
sudo apt update
sudo apt install gcc git
```

### 2. Clonar y entrar

```bash
git clone https://github.com/2Esio/redes-2027-1.git
cd redes-2027-1/practica1
```

### 3. Compilar

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic bit_flip.c -o bit_flip
```

### 4. Ejecutar

```bash
./bit_flip
```

Escribir una palabra, por ejemplo `hola`, y presionar Enter. También se puede ejecutar de una sola vez:

```bash
printf 'hola\n' | ./bit_flip
```

En el servidor utilizado para la práctica, la carpeta de trabajo es:

```bash
cd ~/Documents/Ciencias/2027-1/Redes/practica1
gcc -std=c11 -Wall -Wextra -Wpedantic bit_flip.c -o bit_flip
./bit_flip
```

No hace falta entrar al servidor para revisar la práctica: cualquier computadora con GCC puede compilar el programa después de clonar el repositorio.

## Archivos

```text
practica1/
├── bit_flip.c                   Programa en C, comentado línea por línea
├── RESULTADOS.md                Investigación, resultados y conclusiones
├── evidencias/
│   └── ejecucion-hola.txt        Salida real del programa en Kali
└── protoboard/
    ├── README.md                Montaje y pasos para simular
    ├── directo/
    │   ├── sketch.ino           Barrido de los ocho conductores
    │   └── diagram.json         Cable directo y componentes
    └── cruzado/
        ├── sketch.ino           El mismo barrido, sin cambiar el orden
        └── diagram.json         Cable cruzado y componentes
```

## Qué hace el programa

1. Lee una palabra ASCII visible de entre 1 y 100 caracteres, sin espacios ni acentos.
2. Recorre cada byte desde el bit de mayor peso hasta el de menor peso.
3. Muestra cada bit y llama a `transmitir_bit_nrz`: `1` se imprime como `[ +5V ]` y `0` como `[ -5V ]`.
4. Elige una posición entre los `8 * longitud` bits e invierte exactamente uno.
5. Imprime los nuevos bits, los nuevos voltajes y el mensaje recibido.

### Cómo se cambia un bit

```c
int posicion = rand() % (n * 8);
int indice = posicion / 8;
int bit = 7 - posicion % 8;
recibido[indice] ^= (1u << bit);
```

`posicion` cuenta desde cero sobre toda la cadena de bits. La división entre ocho obtiene el byte; el residuo indica la posición dentro de ese byte. `1u << bit` construye una máscara con un único uno. XOR con esa máscara invierte únicamente el bit seleccionado: `0 ^ 1 = 1` y `1 ^ 1 = 0`.

Ejemplo: `h` vale 104 y sus bits son `01101000`. Al invertir el bit de peso 32 se obtiene `01001000`, que vale 72 y representa `H`.

El generador se inicializa con `srand((unsigned int)time(NULL))`. Dos ejecuciones en el mismo segundo pueden escoger la misma posición. `rand() % total` es una selección sencilla y puede tener un pequeño sesgo; no es un generador criptográfico ni un modelo estadístico preciso del ruido. La simulación fuerza **un error por mensaje**; no pide una probabilidad al usuario.

El trabajo de recorrer e imprimir el mensaje es proporcional a su longitud: **O(n)**. La inversión del bit requiere una sola operación XOR.

### Caracteres recibidos que no se pueden imprimir

Un bit-flip puede producir un byte cero, un carácter de control o un valor mayor que 127. El programa muestra esos bytes como `\xHH`, donde `HH` es su valor hexadecimal. Recorre el mensaje por su longitud, por lo que un byte cero no corta la salida. Los acentos y emojis de entrada se rechazan para mantener el ejercicio en ASCII de un byte por carácter.

Los voltajes son una representación impresa del modelo NRZ de la práctica: el programa de C no genera voltajes en una interfaz de red ni transmite paquetes.

## Ejemplo real y validación

En la ejecución conservada en [ejecucion-hola.txt](practica1/evidencias/ejecucion-hola.txt), el resultado fue:

```text
Texto transmitido: hola
Bit alterado: 3 de 32 (byte 1, peso 32)
Texto recibido: Hola
Bits alterados: 1 de 32
```

La posición y el texto recibido pueden cambiar en otra ejecución. Lo que debe mantenerse es que cambie **exactamente un bit**.

Se compiló en Kali con `-std=c11 -Wall -Wextra -Wpedantic -Werror`. Se verificaron seis entradas válidas, incluida una palabra de cien caracteres, y siete casos inválidos, incluido el fin de entrada. En cada entrada válida se compararon los bits, los valores de los bytes, la posición anunciada y los voltajes. Las 67 líneas del archivo C tienen comentarios.

## Protoboard digital

**Simulaciones listas para abrir:** [cable directo](https://wokwi.com/projects/475205082762594305) · [cable cruzado](https://wokwi.com/projects/475205144008883201). Pulsar el botón verde para iniciar.

Los dos circuitos editables están en [practica1/protoboard](practica1/protoboard/README.md). Se simulan en **Wokwi**, una de las alternativas permitidas por la práctica. El programa principal solicitado está escrito en C; el pequeño `sketch.ino` usa la API de Arduino para generar el barrido del circuito.

| Cable | Orden de TX | Orden esperado de los LEDs RX |
|---|---|---|
| Directo, T568B–T568B | 1, 2, 3, 4, 5, 6, 7, 8 | 1, 2, 3, 4, 5, 6, 7, 8 |
| Cruzado, T568B–T568A | 1, 2, 3, 4, 5, 6, 7, 8 | 3, 6, 1, 4, 5, 2, 7, 8 |

La secuencia transmitida es igual en ambos casos. El cambio de orden aparece por las conexiones del cable virtual.

## Resultados, conclusiones y referencias

Consultar [RESULTADOS.md](practica1/RESULTADOS.md). La práctica separa dos ideas: una alteración del valor de un bit y el mapeo físico de los conductores de un cable.
