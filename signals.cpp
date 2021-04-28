#include "signals.h"


int main() {
    cout << "hello from main" << endl;
       srand(time(0));
  
    //Create shared memory and attach to all processes
    //shared memory space
    int shmID = shmget(IPC_PRIVATE, sizeof(shMemory), IPC_CREAT | 0666);
    if (shmID < 0){
            cout<<"error with shmget "<<endl;
        }
    sharedM = (shMemory *) shmat(shmID, NULL, 0); //get the shared memory
        if(sharedM == (void *) - 1){
            cout<< "error with shmat"<<endl;
        }//end of if

    //call initialize all locks 
    mutexInit();
    signal(SIGUSR1, SIG_IGN);// ignore signal 1
    signal(SIGUSR2, SIG_IGN);// ignore signal 2

    //creating the process for 9 
    pid_t process[8];
    for(int i = 0; i < 8; i++){

        //calling the child process 
        process[i] = fork();
        if(process[i] < 0){
            cout<<"Forking Error" << endl;
        }//end of if
          
        //child proccess 
        if(process[i] == 0 ){
            if(i < 2){
                //creating signal1
                cout << "Creating Signal 1 " << endl;
                signal1();
            } // end  < 2

            else if( i  < 4){
                //creating signal2
                cout << "Creating Signal 2 " << endl;
                signal2();
            } // end < 4

            else if(i < 7){
                cout << "Creating Signal Generator " << endl;
                signalGenerator();
            } //end of < 7
            else{
                cout << "Creating Reporter " << endl;
                report();

            }//end of else report
            
                
        } // end of child
   }//end for() process        
        //parent process 

            cout << "parent process" << endl;
            sleep(30);
            
            endProcess = true;
            kill(0, SIGTERM);
            //detach end of parent process
            shmdt(sharedM);
        
 
}//end of main()

//random number generator
int randomGenerator(int min, int max) {
  return rand() %2;
}//end of randomNum()

//ran
void randomSleep(){
    usleep(rand() % 100000 + 10000);
}

//signal generator
void signalGenerator(){
//signal(SIGUSR1, SIG_IGN);// ignore signal 1
//signal(SIGUSR2, SIG_IGN);// ignore signal 2


cout << "signal Generator ()" << endl;


while(true){
    int randNum = randomGenerator(0, 1);  
    //cout<< "ranum: "<< randNum << endl;
    int randTime = (randomGenerator(10000,100000)); //for microsecond 
    if (randNum == 0  ){
       
        pthread_mutex_lock(&sigUser1SentLock);
        sharedM -> sigUser1SentCount++;
        pthread_mutex_unlock(&sigUser1SentLock);
        kill(0,SIGUSR1);
    }//end of 0-50

    else if(randNum == 1){
      
        pthread_mutex_lock(&sigUser2SentLock);
        sharedM -> sigUser2SentCount++;
        pthread_mutex_unlock(&sigUser2SentLock);
        kill(0,SIGUSR2);
    }//end of 50-100



    //suspend execution 
    randomSleep();
  //detach
           if(endProcess){
            shmdt(sharedM);
        }

    }//end of while loop
  
}//end of signal generator 


//to Initialize the locks for sent and receive 1&2
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
  //cout << "signal1()" << endl;
    signal(SIGUSR2, SIG_IGN);// ignore signal 2
    signal(SIGUSR1, sig1Handler);

   signal(SIGTERM, sig1Handler);
    while(true){
        
       
        sleep(1);
        if(endProcess){
            shmdt(sharedM);
        }
    }//end of while


}//end of signal1()

void signal2(){
    //cout << "signal2()" << endl;

    signal(SIGUSR1, SIG_IGN); // ingnore signal 1 
    signal(SIGUSR2, sig2Handler);

    signal(SIGTERM, sig2Handler);
    while(true){
        
        
        sleep(1);

        if(endProcess){
        shmdt(sharedM);
        }

    }// end of while



}//end of sig2()

void sig1Handler(int theSignal){
    

    if(theSignal = SIGUSR1){
        pthread_mutex_lock(&sigUser1receiveLock);
        sharedM -> sigUser1receiveCount++;//accecssing the shared memory
        pthread_mutex_unlock(&sigUser1receiveLock);
        
    }//end of if
    else if(theSignal == SIGTERM ){
        shmdt(sharedM);

    }

}//end of sig1Handler()

void sig2Handler(int theSignal){


     if(theSignal = SIGUSR2){
        pthread_mutex_lock(&sigUser2receiveLock);
        sharedM -> sigUser2receiveCount++; //accecssing the shared memory
        pthread_mutex_unlock(&sigUser2receiveLock);
    }//end of if
       else if(theSignal == SIGTERM ){
        shmdt(sharedM);

    }

}

void reportHandler(int theSignal){
    //gets time when gets signal 
   
    timeval currentTime;
    gettimeofday(&currentTime, NULL);

    if(theSignal == SIGUSR1){

        signal1count++;
        signal1past = elapsedTime;
        elapsedTime = (double) (currentTime.tv_sec - startTime.tv_sec) + (double) ((currentTime.tv_usec - startTime.tv_usec)/1000000.0 );
        timeSum1 = timeSum2 + (elapsedTime - signal1past);
        counter++;

    }//end of if
    
    else if(theSignal == SIGUSR2){

        signal2count++;
        signal2past = elapsedTime;
        elapsedTime = (double) (currentTime.tv_sec - startTime.tv_sec) + (double) ((currentTime.tv_usec - startTime.tv_usec)/1000000.0 );
        
        timeSum2 = timeSum2 + (elapsedTime - signal2past);
        counter++;

    }//end if else if
    

    //if  checks for ever 10 signals
    if(counter % 10 == 0 ){
    avgTime1 = timeSum1/signal1count; 
    avgTime2 = timeSum2/signal2count; 

    cout << endl;
    cout << "Current Time: " << elapsedTime << endl;
    cout << "SIGUSR1 " << "Sent:  " << sharedM -> sigUser1SentCount << " Received: " << (sharedM -> sigUser1receiveCount/2) << " Avg time between sigs" << avgTime1 << endl;
    cout << "SIGUSR2 " << "Sent:  " << sharedM -> sigUser2SentCount << " Received: " << (sharedM -> sigUser2receiveCount/2) << " Avg time between sigs" << avgTime2 << endl;
    cout << "Total Signals sent: " << (sharedM -> sigUser1SentCount + sharedM -> sigUser2SentCount ) << endl;
    cout << "Total Signals Received" << counter << endl;
    cout << endl;


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
    

    }//end of if cheching for 10 signals 
   
    

}// end of reporthandler 

void report(){
    //cout << "report() " << endl;
        signal(SIGUSR1, reportHandler);
        signal(SIGUSR2, reportHandler);
        signal(SIGTERM, reportHandler);
        signal(SIGTERM, reportHandler);
        gettimeofday(&startTime, NULL);
    while(true){
        
        sleep(1);
        
        if(counter == MAX_SIGNAL_COUNT ){

            shmdt(sharedM);
            kill(0, SIGTERM);
    }//end of if 


    }//end of while

}//end of report