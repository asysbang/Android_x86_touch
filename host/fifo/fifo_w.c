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
	if(argc != 2) {
		printf("no filename found\n");
		return 0;
	}
	int ret  = mkfifo(argv[1],0666);
	if(ret < 0 ) {
		printf("create fifo failed %s\n",argv[1]);
		return -1;
	}
	printf("begin open \n");
	int fd = open(argv[1],O_WRONLY);
	printf("end open \n");
	char buf[256];
	int num =1;
	while(1){
		memset(buf,0x00,sizeof(buf));
		sprintf(buf,"from write data:%04d",num++);
		write(fd,buf,strlen(buf));
		sleep(2);
	}
	close(fd);
	return 0;
}



