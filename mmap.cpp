#include <iostream>
#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<assert.h>
#include<sys/mman.h>
#include<memory.h>
#include"socketfd.h"

using namespace std ;
#define SHARED_SIZE 120
const char*shm_name = "/myshm" ;
char* shm_add ;
void handler(int arg) ;

int main()
{

    cout << "创建共享内存" << endl ;
    //使用shm_open 来创建共享内存的对象，使用方法和open相同
    int sh = shm_open(shm_name, O_CREAT|O_RDWR|O_TRUNC, 0677)  ;

    //指定共享内存对象控制的空间大小
    ftruncate(sh, SHARED_SIZE) ;
    
    //将所创建的共享内存对象映射到内存页上
    char* a = (char*)mmap(NULL, SHARED_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, sh, 0);
    memset(a, SHARED_SIZE, '\0') ;
    shm_add = a ;
    assert(a != MAP_FAILED) ;
    //关掉文件
    close(sh) ;
    //往共享内存中写数据
    const char* shm_data = "hello_world!" ;
    int len = strlen(shm_data) ;
    signal(SIGINT, handler) ;

    cout <<"写数据："<<"" ;
    int i =0 ;
    for(;;)
    {
        memcpy(a, shm_data+i, len) ;
        cout <<*a<<endl ; 
        sleep(1) ;
        i++ ;
        if(i == len)break ;
    }
    munmap(shm_add, SHARED_SIZE) ;
}

void handler(int arg)
{
    munmap(shm_add, SHARED_SIZE) ;
}
