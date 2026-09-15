# Redes de Computadoras · 2027-1

**Marco Flores Cid · Facultad de Ciencias, UNAM**

Práctica 1: capa física, emulación de un bit-flip en **C** y probador de cable RJ-45 en protoboard digital.


### 1. Instalar las herramientas

En linux si todavía no están instaladas:

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


## Protoboard digital

**Simulaciones :** [cable directo](https://wokwi.com/projects/475205082762594305) · [cable cruzado](https://wokwi.com/projects/475205144008883201). 

