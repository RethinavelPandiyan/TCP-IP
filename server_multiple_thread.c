#include<stdio.h>
#include<pthread.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
int sfd,ret_value,new_sfd,length,i;
void* client_fun(void* arg){
        int client_id=*(int*)arg;
        free(arg);
        char rec_data[50];
        printf("Client connected: %ld\n",pthread_self());
        while(1){
                if(read(client_id,rec_data,sizeof(rec_data))==0){
                        printf("Client %ld disconnected.\n",pthread_self());
                        close(client_id);
                        return NULL;
                }
                printf("Received: %s\n",rec_data);
                for(i=0;rec_data[i];i++)
                        rec_data[i]=rec_data[i]^32;
                rec_data[i]='\0';
                write(client_id,rec_data,sizeof(rec_data));
        }
        return NULL;
}
int main(){
        struct sockaddr_in srv,cln;
        pthread_t t1;
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
                return 1;
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
        int* client_fd=malloc(sizeof(int));
        *client_fd=new_sfd;
        if(pthread_create(&t1,NULL,client_fun,client_fd)!=0){
                printf("Client disconnected..\n");
                close(new_sfd);
                free(client_fd);
        }
        pthread_detach(t1);
        }
        return 0;
}
