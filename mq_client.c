#include "utilIPC.h"
#include <signal.h>
#include <sys/epoll.h>

int pipefd[2];
void signalHandler(int signo);
#define MAXEVENTNUM 16

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("usage: mq_client <fileName>\n");
		exit(-1);
	}
	mqd_t mqd = Mq_open("/FileTrans", O_WRONLY, 0, NULL); //don't create, write only
	struct Request request;
	request.pid = getpid();
	memset(request.requestFileName, 0, sizeof(request.requestFileName));//set every character to '\0'
	strcpy(request.requestFileName, argv[1]);

	char name[20];
	sprintf(name, "/FileTrans_%d", request.pid);
	/*struct mq_attr attr;
	attr.mq_msgsize = (long)sizeof(struct Response);
	attr.mq_maxmsg = 1;*/

	mqd_t mqd1 = Mq_open(name, O_RDONLY | O_CREAT, S_IRWXU, NULL);//create, read only

	pipe(pipefd);	
	
	signal(SIGUSR1, signalHandler);
	struct sigevent sigev;
	sigev.sigev_notify = SIGEV_SIGNAL;//notification method: signal
	sigev.sigev_signo = SIGUSR1;

	Mq_notify(mqd1, &sigev); //register to be notified when message comes

	int epfd = epoll_create(5);
	struct epoll_event event;
	event.data.fd = pipefd[0];
	event.events = EPOLLIN;
	epoll_ctl(epfd, EPOLL_CTL_ADD, pipefd[0], &event);

	struct epoll_event events[MAXEVENTNUM];

	Mq_send(mqd, (char*)&request, sizeof(request), 0); //priority set 0

	int nready = 0;
	while((nready = epoll_wait(epfd, events, MAXEVENTNUM, -1)) <= 0);
	int i;
	printf("ready: %d\n", nready);
	if (nready > 0)
	{
		for (i = 0; i < nready; ++i)
		{
			Mq_notify(mqd1, &sigev); // reregister first
			struct Response response;
			Mq_receive(mqd1, (char*)&response, sizeof(response), NULL);

			char fileName[20];
			sprintf(fileName, "%d%s", request.pid, argv[1]);
			int fd = Open(fileName, O_WRONLY | O_CREAT | O_EXCL, S_IRWXU);//create a file to store the content
			Write(fd, response.responseFile, response.size);
			close(fd);
		}
	}

	
	Mq_close(mqd1);
	Mq_unlink(name);
	return 0;
}

void signalHandler(int signo)
{
	Write(pipefd[1], "", 1);
	return;
}
