#include <mutex>
#include <thread>
#include <iostream>
#include <vector>
#include <functional>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <tbb/concurrent_unordered_set.h>
#include "bitset.h"
#include "mrlock.h"

#define N 1000
#define thread_num 5

using namespace tbb;

MRLock<Bitset> m(1000);

concurrent_unordered_set<int> set; 

void insertIfContains(int x,int y);
void deleteThenInsert(int x,int y);




template <typename T>
uint32_t lock(T * keys){
	int i;
	uint32_t unlockVar;
	Bitset  b;
	b.Resize(1000);
	
	for(i=0;i<thread_num;i++)
	{
		b.Set(keys[i]);
	}	
	unlockVar = m.Lock(b);
	return unlockVar;
}

uint32_t lock(int key1,int key2){
	int i;
	uint32_t unlockVar;
	Bitset  b;
	b.Resize(1000);
	
	
	b.Set(key1);
	
	b.Set(key2);	
	unlockVar = m.Lock(b);
	return unlockVar;
}


template <typename T>
void unlock(T un){
	int i;
		
	m.Unlock(un);
}


void* test(void *args){
    int * keys= (int *)args;
    int i,temp;

	for(i=0;i<thread_num;i++)
		{	do{
			temp=rand()%N;
			}while(temp==keys[i]);
			insertIfContains(temp,keys[i]);
			std::cout<<"IIC"<<temp<<"and"<<keys[i];	
		}	
	for(i=0;i<thread_num;i++)
		{
			do{
			temp=rand()%N;
			}while(temp==keys[i]);
			deleteThenInsert(temp,keys[i]);
		}	
    pthread_exit(0);
}







int main(){

	
    int i,j,count=1;
    pthread_t *tid;
    int **keys;
	
	//seeding the set
	for(i=0;i<6;i++)
	set.insert(i);
	
    tid = (pthread_t *)malloc(sizeof(pthread_t)*thread_num);

	keys = (int**)malloc(sizeof(int *)*thread_num);
    for(i=0;i<thread_num;i++)
        keys[i]=(int*)malloc(sizeof(int)*thread_num);

    for(j=0;j<thread_num;j++)
        for(i=0;i<thread_num;i++)
            {
                keys[j][i]=count++;
               // std::cout<<keys[j][i]<<"\n";
            }

    for (i = 0; i < thread_num; i++) 
    { 
        if(pthread_create( tid + i, 0, test, (void*)keys[i]) != 0) 
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



void insertIfContains(int x,int y){
	int un=lock(x,y);
	if((set.find(x))!=set.end())
		set.insert(y);

	unlock(un);

}


void deleteThenInsert(int x,int y){
	int un=lock(x,y);
	if(set.unsafe_erase(x))
		set.insert(y);

	unlock(un);

}
