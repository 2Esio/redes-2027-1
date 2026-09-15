# Probador RJ-45 en protoboard digital

## Archivos y ejecución

### Abrir directamente los circuitos publicados

- [Simular cable directo](https://wokwi.com/projects/475205082762594305).
- [Simular cable cruzado](https://wokwi.com/projects/475205144008883201).

Abrir el enlace y pulsar **Start the simulation**. Los enlaces están guardados como proyectos públicos; los archivos del repositorio permiten recrearlos y editarlos.

### Recrear desde los archivos

Cada carpeta contiene un proyecto de Wokwi completo: `sketch.ino` y `diagram.json`. Los dos sketches son iguales; el cableado cambia en los diagramas.

1. Abrir [un proyecto nuevo de Arduino Uno en Wokwi](https://wokwi.com/projects/new/arduino-uno).
2. Abrir `directo/sketch.ino` de este repositorio, copiar todo y reemplazar el contenido de la pestaña `sketch.ino` en Wokwi.
3. Copiar todo `directo/diagram.json` y reemplazar el contenido de la pestaña `diagram.json`.
4. Pulsar el botón verde **Start the simulation**. La primera compilación puede tener cola de espera.
5. Enfocar el área del circuito y pulsar **F** para ajustar el zoom. El monitor serie muestra el pin TX activado.
6. Observar los LEDs RX durante un ciclo completo: deben encenderse **1, 2, 3, 4, 5, 6, 7, 8**.
7. Detener la simulación. Para probar el cable cruzado, reemplazar `diagram.json` con el de `cruzado/diagram.json` y volver a iniciar. El sketch no necesita cambios.
8. Ahora los LEDs deben encenderse **3, 6, 1, 4, 5, 2, 7, 8**.

Cada pulso dura 700 ms, seguido de 200 ms de apagado. Al terminar los ocho conductores hay una pausa adicional de un segundo. Un ciclo completo dura aproximadamente 8.2 segundos.

## Componentes

- Un Arduino Uno como generador de la secuencia.
- Una protoboard digital completa.
- Ocho LEDs verdes, identificados como RX1 a RX8.
- Ocho resistencias de 220 ohms, una en serie con cada LED.
- Ocho conductores que representan el cable RJ-45.
- Puentes para alimentación de las señales y referencia GND común.
- Un analizador lógico de ocho canales para registrar las señales recibidas.

## Conexiones

Los contactos superiores de la protoboard representan el extremo TX y los inferiores representan RX. Los conectores RJ-45 se modelan por sus ocho contactos numerados; el simulador no incluye una carcasa RJ-45 en este montaje.

| Pin del cable | Salida Arduino | Columna de protoboard | LED receptor |
|---|---|---|---|
| 1 | D2 | 4 | RX1 |
| 2 | D3 | 11 | RX2 |
| 3 | D4 | 18 | RX3 |
| 4 | D5 | 25 | RX4 |
| 5 | D6 | 32 | RX5 |
| 6 | D7 | 39 | RX6 |
| 7 | D8 | 46 | RX7 |
| 8 | D9 | 53 | RX8 |

Por ejemplo, D2 llega al contacto superior de la columna 4. En el cable directo, un puente lo conecta con el contacto inferior de la columna 4 y enciende RX1. En el cruzado, ese puente termina en la columna 18 y enciende RX3.

Cada contacto RX llega a su resistencia de 220 ohms, después al ánodo del LED. El cátodo del LED regresa al riel negativo, conectado a GND del Arduino. Los grupos superior e inferior de la protoboard están separados por la ranura central: los ocho puentes de esa ranura son el cable que se prueba. El analizador toma las señales RX antes de las resistencias.

### Mapa de conductores

| TX | RX directo | RX cruzado | Color T568B en el extremo TX |
|---|---|---|---|
| 1 | 1 | 3 | Blanco/naranja |
| 2 | 2 | 6 | Naranja |
| 3 | 3 | 1 | Blanco/verde |
| 4 | 4 | 4 | Azul |
| 5 | 5 | 5 | Blanco/azul |
| 6 | 6 | 2 | Verde |
| 7 | 7 | 7 | Blanco/café |
| 8 | 8 | 8 | Café |

Los cables dibujados usan naranja, verde, azul y café; la numeración de los contactos distingue los hilos del mismo color. Se representa el cruce clásico de los pares 1–2 y 3–6 (10/100BASE-T), no el cruce de los cuatro pares.

## Registro de señales

El analizador lógico conecta D0 a RX1, D1 a RX2, y así hasta D7 a RX8. Al detener la simulación, Wokwi descarga `rj45-directo.vcd` o `rj45-cruzado.vcd`. Puede abrirse con GTKWave o PulseView para ver el orden de los pulsos. Cada canal debe tener un pulso por ronda y solo un canal debe estar activo a la vez.

## Alcance del circuito

El circuito comprueba de forma visual la continuidad y el orden de los ocho conductores. Usa 0 V y +5 V del Arduino para encender LEDs. Es una parte separada del programa de consola, que imprime el modelo NRZ de -5 V y +5 V solicitado.

La referencia GND común es un puente del banco de simulación, adicional a los ocho conductores modelados. Por ello este diseño demuestra el mapeo de los pines en un banco compartido; no reproduce la electrónica de retorno de todos los probadores comerciales con módulo remoto. Tampoco certifica categoría del cable, velocidad Ethernet, diafonía o longitud.

## Referencias

- [Editor de circuitos de Wokwi](https://docs.wokwi.com/guides/diagram-editor).
- [Formato diagram.json](https://docs.wokwi.com/diagram-format).
- [Guía del analizador lógico](https://docs.wokwi.com/guides/logic-analyzer).
- [Digi: conexiones de cable cruzado](https://www.digi.com/support/knowledge-base/pin-out-for-a-crossover-ethernet-cable).
