/*
#######################################################################################
# UNLaM - INGENIERIA EN INFORMATICA - SISTEMAS DE COMPUTACION 2 / SISTEMAS OPERATIVOS #
#######################################################################################
# NOMBRE DEL PROGRAMA: Server.cpp
# OBJETIVO: Es el encargado de montar un servidor y contabilizara mensajes de los
# clientes, segun ciertos comandos, dara respuesta al cliente segun lo solicitado.
# Ver la ayuda del comando
#####################################################################################
# TSCHOPP FERNANDO   tschoppfernando@gmail.com
#
# Fecha de Creación: 10/06/2014
#####################################################################################
# Release Version: 1.0.0 10/06/2014
# Comentarios: Se liberó la version 1.0.0
#
# Entrega N° 1
#####################################################################################
*/


#include "Socket.h"  // For Socket, ServerSocket, and SocketException
#include <iostream>           // For cout, cerr
#include <cstdlib>            // For atoi()  
#include "pthread.h" //Posix Threads
#include <sys/stat.h> // For Umask
#include <signal.h>
#include <ctime>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <map>
#include <list>
#include "semaforos.c"



#define FIN 0
#define TIME 1
#define MSJ 2
#define INI 3
#define DIP 4
#define LOG 5

const int RCVBUFSIZE = 1024;

int CLIENTE_NRO=0;

void prepareDaemon();
void HandleTCPClient(TCPSocket *sock,int);     // TCP client handling function
void *ThreadMain(void *arg);               // Main program of a thread  
int CheckMsjType(string );
void SendTime(TCPSocket *,time_t,time_t);
void SendMsjCount(TCPSocket *,int);
void SendHoraConexion(TCPSocket *,time_t);
void SendIP(TCPSocket *);
void SendLog(int,TCPSocket *);
string NumberToString(int );
void SaveLog(int ,string );
void SaveClientStatus(bool,int,string, time_t,time_t,int );
string ZeroPadNumber(int ,int );
string PadLeft(int ,int ,char );
string PadStringLeft(string ,int ,char );

string PadRight(string ,int );
void GenerateFile();
void help();
void signalHandler( int);

map<int, string> CLs;  
key_t claveMutex;
int IdMutex;
list<TCPSocket*> lstSockets;
list<pthread_t> lstThreads;
bool endServer=false;
int main(int argc, char *argv[]) 
{

  if (argc!=2)
  {
    if(argc == 1)
      cout<<"Parámetros insuficientes"<<endl;

    cerr << "Usage: " << argv[0] << " <Server Port> " << endl;
    cerr << "Para ver la ayuda: " << argv[0] << " <help> " << endl;
    exit(1);
  }

    for (int i = 0; i < argc; ++i) 
    {      
      string strParam = string(argv[i]);
      if(strParam.compare("help")==0)
      {
       help();
       exit(1);
      }
    }

    signal(SIGINT, signalHandler);  

  
  claveMutex = ftok("/", 6000);

  prepareDaemon();
  unsigned short echoServPort = atoi(argv[1]);    // First arg:  local port  

  
  try 
  {
    TCPServerSocket servSock(echoServPort);   // Socket descriptor for server  
    
    while(!endServer)
    { 
      // Create separate memory for client argument  
      TCPSocket *clntSock = servSock.accept();
      lstSockets.push_back(clntSock);
      // Create client thread  
      pthread_t threadID; // Thread ID from pthread_create()  
      if (pthread_create(&threadID, NULL, ThreadMain, (void *) clntSock) != 0) 
      {
        cerr << "Unable to create thread" << endl;
        exit(1);
      }
      lstThreads.push_back(threadID);     
    }
  }
  catch (SocketException &e)
  {
    cerr << e.what() << endl;
    exit(1);
  }
  
  return 0;
}

// Se crea el demonio o servicio
void prepareDaemon()
{
    /* Our process ID and Session ID */
        pid_t pid, sid;
        
        /* Fork off the parent process */
        pid = fork();
        if (pid < 0) {
                exit(EXIT_FAILURE);
        }
        /* If we got a good PID, then
           we can exit the parent process. */
        if (pid > 0) {
                exit(EXIT_SUCCESS);
        }

        /* Change the file mode mask */
        umask(0);
                
        /* Open any logs here */        
                
        /* Create a new SID for the child process */
        sid = setsid();
        if (sid < 0) {
                /* Log the failure */
                exit(EXIT_FAILURE);
        }
        

        
        /* Change the current working directory */
        if ((chdir("/tmp")) < 0) {
                /* Log the failure */
                exit(EXIT_FAILURE);
        }
        
        /* Close out the standard file descriptors */
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);  
}

