#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

int main(int argc, char* argv[]){

    if (argc < 1)
    {
        printf("==>  %s HOST PORT\n", argv[0]);
        exit(1);
    }
    

    int sock;
    int buff[2];
    int number[1];
    int PORT = atoi(argv[2]); // Port that will be accessed
    int rval;
    int value[1]; // Random value that the server will send to the client

    struct sockaddr_in server;
    struct hostent *hp;

    /*
        calling the function that creates the socket and storing its return in a variable.

        Parameters:
            1st - The first parameter indicates that IPV4 will be used
            2nd - Indicates the protocol, in this case TCP
            3rd - Flags, by default 0 also indicates TCP

    */
    sock = socket(AF_INET,SOCK_STREAM,0);

    if(sock < -1){
        perror("*Socket starting failed!");
        exit(1);
    }

    /*
        Indicating that we will use it with IPV4 addresses
    */
    server.sin_family = AF_INET;

    /*
        Stored the IP to make the connection on the socket
    */
    hp = gethostbyname(argv[1]);

    if(hp == 0){
        perror("Invalid IP or the server is not running!");
        close(sock);
        exit(1);
    }

    /*
        Defining the port the client will connect to
    */
    memcpy(&server.sin_addr, hp->h_addr, hp->h_length);
    server.sin_port = htons(PORT);

    printf("\nConnecting to %s on TCP port %d \n",argv[1],PORT);


    /*
        calling the connect() function, and storing its return to check for failures.
    */
    int connection = connect(sock, (struct sockaddr *)&server,sizeof(server));


    if(connection < 0){
        perror("Connection failed!!");
        close(sock);
        exit(1);
    }else{
        printf("\nConnection established!\n");            
    }

        /* 
            Getting the random number from the server
        */
        recv(sock,value,sizeof(value),0);
        printf("valor:  %d\n", value[0]);

        /*
            The stop condition is if the number previously entered by the User is equal to the random number, then when the loop is broken, the User has hit the number.
        */
        while(number[0] != value[0]){

            printf("Please type a number between 0 - 10:  "); 
            scanf("%d",&number[0]);

            if(number[0] > 10 || number[0] <= -1){
                printf("the number is not between 0 - 10\n");
                printf("Please type a number between 0 - 10:  "); 
                scanf("%d",&number[0]);
            }else{
                if((value[0] - 1) > number[0]){
                    printf("too low\n");
                }else if((value[0] + 1) < number[0]){
                    printf("too high\n");
                }
                if(send(sock, number, sizeof(number),0) <  0){
                    perror("Send Failed!!");
                    close(sock);
                    exit(1);
                }
            }



        }

    printf("That's the correct answer!\n");

close(sock);
return 0;
}