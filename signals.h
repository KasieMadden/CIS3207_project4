#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <signal.h>


using namespace std;

void timeHandler();
int randomGenerator(int min, int max);
void signalGenerator();
void mutexInit();

//actually counters
typedef struct shMemory{
    int test = 0 ;
    int sigUser1SentCount = 0;
    int sigUser2SentCount = 0;
    int sigUser1reciveCount = 0;
    int sigUser2reciveCount = 0;
};

//golbal locks for the counters  for 2 signals both sent and recive 
pthread_mutex_t sigUser1SentLock;
pthread_mutex_t sigUser2SentLock;
pthread_mutex_t sigUser1reciveLock;
pthread_mutex_t sigUser2reciveLock;
