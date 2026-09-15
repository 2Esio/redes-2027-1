# Práctica 1 · Capa física

**Marco Flores Cid · Redes de Computadoras · Semestre 2027-1**

## 1. ¿Qué es un bit-flip?

Un bit-flip ocurre cuando un bit cambia de 0 a 1 o de 1 a 0. En una transmisión, el emisor envía señales físicas y el receptor interpreta sus niveles o cambios para recuperar los bits. Si la señal se distorsiona, el receptor puede tomar una decisión incorrecta y recuperar el valor contrario.

El ruido, la interferencia y la degradación de la señal pueden producir errores. Un cambio en un solo bit puede modificar una letra, un número o una instrucción. También pueden aparecer varios errores en una ráfaga. En el programa se representa el efecto de un error aislado mediante XOR; no se simula la forma analógica del ruido.

Cisco explica el bit-flip como una inversión 1→0 o 0→1 y describe cómo el ruido y la distorsión pueden provocar una lectura incorrecta. También presenta la corrección de errores mediante redundancia: [Understanding FEC and Its Implementation in Cisco Optics](https://www.cisco.com/c/en/us/products/collateral/interfaces-modules/transceiver-modules/implementation-optics-wp.html).

## 2. Desarrollo en C

Se usaron arreglos, ciclos, desplazamientos y la operación XOR. Los bytes se manejan como `unsigned char` para evitar que valores altos se interpreten como números negativos. Cada byte se muestra con ocho bits, empezando por el más significativo.

La función `transmitir_bit_nrz` representa el uno con +5 V y el cero con -5 V. El voltaje se imprime como texto. Para conservar la entrada, se hace una copia y se altera un único bit de esa copia.

La posición se elige con `rand() % (n * 8)`, usando el tiempo Epoch como semilla. La posición interna cuenta desde cero y la posición presentada al usuario cuenta desde uno. Una máscara con un solo bit encendido permite invertir exactamente ese bit mediante XOR.

## 3. Resultado del programa

Se compiló y ejecutó el programa en Kali Linux. La [salida completa guardada](evidencias/ejecucion-hola.txt) contiene tanto los bits como los voltajes originales y recibidos.

| Dato | Resultado observado |
|---|---|
| Entrada | `hola` |
| Bits originales | `01101000 01101111 01101100 01100001` |
| Posición alterada, contando desde 1 | 3 de 32 |
| Byte afectado | Primero: `h`, valor 104 |
| Máscara aplicada | `00100000`, valor 32 |
| Bits recibidos | `01001000 01101111 01101100 01100001` |
| Salida | `Hola` |

El tercer bit de la primera letra pasó de 1 a 0. Su representación pasó de `[ +5V ]` a `[ -5V ]`. Los otros 31 bits conservaron su valor. Esto muestra que una alteración pequeña en la señal puede cambiar el contenido recuperado.

En esta ejecución hay un bit alterado entre 32: la proporción observada es `1/32 = 0.03125`, o 3.125 %. Este valor describe el mensaje simulado y no una tasa medida de una red real. Como el programa siempre fuerza un solo error, una palabra de otra longitud tendrá otra proporción.

## 4. Validación

La compilación con `gcc -std=c11 -Wall -Wextra -Wpedantic -Werror` terminó correctamente. Se comprobaron trece casos:

- Seis válidos: `A`, `hola`, `redes`, `0`, `~` y cien letras `a`.
- Siete inválidos: línea vacía, espacio, palabra con espacios, 101 caracteres, caracteres no ASCII, tabulador y fin de entrada sin texto.

En las entradas válidas se verificó que el número de bits cambiados fuera exactamente uno, que coincidiera con la posición anunciada, que los valores decimales correspondieran a los bits y que cada señal NRZ correspondiera al valor impreso. Las entradas inválidas terminaron con código de error 1. Las 67 líneas del programa tienen comentario.

La representación `\xHH` conserva los bytes no imprimibles del mensaje modificado. Esta decisión evita perder información cuando la alteración produce un byte cero o un carácter de control.

## 5. Probador RJ-45

Se prepararon dos circuitos de Wokwi con protoboard, Arduino Uno, ocho resistencias y ocho LEDs receptores. El Arduino activa TX1 a TX8 en orden. El cableado determina qué LED RX recibe cada pulso.

| Paso | TX activo | RX directo | RX cruzado |
|---|---|---|---|
| 1 | 1 | 1 | 3 |
| 2 | 2 | 2 | 6 |
| 3 | 3 | 3 | 1 |
| 4 | 4 | 4 | 4 |
| 5 | 5 | 5 | 5 |
| 6 | 6 | 6 | 2 |
| 7 | 7 | 7 | 7 |
| 8 | 8 | 8 | 8 |

El directo mantiene la correspondencia pin a pin. El cruzado T568B–T568A intercambia 1 con 3 y 2 con 6; los contactos 4, 5, 7 y 8 conservan su posición. Esta correspondencia se documenta en [Digi International: Pin Outs For A Crossover Ethernet Cable](https://www.digi.com/support/knowledge-base/pin-out-for-a-crossover-ethernet-cable).

Los pasos para repetir la simulación están en [protoboard/README.md](protoboard/README.md). El montaje usa una referencia GND común del banco y contactos numerados para modelar los RJ-45. Su alcance es comprobar el orden y continuidad, sin certificar características de un enlace Ethernet.

Ambos proyectos se compilaron y ejecutaron en Wokwi. En el directo se observó TX7 con RX7 encendido. En el cruzado se observó TX6 con RX2 encendido, a los 4.980 segundos de la simulación. El monitor mostró barridos de TX1 a TX8 y el analizador registró transiciones en los canales receptores. Los proyectos publicados permiten repetir la observación: [directo](https://wokwi.com/projects/475205082762594305) y [cruzado](https://wokwi.com/projects/475205144008883201).

## 6. Conclusiones

1. Una misma palabra se puede representar como caracteres, bytes, bits y señales. Estas representaciones permiten observar qué parte cambia durante una transmisión con error.
2. Invertir un único bit puede cambiar el mensaje recibido, aunque los demás bits lleguen correctamente. En el ejemplo, `hola` se convirtió en `Hola`.
3. XOR con una máscara de un solo bit permite representar un error aislado de forma sencilla y comprobable. El experimento fuerza ese error y no estima la probabilidad real del ruido de una red.
4. El probador de cable permite relacionar cada pin transmisor con un pin receptor. El orden de los LEDs depende del cableado, aunque el generador use siempre la misma secuencia.
5. Git permite conservar los archivos y GitHub facilita que el ayudante clone, compile y revise el mismo programa. Las instrucciones de ejecución y las evidencias permiten repetir la revisión.
