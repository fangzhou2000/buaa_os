#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
void *thread_function(void* arg);
int main()
{
        int result;
        //���������̱߳�ʾ��ָ�� �ڹ������
        pthread_t rabbit,tortoise;
        //ʹ��pthread_create���������߳�,���������ֵ50m
        result=pthread_create(&rabbit,NULL,thread_function,(void*)50);
        if(result!=0)
        {
                perror("pthrad creat fail!\r\n");
                exit(EXIT_FAILURE);
        }
        result=pthread_create(&tortoise,NULL,thread_function,(void*)50);
        if(result!=0)
        {
                perror("pthrad creat fail!\r\n");
                exit(EXIT_FAILURE);
        }
        //�����̵߳���pthreat_join(),�Լ�������
        //ֱ��rabbit��tortoise�߳̽�������������
        /*�����pthread_join()��sleep(10)�������ˣ��ɲ鿴�������еĽ����*/
        pthread_join(rabbit,NULL);
        pthread_join(tortoise,NULL);
        //sleep(10);//�������ʱ��
        printf("Control thread_id=%lx\r\n",pthread_self());
        printf("Finish\r\n");
        return 0;
}
void *thread_function(void *arg)
{
        int i;
        int distance= (int *) arg;
        for(i=0;i<=distance;i++)
        {
                printf("p_id=%lx,Run=%d\r\n",pthread_self(),i);
                int time=(int)(srand48()*100000);//����΢������ʱ��
                usleep(time);//����usʱ��
        }
        return (void*)0;//�������֮�󷵻�0 ��ʾ����
}
    

