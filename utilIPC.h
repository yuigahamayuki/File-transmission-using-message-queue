#ifndef _UTILIPC_H_
#define _UTILIPC_H_

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <mqueue.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXFILESIZE 12 * 1024
struct Request {
	short pid;
	char requestFileName[18];
};

struct Response {
	int size;
	char responseFile[MAXFILESIZE];
};

void err_exit(const char *str);
mqd_t Mq_open(const char *name, int oflag, mode_t mode, struct mq_attr *attr);
void Mq_close(mqd_t mqd);
void Mq_unlink(const char *name);
void Mq_getattr(mqd_t mqd, struct mq_attr *attr);
void Mq_send(mqd_t mqd, const char *ptr, size_t len, unsigned int prio);
ssize_t Mq_receive(mqd_t mqd, char *ptr, size_t len, unsigned int *prio);
void Mq_notify(mqd_t mqd, const struct sigevent *sevp);
ssize_t Read(int fd, void *buf, size_t count);
ssize_t Write(int fd, const void *buf, size_t count);
int Open(const char *pathname, int flags, mode_t mode);
#endif //utilIPC.h
