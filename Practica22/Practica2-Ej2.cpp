#include <sys/socket.h>
#include <sys/types.h>

#include <netdb.h>
#include <string.h>
#include <iostream>
#include <time.h>

//int socket(int domain, int type, int protocol);

//Tenemos que separar esto en dos metodos independientes y tal
void DameHora(){
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



}



//El argv por si solo no hace nada. Luego cuando lo ejecutes tienes que pasarle un parámetro que es el localhost y el puerto
//Ej: Redes 2 localhost [ip? puerto?]
int main (int argc, char **argv) {
	struct addrinfo hints;
	struct addrinfo *res;

	memset((void*) &hints, '\0', sizeof(struct addrinfo));
	hints.ai_family = AF_INET;      //Porque es IPv4
	hints.ai_socktype = SOCK_DGRAM; // DGRAM siempre es UDP
	
	//argv[1] = direccion , argv[2] = puerto(host)
	int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

	if (rc != 0) {
		std::cout << "error getaddrinfo(): " << gai_strerror(rc) << std::endl;
		return -1;
	}



	int sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	bind (sd, res->ai_addr, res->ai_addrlen);

	freeaddrinfo(res);
	
	
	while(true) {

		char buf[256];
		struct sockaddr src_addr;
		socklen_t addrlen;

		char host [NI_MAXHOST];
		char serv [NI_MAXSERV];
		
		//Recibe el input(el mensaje) del cliente (nosotros)
		ssize_t s = recvfrom(sd, buf, 255, 0, /*IP y puerto del otro extremo (variable de salida)*/ &src_addr, /*argumento de entrada/salida*/&addrlen);
		
		getnameinfo(&src_addr, addrlen, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
		
	
		  if (buf[0] ==  't')
		    //llamar a DameHora
		    std::cout << "Holi te doy la hora" << std::endl;
		  	  
		  else  if (buf[0] ==  'd')
		    std::cout << "Holi te doy un dia" << std::endl;

	
		  buf[s] = '\0';

		  std::cout <<"Msj: " << buf << std::endl;
		

		//sendto(/*En este caso solo hay un canal*/ sd, buf, s, 0, &src_addr, addrlen);

	}

	return 0;
}
