#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>


#define PORT 20000
#define BACKLOG 5
#define LENGTH 512


typedef struct
{
	int sockfd;
	char buffer[LENGTH];
} thread_arg;

void handleSocket(void* arg)
{
	thread_arg t_arg = *((thread_arg*)arg);
	int nrecv;

	bzero(t_arg.buffer, LENGTH);

	while((nrecv = read(t_arg.sockfd, t_arg.buffer, 255)) > 0)
	{
		printf("The line length is %d\n", nrecv);
		if(t_arg.buffer[nrecv-1] == '\n')
		{
			t_arg.buffer[nrecv-1] = '\0';
		}

		printf("reveive message: %s\n", t_arg.buffer);
	}
}

int main(int argc, char* argv[])
{
	int sockfd;
	int nsockfd;
	int sin_size;
	struct sockaddr_in addr_local;
	struct sockaddr_in addr_remote;
	char revbuf[LENGTH];

	pthread_t tid;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		fprintf(stderr, "Fail to obtain socket descriptor && errno = %d\n", errno);
		exit(1);
	}
	else
	{
		printf("Obtain socket descriptor successfully!\n");
	}

	addr_local.sin_family = AF_INET;
	addr_local.sin_port = htons(PORT);
	addr_local.sin_addr.s_addr = INADDR_ANY;
	bzero(&addr_local.sin_zero, 8);
	
	//bind a port
	//int bind(int sockfd, const struct sockaddr *addr,
    //            socklen_t addrlen);

	if(bind(sockfd, (struct sockaddr*)&addr_local, sizeof(struct sockaddr)) == -1)
	{
		fprintf(stderr, "Fail to bind port && errno = %d\n", errno);
		exit(1);
	}
	else
	{
		printf("Bind port %d successfully!\n", PORT);
	}

	//listen remote connecting
	//int listen(int sockfd, int backlog);
	if(listen(sockfd, BACKLOG) == -1)
	{
		fprintf(stderr, "Fail to listen the port && errno = %d\n", errno);
		exit(1);
	}
	else
	{
		printf("Listening the port %d successfully!\n", PORT);
	}

	int success = 0;
	
	
	int stop = 0;

	while(success == 0)
	{
		sin_size = sizeof(struct sockaddr_in);

		//wait connection && obtain new socket file descriptor
		//int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
		if((nsockfd = accept(sockfd, (struct sockaddr*)&addr_remote, &sin_size))== -1)
		{
			fprintf(stderr, "Fail to btain a new file descriptor && errno = %d\n", errno);
			exit(1);

		}
		else
		{
			printf("Obtain a new file descriptor %d\n", nsockfd);
		}

		
		thread_arg arg;
		arg.sockfd = nsockfd;

		if(pthread_create(&tid, NULL, handleSocket, &arg) < 0)
		{
			printf("Fail to create new thread!\n");
			return 0;
		}
		else
		{
			printf("Create new thread!\n");
		}
		
	}

	return 0;

}