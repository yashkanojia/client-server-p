#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <stdio.h> 
#include <unistd.h> 
#include <sys/socket.h> 

//help from tutorial code

int clientport=8080;
char mssg[100];
char format[100];
int z=0;
int main(int argc, char const *argv[]) {
    //details to connect the socket and port
    struct sockaddr_in clientconnection;
    clientconnection.sin_family=AF_INET;
    clientconnection.sin_addr.s_addr=INADDR_ANY;
    clientconnection.sin_port=htons(clientport);
    int size=sizeof(clientconnection);
    
    //socket creation
    int clientsocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(clientsocket<0){
        printf("socket creation failed\n");
    }

    //connect to the server socket
    int connectionerror=connect(clientsocket,(struct sockaddr *)&clientconnection,size);
    if(connectionerror<0){
        printf("connection not established with server...:)\n");
        exit(1);
    }
    printf("connection established with the server......\n");

    //sending the file name
    printf("please enter the file name :\n");
    scanf("%s",mssg);
    send(clientsocket,mssg,sizeof(mssg),0);

    //recieving the mssg from the server
    char rmssg[100];
    int recvd=read(clientsocket,rmssg,100);
     if(recvd<0){
        perror("server failed to search\n");
        exit(1);
    }
    else{
        printf("%s\n",rmssg);
    }
    
    // if file found then recieve the file from the server
     char foundmssg[]="your requested file found in the server...wait";
     //strcat(mssg,format);
    if(strcmp(rmssg,foundmssg)==0){
        FILE *file=fopen((char *)mssg,"w");
        if(file==NULL){
            printf("failed to download the file\n");
            exit(1);
        }
        //write the data to the file 
        char data[1000];
        while(1){
            int n=recv(clientsocket,data,sizeof(data),0);
            if(n<=0){
                break;
            }
           int e=fwrite(data,sizeof(char),n,file);
           if(e<0){
               printf("failed to write the file\n");
               exit(1);
           }
        }
        printf("download is successfull...\n");
        fclose(file);
    }
    else{
        printf("download is unsuccesfull..\n");
        exit(1);
    }
    close(clientsocket);
    return 0;
}