#ifndef SOCKET_H
#define SOCKET_H

#include <errno.h>             // For errno
#include <cstring>
#include <cstdlib>


#include <string>            // For string
#include <cstring>
#include <cstdlib>           // For atoi
#include <sys/types.h>       // For data types
#include <sys/socket.h>      // For socket(), connect(), send(), and recv()
#include <netdb.h>           // For gethostbyname()
#include <arpa/inet.h>       // For inet_addr()
#include <unistd.h>          // For close()
#include <netinet/in.h>      // For sockaddr_in
typedef void raw_type;       // Type used for raw data on this platform


using namespace std;


/**
 *   Manejamos los problemas/errores de sockets
 */
class SocketException : public exception 
{
  private:
    string userMessage;  // Exception message
  public:
    /**
     *   Constructor de SocketException con un mensaje explicativo
     *   @param message mensaje explicativo
     *   @param incSysMsg true si es un mensaje del sistema (de strerror(errno))
     */
    SocketException(const string &message, bool inclSysMsg = false) throw();
    /**
     */
    ~SocketException() throw();
    /**
     *   Get the exception message
     *   @return exception message
     */
    const char *what() const throw();
};

/**
 *   Clase base de Socket para comunicacion 
 */
class Socket 
{  
  private:
    //Constructor    
    Socket(const Socket &sock);
    //Sobrecarga del operador =
    void operator=(const Socket &sock);

  protected:
    int sockDesc;          // Socket descriptor
    Socket(int type, int protocol) throw(SocketException);
    Socket(int sockDesc);

  public:
    /**
     *   Cierra y quita de memoria el socket.
     */
    ~Socket();

    /**
     *   Recupera la direccion local
     *   @return la direccion local
     *   @exception SocketException es lanzado si falla
     */
    string getLocalAddress() throw(SocketException);

    /**
     *   Recupera el puerto 
     *   @return la direccion local
     *   @exception SocketException es lanzado si falla
     */
    unsigned short getLocalPort() throw(SocketException);

    /**
     *   Seteamos el puerto local
     *   @param puerto 
     *   @exception SocketException es lanzado si falla el seteo del puerto
     */
    void setLocalPort(unsigned short localPort) throw(SocketException);

    /**
     *   Seteamos la dirección local y el puerto, si se omite el puerto se genera un puerto random
     *   @param localAddress direccion local
     *   @param localPort puerto local
     *   @exception SocketException es lanzado si falla
     */
    void setLocalAddressAndPort(const string &localAddress, 
      unsigned short localPort = 0) throw(SocketException);


    void CloseConnection();

  /**
   *   Resolve the specified service for the specified protocol to the
   *   corresponding port number in host byte order
   *   @param service service to resolve (e.g., "http")
   *   @param protocol protocol of service to resolve.  Default is "tcp".
   */
  static unsigned short resolveService(const string &service,
                                       const string &protocol = "tcp");


};



/**
 *   Socket que es capaz de conectar, enviar y recibir
 */
class CommunicatingSocket : public Socket
{
  protected:
    CommunicatingSocket(int type, int protocol) throw(SocketException);
    CommunicatingSocket(int newConnSD);
  
  public:
    /**
     *   Establece una conexion por socket con la direccion remota  
     *   y su puerto
     *   @param foreignAddress foreign address (IP address or name)
     *   @param foreignPort foreign port
     *   @exception SocketException thrown if unable to establish connection
     */
    void connect(const string &foreignAddress, unsigned short foreignPort)
      throw(SocketException);

    /**
     *   Escribimos el buffer en el socket.Llamar a connect antes de 
     *   llamar a send
     *   @param buffer buffer que sera escrito
     *   @param bufferLen numbero de bytes a escribir en el socket
     *   @exception SocketException thrown if unable to send data
     */
    void send(const void *buffer, int bufferLen) throw(SocketException);

    /**
     *   Recibe en buffer la cantidad de bytes especificada por bufferLen
     *   Llamar a connect() antes de llamar a recv()
     *   @param buffer buffer donde recibir los datos
     *   @param bufferLen  cantidad maxima de bytes a leer del socket
     *   @return devuelve la cantidad de bytes leidas, 0 si es EOF, y -1  error
     *   @exception SocketException se lanza si nose pudo recibir
     */
    int recv(void *buffer, int bufferLen) throw(SocketException);

    /**
     *   Recupera la direccion remota
     *   @return 
     *   @exception SocketException se lanza si hubo error
     */
    string getForeignAddress() throw(SocketException);

    /**
     *   Recupera el puerto remoto de la conexion
     *   @return foreign port
     *   @exception SocketException se lanza si hubo error
     */
    unsigned short getForeignPort() throw(SocketException);
};

/**
 *   TCP socket usada para la comunicacion con otros TCP Sockets
 */
class TCPSocket : public CommunicatingSocket 
{
  private:
    // Acceso para TCPServerSocket::accept() connection creation
    friend class TCPServerSocket;
    TCPSocket(int newConnSD);

