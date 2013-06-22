/**
 * gcc cb.c -lws2_32 -o cb
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

#define PORT 3550 /* El puerto que sera abierto */
#define BACKLOG 2 /* El numero de conexiones permitidas */
#define bufferSize 128 //Number of bytes to be sent

//This function dictates what kind of processing will be done when the RFC is sent from the client. 
//Up to this point, only lines where the rfc is found ill be sent.
void doprocessing (int sock)
{
//============================================Global variables definition

    char buffer[bufferSize]; //This is the array which holds the recieved message from the client
	char file[] = "Loans.txt"; //This is the string which holds the directory location of the file that holds the loans
	FILE *filePtr; //File pointer used to create the file
    int recvMsgSize; //Integer that holds the size of the recieved message 
    
/*============================================= Receive message from client */
    if ((recvMsgSize = recv(sock, buffer, bufferSize, 0)) < 0){
        perror("ERROR reading to socket");
	}
	
//===================================================START FILE READING
	if(buffer[0] == 'r'){
		if((filePtr = fopen(file, "r")) != NULL)//If the file can open, do the following:
		{
		//=======================Variables:
		
			char line[bufferSize]; //Create a char array which will hold each line of the loans file
			char *readLine = &(line[0]); //Pointer that points to the array of char, used to write each line that was read
			
			//==================Start Reading lines
			
			fgets(readLine,bufferSize,filePtr); //This reads the first line, and discards it,  this it to eliminate the 'header' of the 
			//loans file
			
			//===============================Finding the RFC algorithm
			while(readLine = fgets(readLine,bufferSize,filePtr)){ //While we still can read lines, we do the following:
				int indexOfLine; //This int will be used as the index for each line read from the file
				
				for(indexOfLine=0;line[indexOfLine] != '|';indexOfLine++){} //This for assures that the first column of the file loans is
				//ignored, in order to find the RFC column
				int indexOfBuffer=2,numberOfCoincidences=0;//indexOfBuffer is the index that allows us to read all the characters
				//in the recieved message buffer, numberOfCOincidences counts how many of those characters where equal (Line and buffer)
				
				for(indexOfLine++;line[indexOfLine] != '|';indexOfLine++){ /*Cycle that reads each of the characters in the buffer and in
				the column RFC of the line, we compare each character in the next 'if' and when they are the same, we add 1 to the number
				of coincidences found
				*/
					if(line[indexOfLine] == buffer[indexOfBuffer]){
						numberOfCoincidences++;
					}
					indexOfBuffer++; //we move the index of the buffer one space
				}
				/*After checking all the line from the first '|' to the second '|', whe check if the number of coincidences is 
				equal to the number of characters in the buffer
				*/
				if(numberOfCoincidences == (indexOfBuffer-2)){
					for(indexOfLine; line[indexOfLine]!='\n';indexOfLine++){}
					line[indexOfLine++] = '$';
					line[indexOfLine++] = '\0';
					printf("%s \n", line); //Print the line with the RFC, used for debugging
					 //With this line we send the line that has the RFC to the socket
					if(send(sock, readLine, indexOfLine,0) != indexOfLine){
						perror("Error while writing to the Socket");
					}
				}
				for(indexOfLine=0;indexOfLine < bufferSize;indexOfLine++){line[indexOfLine] = '0';} //Erase all infomation in the string 
			} 
				fclose(filePtr);
		}
		else //If the file could not be opened, print the message: "Could not open"  and the name of the file
		{
			printf("Could not open %s\n", file);
		}
	}
	
	else if (buffer[0] == 'w'){
	int indexOfBuffer = 2;
		if((filePtr = fopen(file, "a")) != NULL){//If the file can open, do the following:
			for(indexOfBuffer = 2; buffer[indexOfBuffer] != '\n' ; indexOfBuffer++){
				fputc(buffer[indexOfBuffer],filePtr);
			}
			printf("Done writing \n");
			fclose(filePtr);
		}
		else{
			printf("Could not open file");
		}
	}
	else if(buffer[0] == 'c'){
		//Algorithm to close a credit
		if((filePtr = fopen(file, "r+")) != NULL)//If the file can open, do the following:
		{
		//=======================Variables:
		
			char line[bufferSize]; //Create a char array which will hold each line of the loans file
			char *readLine = &(line[0]); //Pointer that points to the array of char, used to write each line that was read
			
			//==================Start Reading lines
			
			fgets(readLine,bufferSize,filePtr); //This reads the first line, and discards it,  this it to eliminate the 'header' of the 
			//loans file
			
			//===============================Finding the RFC algorithm
			while(readLine = fgets(readLine,bufferSize,filePtr)){ //While we still can read lines, we do the following:
				int indexOfLine; //This int will be used as the index for each line read from the file
				
				for(indexOfLine=0;line[indexOfLine] != '|';indexOfLine++){} //This for assures that the first column of the file loans is
				//ignored, in order to find the RFC column
				int indexOfBuffer=2,numberOfCoincidences=0;//indexOfBuffer is the index that allows us to read all the characters
				//in the recieved message buffer, numberOfCOincidences counts how many of those characters where equal (Line and buffer)
				
				for(indexOfLine++;line[indexOfLine] != '|';indexOfLine++){ /*Cycle that reads each of the characters in the buffer and in
				the column RFC of the line, we compare each character in the next 'if' and when they are the same, we add 1 to the number
				of coincidences found
				*/
					if(line[indexOfLine] == buffer[indexOfBuffer]){
						numberOfCoincidences++;
					}
					indexOfBuffer++; //we move the index of the buffer one space
				}
				/*After checking all the line from the first '|' to the second '|', whe check if the number of coincidences is 
				equal to the number of characters in the buffer
				*/
				if(numberOfCoincidences == (indexOfBuffer-2)){
					
				//==================================================Look for the date
					int numberOfPipes =0;
					while(numberOfPipes<3){
						indexOfLine++;
						if(line[indexOfLine]=='|'){
							numberOfPipes++;
						//	printf("%d\n",numberOfPipes);
						}
					}
					indexOfLine++;
					indexOfBuffer++;
					//====new variables
					numberOfCoincidences = 0;
					int spaces =0;
					
					//printf("Buffer's date: %c \nLine's date: %c\n", buffer[indexOfBuffer],line[indexOfLine]);
					
					for(indexOfLine;line[indexOfLine] != '|';indexOfLine++){
						printf("%c",line[indexOfLine]);
						if(line[indexOfLine] == buffer[indexOfBuffer]){
							numberOfCoincidences++;
						}
						indexOfBuffer++;
						spaces++;
					}
					printf("\nNumber of index: %d\n",indexOfLine);
					if(numberOfCoincidences == spaces){
						printf("found the date: \n%s\n",line);
						indexOfLine++;
						for(indexOfLine; line[indexOfLine]!='|';indexOfLine++){}
						indexOfLine++;
						printf("index of line: %d\nchar: %c\n",indexOfLine,line[indexOfLine]);
/*					
						if(line[indexOfLine] == 'N'){printf("You cannot close an already closed credit\n");}
						else if(line[indexOfLine]=='Y'){line[indexOfLine] = 'N';}
						printf("%s\n",line);
*/						
						//int pos = ftell(filePtr);
						//printf("%d\n",fseek(filePtr,pos-1,pos));
						if(fseek(filePtr,-2,SEEK_CUR) == 0){
							fputc('N',filePtr);
						}
						fseek(filePtr,2,SEEK_CUR);
					}					
				}
				for(indexOfLine=0;indexOfLine < bufferSize;indexOfLine++){line[indexOfLine] = '0';} //Erase all infomation in the string 
			} 
				fclose(filePtr);
		}
		else //If the file could not be opened, print the message: "Could not open"  and the name of the file
		{
			printf("Could not open %s\n", file);
		}
		
	}

	else{
		printf("No\n");
	}
		
//=====================================================ENDS FILE READING	
//=====================================================STARTS FILE WRITING
if((filePtr = fopen(file, "r")) != NULL)//If the file can open, do the following:
    {
		
    }
    else //If the file could not be opened, print the message: "Could not open"  and the name of the file
    {
        printf("Could not open %s\n", file);
    }
//====================================================ENDS FILE WRITING
    closesocket(sock);    /* Close socket */
}

