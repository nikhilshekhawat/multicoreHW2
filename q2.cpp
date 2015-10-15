#include <mutex>
#include <thread>
#include <iostream>
#include <vector>
#include <functional>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "bitset.h"
#include "mrlock.h"


#define thread_num 5


template <typename T>
void lock(T * keys){
	Bitset * b = new Bitset();
	MRLock<uint32_t> * m = new MRLock<uint32_t>(b);
}



template <typename T>
void unlock(T * keys){

}

void* test(void *args){
    int * keys= (int *)args;
    lock(keys);
    usleep(rand()%200);
    unlock(keys);
    pthread_exit(0);
}


int main(){

	
    int i,j;
    pthread_t *tid;


    tid = (pthread_t *)malloc(sizeof(pthread_t)*thread_num);

    for (i = 0; i < thread_num; i++) 
    { 
        if(pthread_create( tid + i, 0, test, (void*)1) != 0) 
        { 
            perror("pthread_create() failure."); 
            exit(1); 
        } 
    } 


    for (int i = 0; i < thread_num; i++)
       pthread_join(tid[i], NULL);
    
    std::cout<<"program completed successfully"<<"\n";
    return 1;
}

