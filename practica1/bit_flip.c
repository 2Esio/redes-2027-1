#include <stdio.h> // Permite leer la palabra e imprimir resultados.
#include <stdlib.h> // Proporciona rand y srand para elegir el bit.
#include <string.h> // Proporciona strlen y memcpy para manejar los bytes.
#include <time.h> // Proporciona la hora usada como semilla.
void transmitir_bit_nrz(int bit) { // Representa un bit mediante el voltaje pedido.
    if (bit == 1) { // Revisa si el bit vale uno.
        printf("[ +5V ] "); // Imprime el nivel alto para uno.
    } else { // Atiende el caso en que el bit vale cero.
        printf("[ -5V ] "); // Imprime el nivel bajo para cero.
    } // Termina la decision del voltaje.
} // Termina la funcion de la practica.
void mostrar(unsigned char palabra[], int n) { // Muestra cada byte en bits y voltajes.
    for (int i = 0; i < n; i++) { // Recorre los bytes de la palabra.
        printf("Byte %d (%u)\nBits: ", i + 1, (unsigned int)palabra[i]); // Indica la posicion y el valor decimal.
        for (int j = 7; j >= 0; j--) { // Recorre los ocho bits del mayor al menor.
            printf("%d", (palabra[i] >> j) & 1); // Desplaza el byte y obtiene el bit con AND.
        } // Termina la impresion de bits.
        printf("\nSenal: "); // Introduce los voltajes del mismo byte.
        for (int j = 7; j >= 0; j--) { // Recorre nuevamente los ocho bits.
            transmitir_bit_nrz((palabra[i] >> j) & 1); // Usa la funcion solicitada para cada bit.
        } // Termina la impresion de voltajes.
        printf("\n"); // Separa el siguiente byte.
    } // Termina el recorrido de la palabra.
} // Termina la funcion que muestra la transmision.
int main(void) { // Comienza el programa.
    unsigned char original[102], recibido[102]; // Reserva espacio para hasta cien caracteres y el final de linea.
    printf("Introduce una palabra ASCII de 1 a 100 caracteres, sin espacios: "); // Solicita una entrada sencilla.
    if (fgets((char *)original, sizeof original, stdin) == NULL) { // Lee una linea y comprueba que exista entrada.
        fprintf(stderr, "Error: no se recibio una palabra.\n"); // Explica el error de lectura.
        return 1; // Termina indicando entrada incorrecta.
    } // Termina la comprobacion de lectura.
    int n = (int)strlen((char *)original); // Cuenta los bytes leidos.
    if (n > 0 && original[n - 1] == '\n') { // Comprueba si fgets incluyo el salto de linea.
        original[--n] = '\0'; // Quita el salto de linea y actualiza la longitud.
    } // Termina la eliminacion del salto.
    if (n < 1 || n > 100) { // Rechaza entradas vacias o demasiado largas.
        fprintf(stderr, "Error: usa entre 1 y 100 caracteres.\n"); // Explica el limite de entrada.
        return 1; // Termina para evitar operar sobre una entrada invalida.
    } // Termina la validacion de longitud.
    for (int i = 0; i < n; i++) { // Revisa cada byte de la entrada.
        if (original[i] < 33 || original[i] > 126) { // Rechaza espacios, controles y caracteres fuera de ASCII visible.
            fprintf(stderr, "Error: usa ASCII visible, sin espacios ni acentos.\n"); // Explica el formato aceptado.
            return 1; // Termina ante una palabra no admitida.
        } // Termina la comprobacion del caracter.
    } // Termina la validacion de la palabra.
    memcpy(recibido, original, n); // Copia los bytes para conservar el mensaje original.
    srand((unsigned int)time(NULL)); // Inicializa el generador con el tiempo Epoch en segundos.
    int posicion = rand() % (n * 8); // Elige una posicion entre todos los bits del mensaje.
    int indice = posicion / 8; // Obtiene el byte al que pertenece el bit elegido.
    int bit = 7 - posicion % 8; // Traduce la posicion al desplazamiento dentro del byte.
    recibido[indice] ^= (1u << bit); // Invierte solo ese bit mediante XOR con una mascara.
    printf("\nTexto transmitido: %s\n\nORIGINAL\n", (char *)original); // Muestra la palabra sin alterar.
    mostrar(original, n); // Imprime los bits originales y su representacion NRZ.
    printf("\nBit alterado: %d de %d (byte %d, peso %u)\n", posicion + 1, n * 8, indice + 1, 1u << bit); // Identifica el cambio contando desde uno.
    printf("\nRECIBIDO\n"); // Introduce los datos modificados.
    mostrar(recibido, n); // Imprime los nuevos bits y los nuevos voltajes.
    printf("\nTexto recibido: "); // Introduce la reconstruccion del mensaje.
    for (int i = 0; i < n; i++) { // Recorre por longitud para incluir incluso un byte cero.
        if (recibido[i] >= 32 && recibido[i] <= 126) { // Comprueba si el byte es ASCII imprimible.
            printf("%c", recibido[i]); // Imprime el caracter visible.
        } else { // Atiende bytes que no se pueden mostrar como letras.
            printf("\\x%02X", (unsigned int)recibido[i]); // Representa el byte en hexadecimal sin perder informacion.
        } // Termina la seleccion de representacion.
    } // Termina la reconstruccion del texto.
    printf("\nBits alterados: 1 de %d\n", n * 8); // Resume la unica inversion realizada.
    return 0; // Termina correctamente.
} // Termina el programa.
