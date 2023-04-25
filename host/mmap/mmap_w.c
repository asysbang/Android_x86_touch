/*
 * mmap_w.c
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
	if (argc !=2) {
		printf("./a.out filename \n");
		return -1;
	}
	int fd = open(argv[1],O_RDWR|O_CREAT|O_TRUNC,0666);
	int length = sizeof(Student);
	ftruncate(fd,length);
	Student *stu = mmap(NULL,length,PROT_READ|PROT_WRITE,MAP_SHARED,fd ,0);
	if (stu ==MAP_FAILED) {
		printf("mmap error");
		return -1;
	}
	int num =1;
	while(1) {
		stu->sid=num;
		sprintf(stu->sname,"from mmap write-%03d",num++);
		sleep(2);
	}
	munmap(stu,length);
	close(fd);
	return 0;
}



