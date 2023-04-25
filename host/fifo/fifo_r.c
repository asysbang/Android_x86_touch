/*
 * fifo_w.c
 *
 *  Created on: 2023年4月20日
 *      Author: karl
 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <error.h>
#include <string.h>
int main(int argc, char *argv[]) {
	if(argc !=2){
		printf("no file name found\n");
		return 0;
	}
	printf("open to read\n");
	int fd = open(argv[1],O_RDONLY);
	printf("end open\n");
	char buf[256];
	int ret;
	while(1){
		memset(buf,0x00,sizeof(buf));
		ret = read(fd,buf,sizeof(buf));
		if(ret>0) {
			printf("read : %s\n",buf);
		}
	}
	close(fd);
	return 0;
}