// TCP client handling function
void HandleTCPClient(TCPSocket *sock, int clientID)
{
  // cout << " with thread " << pthread_self() << endl;
  string filename= "LogClienteNro";
  stringstream ss;
  ss << clientID;
  filename= filename.append(ss.str());

  if( remove(filename.c_str()) != 0 )
  {

  }
  

  int msjCount=0;
  bool endClient = false;

  time_t startTime;
  time_t endTime;
  //struct tm * timeinfo;
  time ( &startTime );
  //timeinfo = localtime ( &startTime );
  //cout<<"Current local time and date:"<< asctime (timeinfo);
    
  char echoBuffer[RCVBUFSIZE];
  int recvMsgSize; 

  time ( &endTime );
  SaveClientStatus(true,clientID,sock->getForeignAddress(),startTime,endTime,msjCount);


  // Send received string and receive again until the end of transmission
  while(!endClient)
  {

      string strMsj=""; 
      recvMsgSize = sock->recv(echoBuffer, RCVBUFSIZE); 
      
      if(recvMsgSize > 0)
      {
        strMsj = string(echoBuffer);
        strMsj = strMsj.substr(0,recvMsgSize);
      }    
      
      switch(CheckMsjType(strMsj))
      {
        case FIN:
          endClient=true;                  
          break;
        case TIME:
          //cout<<"RECV TIME"<<endl;
          time ( &endTime );
          SendTime(sock,startTime,endTime);
          break;
        case MSJ:            
          SendMsjCount(sock,msjCount);
          break;
        case INI:                  
          SendHoraConexion(sock,startTime);
          break;
        case DIP:
          SendIP(sock);
          break;
        case LOG:        
          SendLog(clientID,sock);
          break;
        default:
          //no hago nada mas que contar los msj
          msjCount++;
          //cout<<endl<<"RECV:"<<strMsj<<" Size:"<<recvMsgSize<<" with thread " << pthread_self() <<" MsjCount"<<msjCount<< endl;                       
          break;
      }
     
        SaveLog(clientID,strMsj);
        time ( &endTime );

        pedirSemaforo(IdMutex);
        SaveClientStatus(false,clientID,sock->getForeignAddress(),startTime,endTime,msjCount);
        devolverSemaforo(IdMutex);

        GenerateFile();

      //cout<<endl<<"RECV:"<<strMsj<<" Size:"<<recvMsgSize<<" with thread " << pthread_self() << endl;   
      //sock->send(echoBuffer, recvMsgSize);
    // Destructor closes socket
  }

}


void *ThreadMain(void *clntSock) 
{
  pthread_detach(pthread_self()); 
  CLIENTE_NRO++;
  
  HandleTCPClient((TCPSocket *) clntSock,CLIENTE_NRO);

  
  delete (TCPSocket *) clntSock;
  //cout<<"Cerrando al Cliente Nro:"<<CLIENTE_NRO<<endl;
  return NULL;
}


void SaveLog(int id,string msj)
{
  string filename= "LogClienteNro";
  stringstream ss;
  ss << id;
  filename= filename.append(ss.str());

  ofstream ofs;
  ofs.open (filename.c_str(), std::ofstream::out | std::ofstream::app);

  ofs<<msj<<endl;

  ofs.close();
}

void SaveClientStatus(bool firstTime,int clientID,string ip,time_t startTime,time_t endTime, int msjCount)
{
  struct tm * timeinfo;
  
  timeinfo = localtime ( &startTime );

  int hora = timeinfo->tm_hour;
  int min = timeinfo->tm_min;
  int seg = timeinfo->tm_sec;
  int difference = (int)difftime(endTime, startTime);

  string strLinea= "Cliente #";
  strLinea = strLinea.append(ZeroPadNumber(clientID,3)).append(", ").append(PadStringLeft(ip,15,' '));
  strLinea = strLinea.append(", ").append(ZeroPadNumber(hora,2)).append(":").append(ZeroPadNumber(min,2)).append(":").append(ZeroPadNumber(seg,2));
  strLinea = strLinea.append(", ").append(PadLeft(difference,5,' ')).append(" Seg., ").append(PadLeft(msjCount,5,' ')).append(" Mensajes");

  CLs[clientID] = strLinea;

}

