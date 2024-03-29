#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
pthread_t tid1, tid2;
int status;
int nomor;
void* tulis(void *arg)
{
    status = 0;
    printf("Masukan nomor : ");
    scanf("%d", &nomor);
    status = 1;
    return NULL;
}
void* baca(void *arg)
{
    while(status != 1)
    {
    }
    printf("Nomor %d\n", nomor);
}
int main(void)
{
    pthread_create(&(tid1), NULL, tulis, NULL);
    pthread_create(&(tid2), NULL, baca, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    return 0;
}