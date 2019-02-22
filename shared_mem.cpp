#include <iostream>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include"sys.h"
using namespace std ;

//共享内存:多个进程间共享内存区域的一种进程通信方式,在多个
//进程间对内存段进行映射的方式实现内存共享的,共享内存是IPC
//最快捷的方式,因为没有中间过程
//多个进程间的共享内存是同一块物理空间,仅仅是地址不同而已
//,不需要进行复制,直接使用此段空间

typedef int sem_t ;

union semun{

    int val ;//整型变量
    struct semid_ds *buf ; //semid_ds 结构体指针
    unsigned short *arr ; 
}args;

sem_t CreateSem( key_t key, int value )
{
    union semun sem ; //信号量结构体变量
    sem_t semid ;     //信号量ID
    sem.val = value ;    //设置初始值
    semid = semget( key, 0, IPC_CREAT|0666 ) ;

    if( -1 == semid )
    {
        cout << "create sem error!" << endl ;
        return -1 ;
    }
    semctl( semid, 0, SETVAL, sem ) ;//发送命令,建立value个值的信号量
    return semid ;//返回建立的信号量
}

//添加信号量
int Sem_P( sem_t semid )
{
    //建立信号量结构值
    struct sembuf sops = { 0, +1,IPC_NOWAIT } ;
    //发送命令
    return semop( semid, &sops, 1 ) ;

}

//减小信号量
int Sem_V( sem_t semid )
{

    struct sembuf sops = { 0, -1, IPC_NOWAIT } ;
    
    return semop( semid, &sops, 1 ) ;

}

//销毁信号
void DestroySem( sem_t semid )
{
    union semun sem ;
    sem.val = 0 ;
    semctl( semid, 0, IPC_RMID, sem ) ;
}
static char msg[] = "hello , shared memory!\n" ;
int main()
{   
    
    key_t key ;
    int semid , shmid ;
    char i, *shms ,*shmc ;
    struct semid_ds buf ;
    int value = 0 ;
    char buffer[80] ;
    pid_t p ;

    key = ftok("/ipc/sem" , 5) ;
    
    shmid = shmget( key, 1024, IPC_CREAT|0604 ) ;
    cout<<key<<endl;
    semid = CreateSem( key, 0 ) ;

    p = fork() ;
    
    if( p < 0 )
    {
        cout << "进程创建失败" << endl ;
        exit(1)  ;
    }

    if( p > 0 )
    {
        shms = ( char* )shmat( shmid, 0, 0 ) ;
        memcpy( shms, msg, strlen(msg)+1 ) ;
        sleep(10) ;
        Sem_P(semid) ; //获得共享内存的信号量
        shmdt( shms ) ; //摘除共享内存
        DestroySem( semid ) ;//销毁信号量
    }
    else if( p == 0 )
    {
        //挂接共享内存
        shmc = ( char*) shmat( shmid, 0, 0 ) ;
        //减少信号量
        Sem_V( semid ) ;
        cout << "共享内存的值为:"<< shmc<< endl  ;
        shmdt( shmc ) ;
    }
    return 0;
}

