#include "signals.h"

//Creation of the main process, the 3 signal generator processes, the 4 signal catcher processes 
//and the reporter process. (one source for signal generator, one source for signal catcher).

//Design and implementation of interval clocks


//Create shared memory and attach to all processes



struct shMemory{
    int test;
};

int main() {

int shmID = shmget(IPC_PRIVATE, sizeof(shMemory), IPC_CREAT | 0666);
    if (shmID < 0){
        cout<< "failed to create shared memory region"<<endl;
    }

    struct shMemory *shMemory;
    shMemory = (struct shMemory *) shmat(shmID, NULL, 0); //get the shared memory

    



}//end of main()



//signal generator
void signalGen(){

}

void signalHan(){

}

void reporter(){

}