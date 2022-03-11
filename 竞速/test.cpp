//#include "pageReplace.h"
#include<iostream>
#include<cstring>
#include<stdlib.h>
using namespace std;

#define MAX_PHY_PAGE 4
#define MAX_PAGE 12
#define get_Page(x) (x >> MAX_PAGE)

long physic_memery[MAX_PHY_PAGE];
#define init -1

void pageReplace_FIFO(long * physic_memery, long nwAdd) {
	int flag = 0;
	static int clock = 0;
	for (int i = 0; i < MAX_PHY_PAGE; i++) {
		if ((nwAdd >> MAX_PAGE) == physic_memery[i]) {
			return;
		}
	}
	for (int i = 0; i < MAX_PHY_PAGE; i++) {
		if (physic_memery[i] == init) {
			physic_memery[i] = get_Page(nwAdd);
			flag = 1;
			break;
		}
	}
	if (flag == 0) {
		physic_memery[(clock++) % MAX_PHY_PAGE] = get_Page(nwAdd);
	}
}

void swap(long *a,long *b)//传入时用&a，&b
{
    long temp;
    temp=*a;
    *a=*b;
    *b=temp;
}

void pageReplace_CONST(long * physic_memery, long nwAdd) {
	static long value[MAX_PHY_PAGE];
	static int ptr = 0;
	static int clock = 16;
	for (int i = 0; i < MAX_PHY_PAGE; i++) {
		if ((nwAdd >> MAX_PAGE) == physic_memery[i]) {
			value[i]++;
			if(ptr < 16 && value[i] > 10) {
				swap(&physic_memery[i], &physic_memery[ptr]);
				swap(&value[i], &value[ptr]);
				ptr++;
			}
			return;
		}
	}
	for (int i = 0; i < MAX_PHY_PAGE; i++) {
		if (physic_memery[i] == init) {
			physic_memery[i] = get_Page(nwAdd);
			value[i]++;
			return;
		}
	}
	physic_memery[clock++] = get_Page(nwAdd);
	if(clock == MAX_PHY_PAGE) {
		clock = ptr;
	}
}

void pageReplace_VALUE(long * physic_memery, long nwAdd) {
	long value[MAX_PHY_PAGE];
	static int ptr = 0;
	static int clock = 16;
	for (int i = 0; i < MAX_PHY_PAGE; i++) {
		if ((nwAdd >> MAX_PAGE) == physic_memery[i]) {
			value[i]++;
			while(i>0 && value[i] >= value[i - 1]) {
				swap(&physic_memery[i], &physic_memery[i - 1]);
				swap(&value[i], &value[i - 1]);
				i--;
			}
			return;
		}
	}
	for (int i = 0; i < MAX_PHY_PAGE; i++) {
		if (physic_memery[i] == init) {
			physic_memery[i] = get_Page(nwAdd);
			value[i]++;
			return;
		}
	}
	physic_memery[clock++] = get_Page(nwAdd);
	if(clock == MAX_PHY_PAGE) {
		clock = 16;
	}
}

void pageReplace_RANDOM(long * physic_memery, long nwAdd) {
	int flag = 0;
	static int clock = 0;
	for (int i = 0; i < MAX_PHY_PAGE; i++) {
		if ((nwAdd >> MAX_PAGE) == physic_memery[i]) {
			return;
		}
	}
	for (int i = 0; i < MAX_PHY_PAGE; i++) {
		if (physic_memery[i] == init) {
			physic_memery[i] = get_Page(nwAdd);
			flag = 1;
			break;
		}
	}
	if (flag == 0) {
		physic_memery[rand()%MAX_PHY_PAGE] = get_Page(nwAdd);
	}
}

void pageReplace_SECOND_CHANCE(long * physic_memery, long nwAdd) {
	static int chance[MAX_PHY_PAGE];
	static int clock = 0;
	for (int i = 0; i < MAX_PHY_PAGE; i++) {
		if ((nwAdd >> MAX_PAGE) == physic_memery[i]) {
			chance[i] = 1;
			return;
		}
	}
	for (int i = 0; i < MAX_PHY_PAGE; i++) {
		if (physic_memery[i] == init) {
			physic_memery[i] = get_Page(nwAdd);
			chance[i] = 1;
			return;
		}
	}
	int i = (clock++) % MAX_PHY_PAGE;
	while(chance[i] == 1 ) {
		chance[i] = 0;
		clock++;
		i = (clock++) % MAX_PHY_PAGE;
	}
	physic_memery[i] = get_Page(nwAdd);
}

void pageReplace_SECOND_CHANCE_RANDOM(long * physic_memery, long nwAdd) { 
	static int chance[MAX_PHY_PAGE];
	static int clock = 0;
	for (int i = 0; i < MAX_PHY_PAGE; i++) {
		if ((nwAdd >> MAX_PAGE) == physic_memery[i]) {
			chance[i] = 1;
			return;
		}
	}
	for (int i = 0; i < MAX_PHY_PAGE; i++) {
		if (physic_memery[i] == init) {
			physic_memery[i] = get_Page(nwAdd);
			chance[i] = 0;
			return;
		}
	}
	clock = rand()%64;
	while (clock == 1) {
		chance[i] = 0;
		clock = rand()%64;
	}
	physic_memery[clock] = get_Page(nwAdd);
}

