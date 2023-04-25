#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <binder/ProcessState.h>

#include <gui/SurfaceComposerClient.h>
#include <gui/ISurfaceComposer.h>

#include <ui/DisplayInfo.h>
#include <ui/PixelFormat.h>

#include <system/graphics.h>

#include <cutils/sockets.h>

int main() {
	int clientfd;
	int ret;
	fprintf(stderr, "==========================client start\n");
	clientfd = socket(AF_LOCAL, SOCK_STREAM, 0);
	if (-1 == clientfd) {
		fprintf(stderr, "==========================socket = -1\n");
	}
	sockaddr_un clientaddr;
	bzero(&clientaddr, sizeof(clientaddr));
	strcpy(clientaddr.sun_path + 1, "/tmp/mysocket");
	clientaddr.sun_family = AF_LOCAL;
	socklen_t addrlen = 1 + strlen("/tmp/mysocket")
			+ sizeof(clientaddr.sun_family);
	ret = connect(clientfd, (struct sockaddr *) &clientaddr, addrlen);
	if (-1 == ret) {
		fprintf(stderr, "==========================ret = -1\n");
	}
	const char* msg = "client send hello\n";
	write(clientfd, msg, strlen(msg));
	char buf[100];
	int read_len = read(clientfd, &buf, sizeof(buf));
	fprintf(stderr, "=========================client read %d,%s \n", read_len, buf);
	return -1;
}