  public:
    /**
     *   Constructor de TCPSocket
     *   @exception SocketException es lanzado si no se pudo crear el TCPSocket
     */
    TCPSocket() throw(SocketException);

    /**
     *   Constructor de TCPSocket con una conexcion a la direccion y puerto
     *   pasados por parametro
     *   @param foreignAddress foreign address (direccion IP o nombre del host)
     *   @param foreignPort puerto remoto
     *   @exception SocketException se lanza si hubo error
     */
    TCPSocket(const string &foreignAddress, unsigned short foreignPort) 
        throw(SocketException);
};




/**
 *   TCP socket class para el Servidor
 */
class TCPServerSocket : public Socket 
{
  private:
    void setListen(int queueLen) throw(SocketException);
  
  public:
    /**
     *   Constructor del TCP Socket para usarlo como servidor  aceptando conexiones
     *   en un determinado puerto y cualquier interfaz
     *   @param localPort Puerto local del servidor, si se especifica 0 
     *   se asigna cualquier puerto no usado por el sistema
     *   @param queueLen longitud maxima para la cola de solicitudes 
     *   de conexciones pendientes por defecto se toma el valor 5
     *   @exception SocketException es lanzado si no se puede crear
     */
    TCPServerSocket(unsigned short localPort, int queueLen = 5) 
        throw(SocketException);

    /**
     *   Constructor del TCP Socket para usarlo como servidor  aceptando conexiones
     *   en un determinado puerto y cualquier interfaz
     *   @param localAddress direccion local del servidor socket
     *   @param localPort Puerto local del servidor, si se especifica 0 
     *   se asigna cualquier puerto no usado por el sistema
     *   @param queueLen longitud maxima para la cola de solicitudes 
     *   de conexciones pendientes por defecto se toma el valor 5*  
     *   @exception SocketException es lanzado si no se pudo crear
     */
    TCPServerSocket(const string &localAddress, unsigned short localPort,
        int queueLen = 5) throw(SocketException);

    /**
     *   Espera por una conexcion en el socket (Bloqueante)
     *   @return una nueva conexcion
     *   @exception SocketException se lanza si en el intento de aceptar una conexion falla
     */
    TCPSocket *accept() throw(SocketException);
};

/**
  *   UDP socket class
  */
class UDPSocket : public CommunicatingSocket 
{
  private:
    void setBroadcast();
  public:
    /**
     *   Constructor de  UDP socket
     *   @exception SocketException se lanza si no se puede crear
     */
    UDPSocket() throw(SocketException);

    /**
     *   Constructor de UDPSocket
     *   @param localPort puerto local
     *   @exception SocketException se lanza si falla
     */
    UDPSocket(unsigned short localPort) throw(SocketException);

    /**
     *   Construye un UDPSocket con una direccion y puerto pasada por parametro
     *   @param localAddress direccion local
     *   @param localPort puerto
     *   @exception SocketException se lanza si no se pudo crear
     */
    UDPSocket(const string &localAddress, unsigned short localPort) 
        throw(SocketException);

    /**
     *   Desconecta la direccion y puerto remota
     *   @return true si fue exitoso
     *   @exception SocketException si hubo error
     */
    void disconnect() throw(SocketException);

    /**
     *   Envia el buffer como un paquete UDP Datagrama a la 
     *   direccion y puerto especificado
     *   @param buffer buffer que sera escrito
     *   @param bufferLen numero de bytes a escribir
     *   @param foreignAddress direccion o nombre del host remoto
     *   @param foreignPort puerto al cual se enviara
     *   @return true si fue exitoso
     *   @exception SocketException se lanza si no pudo enviar el datagrama
     */
    void sendTo(const void *buffer, int bufferLen, const string &foreignAddress,
              unsigned short foreignPort) throw(SocketException);

    /**
     *   Lee la cantidad de bytes de bufferlen del socket. 
     *   Buffer contendra los datos recibidos
     *   @param buffer buffer donde recibir los datos
     *   @param bufferLen cantidad maxima de bytes a recibir
     *   @param sourceAddress direccion origne
     *   @param sourcePort puerto origen
     *   @return  el numero de bytes recibidos y -1 si hubo error
     *   @exception SocketException se lanza si no pudo 
     */
    int recvFrom(void *buffer, int bufferLen, string &sourceAddress, 
                 unsigned short &sourcePort) throw(SocketException);

    /**
     *   Setter de Multicast TTL
     *   @param multicastTTL multicast TTL
     *   @exception SocketException se lanza si huubo error
     */
    void setMulticastTTL(unsigned char multicastTTL) throw(SocketException);

    /**
     *   Join el grupo especificado de multicast
     *   @param multicastGroup 
     *   @exception SocketException 
     */
    void joinGroup(const string &multicastGroup) throw(SocketException);

    /**
     *   Leave el grupo especificado de multicast
     *   @param multicastGroup 
     *   @exception SocketException 
     */
    void leaveGroup(const string &multicastGroup) throw(SocketException);


};

#endif