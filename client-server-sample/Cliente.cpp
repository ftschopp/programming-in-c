/*
#######################################################################################
# UNLaM - INGENIERIA EN INFORMATICA - SISTEMAS DE COMPUTACION 2 / SISTEMAS OPERATIVOS #
#######################################################################################
# NOMBRE DEL PROGRAMA: Cliente.cpp
# OBJETIVO: Es el encargado comunicarse con un servidor y enviarle mensajes.
# Ver la ayuda del comando
#####################################################################################
# TSCHOPP FERNANDO    tschoppfernando@gmail.com
#
# Fecha de Creación: 10/06/2014
#####################################################################################
# Release Version: 1.0.0 10/06/2014
# Comentarios: Se liberó la version 1.0.0
#
# Entrega N° 1
#####################################################################################
*/

#include "Socket.h"  // For Socket and SocketException
#include <iostream>           // For cerr and cout
#include <cstdlib>            // For atoi()
#include <cstring>
#include <string>
#include <algorithm>

#define FIN 0
#define TIME 1
#define MSJ 2
#define INI 3
#define DIP 4
#define LOG 5
#define CLOSESERVER 6

using namespace std;

const int RCVBUFSIZE = 1024;    // Size of receive buffer


int CheckMsjType(string);
void help();

int main(int argc, char *argv[]) 
{

  for (int i = 0; i < argc; ++i) 
  {      
    string strParam = string(argv[i]);
    if(strParam.compare("help")==0)
    {
     help();
     exit(1);
    }
  }

 if (argc!=3)
 {
    if(argc < 3)
      cout<<"Parámetros insuficientes"<<endl;

    cerr << "Usage: " << argv[0] << " <ip> <puerto> " << endl;
    cerr << "Para ver la ayuda: " << argv[0] << " <help> " << endl;
    exit(1);
  }

    

  string servAddress = argv[1]; // First arg: server address  
  unsigned short echoServPort = atoi(argv[2]);

  try 
  {
    char echoBuffer[RCVBUFSIZE];
    int recvMsgSize;
    string strResponse="";

    // Establish connection with the echo server
    TCPSocket sock(servAddress, echoServPort);

    string input = "";
    cout << "Porfavor ingrese un mensaje al servidor (FIN para terminar):\n>";
    getline(cin, input);
    bool endClient=false;

    while(!endClient)
    {
      char msj[RCVBUFSIZE];
      strncpy(msj,input.c_str(),sizeof(msj));
      msj[sizeof(msj)-1]=0;
      int sse=0;
      //cout<<"voy a enviar:"<<msj<<" length:"<<input.length()<<endl;
      sock.send(msj, input.length());

      int msjType = CheckMsjType(input);

      if(msjType == TIME ||msjType == MSJ||msjType == INI||msjType == DIP )
      {
          //Espero la respuesta
          recvMsgSize = sock.recv(echoBuffer, RCVBUFSIZE);         
          
          if(recvMsgSize > 0)
          {
            strResponse = string(echoBuffer);
            strResponse = strResponse.substr(0,recvMsgSize);
            //strResponse.erase(std::remove(strResponse.begin(), strResponse.end(), '\n'), strResponse.end());
            cout<<strResponse<<endl;
          }                        
      }

      if(msjType == LOG)
      {
        int cantLines=0;
        
        recvMsgSize = sock.recv(echoBuffer, RCVBUFSIZE);

        strResponse = string(echoBuffer);
        strResponse = strResponse.substr(0,recvMsgSize);
        cantLines = atoi(strResponse.c_str());

        cout<<"Mostrando Log(lineas):"<<cantLines<<endl;
        string strOK ="OK";
        sock.send(strOK.c_str(), strOK.length());
        

        for(int i=0;i <cantLines;i++)            
        {
           recvMsgSize = sock.recv(echoBuffer, RCVBUFSIZE);         
          
           if(recvMsgSize > 0)
           {
             strResponse = string(echoBuffer);
             strResponse = strResponse.substr(0,recvMsgSize);
             cout<<strResponse<<endl;
           }     
          sock.send(strOK.c_str(), strOK.length());
        }
      }
      if(msjType == CLOSESERVER)
      {
        cout<<"Conexión finalizada por el servidor";
        endClient=true;
      }


      
      if(msjType == FIN)
      {
        endClient=true;
      } 
      else
      {
        //Print Prompt
        cout<<">";
        getline(cin, input);  
      }
      

    }     
    cout<<"Sale"<<endClient<<endl;

/*
    // Send the string to the echo server
    sock.send(echoString, echoStringLen);
  
    char echoBuffer[RCVBUFSIZE + 1];    // Buffer for echo string + \0
    int bytesReceived = 0;              // Bytes read on each recv()
    int totalBytesReceived = 0;         // Total bytes read
    // Receive the same string back from the server
    cout << "Received: ";               // Setup to print the echoed string
    while (totalBytesReceived < echoStringLen) {
      // Receive up to the buffer size bytes from the sender
      if ((bytesReceived = (sock.recv(echoBuffer, RCVBUFSIZE))) <= 0) {
        cerr << "Unable to read";
        exit(1);
      }
      totalBytesReceived += bytesReceived;     // Keep tally of total bytes
      echoBuffer[bytesReceived] = '\0';        // Terminate the string!
      cout << echoBuffer;                      // Print the echo buffer
    }
    cout << endl;

    // Destructor closes the socket
    */
  }
  catch(SocketException &e)
  {

    cout<<"errSocketexception"<<endl;
    cerr << e.what() << endl;
    exit(1);
  }
  catch(exception &ex)
  {
    cout<<"error general"<<endl;
  }

  return 0;
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
  if(str.compare("CLOSESERVER")==0)
    return CLOSESERVER;
  return -1;
}



void help()
{
  cout<<"./Cliente <ip> <puerto>"<<endl;
  cout<<"Este es un cliente que se conectara a un servidor y enviará mensajes"<<endl;
  cout<<"Los clientes podran enviarle mensajes, y el servidor los guardará y los contará"<<endl;
  cout<<"y antes determinados comandos el cliente recibira las siguientes respuestas, por ejemplo: "<<endl;
  cout<<" * Si envia LOG recibe el log con todos los mensajes enviados"<<endl;
  cout<<" * Si envia MSJ recibe la cantidad de mensajes enviados"<<endl;
  cout<<" * Si envia INI recibe la Fecha y Hora de inicio de conexión"<<endl;
  cout<<" * Si envia TIME recibe el tiempo total conectado"<<endl;
  cout<<" * Si envia DIP recibe su ip."<<endl;
  cout<<" * Si envia FIN termina la conexion con el servidor"<<endl;
  cout<<"Si recibe cualquier mensaje distinto a los comandos anteriores lo guardara y contara"<<endl;
  cout<<"Si el cliente recibe una señal SIGINT finaliza la conexión y termina.";

}