#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#define eps 1e-12
void swap(int *a,int *b)//传入时用&a，&b
{
    int temp;
    temp=*a;
    *a=*b;
    *b=temp;
}

int compare(const void*a,const void*b)
{
    if(*(int*)a>*(int*)b)return 1;
    else if(*(int*)a<*(int*)b)return -1;
    else return 0;
}

u_char check_buf[2*BY2PG];

int get_checksum(const char *path) {
	int r, i, j;
	struct Fd* fd;
	struct Filefd* ffd;
	int size;
	u_char ans = 0;
	r = open(path, O_RDONLY);
	if (r < 0) {
		return r;
		writef("1\n");
	}
	fd = num2fd(r);
	ffd = (struct Filefd *)fd;
	size = ffd->f_file.f_size;
	for (i = 0; i < size; i += BY2PG) {
		int n;
		u_char nowv = 0;
		n = file_read(fd, check_buf, BY2PG, i);
		for (j = 0; j < n; j++) {
			nowv = nowv + check_buf[j];
		}
		ans = ans ^ (~nowv);
	} 
	ffd->f_file.f_checksum = ans;
	return fd2num(fd);
} 

int main()
{
	unsigned char nowv = 0;
	nowv = 256;
	printf("%d", nowv);
	return 0;
}

