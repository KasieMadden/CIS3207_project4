#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <signal.h>


using namespace std;

void timeHandler();
int randomGenerator(int min, int max);
void signalGenerator();
void mutexInit();
void signal1();
void signal2();
void sig1Handler(int theSignal);
void sig2Handler(int theSignal);
void blockSignal(int theSignal);
void unblockSignal(int theSignal);

//actually counters
typedef struct shMemory{
    
    int sigUser1SentCount = 0;
    int sigUser2SentCount = 0;
    int sigUser1reciveCount = 0;
    int sigUser2reciveCount = 0;
    int allSignalsSent = 0;
    int allSignalsRecived = 0;

};

//global memory
shMemory *sharedM;

//golbal locks for the counters  for 2 signals both sent and recive 
pthread_mutex_t sigUser1SentLock;
pthread_mutex_t sigUser2SentLock;
pthread_mutex_t sigUser1reciveLock;
pthread_mutex_t sigUser2reciveLock;
