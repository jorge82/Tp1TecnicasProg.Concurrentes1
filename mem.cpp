#include "Imagen.h"
#include "ProcesadorImagenes.h"
#include "Logger.h"

#include <string>
#include <cstring>
#include <signal.h> 
#include <sys/types.h> 
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h> 
#include <sys/ipc.h>
#include <sys/shm.h>

#include <mutex>          // std::mutex

std::mutex mtx;           // mutex for critical section
  
#define Recieved 2
#define Ready 1 
#define NotReady -1 
#define Finished 3 
#define BUFFSIZE 1000
#define ERROR -1
#define OK 0

#include <chrono> 
using namespace std::chrono; 

using namespace std;

#include <mutex>  

#include <csignal>

/*  
    Ejecucion: /procesadorSharedMem  <tam_imagen> <num_camaras>
*/
/* false si no se quiere log*/
 struct memory { 
        char buff[BUFFSIZE]; 
        int pid, parent_pid;  
    };
bool logToFile=false;
  struct memory* shmptr; 
   int shmid;
void signalHandler( int signum ) {
 
    printf("\n Clearing shared mamery .  Ctrl+C pressed \n"); 
    //shmdt(shmptr);
    //shmctl(shmid, IPC_RMID, NULL);

   exit(signum);  
}
int main(int argc, const char** argv) {
  
    if (argc!=3){
       perror ("Faltan argumentos. Debe ingresar N(ancho y alto de las imagenes) y C(numero de camaras)");
       return ERROR;
    }
    
    // comienzo a contar el tiempo
    auto start = high_resolution_clock::now(); 
   
    int n= atoi(argv[1]); 
    int cant= atoi(argv[2]);   
    int alto=n;
    int ancho=n;


   

  
    string data_in;
    vector <Imagen> imagenes;
    Imagen imagen(ancho,alto);
    Logger* logger=Logger::getLogger("Log.txt");
    string log;
    int status;


    if(logToFile){
        log= "Comienza el programa con " + to_string(cant) +" imagenes de " + to_string(n) + "x" +to_string(n)+ " pixeles";   ;
        logger->writeToLogFile(log);
    }

     //genero una clave unica
    key_t key= ftok(".", 65);

    if ( key <= 0 ) {
        perror ("Error ftok");
       return ERROR;
    }

    //shmget devuelve un indentiifcador unico
     shmid= shmget(key, 10000, 0666|IPC_CREAT);

    if ( shmid <= 0 ) {
        perror ("Error shmget");
       return ERROR;
    }

    // shamt para apuntar a la memoria compartida
    shmptr= (struct memory*) shmat(shmid, NULL,0);

     if ( shmptr==  (void*) -1 ) {
        perror ("Error shmat");
       return ERROR;
    }

    status = Ready;

    int parent_pid;

    parent_pid=getpid();
    shmptr->parent_pid = parent_pid; 

   

    int pid;
    for (int i=0; i < cant; ++i)
    {
        pid=fork();
         
        if(pid==0){
             int process_id= getpid();
            //seteo un semilla distinta para numero random
            srand (time(NULL)+i);
            Imagen Imagen(ancho, alto);
            Imagen.generarImagenAleatoria();
            if(logToFile){
                log= "Proceso id " + to_string(process_id) + " comienza a procesar su imagen";
                logger->writeToLogFile(log);
            }
            Imagen.procesarImagen();
              if(logToFile){
                log= "Proceso id " + to_string(process_id) + " Termina de  procesar su imagen";
                logger->writeToLogFile(log);
            }
            Imagen.mostrarImagen();

            string serial=Imagen.serializeImagen();
            //Tamaño de la imagen
            int len = serial.length();
            char char_array[len + 1];
            // string A char array
            strcpy(char_array, serial.c_str());

            if(logToFile){
                log= "Process id " + to_string(process_id) + " envia la imagen procesada al padre :" + serial ;
                logger->writeToLogFile(log);
            }
            
            mtx.lock();    

            // while(status != Ready){

            // }    
            //status = NotReady; 
            shmptr->pid = process_id; 
            memcpy(shmptr->buff, char_array, sizeof(char_array));
            shmdt(shmptr);
            if(logToFile){
                log= "Process id " + to_string(process_id) + " termino";
                logger->writeToLogFile(log);
            }
            return OK;
        }else if (pid<0){
            perror ("Error in creating child process");
            return ERROR;
        }     
            
    }

     signal(SIGINT, signalHandler);
    if(logToFile){
        log= "Process id " + to_string(parent_pid) + " :esperando a los procesos hijos que terminen";
        logger->writeToLogFile(log);
    }
    //Espero  que todos los hijos terminen
    for(int i=0; i<cant; i++){
        wait(NULL);
        data_in=shmptr->buff;

        mtx.unlock();
        imagen.desSerializeImagen(data_in);
        
        if(logToFile){
            log= "Proceso padre recibe de " + to_string(shmptr->pid) + " la imagen " + data_in;
            logger->writeToLogFile(log);
        }    
    
        //imagen.mostrarImagen();
        imagenes.push_back(imagen);
        //status = Ready;

        if(logToFile){
        log= "Process id " + to_string(parent_pid) + " :termino hijo " + to_string(i+1) + " de " + to_string(cant);
        logger->writeToLogFile(log);
         }   
    }


    ProcesadorImagenes procesador(cant, ancho, alto);
    if(logToFile){
        log= "Se comienza el aplanado de imagenes ";
        logger->writeToLogFile(log);
    }

    procesador.aplanarImagenes(imagenes);
    cout<<"Imagen final"<<endl;
    procesador.devolverImagen()->mostrarImagen();

    shmdt(shmptr);
    shmctl(shmid, IPC_RMID, NULL);

    // Calculo la duracion de ejecuciòn
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start); 
    if(logToFile){
        log= "Se termina el programa , tiempo de ejecucion: " + to_string(duration.count())+ " segundos";
        logger->writeToLogFile(log);
    }
    return OK;
}
