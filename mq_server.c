#include "utilIPC.h"

int main()
{
	struct mq_attr attr;
	attr.mq_msgsize = (long)sizeof(struct Request);
	attr.mq_maxmsg = 1;
	mqd_t mqd = Mq_open("/FileTrans", O_RDONLY | O_CREAT, S_IRWXU, &attr);// create, read only, stores client's pid and requested file name 

	pid_t pid;
	while(1)
	{
		struct Request request;
		Mq_receive(mqd, (char *)&request, sizeof(request), NULL); //if no request, block here
		if ((pid = fork()) == 0)
		{
			const char *fileName = request.requestFileName;
			int fd = Open(fileName, O_RDONLY, 0);//don't create, read only
			int fileSize = lseek(fd, 0, SEEK_END);
			lseek(fd, 0, SEEK_SET);
			//char fileBuf[MAXFILESIZE] = {'\0'};
			struct Response response;
			response.size = fileSize;
			memset(response.responseFile, 0, sizeof(response.responseFile));//set every character to '\0'
			Read(fd, response.responseFile, fileSize);
			//Read(fd, fileBuf, fileSize);
			close(fd);

			char name[20];
			sprintf(name, "/FileTrans_%d", request.pid);
			mqd_t mqd1 = Mq_open(name, O_WRONLY, 0, NULL); //don't create, write only
			Mq_send(mqd1, (char*)&response, fileSize + sizeof(int), 0); //priority set 0			
			exit(0);
		}
	}

	Mq_close(mqd);
	Mq_unlink("/FileTrans"); //delete the corresponding mq file
	return 0;
}
