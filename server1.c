#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h> //for threading

void *connection_handler(void *);

int main(int argc,char *argv[])
{
	int socket_desc,new_socket,c,*new_sock; int cli=1;
	struct sockaddr_in server,client; char buff[100];
	char *message;
	//Creating socket
	socket_desc=socket(AF_INET,SOCK_STREAM,0);
	if (socket_desc==-1)
	{
		puts("Could not create socket");
	}
	//sockaddr_in structure values being intitalized.
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(8800);
	//Bindiing the address with port
	if( bind(socket_desc,(struct sockaddr *)&server,sizeof(server)) < 0)
	{
		puts("bind failed");
		return 1;
	}
	puts("bind done");
	//Listening to port 
	listen(socket_desc , 3);
	//Accept incoming connection
	puts("Waiting for incoming connections.../.../");
	c=sizeof(struct sockaddr_in);
	while((new_socket=accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
	{	//for every new client added to the system we use the address in new_socket
		printf("Connection accepted");
		//Now Reply to the client
		sprintf(buff,"client %d your handler is being assigned,kindly wait\n",cli);
		//snprintf(message,sizeof(message),"client %d you are being given a handler ",cli1);
		cli++;
		write(new_socket,buff,strlen(buff));
		pthread_t sniffer_thread;
		new_sock=malloc(1);
		*new_sock=new_socket;
		if(pthread_create(&sniffer_thread,NULL,connection_handler,(void*)new_sock)<0)
		{
			perror("could not create thread");
			return 1;
		}

		puts("Handler assigned");
		if (new_socket<0)
		{
			perror("accept failed");
			return 1;
		}
	}
return 0;
}

void *connection_handler(void *socket_desc)
{
	//Get the socket descriptor
	int sock=*(int*)socket_desc; //type casting socket_dsec into int pointer and then taking the value of the pointer
	int read_size;
	char *message , client_message[2000];
	//Send some messages to the client
	message="Cool,it wokrs\n";
	write(sock,message,strlen(message));
	message="Now type something and i will echo it back to you\n";
	write(sock,message,strlen(message));
	//Receive a message from client
	while((read_size=recv(sock,client_message,2000,0))>0)
	{

	//Send the message back to client
	write(sock,client_message,strlen(client_message));
	}
	if(read_size==0)
	{
		puts("Client disconnected");
		fflush(stdout);
	}
	else if(read_size==-1)
	{
		perror("recv failed");
	}

//Free the socket pointer before termination
free(socket_desc);

return 0;
}
