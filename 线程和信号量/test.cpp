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

int a = 0;

void fun() {
	printf("%d", a);
}
int main()
{
	int a = 1;
	fun();
	return 0;
}

