/*
 * mmap_r.c
 *
 *  Created on: 2023年4月19日
 *      Author: karl
 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
typedef struct _Student{
	int sid;
	char sname[20];
} Student;


int main(int argc, char *argv[]) {
	int fd = open(argv[1],O_RDWR);
	int length = sizeof(Student);
	Student *stu = mmap(NULL,length,PROT_READ|PROT_WRITE,MAP_SHARED,fd ,0);
	if (stu ==MAP_FAILED) {
		printf("mmap error");
		return -1;
	}
	while(1) {
		printf("sid=%d,sname=%s\n",stu->sid,stu->sname);
		sleep(2);
	}
	munmap(stu,length);
	close(fd);
	return -1;
}



