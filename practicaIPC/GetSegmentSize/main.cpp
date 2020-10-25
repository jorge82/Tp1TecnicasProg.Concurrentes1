#include <iostream> 
#include <cstdio>
#include <sys/ipc.h> 
#include <sys/shm.h> 
using namespace std; 

int main( ) {
    key_t key = 15;
    int shmid_1, shmid_2;

    int segmentSize=1;

    while ((shmid_1=shmget(key, segmentSize, 0640|IPC_CREAT)) != -1){
       
      cout << "Shared memory identifier is : " << shmid_1 << 
      " the size of the segment is "<< segmentSize<<endl;
      segmentSize++;
    }
    cout << "!!!The maximun segment size is : " << segmentSize -1<< endl;


    while ((shmid_1=shmget(key +1, segmentSize, 0640|IPC_CREAT)) != -1){
       
      //cout << "Shared memory identifier is : " << shmid_1 << 
      //" the size of the segment is "<< segmentSize<<endl;
      segmentSize--;
    }
    cout << "!!!The minimun  segment size is : " << segmentSize +1<< endl;

  return 0;
}

