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

#define TEST_SIGNAL_COUNT 200

using namespace std;


int randomGenerator(int min, int max);
void randomSleep();
void signalGenerator();
void mutexInit();
void signal1();
void signal2();
void sig1Handler(int theSignal);
void sig2Handler(int theSignal);
void report();
void reportHandler(int theSignal);



//actually counters
typedef struct shMemory{
    
    int sigUser1SentCount = 0;
    int sigUser2SentCount = 0;
    int sigUser1receiveCount = 0;
    int sigUser2receiveCount = 0;
    int allSignalsSent = 0;
    int allSignalsReceived = 0;


}shMemory;

//global crap
shMemory *sharedM;

//for time in report handler 
double signal1count = 0; 
double signal2count = 0; 
double signal1past = 0; 
double signal2past = 0;
double elapsedTime = 0;
double avgTime1 = 0;
double avgTime2 = 0; 
double timeSum1 = 0;
double timeSum2 = 0;
int counter = 0;
bool endProcess = false;

timeval startTime;



//golbal locks for the counters  for 2 signals both sent and recive 
pthread_mutex_t sigUser1SentLock;
pthread_mutex_t sigUser2SentLock;
pthread_mutex_t sigUser1receiveLock;
pthread_mutex_t sigUser2receiveLock;