void GenerateFile()
{
  ofstream ofs;
  ofs.open ("ServerStatus", std::ofstream::out);

  for( map<int,string>::iterator ii=CLs.begin(); ii!=CLs.end(); ++ii)
  {
    ofs<<(*ii).second << endl;
  }

  ofs.close();

}

int CheckMsjType(string str)
{

  //Verifico el tipo de msj que llega
  if(str.compare("FIN")==0)
    return FIN;      
  if(str.compare("TIME")==0)
    return TIME;
  if(str.compare("MSJ")==0)
    return MSJ;
  if(str.compare("DIP")==0)
    return DIP;
  if(str.compare("INI")==0)
    return INI;
  if(str.compare("LOG")==0)
    return LOG;

  return -1;
}

void SendTime(TCPSocket *sock,time_t startTime,time_t endTime)
{
  string str = "Tiempo Transcurido:";
  
  double difference = difftime(endTime, startTime);

  int days = difference / (60 * 60 * 24);
  int hours = (difference / (60 * 60))-(days * 24);
  int minutes = (difference / 60) - (days*60*24)-(hours * 60);
  int seconds = difference - (days*24*60*60)-(hours*60*60)-(minutes*60);

  cout<<days<<"dias "<<hours<<" Horas "<<minutes<<" Minutos"<<seconds<<" segundos"<<endl;
  
  str = str.append(NumberToString(days)).append(" dias, ").append(NumberToString(hours)).append(" horas, ");
  str = str.append(NumberToString(minutes)).append(" minutos, ").append(NumberToString(seconds)).append(" segundos ");

  char msj[RCVBUFSIZE];
  strncpy(msj,str.c_str(),sizeof(msj));
  msj[sizeof(msj)-1]=0;

  sock->send(msj, str.length());
}

void SendMsjCount(TCPSocket *sock,int msjCount)
{
  string str="La Cantidad de mensajes recibidos es de: ";
  stringstream ss;
  ss << msjCount;
  str= str.append(ss.str());
  
  char msj[RCVBUFSIZE];
  strncpy(msj,str.c_str(),sizeof(msj));
  msj[sizeof(msj)-1]=0;

  //cout<<"Voy a enviar:"<<msj;
  sock->send(msj, str.length());
}

void SendHoraConexion(TCPSocket *sock,time_t startTime)
{
  struct tm * timeinfo;
  timeinfo = localtime ( &startTime );
  string strTime = "Conexión comenzada desde:";
  strTime = strTime.append(string(asctime(timeinfo)));

  char msj[RCVBUFSIZE];
  strncpy(msj,strTime.c_str(),sizeof(msj));
  msj[sizeof(msj)-1]=0;

  sock->send(msj, strTime.length());  
}
void SendIP(TCPSocket *sock)
{

  string str="Server IP:";
  str = str.append(sock->getLocalAddress());
  str = str.append(":").append(NumberToString(sock->getLocalPort()));  
  str = str.append("\nCliente IP:").append(sock->getForeignAddress());
  str = str.append(":").append(NumberToString(sock->getForeignPort()));

  char msj[RCVBUFSIZE];
  strncpy(msj,str.c_str(),sizeof(msj));
  msj[sizeof(msj)-1]=0;

  sock->send(msj, str.length());
}

void SendLog(int clientID,TCPSocket *sock)
{
  string filename= "LogClienteNro";
  stringstream ss;
  ss << clientID;
  filename= filename.append(ss.str());

  ifstream fs;
  fs.open (filename.c_str());

  char echoBuffer[RCVBUFSIZE];
  char msj[RCVBUFSIZE];
  std::string line; 
  int bytesread=0;
  int cantLines=0;
  int recvMsgSize=0;
  for (int i = 0; std::getline(fs, line); i++)
    cantLines++;

  //Enviamos la cantidad de lineas del archivo
  string str = NumberToString(cantLines);
  //sock->send(str.c_str(), str.length());

  strncpy(msj,str.c_str(),sizeof(msj));
  msj[sizeof(msj)-1]=0;

  sock->send(msj, str.length());

  string strMsj="";    
  recvMsgSize = sock->recv(echoBuffer, RCVBUFSIZE);   
  if(recvMsgSize > 0)
  {
   strMsj = string(echoBuffer);
   strMsj = strMsj.substr(0,recvMsgSize);
   //cout<<"Recibi:"<<strMsj<<endl;
  }      
  fs.close();
  //cout<<"lineas"<<cantLines<<endl;
  fs.open (filename.c_str());

  for(int j=0;j<cantLines;j++)
  {
    getline (fs,line);  
    //cout<<"Line:"<<line<<endl; // Prints our STRING.

     char msj[RCVBUFSIZE];
     strncpy(msj,line.c_str(),sizeof(msj));
     msj[sizeof(msj)-1]='0';

     sock->send(msj, line.length());     
     recvMsgSize = sock->recv(echoBuffer, RCVBUFSIZE); 
      if(recvMsgSize > 0)
      {
       strMsj = string(echoBuffer);
       strMsj = strMsj.substr(0,recvMsgSize);
      }     
  }      

  fs.close();
}

