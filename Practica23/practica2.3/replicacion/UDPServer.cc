#include "UDPServer.h"
#include "Serializable.h"

UDPServer::UDPServer(const char * serv, const char * port)
    :socket(serv, port)
{
    pthread_mutex_init(&mutex, 0);
};

UDPServer::~UDPServer()
{
    pthread_mutex_destroy(&mutex);
};

// ----------------------------------------------------------------------------

extern "C" void * _server_thread(void *arg)
{
    UDPServer * server = static_cast<UDPServer *>(arg);

    server->server_thread();

    return 0;
}

// ----------------------------------------------------------------------------

int UDPServer::start()
{
}

// ----------------------------------------------------------------------------

void UDPServer::server_thread()
{
}

// ----------------------------------------------------------------------------

void UDPServer::add_connection (Socket * s)
{
}

// ----------------------------------------------------------------------------

void UDPServer::del_connection (Socket * s)
{
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

