#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define PORT 20000
#define LENGTH 512

int my_write(int fd, void* buffer, int length)
{

}

int main(int argc, char* argv[])
{
	int sockfd;
	char recvbuf[LENGTH];
	struct sockaddr_in remote_addr;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		fprintf(stderr, "Fail to obtain socket descriptor && errno = %d\n", errno);
		exit(1);
	}
	else
	{
		printf("Obtain socket descriptor successfully!\n");
	}

	remote_addr.sin_family = AF_INET;
	remote_addr.sin_port = htons(PORT);
	inet_pton(AF_INET, "127.0.0.1", &remote_addr.sin_addr);
	bzero(&(remote_addr.sin_zero), 8);

	//connect the remote
	if(connect(sockfd, (struct sockaddr*)&remote_addr, sizeof(struct sockaddr)) == -1)
	{
		fprintf(stderr, "Fail to connect the remote host && errno = %d\n", errno);
		exit(1);
	}
	else
	{
		printf("[Client] Connected to server at port %d...ok!\n", PORT);
	}

	//write(sockfd, "Hello Again!", LENGTH);
	printf("Press Q to quit!\n");

	while(fgets(recvbuf, LENGTH - 1, stdin) != NULL)
	{
		if(strlen(recvbuf) > 1)
		{
			write(sockfd, recvbuf, strlen(recvbuf));
		}
		
	}


}