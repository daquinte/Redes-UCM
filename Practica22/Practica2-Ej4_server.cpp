#include <sys/socket.h>
#include <sys/types.h>

#include <netdb.h>
#include <string.h>
#include <iostream>
#include <time.h>




//El argv por si solo no hace nada. Luego cuando lo ejecutes tienes que pasarle un parámetro que es el localhost y el puerto
//Ej: Redes 2 localhost [ip? puerto?]
int main (int argc, char **argv) {
	struct addrinfo hints;
	struct addrinfo *res;
	bool salir = false;

	memset((void*) &hints, '\0', sizeof(struct addrinfo));
	hints.ai_family = AF_INET;      //Porque es IPv4
	hints.ai_socktype = SOCK_DGRAM; // DGRAM siempre es UDP
	
	//argv[1] = direccion , argv[2] = puerto(host)
	//Res se rellena con los parametros de argv
	int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

	if (rc != 0) {
		std::cout << "error getaddrinfo(): " << gai_strerror(rc) << std::endl;
		return -1;
	}



	int sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	//BIND: Define la dirección en la que se va a escuchar
	bind (sd, res->ai_addr, res->ai_addrlen);

	//LISTEN: Pone el server socket en modo pasivo, esperando que el cliente se conecte
	//socket propio, y luego un int que son las conexiones pendientes en la cola del socket
	//Si llega un request y la cola está llena, da error de tipo "ECONNREFUSED"
	listen(sd, 5);
	
	//Mientras el booleano salir sea falso se siguen mandando y recibiendo mensajes. Este solo se volverá true al darle a la Q
	while(!salir) {

		char buf[256];
		struct sockaddr src_addr;
		socklen_t addrlen = sizeof(src_addr); 
		
		//ACCEPT: devuelve un socket nuevo que hace referencia a la conexión establecida (Es decir, tienes los dos sockets aqui)
		//Extrae la primera conexion de la cola de conexiones pendientes, y la convierte en socket ya conectado.
		//Es bloqueante, pero puedes usar select()
		//Aceptamos el siguiente en la cola
		int socketCliente = accept(sd, &src_addr, &addrlen);

		char host [NI_MAXHOST];
		char serv [NI_MAXSERV];
		
		//Recibe el input(el mensaje) del cliente (nosotros)
		//s = nº de bytes
		ssize_t s = recvfrom(socketCliente, buf, 255, 0, /*IP y puerto del otro extremo (variable de salida)*/ &src_addr, /*argumento de entrada/salida*/&addrlen);
		
		//Inicializa en lo último que se ha recibido
		buf[s]='\0';
		
	
		if (s == 0){
		  salir = true;
		}

		//Si queremos mostrar en número de bytes se deberá hacer dentro de este if escribiendo el valor de s
		//Si has recibido bytes, s no es cero
		getnameinfo(&src_addr, addrlen, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
		std::cout << s << " bytes de " << host << ":" << serv << std::endl;

		//Le paso el buf de nuevo, para que sea un eco.
	       sendto(sd, buf, 256, 0, (struct sockaddr *) &src_addr, addrlen);
   
	  
	}

	std::cout << "Conexión terminada" << std::endl;
	freeaddrinfo(res);


	return 0;
}
