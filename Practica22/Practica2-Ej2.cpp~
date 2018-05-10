#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <iostream>

//int socket(int domain, int type, int protocol);

//El argv por si solo no hace nada. Luego cuando lo ejecutes tienes que pasarle un parámetro que es el localhost y el puerto
//Ej: Redes 2 localhost [ip? puerto?]
int main(int argc, char **argv){

  struct addrinfo hints;   //Sin puntero porque lo vamos a inicializar, y no queremos memoria dinamica
  struct addrinfo *res;    //Aquí si nos interesa

  memset((void*) &hints, '\0', sizeof(struct addrinfo)); //Inicializa a 0 el struct enterito
  hints.ai_family = AF_INET;                             //Podría ser AF_UNSPEC, pero ponemos AF_INET para IPv4
  hints.ai_socktype = SOCK_DGRAM;                        // DGRAM siempre es UDP

  //Recibe: const char * Nodo, const char * service, addrinfo *hints, addrinfo**res 
  //nodo = Host de internet, service = servicio
  //Se genera una lista enlazada de structs, y a su vez se inicializa el puntero a res
  int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

  if (rc != 0) {
    std::cout << "error getaddrinfo(): " << gai_strerror(rc) << std::endl;
    return -1;
  }
  
  // Esto es para las conexiones de mensajes y demás. Realmente no lo necesitamos ahora
  //int sd = socket(res->ai_family ,res->ai_socktype, 0);
  //bind (sd, res->ai_addr, res->ai_addrlen);
  //-----------------------------------------------------------------

  //Creamos otro addrinfo al que damos el valor de res para no modificar este, y vamos sacando los nombres
  for(struct addrinfo* tmp = res; tmp !=0; tmp = tmp->ai_next){
        //Máximas longitudes de host y serv(que ya te da el SO), y arrays de chars
        char host[NI_MAXHOST];
	char serv [NI_MAXSERV];
	getnameinfo(tmp->ai_addr, tmp->ai_addrlen, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV); 
	  
	std::cout << host << "   " << tmp->ai_family << "   " << tmp->ai_socktype << std::endl;
		
    
}
  //Eliminamos memoria dinamica
  freeaddrinfo(res);

  return 0;

}

