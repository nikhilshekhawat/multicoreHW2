#include <mutex>
#include <thread>
#include <iostream>
#include <vector>
#include <functional>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define N 1000
#define keysLength 5
#define thread_num 5



std::mutex * mutexes;



template <typename T>
void lock(T * keys){
    int i,j;
    bool ret=true;
    while(true){
    for(i=0;i<keysLength;i++)
    {
        ret=mutexes[keys[i]].try_lock();
        if(ret==false)
            break;
    }
    if(ret==false)
    {
        for(;i>=0;i--)
            mutexes[keys[i]].unlock();
    }
    if(ret==true)
       { //std::cout << "locked\n";
        break;
        }
    }
}


template <typename T>
void unlock(T * keys){
    int i;
    for(i=0;i<keysLength;i++)
    {
       mutexes[keys[i]].unlock();
        //std::cout <<"unlocked"<<keys[i]<<"\n";
    }     
}
 

void* test(void *args){
    int * keys= (int *)args;
    lock(keys);
    usleep(rand()%200);
    unlock(keys);
    pthread_exit(0);
}

int main()
{
    
    int i,j;
    pthread_t *tid;
    int **keys;


    tid = (pthread_t *)malloc(sizeof(pthread_t)*thread_num);
    keys = (int**)malloc(sizeof(int *)*thread_num);
    for(i=0;i<thread_num;i++)
        keys[i]=(int*)malloc(sizeof(int)*keysLength);
    

   // keys = new int[thread_num][keysLength];
    for(j=0;j<thread_num;j++)
        for(i=0;i<keysLength;i++)
            {
                keys[j][i]=((i+1)*(j+1));
                //std::cout<<keys[j][i]<<"\n";
            }

    mutexes = new std::mutex[N];


    for (i = 0; i < thread_num; i++) 
    { 
        if(pthread_create( tid + i, 0, test, (void*)keys[i]) != 0) 
        { 
            perror("pthread_create() failure."); 
            exit(1); 
        } 
    } 


    lock(keys[0]);
    unlock(keys[0]);
    for (int i = 0; i < thread_num; i++)
       pthread_join(tid[i], NULL);
    delete[] keys;
    delete[] mutexes;
    std::cout<<"program completed successfully"<<"\n";
    return 1;
    


}
