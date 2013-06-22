#include <stdio.h>
  #include <winsock.h>
  #include <string.h>
  #include <conio.h>
  #define puerto 3550   // puerto del servidor por el que se conectaran los clientes

  main(){




  WSADATA wsa;   // winshock
  int sock;
  char buffer[20];
  struct sockaddr_in server;  // estructura del los datos del server
  WSAStartup(MAKEWORD(2,0),&wsa);  // Version winshock  y start

  if((sock=socket(AF_INET,SOCK_STREAM,0))==-1){; // creamos el socket
  printf("Error creando el socket");
  getch();
  exit(0);
  }

  server.sin_family= AF_INET; // familia socket
  server.sin_addr.s_addr=INADDR_ANY;  // local host
  server.sin_port=htons(puerto); //Puerto

  if(bind(sock,(struct sockaddr *)&server,sizeof(server))==-1 ){//Asociamos
  printf("error asociando");
  getch();
  exit(0);
  }

  printf("Esperando Clientes\n");
  listen(sock,5); // Maximos de clientes permitidos 5

  if((sock=accept(sock,0,0))<0){   // Aceptamos conexion cliente
  printf("error aceptando connexion");
  exit(0);
  }

   puts("Cliente conectado");

    if(recv(sock,buffer,sizeof(buffer),0)==-1){ // recivimos lo que nos envia el cliente
     printf("error reciviendo");
     exit(0);
    }
     printf("Esto es lo que envio el cliente;\n");
     puts(buffer);
     getch();
     closesocket(sock);  // Cerramos socket

      return 0;
  }