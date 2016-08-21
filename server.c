#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>

#define PORT 2000
#define MAX_CLIENTS 100

static unsigned int cli_count = 0;
static int uid = 10;

/* Client structure */
typedef struct {
    struct sockaddr_in addr;
    int connfd;
    int uid;
    char name[32];
} client_t;

client_t *clients[MAX_CLIENTS];

/* Handle all communication with the client */
void *client_process(void *arg){
    char buff_out[1024];
    char buff_in[1024];
    int rlen;

    cli_count++;
    client_t *cli = (client_t *)arg;

    printf("Accepting connection");
    pthread_detach(pthread_self());

    return NULL;
}

int main(int argc, char *argv[]){
    int listenfd = 0, connfd = 0;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    pthread_t tid;

    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        perror("Error creating Socket");
        exit(errno);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    if(bind(listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0){
        perror("Error binding Socket");
        exit(errno);
    }

    if(listen(listenfd, 10) < 0){
        perror("Error listening Socket");
        exit(errno);
    }

    printf("Server started\n");

    while(1){
        socklen_t clilen = sizeof(client_addr);
        connfd = accept(listenfd, (struct sockaddr*)&client_addr, &clilen);

        if((cli_count+1) == MAX_CLIENTS){
            printf("Max clients reached\n");
            printf("Rejecting");
            printf("\n");
            close(connfd);
            continue;
        }

        /* Client settings */
        client_t *cli = (client_t *)malloc(sizeof(client_t));
        cli->addr = client_addr;
        cli->connfd = connfd;
        cli->uid = uid++;
        sprintf(cli->name, "%d", cli->uid);

        /* Add client to the queue and fork thread */
        // queue_add(cli);
        pthread_create(&tid, NULL, &client_process, (void*)cli);

        /* Reduce CPU usage */
        sleep(1);
    }
}


















