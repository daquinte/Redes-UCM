
#include <time.h>
#include <math.h>
#include <locale.h>
#include <string>
#include <iostream>

int main ()
{
  time_t tiempo;
  struct tm * strTiempo;
  char buffer [80];         //string donde se almacena la cadena del strftime

  time (&tiempo);
  strTiempo = localtime (&tiempo); //Tiempo local
  //Año
  strftime (buffer,80,"Estamos en el año %Y.",strTiempo);
  std::cout << buffer << std::endl;
  //Día y hora
  setlocale(LC_ALL, "es_ES.utf8");
  strftime (buffer,80,"Hoy es %A, son las %H:%M%p",strTiempo);
  std::cout << buffer << std::endl;

  return 0;

}