void pageReplace_SECOND_CHANCE_PLUS(long * physic_memery, long nwAdd) { 
	static int chance[MAX_PHY_PAGE];
	static int clock = 0;
	for (int i = 0; i < MAX_PHY_PAGE; i++) {
		if ((nwAdd >> MAX_PAGE) == physic_memery[i]) {
			chance[i] = 1;
			if (physic_memery[MAX_PHY_PAGE - 1] == init) {
				return;
			} 
			long temp = physic_memery[i];
			int length = MAX_PHY_PAGE;
			int j;
			for(j = i ; physic_memery[j] != init && j != MAX_PHY_PAGE - 1; j++);
			length = (j == MAX_PHY_PAGE -1 ) ? MAX_PHY_PAGE : j;
			if(i >= clock) {
				if (clock > 0) {
					for (j = i; j < length - 1; j++) {
						physic_memery[j] = physic_memery[j + 1];
					}
					physic_memery[length - 1] = physic_memery[0];
					for (j = 0; j < clock - 1; j++) {
						physic_memery[j] = physic_memery[j + 1];
					}
					physic_memery[clock - 1] = temp;
				} else {
					for (j = i; j < length - 1; j++) {
						physic_memery[j] = physic_memery[j + 1];
					}
					physic_memery[length - 1] = temp;
				}
			} else {
				for (j = i; j < clock - 1; j++) {
					physic_memery[j] = physic_memery[j + 1];
				}
				physic_memery[clock - 1] = temp;
			}
			return;
		}
	}
	for (int i = 0; i < MAX_PHY_PAGE; i++) {
		if (physic_memery[i] == init) {
			physic_memery[i] = get_Page(nwAdd);
			chance[i] = 1;
			return;
		}
	}
	while(chance[clock] == 1 ) {
		chance[clock] = 0;
		clock++;
		if (clock >= MAX_PHY_PAGE) clock = 0;
	}
	physic_memery[clock] = get_Page(nwAdd);
}

void pageReplace_LRU(long * physic_memery, long nwAdd) {
	int flag = 0;
	static int clock = 0;
	for (int i = 0; i < MAX_PHY_PAGE; i++) {
		if ((nwAdd >> MAX_PAGE) == physic_memery[i]) {
			if (physic_memery[MAX_PHY_PAGE - 1] == init) {
				return;
			} 
			long temp = physic_memery[i];
			int length = MAX_PHY_PAGE;
			int j;
			for(j = i ; physic_memery[j] != init && j != MAX_PHY_PAGE - 1; j++);
			length = (j == MAX_PHY_PAGE -1 ) ? MAX_PHY_PAGE : j;
			if(i >= clock) {
				if (clock > 0) {
					for (j = i; j < length - 1; j++) {
						physic_memery[j] = physic_memery[j + 1];
					}
					physic_memery[length - 1] = physic_memery[0];
					for (j = 0; j < clock - 1; j++) {
						physic_memery[j] = physic_memery[j + 1];
					}
					physic_memery[clock - 1] = temp;
				} else {
					for (j = i; j < length - 1; j++) {
						physic_memery[j] = physic_memery[j + 1];
					}
					physic_memery[length - 1] = temp;
				}
			} else {
				for (j = i; j < clock - 1; j++) {
					physic_memery[j] = physic_memery[j + 1];
				}
				physic_memery[clock - 1] = temp;
			}
			return;
		}
	}
	for (int i = 0; i < MAX_PHY_PAGE; i++) {
		if (physic_memery[i] == init) {
			physic_memery[i] = get_Page(nwAdd);
			return;
		}
	}
	physic_memery[clock] = get_Page(nwAdd);
	clock++;
	if (clock == MAX_PHY_PAGE) {
		clock = 0;
	}
}



long data[100000000];
long oldhas[100000000];
long has[100000000];
long cost = 0;
long change = 2;

int main(){
	long i = 0;
	for(long j = 0; j < MAX_PHY_PAGE; j++) {
		physic_memery[j] = init; 
	}
	while(~scanf("%d", &data[i])) {
		memset(oldhas, 0, sizeof(long));
		for(long j = 0; j < MAX_PHY_PAGE; j++) {
			if (physic_memery[j] != init) {
				oldhas[physic_memery[j]] = 1;
			}	
		}
		
		//pageReplace_FIFO(physic_memery, data[i]);
		//pageReplace_LRU(physic_memery, data[i]);
		//pageReplace_SECOND_CHANCE(physic_memery, data[i]);
		//pageReplace_RANDOM(physic_memery, data[i]);
		pageReplace_VALUE(physic_memery, data[i]);
		
		for(long j = 0; j < MAX_PHY_PAGE; j++) {
			cout << physic_memery[j] << " "; 
		}
		cout << endl; 
		
		long interput = 1;
		long time = 0;
		memset(has, 0, sizeof(long));
		for(long j = 0; j < MAX_PHY_PAGE; j++) {
			if (physic_memery[j] != init) {
				has[physic_memery[j]] = 1;
			}	
		}
		
		for(int j = 0; j < MAX_PHY_PAGE; j++) {
			if(physic_memery[j] != init) {
				if(has[physic_memery[j]] != oldhas[physic_memery[j]]){
					time++;
				}
			}
		}
		
		if (time == 0) {
			interput = 0;
		}
		
		cost += interput + change * time;
		cout << "cost += " << interput << " + " << change << " * " << time << " = " << cost <<endl;
 		
		i++;	
	}
	cout << cost << endl;
	
}
