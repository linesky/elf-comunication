#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
struct shm{
	int sem1;
	int sem2;
	size_t lens;
	char buf[2080];
};
int main(){
	char *shmp="hello100";
	char runs[80];
	int fd = shm_open(shmp,O_CREAT | O_EXCL | O_RDWR,S_IRUSR | S_IWUSR);
	if (fd==-1)exit(1);
	if (ftruncate(fd,sizeof(struct shm))==-1)exit(1);
	struct shm *shm1=mmap(NULL,sizeof(*shm1),PROT_WRITE | PROT_READ ,MAP_SHARED,fd,0);
	if (shm1==MAP_FAILED)goto exitss;
	shm1->sem1=0;
	shm1->sem2=0;
	strcpy(shm1->buf,"hello world...\n");
	puts("\ec\e[47;30m\n");
	puts(shm1->buf);
	puts("enter father\n");
	strcpy(runs,"./shm2 ");
	strcat(runs,shmp);
	strcat(runs," & ");
	puts(runs);
	system(runs);
	while(1){
		if (shm1->sem1==1)break;
	}
	exitss:
	puts("exit father");
	shm_unlink(shmp);
	return 0;
}
