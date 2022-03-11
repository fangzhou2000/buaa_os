#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
void *thread_function(void* arg);
int main()
{
        int result;
        //定义两个线程标示符指针 乌龟和兔子
        pthread_t rabbit,tortoise;
        //使用pthread_create创建两个线程,并传入距离值50m
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
        //主控线程调用pthreat_join(),自己会阻塞
        //直到rabbit和tortoise线程结束，方可运行
        /*如果将pthread_join()和sleep(10)都屏蔽了，可查看程序运行的结果。*/
        pthread_join(rabbit,NULL);
        pthread_join(tortoise,NULL);
        //sleep(10);//休眠秒的时间
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
                int time=(int)(srand48()*100000);//产生微妙的随机时间
                usleep(time);//休眠us时间
        }
        return (void*)0;//运行完成之后返回0 表示结束
}
    

