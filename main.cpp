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


bool logToFile=true;


struct memory { 
    char buff[BUFFSIZE]; 
    int status,status2, pid, parent_pid;  
};

struct memory* shmptr; 
string data_in;
vector <Imagen> imagenes;
Imagen imagen(8,8);
Logger* logger=Logger::getLogger("Log.txt");
string log;
int status;

void handler(int signum);
int main(int argc, const char** argv) {
  
    
    if (argc!=3){
       perror ("Faltan argumentos. Debe ingresar N(ancho y alto de las imagenes) y C(numero de camaras)");
       return ERROR;
    }
    
    // comienzzo a contar el tiempo
    auto start = high_resolution_clock::now(); 
   
    int n= atoi(argv[1]); 
    int cant= atoi(argv[2]);   
    int alto=n;
    int ancho=n;


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
    int shmid= shmget(key, 10000, 0666|IPC_CREAT);

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

    
    signal(SIGUSR1, handler); 

    status = Ready;

    int parent_pid, pid;

    parent_pid=getpid();
    shmptr->parent_pid = parent_pid; 


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
 
            string serial=Imagen.serializeImagen();
            //Tamaño de la imagen
            int n = serial.length();
            char char_array[n + 1];
            // string A char array
            strcpy(char_array, serial.c_str());

            if(logToFile){
                log= "Process id " + to_string(process_id) + " envia la imagen procesada al padre :" + serial ;
                logger->writeToLogFile(log);
            }

            while(status != Ready){
                //cout<<"Process "<< process_id<<" waiting"<<endl;
            }    
            status = NotReady; 
            shmptr->pid = process_id; 
            memcpy(shmptr->buff, char_array, sizeof(char_array));
            kill(parent_pid, SIGUSR1);
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

    if(logToFile){
        log= "Process id " + to_string(pid) + " :esperando a los procesos hijos que terminen";
        logger->writeToLogFile(log);
    }
    //Espero  que todos los hijos terminen
    for(int i=0; i<cant; i++){
        wait(NULL);

        if(logToFile){
        log= "Process id " + to_string(pid) + " :termino hijo " + to_string(i+1) + " de " + to_string(cant);
        logger->writeToLogFile(log);
         }   
    }


    ProcesadorImagenes procesador(cant, ancho, alto);
    if(logToFile){
        log= "Se comienza el aplanado de imagenes ";
        logger->writeToLogFile(log);
    }

    procesador.aplanarImagenes(imagenes);
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
void handler(int signum) 
{ 
    if (signum == SIGUSR1) { 
        data_in=shmptr->buff;
        imagen.desSerializeImagen(data_in);
        
        if(logToFile){
            log= "Proceso padre recibe de " + to_string(shmptr->pid) + " la imagen " + data_in;
            logger->writeToLogFile(log);
        }    

        imagen.mostrarImagen();
        imagenes.push_back(imagen);
        status = Ready;
    } 
}