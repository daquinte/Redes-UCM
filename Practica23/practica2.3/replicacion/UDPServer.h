#ifndef UDPSERVER_H_
#define UDPSERVER_H_

#include <pthread.h>
#include <vector>

#include "Socket.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// Esta es una función envoltorio que ejecuta el código de thread servidor
// static_cast<UDPServer *>(arg)->server_thread();
extern "C" void * _server_thread(void *arg);

/**
 *  Esta clase representa un servidor UDP que usa el modelo pre-fork. Cuando
 *  se inicia UDPServer::start() se escucha en el puerto indicado y se
 *  crean THREAD_POOL_SIZE threads. Cada thread se encargará de gestionar los
 *  mensajes recibidos.
 */
class UDPServer
{
public:
    /**
     *  El constructor inicializa el socket del servidor con los parámetros
     *  dados:
     *    @param serv, IP/FQDN del servidor
     *    @param port, puerto
     */
    UDPServer(const char * serv, const char * port);

    virtual ~UDPServer();

    /**
     * Crea un pool de threads, cada uno tratará un mensaje
     *    @return 0 en caso de éxito o -1 si error
     */
    int start();

    //--------------------------------------------------------------------------
    //  Intefaz UDPServer que debe implementar cada servidor
    //--------------------------------------------------------------------------
    /**
     *  Este método se ejecuta cada vez que se recibe un mensaje
     *    @param buffer, el objeto recibido resultado de Socket::recv. Debe
     *    reconstruirse con Serializable::from_bin
     */
    virtual void do_message(char * buffer) = 0;

protected:
    /**
     *  Lista de conexiones. Cada conexión cliente es un Socket. El mutex
     *  controla el acceso concurrente a la estructura.
     */
    pthread_mutex_t mutex;

    std::vector<Socket *> connections;

    /**
     *  Representa el socket del servidor, en el que se reciben los mensajes
     */
    Socket socket;

private:
    friend void * _server_thread(void * arg);

    /**
     *  Número de threads del servidor
     */
    static const int THREAD_POOL_SIZE = 5;

    /**
     *  Lógica principal del thread servidor
     *    1. Permanece en un bucle
     *    2. Recibe el mensaje en un buffer Socket::recv, y que crea un nuevo
     *    Socket para la conexion cliente.
     *    3. Añade el cliente a la lista de clientes.
     *    4. Llama a UDPServer::do_message para tratar el mensaje
     */
    void server_thread();

    /**
     * Añade el socket a la lista de conexiones si no está incluida, en caso
     * contrario elimina el objeto. Esta función debe bloquear el mutex.
     */
    void add_connection(Socket * sock);

    /**
     *  Borrar la conexión. Esta función debe bloquear el mutex.
     */
    void del_connection(Socket * sock);
};

#endif /* SERVER_H_ */
