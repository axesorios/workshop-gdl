/**
* Este aplicacion representa a una entidad crediticia la cual
* concentra todos los creditos otorgados a los clientes de
* diferentes entidades.
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <winsock2.h>
#include <unistd.h>
#define PORT 3550 /* El puerto que será abierto */
#define BACKLOG 5 /* El número de conexiones permitidas */
#define BUFFER_SIZE 127 // cambiar el buffer a 128
void doprocessing(int sock) {
int n;
char buffer[BUFFER_SIZE];
memset(&(buffer), '0', BUFFER_SIZE);
int recvMsgSize;
int acm = 0;
/* Send received string and receive again until end of transmission */
do /* zero indicates end of transmission */
{
printf("buffer3: %d*\n", recvMsgSize);
/* See if there is more data to receive */
if ((recvMsgSize = recv(sock, buffer, BUFFER_SIZE, 0)) < 0) {
perror("ERROR reading to socket 3");
}
acm += recvMsgSize;

printf("buffer3: %d*\n", acm);
if (acm == BUFFER_SIZE)
{
/* Echo message back to client */
if (send(sock, buffer, recvMsgSize, 0) != recvMsgSize)
perror("ERROR writing to socket 2");
printf("buffer3: %d*\n", recvMsgSize);
//leerfile(buffer, sock);
process(buffer, sock);
break; 
} 
} while (recvMsgSize > 0);
closesocket(sock); /* Close client socket */
}
process(const char* buffer, int sock){
if (buffer[0] == 'w'){
FILE * pFile;
/*Format of buffer :
"w115|34567ZXCVB|PATRIA|INDIA|35000|30/6/2013|VERY BAD|Y" */
int indexOfBuffer = 1;
if((pFile = fopen("rfc.txt", "a")) != NULL){
for(indexOfBuffer = 1; buffer[indexOfBuffer] != '\n' ; indexOfBuffer++){
fputc(buffer[indexOfBuffer],pFile);
}
printf("Done writing \n");
fclose(pFile);
}else{
printf("Could not open file");
}
}else if(buffer[0] == 'r'){
leerfile(buffer, sock);
//memset(&(buffer), '0', 256);
}
}
BOOL initW32() {
WSADATA wsaData;
WORD version;
int error;
version = MAKEWORD( 2, 0 );
error = WSAStartup(version, &wsaData);
/* check for error */
if (error != 0) {
/* error occured */
return FALSE;
}
/* check for correct version */
if (LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 0) {
/* incorrect WinSock version */
WSACleanup();
return FALSE;
}
}
int leerfile(const char* buffer, int sock) {
FILE * pFile;
char mystring[100];
boolean found = FALSE;
char rfc[11] = "XX ";
strncpy(rfc, buffer+1, 10);
printf("RFC: %s\n", rfc);
pFile = fopen("rfc.txt", "r");
if (pFile == NULL )
perror("Error opening file");
else {
while (fgets(mystring, 100, pFile) != NULL) {

if (strncmp(mystring + 4, rfc, 10) == 0) {
puts(mystring);
send(sock, mystring, strlen(mystring), 0);
found = TRUE;
puts("encontrado\n");
}
}
if (found == FALSE){
puts("no se encontro\n");
send(sock, "No se encontro\n", 15, 0);
}
}
fclose(pFile);
return 0;
}
int main() {
initW32(); /* Necesaria para compilar en Windows */
int fd, fd2; /* los descriptores de archivos */
struct sockaddr_in server; /* para la información de la dirección del servidor */
struct sockaddr_in client;/* para la información de la dirección del cliente */
unsigned int sin_size;
pid_t pid;
/* A continuación la llamada a socket() */
if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
printf("error en socket()\n");
exit(-1);
}
server.sin_family = AF_INET;
server.sin_port = htons(PORT);
server.sin_addr.s_addr = INADDR_ANY;/* INADDR_ANY coloca nuestra dirección IP automáticamente */
//bzero(&(server.sin_zero),8);
memset(&(server.sin_zero), '0', 8); /* escribimos ceros en el reto de la estructura */
/* A continuación la llamada a bind() */
if (bind(fd, (struct sockaddr*) &server, sizeof(struct sockaddr)) == -1) {
printf("error en bind() \n");
exit(-1);
}
if (listen(fd, BACKLOG) == -1) { /* llamada a listen() */
printf("error en listen()\n");
exit(-1);
}
while (1){
sin_size = sizeof(struct sockaddr_in);
/* A continuación la llamada a accept() */
if ((fd2 = accept(fd, (struct sockaddr *) &client, &sin_size)) == -1) {
printf("error en accept()\n");
exit(-1);
}
printf("Se obtuvo una conexion desde %s\n", inet_ntoa(client.sin_addr));
doprocessing(fd2);
} /* end while */
}