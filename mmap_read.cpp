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
int main(int argc, char**argv)
{

    int sh = shm_open(shm_name, O_CREAT|O_RDWR, 0677)  ;
    
    char* a = (char*)mmap(NULL, SHARED_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, sh, 0);
    signal(SIGINT, handler) ;
    shm_add = a ;
    int i = 0 ;
    cout << "读数据:" << endl ;
    while(1)
    {
        cout << *(a+i) <<endl  ;
        i++ ;
        sleep(1) ;
    }
    munmap(shm_add, SHARED_SIZE) ;
    return 0;
}

void handler(int arg)
{
    munmap(shm_add, SHARED_SIZE) ;
}
