#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
int main(){
        struct sockaddr_in srv,cln;
        int sfd,ret_value,new_sfd,length,i;
        char rec_data[50],snd_data[50];
        sfd=socket(AF_INET,SOCK_STREAM,0);
        if(sfd<0){
                perror("socket");
                return 1;
        }
        srv.sin_family=AF_INET;
        srv.sin_port=htons(2050);
        srv.sin_addr.s_addr=inet_addr("0.0.0.0");
        ret_value=bind(sfd,(struct sockaddr*)&srv,sizeof(srv));
        if(ret_value<0){
                perror("bind");
               }
        listen(sfd,1);
        length=sizeof(cln);
        printf("Server ready..\n");
        while(1){
        new_sfd=accept(sfd,(struct sockaddr*)&cln,&length);
        if(new_sfd<0){
                perror("accept");
                return 1;
        }
        printf("Connected..\n");
        if(fork()==0){
        while(1){
                if(read(new_sfd,rec_data,sizeof(rec_data))==0){
                        printf("Disconnected..\n");
                        return 0;
                }
                printf("Received: %s\n",rec_data);
                for(i=0;rec_data[i];i++){
                        rec_data[i]=rec_data[i]^32;
                }
                rec_data[i]='\0';
                write(new_sfd,rec_data,sizeof(rec_data));
        }
        }
        }
        return 0;
}



