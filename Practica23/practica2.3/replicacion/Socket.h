#ifndef SOCKET_H_
#define SOCKET_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

#include <iostream>
#include <stdexcept>

#include <ostream>

// -----------------------------------------------------------------------------
// Definiciones adelantadas
// -----------------------------------------------------------------------------

class Serializable;
class Socket;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
 *  Esta función compara dos Socks, realizando la comparación de las structuras
 *  sockaddr: familia (INET), dirección y puerto, ver ip(7) para comparar
 *  estructuras sockaddr_in. Deben comparar el tipo (sin_family), dirección
 *  (sin_addr.s_addr) y puerto (sin_port). La comparación de los campos puede
 *  realizarse con el operador == de los tipos básicos asociados.
 */
bool operator== (const Socket &s1, const Socket &s2);

/**
 *  Imprime la dirección y puerto en número con el formato:"dirección_ip:puerto"
 */
std::ostream& operator<<(std::ostream& os, const Socket& dt);

/**
 * Clase base que representa el extremo local de una conexión UDP. Tiene la lógica
 * para inicializar un sockect y la descripción binaria del extremo
 *   - dirección IP
 *   - puerto
 */
class Socket
{
public:
    /**
     * El máximo teórico de un mensaje UDP es 2^16, del que hay que
     * descontar la cabecera UDP e IP (con las posibles opciones). Se debe
     * utilizar esta constante para definir buffers de recepción.
     */
    static const int32_t MAX_MESSAGE_SIZE = 32768;

    /**
     *  Construye el socket UDP con la dirección y puerto dados. Esta función
     *  usara getaddrinfo para obtener la representación binaria de dirección y
     *  puerto.
     *
     *  Además abrirá el canal de comunicación con la llamada socket(2).
     *
     *    @param address cadena que representa la dirección o nombre
     *    @param port cadena que representa el puerto o nombre del servicio
     *
     *  En caso de error lanzar una excepcion std::runtime_error
     */
    Socket(const char * address, const char * port);

    /**
     *  Inicializa un Socket copiando los parámetros del socket
     */
    Socket(struct sockaddr * _sa, socklen_t _sa_len)
        :sd(-1), sa(*_sa), sa_len(_sa_len){};

    virtual ~Socket(){};

    /**
     *  Recibe un mensaje de aplicación
     *
     *    @param obj buffer para recibir los datos de la red. Se usará para la
     *    reconstrucción del objeto mediante Serializable::from_bin del interfaz.
     *    El buffer debe ser de tamaño MAX_MESSAGE_SIZE.
     *
     *    @param sock que identificará al extremo que envía los datos si es
     *    distinto de 0 se creará un objeto Socket con la dirección y puerto.
     *
     *    @return 0 en caso de éxito o -1 si error (cerrar conexión)
     */
    int recv(char * obj, Socket ** sock);

    int recv(char * obj)
    {
        return recv(obj, 0);
    }

    /**
     *  Envía un mensaje de aplicación definido por un objeto Serializable.
     *  NOTA:La función espera que el objeto esté serializado.
     *
     *    @param obj en el que se enviará por la red
     *
     *    @param sock con la dirección y puerto destino
     *
     *    @return 0 en caso de éxito o -1 si error
     */
    int send(Serializable * obj, Socket * sock);

    /**
     *  Enlaza el descriptor del socket a la dirección y puerto
     */
    int bind();

    friend std::ostream& operator<<(std::ostream& os, const Socket& dt);

    friend bool operator== (const Socket &s1, const Socket &s2);

protected:

    /**
     *  Descriptor del socket
     */
    int sd;

    /**
     *  Representación binaria del extremo, usada por servidor y cliente
     */
    struct sockaddr sa;
    socklen_t       sa_len;
};

#endif /* SOCKET_H_ */

