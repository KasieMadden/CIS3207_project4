#include "signals.h"

/* 
Create shared counters and individual locks for each counter.
Proper/safe access to send and receive counts
Description of testing and documentation (did they include logs in processes, e.g.).

*/




shMemory *sharedM;


int main() {
  
         //Create shared memory and attach to all processes

    //shared memory space
    int shmID = shmget(IPC_PRIVATE, sizeof(shMemory), IPC_CREAT | 0666);
        if (shmID < 0){
            cout<<"error with shmget "<<endl;
        }

    //cout<<"shmID\t"<< shmID << endl;



    shMemory *shMemory;
    shMemory = (struct shMemory *) shmat(shmID, NULL, 0); //get the shared memory

    //shared memory porinter
    struct SharedMem *sharedMem;
    sharedMem = (struct SharedMem *) shmat(shmID, NULL, 0);
        if(sharedMem == (SharedMem *) - 1){
            cout<< "error with shmat"<<endl;
        }//end of if
      // cout<<"sharedMem\t"<< sharedMem << endl;








}//end of main()

void timeHandler(){

    struct timeval startTime; gettimeofday(&startTime, NULL); 
    struct timeval currentTime; 
    double elapsedTime = 0; 

    while (elapsedTime <= 5) {
    gettimeofday(&currentTime, NULL); 

    elapsedTime = (double) (currentTime.tv_sec - startTime.tv_sec) + ((double) (currentTime.tv_usec - startTime.tv_usec) / 1000000); //using the struct vals, find the difference between the start time and current time to calculate the actual elapsed time

    printf("%lf\n", elapsedTime);
    sleep(1);
    }

}

//random number generator
int randomGenerator(int min, int max) {
    int randNum;
    srand(time(0));
    randNum = (rand() % (min - max + 1))+ min;
    return randNum;
}//end of randomNum()


//signal generator
void signalGenerator(){
    int randNum = randomGenerator(0, 100);  
    int randTime = (randomGenerator(10000,100000));

    while(true){
    if (randNum >= 0 && randNum <= 50  ){
        pthread_mutex_lock(&sigUser1SentLock);
        sharedM-> sigUser1SentCount;
        pthread_mutex_unlock(&sigUser1SentLock);
        kill(0,SIGUSR1);

    }
    else( randNum > 50 && randNum <= 100){
        pthread_mutex_lock(&sigUser1SentLock);
        sharedM-> sigUser2SentCount;
        pthread_mutex_unlock(&sigUser1SentLock);
        kill(0,SIGUSR2);

    }
        usleep(randTime);

    }//end of while loop
}//end of signal generator 

//to Initialize the locks 
void mutexInit(){
    int success = 0; 
    success = pthread_mutex_init(&sigUser1SentLock, NULL); 
    if(success != 0){
        cout<<"Initializer for lock failed" <<endl;
        exit(1);
    }
    success = pthread_mutex_init(&sigUser2SentLock, NULL); 
    if(success != 0){
        cout<<"Initializer for lock failed" <<endl;
        exit(1);
    }
    success = pthread_mutex_init(&sigUser1reciveLock, NULL); 
    if(success != 0){
        cout<<"Initializer for lock failed" <<endl;
        exit(1);
    }
    success = pthread_mutex_init(&sigUser2reciveLock, NULL); 
    if(success != 0){
        cout<<"Initializer for lock failed" <<endl;
        exit(1);
    }

}


void blockSignal(){

}

void unblockSignal(){
    
}

void sig1 (int signal){


}

void sig2(){

}

void sig1Handler(){

}

void sig2Handler(){

}


void reporter(){

}