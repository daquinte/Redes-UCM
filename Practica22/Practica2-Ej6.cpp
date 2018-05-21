#include <sys/socket.h>
#include <sys/types.h>

#include <netdb.h>
#include <string.h>
#include <iostream>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

//Para que compile hay que usar -pthread


class Thread{

public:
  Thread(int socket){ threadSocket = socket; salir = false;};
  virtual ~Thread(){};


  void do_message(){

    
    

    //Mientras el booleano salir sea falso se siguen mandando y recibiendo mensajes. Este solo se volverá true al darle a la Q
	while(!salir) {

		char buf[256];

		struct sockaddr src_addr;
		socklen_t addrlen = sizeof(src_addr); 

		char host [NI_MAXHOST];
		char serv [NI_MAXSERV];
		
		//Recibe el input(el mensaje) del cliente (nosotros)
		//s = nº de bytes
		ssize_t s = recvfrom(threadSocket, buf, 255, 0, /*IP y puerto del otro extremo (variable de salida)*/ &src_addr, /*argumento de entrada/salida*/&addrlen);
		
		//Inicializa en lo último que se ha recibido
		buf[s]='\0';
		
		
		//Si has recibido bytes, s no es cero
		if (s != 0){
                      getnameinfo(&src_addr, addrlen, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
		      std::cout << s << " bytes de " << host << ":" << serv << std::endl; //Bytes 
		}

		std::cout << "Thread: " << pthread_self() << "\n"; //ID del thread
	       

		int aux;

		struct tm* strTiempo;
		time_t tiempo;
		time (&tiempo);
		strTiempo = localtime(&tiempo); //Igualamos el struct del tiempo con el tiempo local

		//Revisa el buffer desde el inicio
		switch(buf[0]){

		//Si se pulsa la d se guarda el día, mes y año y se envia al cliente (src_addr)
		case 'd':			
		  aux = strftime(buf, 256 ,"Hoy es: %Y-%m-%d\n", strTiempo);
		  sendto(threadSocket, buf, aux, 0, (struct sockaddr *) &src_addr, addrlen);
		  break;

		//Si se pulsa t se guarda la hora y se le envía al cliente (src_addr)
		case 't':
		  aux = strftime(buf, 256 ,"Son las: %H:%M:%S\n", strTiempo);
		  sendto(threadSocket, buf, aux, 0, (struct sockaddr *) &src_addr, addrlen);		
		  break;

		  //Si se pulsa q termina el proceso del servidor
		case 'q':
		 salir = true;
		  std::cout << "Saliendo...\n";
		  break;

		  //Si se pulsa otra tecla muestra las instrucciones de lo que se debe pulsar
		default:
		  std::cout << "Pulsa T para mostrar la hora, D para mostrar el día o Q para salir " << buf[0]<< "\n";
		  break;
		 		
		}

		//std::cout << s << " bytes de " << host << ":" << serv;
   
		//Dejamos un tiempo de espera de 5 segundos para comprobar la concurrencia
		sleep(5);	  

	}
  }

private:
  int threadSocket; //Socket que usará cada thread
  bool salir; //Booleano que mantiene acivo o no el bucle principal
  

};

//Método que llama al envio de mensajes de cada thread al inicio de la rutina y lo pasa a C para que funcione
extern "C"
{
  void* start_routine(void* _thread){

    Thread* t = static_cast<Thread*>(_thread);
    t-> do_message();

    delete t;

    return 0;

  }
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

	//Inicializamos el conjunto de threads (5 en principio por ejemplo)
	int numThreads = 5;
	pthread_t threadID[numThreads]; //Conjunto de ids de cada thread para llamar a cada uno en el for

	for(int i = 0; i < numThreads; i++)
	  {
	    pthread_attr_t attr;

	    Thread* thread = new Thread(sd);

	    pthread_attr_init (&attr);
	    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	    pthread_create(&threadID[i], &attr, start_routine, static_cast<void*>(thread));


	  }

	char c;

	//Si se escribe q no lo guarda
	if (c != 'q')
	std::cin >> c;
	
	
	freeaddrinfo(res);
	

	return 0;
}
