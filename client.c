#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
int main(){
        struct sockaddr_in cln;
        int sfd,new_sfd;
        char rec_data[50],snd_data[50];
        sfd=socket(AF_INET,SOCK_STREAM,0);
        if(sfd<0){
                perror("socket");
                return 1;
        }
        printf("Client ready..\n");
        cln.sin_family=AF_INET;
        cln.sin_port=htons(2050);
        cln.sin_addr.s_addr=inet_addr("127.0.0.1");
        new_sfd=connect(sfd,(struct sockaddr*)&cln,sizeof(cln));
        if(new_sfd<0){
                perror("connect");
                return 1;
        }
        printf("Connected..\n");
        while(1){
                scanf("%s",snd_data);
                write(sfd,snd_data,sizeof(snd_data));
                read(sfd,snd_data,sizeof(snd_data));
                printf("Replay: %s\n",snd_data);
        }
        return 0;
}
