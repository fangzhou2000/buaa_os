#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#define eps 1e-12
void swap(int *a,int *b)//????ʱ??&a??&b
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

struct test {
	unsigned int a[100];
}; 

int main()
{
    struct test t;
	printf("%d", sizeof(t));  
	return 0;
}

