#include <sys/socket.h> 
#include <netinet/in.h> 
#include <stdio.h> 
#include <unistd.h> 
#include <string.h>
#include<sys/stat.h>
#include<sys/sendfile.h>
#include<fcntl.h>
#include <stdlib.h> 


//help from tutorial code

int serverport=8080;
int main(int argc,char const *argv[]){
     //details to connect the socket and port
    struct sockaddr_in serverconnection;
    serverconnection.sin_family=AF_INET;
    serverconnection.sin_addr.s_addr=INADDR_ANY;
    serverconnection.sin_port=htons(serverport);
    int size=sizeof(serverconnection);

    //create a socket
    int serversocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(serversocket<0){
        printf("socket not created\n");
    }

    //socket is bind with given port detials
    int bindingerror=bind(serversocket,(struct sockaddr *)&serverconnection,sizeof(serverconnection));
    if(bindingerror<0){
        printf("binding failed\n");
    }

    //put the socket into the listening mode
    printf("listening to the connection....\n");
    int listeningerror=listen(serversocket,1);
    if(listeningerror<0){
        printf("listening failed\n");
    }

    int accpetedconnection=accept(serversocket,(struct sockaddr*)&serverconnection,&size);
    if(accpetedconnection<0){
        printf("the incoming connection is not establihed\n");
    }
    char recname[100];
    int readingerror=read(accpetedconnection,recname,100);
    if(readingerror<0){
        printf("message from the client is not recived\n");
    }
    printf("searching for the file name %s...\n",recname);


    // file details location etc.
   // char location[100]="C:\\Users\\YASH KANOJIA\\Desktop\\cn\\files\\";

    FILE *file=fopen(recname,"r");
    if(file==NULL){
         char searchedmssg[]="your requested file not found in the server...";
         send(accpetedconnection,searchedmssg,sizeof(searchedmssg),0);
         exit(1);
    }
    else{
        char searchedmsssg[]="your requested file found in the server...wait";
        send(accpetedconnection,searchedmsssg,sizeof(searchedmsssg),0);
        char data[1000]={0};
        int nn;
        while(( nn=fread(data,sizeof(char),1000,file))>0){
            int sendingerror=send(accpetedconnection,data,nn,0);
            if(sendingerror<0){
                printf("failed to send the file \n");
                exit(1);
            }
        }
        printf("file has sended succesfully..\n");
        fclose(file);
        //if file has been found then we need to send the data of the file byte by byte
    }
    close(serversocket);
    return 0;
}