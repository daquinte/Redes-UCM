 #include <time.h>
 #include <iostream>

int main( ) {
  time_t tiempo = 2;        //Lo usamos para sacar el tiempo en segundos
  time_t tiempoLegible = 3; //Sacamos el tiempo en formato legible
  std::cout << "Tiempo en segundos: " << time(&tiempo) << std::endl
	    << "Tiempo en formato legible: "<< ctime(&tiempoLegible)  
	    << std::endl;

  //NOTA: Si se pone el número de segundos en el parámetro de ctime, saldría la           fecha de hoy.

    return 1;
}
