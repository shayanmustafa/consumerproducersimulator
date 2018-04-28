#include<iostream>
#include<pthread.h>
#include<semaphore.h>
#include<random>
#include<cstdlib>
#include<unistd.h>

using namespace std;

#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];
int i = 0;

pthread_mutex_t mutex;
sem_t fillCount;
sem_t emptyCount;

void* producer(void* arg){
	while(true){
		sleep(2);
		int item = rand()%100;
		sem_wait(&emptyCount);
		pthread_mutex_lock(&mutex);
		buffer[i++] = item;
		cout<<"Produced item: "<<item<<endl;
		pthread_mutex_unlock(&mutex);
		sem_post(&fillCount);	
	}
}

void* consumer(void* arg){
	while(true){
		sleep(2);
		sem_wait(&fillCount);
		pthread_mutex_lock(&mutex);
		int item = buffer[--i];
		cout<<"Consumed item: "<<item<<endl;
		pthread_mutex_unlock(&mutex);
		sem_post(&emptyCount);	
	}
}

int main(){
	pthread_t produce, consume;
	sem_init(&emptyCount,0,BUFFER_SIZE);
	sem_init(&fillCount,0,0);
	pthread_mutex_init(&mutex,NULL);
	pthread_create(&produce,NULL,producer,NULL);
	pthread_create(&consume,NULL,consumer,NULL);
	pthread_exit(NULL);
}
