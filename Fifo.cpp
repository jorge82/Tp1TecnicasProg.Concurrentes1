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
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#define ERROR -1
#define OK 0

#include <chrono> 
using namespace std::chrono; 

using namespace std;


#include <csignal>

/*  
    Ejecucion: /procesadorSharedMem  <tam_imagen> <num_camaras>
*/
/* false si no se quiere log*/
int fd;

bool logToFile=false;


int shmid;
int (*mem);

void signalHandler( int signum ) {
 
   printf("\n Cerrado fifo .  Ctrl+C pressed \n"); 
   close(fd);
   exit(signum);  
 }

int copyToFiFo( int file_descriptor, vector<int> &vector){
    cout<<"entre"<<endl;
     int arrayInt[vector.size()];
    for(int i=0; i<vector.size(); i++){
        arrayInt[i]=vector.at(i);
    }
    write(file_descriptor, &arrayInt, sizeof(arrayInt)); 
      cout<<"sali"<<endl;
    return 0;
} 


vector<int> copyToVector( int* array, int tam){

   
    vector<int> aux;
    for(int i=0; i<tam; i++){

        aux.push_back(array[i]);
    }
    return aux;
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


    vector <Imagen> imagenes;

    Logger* logger=Logger::getLogger("Log.txt");
    string log;



    if(logToFile){
        log= "Comienza el programa con " + to_string(cant) +" imagenes de " + to_string(n) + "x" +to_string(n)+ " pixeles";   ;
        logger->writeToLogFile(log);
    }

     // path del fifo
    char  myfifo[] = "./myfifo"; 
  
    // Creo el FIFO si o existe
    mkfifo(myfifo, 0666); 



    int parent_pid;
    parent_pid=getpid();
   
    int pid;
  
    for (int i=0; i < cant; ++i)
    {
        //genero procesos hijos
        pid=fork();
         
        if(pid==0){

            // Open FIFO for write only 
            fd = open(myfifo, O_WRONLY);

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


          

            if(logToFile){
                log= "Process id " + to_string(process_id) + " envia la imagen procesada al padre" ;
                logger->writeToLogFile(log);
            }
            
       
            int tam_imagen= sizeof(Imagen.getImagenVector());
            vector<int> vec= Imagen.getImagenVector();
         
            
            int tam_pixeles= alto*ancho +2;  //se le suma 2 para incluir el ancho y el alto

            //se copia a memoria desde la posicion i*tam_pixeles 
            copyToFiFo( fd,  vec);
            close(fd); 

            if(logToFile){
                log= "Process id " + to_string(process_id) + " termino";
                logger->writeToLogFile(log);
            }
            return OK;
        }else if (pid<0){
            perror ("Error al crear procesos hijos");
            return ERROR;
        }     
            
    }

    signal(SIGINT, signalHandler);


    // abro FIFO para lectura 
    fd = open(myfifo, O_RDONLY);
    if(logToFile){
        log= "Proceso id " + to_string(parent_pid) + " :esperando a los procesos hijos que terminen";
        logger->writeToLogFile(log);
    }
    //Espero  que todos los hijos terminen
    int tam_pixeles= alto*ancho +2;
    for(int i=0; i<cant; i++){
        int data_in[tam_pixeles];
        read(fd, data_in, sizeof(data_in));
        vector<int> vect= copyToVector( data_in,tam_pixeles);
        Imagen imagen(ancho,alto);
        imagen.getImagenFromVector(vect);
        cout<<"Imagen recibida:"<<endl;
        imagen.mostrarImagen();
        imagenes.push_back(imagen);
        wait(NULL);
    }


    ProcesadorImagenes procesador(cant, ancho, alto);
    if(logToFile){
        log= "Se comienza el aplanado de imagenes ";
        logger->writeToLogFile(log);
    }

    procesador.aplanarImagenes(imagenes);
    cout<<"Imagen final"<<endl;
    procesador.devolverImagen()->mostrarImagen();

    close(fd); 

    // Calculo la duracion de ejecuciòn
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start); 
    if(logToFile){
        log= "Se termina el programa , tiempo de ejecucion: " + to_string(duration.count())+ " segundos";
        logger->writeToLogFile(log);
    }
    return OK;
}