BOOL initW32() 
{
		WSADATA wsaData;
		WORD version;
		int error;
		
		version = MAKEWORD( 2, 0 );
		
		error = WSAStartup( version, &wsaData );
		
		/* check for error */
		if ( error != 0 )
		{
		    /* error occured */
		    return FALSE;
		}
		
		/* check for correct version */
		if ( LOBYTE( wsaData.wVersion ) != 2 ||
		     HIBYTE( wsaData.wVersion ) != 0 )
		{
		    /* incorrect WinSock version */
		    WSACleanup();
		    return FALSE;
		}	
}

int main()
{

	 initW32(); /* Necesaria para compilar en Windows */ 
	 	
   int fd, fd2; /* los descriptores de archivos */

   /* para la información de la dirección del servidor */
   struct sockaddr_in server;

   /* para la información de la dirección del cliente */
   struct sockaddr_in client;

   unsigned int sin_size;

   pid_t pid;

   /* A continuación la llamada a socket() */
   if ((fd=socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {
      printf("error en socket()\n");
      exit(-1);
   }

   server.sin_family = AF_INET;

   server.sin_port = htons(PORT);

   server.sin_addr.s_addr = INADDR_ANY;
   /* INADDR_ANY coloca nuestra dirección IP automáticamente */

   //bzero(&(server.sin_zero),8);
   memset(&(server.sin_zero), '0', 8);
   /* escribimos ceros en el reto de la estructura */


   /* A continuación la llamada a bind() */
   if(bind(fd,(struct sockaddr*)&server, sizeof(struct sockaddr))==-1) {
      printf("error en bind() \n");
      exit(-1);
   }

   if(listen(fd,BACKLOG) == -1) {  /* llamada a listen() */
      printf("error en listen()\n");
      exit(-1);
   }

   while(1) {
      sin_size=sizeof(struct sockaddr_in);
      /* A continuación la llamada a accept() */
      if ((fd2 = accept(fd,(struct sockaddr *)&client, &sin_size))==-1) {
         printf("error en accept()\n");
         exit(-1);
      }

      printf("Se obtuvo una conexion desde %s\n", inet_ntoa(client.sin_addr) );
      /* que mostrará la IP del cliente */

      //send(fd2,"Bienvenido a mi servidor.\n",bufferSize,0);
      /* que enviará el mensaje de bienvenida al cliente */
      
      doprocessing(fd2);

   } /* end while */
}

