const int salidas[8] = {2, 3, 4, 5, 6, 7, 8, 9}; // Asocia los pines digitales con TX1 a TX8.
void setup() { // Configura el probador una vez.
  Serial.begin(9600); // Abre el monitor para indicar el pin transmitido.
  for (int i = 0; i < 8; i++) { // Recorre las ocho salidas.
    pinMode(salidas[i], OUTPUT); // Configura cada pin como salida.
    digitalWrite(salidas[i], LOW); // Inicia con todos los LEDs apagados.
  } // Termina la configuracion de salidas.
} // Termina la preparacion.
void loop() { // Repite continuamente la prueba del cable.
  for (int i = 0; i < 8; i++) { // Activa los conductores en orden del uno al ocho.
    Serial.print("TX "); // Identifica el extremo transmisor.
    Serial.println(i + 1); // Muestra el numero del conductor activo.
    digitalWrite(salidas[i], HIGH); // Envia cinco voltios por el conductor seleccionado.
    delay(700); // Mantiene encendido el LED receptor durante 0.7 segundos.
    digitalWrite(salidas[i], LOW); // Apaga el conductor antes de activar el siguiente.
    delay(200); // Deja una pausa visible entre pasos.
  } // Termina un barrido de los ocho conductores.
  Serial.println("Fin del ciclo"); // Separa las rondas de la prueba.
  delay(1000); // Hace una pausa antes de repetir.
} // Termina el ciclo del probador.
