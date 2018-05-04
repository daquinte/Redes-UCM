
 #include <iostream>
 #include <time.h>
#include <math.h>

int main() {
   timespec tiempoStruct;
   long  incr = 0;           //Variable acumuladora del tiempo
   clockid_t reloj = 2;      //ID que le pasamos al getime
   for(int i = 0; i < 10000; i++){
     //Tomamos el tiempo
      clock_gettime(reloj, &tiempoStruct);
     //Lo sumamos a la variable de incremento, separando segundos y nanosegundos
       incr += tiempoStruct.tv_nsec;
       incr +=(long)pow(tiempoStruct.tv_sec,3);
   }
   //Lo sacamos por pantalla
    std::cout << incr<< std::endl;
   return 0;
}
