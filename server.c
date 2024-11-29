
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

struct message {
	char source[50];
	char target[50]; 
	char msg[200]; // message body
};

void terminate(int sig) {
	printf("Exiting....\n");
	fflush(stdout);
	exit(0);
}

int main() {
	int server;
	int target;
	int dummyfd;
	struct message req;
	signal(SIGPIPE,SIG_IGN);
	signal(SIGINT,terminate);
	server = open("serverFIFO",O_RDONLY);
	dummyfd = open("serverFIFO",O_WRONLY);

	while (1) {
		// TODO:
		// read requests from serverFIFO
		// Read requests from serverFIFO
		ssize_t bytesRead = read(server, &req, sizeof(req));
		if (bytesRead != sizeof(req)) {
			if (bytesRead == 0) {
				// EOF: Server FIFO was closed
				printf("Server FIFO closed. Exiting...\n");
				break;
			}
			perror("Failed to read from serverFIFO");
			continue;
		}
		printf("Received a request from %s to send the message %s to %s.\n",req.source,req.msg,req.target);

		// TODO:
		// open target FIFO and write the whole message struct to the target FIFO
		// close target FIFO after writing the message
		// Open target FIFO and write the message struct to it
		char targetFIFO[256];
		snprintf(targetFIFO, sizeof(targetFIFO), "%s_fifo", req.target);

		target = open(targetFIFO, O_WRONLY);
		if (target == -1) {
			perror("Failed to open target FIFO");
			continue;
		}

		if (write(target, &req, sizeof(req)) == -1) {
			perror("Failed to write to target FIFO");
		} else {
			printf("Message successfully sent to %s.\n", req.target);
		}

		close(target);
	}
	close(server);
	close(dummyfd);
	return 0;
}

