#include <iostream>
#include<pthread.h>
#include<unistd.h>
#include<semaphore.h>
using namespace std ;

void *producer( void* arg ) ;
void *consumer( void* arg ) ;
sem_t sem ;
int running = 1 ;

int main()
{

    pthread_t con_t ;
    pthread_t pro_t ;
    //信号量初始化
    sem_init( &sem, 0, 16 ) ;
    
    pthread_create( &pro_t ,NULL , producer , NULL ) ;
    pthread_create( &con_t ,NULL , consumer , NULL ) ;

    sleep(1) ;
    running = 0 ;

    pthread_join( pro_t , NULL );
    pthread_join( con_t , NULL );
    
    sem_destroy( &sem );

    return 0;
}

void *producer( void * arg )
{
    
    int semval = 0 ;
    while( running )
    {
        usleep(1) ;
        sem_post( &sem );//信号量增加
        sem_getvalue( &sem, &semval ) ;
        cout << "生产者数量:" << semval << endl ;
    }
    return NULL ;
}

void *consumer( void * arg )
{

    //信号量的初始值为0
    int semval = 0 ;
    while( running )
    {

        usleep(1) ;
        sem_wait( &sem ) ;//等待信号量
        sem_getvalue( &sem, &semval ) ;//获取信号量的值
        cout<<"消费总数量:"<<semval<<endl ;
    }
    return NULL ;
}
