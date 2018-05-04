 #include <stdio.h>
 #include <sys/types.h>
 #include <unistd.h>
 #include <errno.h>
 #include <iostream>
 #include <string.h>
 #include <iomanip>

int main() {
   /* Comprobar la ocurrencia de error y notificarlo con la llamada perror(3) */      
  int a = setuid(2); //Da -1 (fallo) siempre
 
 //Perror devuelve la cadena que pasas como parámetro y le añade el codigo de      error en forma de texto.
  if (a == -1){
    perror("Error 1"); //pone ":" por sí solo
    char* b =  strerror(errno); 
    std::cout <<"Error 2: "<< b << std::endl;
  }

    return 1;
}

