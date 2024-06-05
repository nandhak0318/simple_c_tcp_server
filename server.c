#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

struct sockaddr_in address;
socklen_t addrlen = sizeof(address);


struct mys{
    int thread_num;
    int soc_num;
};

void* bro(void* arg){
    struct mys* data = (struct mys*)arg;
    int soc = data->soc_num;
    int thread_num = data->thread_num;
    ssize_t valread;
    char* hello = "hello da mapla madurai-la irundhu";
    int new_soc;
    char buffer[1024] = { 0 };
    while((new_soc = accept(soc,(struct sockaddr*)&address,&addrlen))>=0){
        printf("ACPT FROM:%d \n",thread_num);
        valread = read(new_soc, buffer, 1024 - 1);
	    printf("%s\n", buffer);
        send(new_soc,hello,strlen(hello),0);
        printf("SND FROM:%d\n",thread_num);
        close(new_soc);
    }
    free(arg);
    return NULL;
}

int main(){
    pthread_t threads[5];

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons(8083);
    
    int soc = socket(PF_INET,SOCK_STREAM,6);
    printf("SOCKET CREATED %d \n",soc);

    int bin = bind(soc,(struct sockaddr*)&address,sizeof(address));
    printf("BIND CREATED %d \n",bin);

    int lis = listen(soc,120);
    printf("LISTEN STATUS %d \n",lis);
    
    for(int i =0;i<5;i++){
        struct mys* a = malloc(sizeof(struct mys));
        a->thread_num = i;
        a->soc_num = soc;
        pthread_create(&threads[i], NULL, &bro,a);
        printf("thread starting %d \n",i);
    }
    for(int i =0;i<5;i++){
        pthread_join(threads[i],NULL);
        printf("thread ending %d \n",i);
    }
    
    close(soc);
    return 0;
}
