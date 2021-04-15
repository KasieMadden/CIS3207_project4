#include "signals.h"


void timeHandler();


typedef struct shMemory{
    int test = 0 ;
};





int main() {
  
         //Create shared memory and attach to all processes

    //shared memory space
    int shmID = shmget(IPC_PRIVATE, sizeof(shMemory), IPC_CREAT | 0666);
        if (shmID < 0){
            cout<<"error with shmget "<<endl;
        }

    //cout<<"shmID\t"<< shmID << endl;



    struct shMemory *shMemory;
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
    randNum = rand() % min + max;
    return randNum;
}//end of randomNum()


//signal generator
void signalGen(){


}

void signalHan(int signal){

}

void reporter(){

}