#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<time.h>

int main(int argc, char *argv[]){

    if (argc < 2 )
    {
        printf("==>  %s PORT\n", argv[0]);
        exit(1);
    }
    
    int Socket;
    int buff[2];
    int rval;
    int value[1]; // variable that will have the random number
    int count=0; // Counter of how many attempts the user needed to correct the number
    int client;
    int PORT = atoi(argv[1]); /* Declaring the PORT that will receive the connection */

    /* Starting socket using IPV4 and TCP */
    Socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server;

    if(Socket < 0){
        perror("Failed to create Socket!!");
        exit(1);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    /* 
        Calling the bind() function, which opens the socket port, and then checking for errors
    */
    if(bind(Socket, (struct sockaddr *)&server, sizeof(server))){
        perror("Bind Failed: ");
        exit(1);
    }

    printf("Server started on TCP port %d\n", PORT);

    listen(Socket,5);

    /*
        Generating a random value
    */
    srand(time(NULL)); // Line to ignore the time, to generate random number
    value[0] = rand()%10;

    /* 
        Line to print the random number

        printf("\nValue: %d\n",value[0]);
    */

    printf("Waiting for connection... \n");

    /*
        Calling the accept function to accept the client's request to connect
    */

    client = accept(Socket, (struct sockaddr *)0,0);   

    printf("Connection established!\n");
    

    /* Sending the random number to client */
    if(send(client,value,sizeof(value),0) < 0){
        perror("Failed to send value: ");
        close(client);
        exit(1);
    }

    while(1){

        switch (client)
        {
            case -1:
            {
                perror("Accept Failed!!");
            break;
            }
            case 4:
            {
                printf("\nWaiting for user response\n");

                while(buff[0] != value[0]){

                    /* 
                        Receiving the number that the user sent by client
                    */
                    memset(buff,0,sizeof(buff));
                    rval = recv(client,buff,sizeof(buff),0);
                    
                    if(rval < 0){
                        perror("Reading stream message  error");

                    }else if(rval == 0){
                        printf("Ending connection\n");
                        close(Socket);
                        exit(1);
                    }else{
                        printf("\nValor recebido:  %d\n", buff[0]);
                        /* Counting how many times the user sent a number */
                        count++;
                    }
                }
                /* Closing the connection with the client and ending Socket */
                close(client); close(Socket);
                printf("\nThe user guessed the random number %d using %d attempts.\n",value[0], count);
                break;
            }
        }// end switch
    }// end while
return 0;
}