#include "utilIPC.h"

void err_exit(const char *str)
{
	printf("%s: %s\n", str, strerror(errno));
	exit(-1);
}
mqd_t Mq_open(const char *name, int oflag, mode_t mode, struct mq_attr *attr)
{
	mqd_t mqd;
	mqd = mq_open(name, oflag, mode, attr);
	if (mqd == -1)
		err_exit("mq_open error");

	return mqd;
}

void Mq_close(mqd_t mqd)
{
	int res = mq_close(mqd);
	if (res == -1)
		err_exit("mq_close error");

	return;
}

void Mq_unlink(const char *name)
{
	int res = mq_unlink(name);
	if (res == -1)
		err_exit("mq_unlink error");

	return;
}

void Mq_getattr(mqd_t mqd, struct mq_attr *attr)
{
	int res = mq_getattr(mqd, attr);
	if (res == -1)
		err_exit("mq_getattr error");

	return;
}
void Mq_send(mqd_t mqd, const char *ptr, size_t len, unsigned int prio)
{
	int res = mq_send(mqd, ptr, len, prio);
	if (res == -1 && errno != EAGAIN)
		err_exit("mq_send error");

	return;
}

ssize_t Mq_receive(mqd_t mqd, char *ptr, size_t len, unsigned int *prio)
{
	ssize_t n = mq_receive(mqd, ptr, len, prio);
	if (n == -1 && errno != EAGAIN)
		err_exit("mq_receive error");

	return n;
}

void Mq_notify(mqd_t mqd, const struct sigevent *sevp)
{
	int res = mq_notify(mqd, sevp);
	if (res == -1)
		err_exit("mq_notify error");

	return;
}

ssize_t Read(int fd, void *buf, size_t count)
{
	ssize_t n = read(fd, buf, count);
	if (n == -1 && errno != EWOULDBLOCK)
		err_exit("read error");

	return n;
}

ssize_t Write(int fd, const void *buf, size_t count)
{
	ssize_t n = write(fd, buf, count);
	if (n == -1 && errno != EWOULDBLOCK)
		err_exit("write error");

	return n;
}

int Open(const char *pathname, int flags, mode_t mode)
{
	int fd = open(pathname, flags, mode);
	if (fd == -1)
		err_exit("open error");

	return fd;
}