string NumberToString ( int Number )
{
stringstream ss;
ss << Number;
return ss.str();
}

string PadRight(string str,int length)
{
  stringstream ss;
  ss << setw(length) << str;
  return ss.str();
}

string ZeroPadNumber(int num,int width)
{
    std::ostringstream ss;
    ss << std::setw( width ) << std::setfill( '0' ) << num;
    return ss.str();
}


string PadLeft(int num,int width,char car)
{
    std::ostringstream ss;
    ss << std::setw( width ) << std::setfill( car ) << num;
    return ss.str();
}

string PadStringLeft(string str,int width,char car)
{
    std::ostringstream ss;
    ss << std::setw( width ) << std::setfill( car ) << str;
    return ss.str();
}

void help()
{
  cout<<"./Server"<<endl;
  cout<<"Este es un servidor que se ejecutará como daemon y atendera solicitudes de los clientes"<<endl;
  cout<<"Los clientes podran enviarle mensajes, y el servidor los guardará y los contará"<<endl;
  cout<<"y antes determinados comandos devolvera ciertas respuestas, por ejemplo: "<<endl;
  cout<<" * Si recibe LOG devuelve el log del cliente"<<endl;
  cout<<" * Si recibe MSJ devuelve la cantidad de mensajes enviados"<<endl;
  cout<<" * Si recibe INI Fecha y Hora de inicio de conexión"<<endl;
  cout<<" * Si recibe TIME tiempo total conectado"<<endl;
  cout<<" * Si recibe DIP devuelve la ip del cliente."<<endl;
  cout<<" * Si recibe FIN termina la conexion con el cliente"<<endl;
  cout<<"Si recibe cualquier mensaje distinto a los comandos anteriores lo guardara y contara"<<endl;
  cout<<"Si el Servidor recibe una señal SIGINT cierra los sockets y termina.";
  cout<<"ACLARACION"<<endl<<"Los archivos tanto de log de cada cliente como el status de los clientes se guardan en /tmp"<<endl;
  cout<<"Los Nombres de los arhivos son LogClienteNro y ServerStatus";

}

void signalHandler( int signum )
{
  try
  {
    endServer=true;
 //   cout << "Interrupt signal (" << signum << ") received.\n";

  // if(!lstThreads.empty())
  // {
  //   list<pthread_t>::iterator it;
  //   it=lstThreads.begin();
  //   while(it!=lstThreads.end())
  //   {
  //     pthread_t th = *it;
  //     pthread_cancel(th);
  //   }

  // }
  //cout<<"termino de cerrar threads"<<endl;
   if(!lstSockets.empty())
     {
       list<TCPSocket*>::iterator pos;
       pos = lstSockets.begin();
       //cout<<"cerrando sockets..."<<endl;
       while(pos != lstSockets.end())
       {
         //pos->close();
        TCPSocket* s = *pos;
        // string str ="CLOSESERVER";
        // char msj[RCVBUFSIZE];
        // strncpy(msj,str.c_str(),sizeof(msj));
        // msj[sizeof(msj)-1]='0';
        // s->send(msj,str.length());
        s->CloseConnection();
        delete (TCPSocket *) s;
        //cout<<"borro un socket"<<endl;
        pos++;
       }
     }

     exit(0);
  }
  catch (SocketException &e) {
    cerr << e.what() << endl;
    
  }
  catch(exception &ex)
  {
    //cerr << ex<<endl;
  }
    // cleanup and close up stuff here  
    // terminate program  

   //exit(signum);  

}