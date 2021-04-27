#include "signals.h"



int main() {
  
    //Create shared memory and attach to all processes
    //shared memory space
    int shmID = shmget(IPC_PRIVATE, sizeof(shMemory), IPC_CREAT | 0666);
    if (shmID < 0){
            cout<<"error with shmget "<<endl;
        }

    //cout<<"shmID\t"<< shmID << endl;

    sharedM = (shMemory *) shmat(shmID, NULL, 0); //get the shared memory
        if(sharedM == (void *) - 1){
            cout<< "error with shmat"<<endl;
        }//end of if
      // cout<<"sharedMem\t"<< sharedMem << endl;

      mutexInit();
      pid_t process[8];
      for(int i = 0; i <= 8; i++){
          process[i] = fork();

          if(process[i] < 0){
              cout<<"Forking Error" << endl;
          }//end of if
          //child proccess 
          if(process == 0 ){
              if(i < 2){
                  //creating sig1
                  signal1();

              }
              else if( i  < 4){
                  //creating sig2
                 signal2();
              }
              else if(i < 7){
                  signalGenerator();
              }
              else{
                  report();
              }
                
            } // end of child
        //parent process 
          if(process[i] > 0){
            sleep(30); 
            endProcess = true;
            
          //detach end of parent process
          shmdt(sharedM);
            
          }

      }//end for for child/parent

}//end of main()

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
    if (randNum >= 0 && randNum <= 50 ){
        pthread_mutex_lock(&sigUser1SentLock);
        sharedM -> sigUser1SentCount++;
        pthread_mutex_unlock(&sigUser1SentLock);
        kill(0,SIGUSR1);

    }
    else(randNum > 50 && randNum <= 100){
        pthread_mutex_lock(&sigUser1SentLock);
        sharedM -> sigUser2SentCount++;
        pthread_mutex_unlock(&sigUser1SentLock);
        kill(0,SIGUSR2);
    }
        usleep(randTime);

    }//end of while loop
    //detach
    shmdt(sharedM);

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
    success = pthread_mutex_init(&sigUser1receiveLock, NULL); 
    if(success != 0){
        cout<<"Initializer for lock failed" <<endl;
        exit(1);
    }
    success = pthread_mutex_init(&sigUser2receiveLock, NULL); 
    if(success != 0){
        cout<<"Initializer for lock failed" <<endl;
        exit(1);
    }

}//end 


void signal1(){
  
    signal(SIGUSR2, SIG_IGN);// ignore signal 2 
    while(true){
        signal(SIGUSR1, sig1Handler);
        sleep(1);
    }//end of while

    //detach
    shmdt(sharedM);

}//end of signal1()

void signal2(){

    signal(SIGUSR1, SIG_IGN); // ingnore signal 1 
    while(true){
        signal(SIGUSR2, sig2Handler);
        sleep(1);
        if(endProcess){
        shmdt(sharedM);
        }

    }// end of while

    //detach

}//end of sig2()

void sig1Handler(int theSignal){

    if(theSignal = SIGUSR1){
        pthread_mutex_lock(&sigUser1receiveLock);
        sharedM -> sigUser1receiveCount++;//accecssing the shared memory
        pthread_mutex_unlock(&sigUser1receiveLock);
        
    }//end of if

}//end of sig1Handler()

void sig2Handler(int theSignal){

     if(theSignal = SIGUSR2){
        pthread_mutex_lock(&sigUser2receiveLock);
        sharedM -> sigUser2receiveCount++; //accecssing the shared memory
        pthread_mutex_unlock(&sigUser2receiveLock);
    }//end of if

}

void reportHandler(int theSignal){
    //gets time when gets signal 
    timeval currentTime;
    gettimeofday(&currentTime, NULL);

    if(theSignal == SIGUSR1){

        signal1count++;
        signal1past = elapsedTime;
        elapsedTime = (double) (currentTime.tv_sec - startTime.tv_sec) + ((double) (currentTime.tv_usec - startTime.tv_usec));
        sum = sum + (elapsedTime - signal1past);
        counter++;

    }//end of if
    else if(theSignal == SIGUSR2){

        signal2count++;
        signal2past = elapsedTime;
        elapsedTime = (double) (currentTime.tv_sec - startTime.tv_sec) + ((double) (currentTime.tv_usec - startTime.tv_usec));
        sum = sum + (elapsedTime - signal2past);
        counter++;

    }//end if else if
    

    //if  checks for ever 10 signals
    if(counter % 10 == 0 ){
    avgTime1 = timeSum1/signal1count; 
    avgTime2 = timeSum2/signal2count; 

    cout << "Current Time: " << elapsedTime << endl;
    cout << "SIGUSR1 " << "| Sent:  " << sharedM -> sigUser1SentCount << " | Recived: " << sharedM -> sigUser1receiveCount << " |Avg time between sigs" << avgTime1 << endl;
    cout << "SIGUSR2 " << "| Sent:  " << sharedM -> sigUser2SentCount << " | Recived: " << sharedM -> sigUser2receiveCount << " |Avg time between sigs" << avgTime2 << endl;
    cout << "Total Signals sent: " << (sharedM -> sigUser1SentCount + sharedM -> sigUser2SentCount ) << endl;
    cout << "Total Signals Receivced" << counter << endl;
    //resetting back to 0 after every 10 signals 
    signal1count = 0;
    signal2count = 0;
    signal1past = 0; 
    signal2past = 0;    
    avgTime1 = 0; 
    avgTime2 = 0;
    timeSum1 = 0; 
    timeSum2 = 0; 
    elapsedTime = 0;
    sum = 0;

    }//end of if cheching for 10 signals 


}// end of reporthandler 

void report(){

            signal(SIGUSR1, reportHandler);
            signal(SIGUSR2, reportHandler);

    while(true){
        sleep(1);

        if(endProcess){
            shmdt(sharedM);
        }
    }
    //deatch




}