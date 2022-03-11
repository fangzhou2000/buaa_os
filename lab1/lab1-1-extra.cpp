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

unsigned short my16(unsigned short a){
	unsigned short b = 0;
	int i;
	for (i = 0; i < 2; i++){
		b = b << 8;
		b = b | (a & 0x00ff );
		a = a >> 8;
	}
	return b;
}

unsigned int my32(unsigned int a){
	unsigned int b = 0;
	int i;
	for (i = 0; i < 4; i++){
		b = b << 8;
		b = b | (a & 0x000000ff );
		a = a >> 8;
	}
	return b;
}

int main()
{
	unsigned int a = 0x00ab00cd;
	printf("%x", my32(a));
	return 0;
}

