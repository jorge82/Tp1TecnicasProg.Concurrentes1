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


bool logToFile=false;


int shmid;
int (*mem);

void signalHandler( int signum ) {
 
    printf("\n Clearing shared mamery .  Ctrl+C pressed \n"); 
    shmdt(mem);
    shmctl(shmid, IPC_RMID, NULL);

   exit(signum);  
 }
int copyToMem( int *mem, int posicion , vector<int> &vector){

     
    //cout<<"vector size: "<<vec.size()<<endl;
    for(int i=0; i<vector.size(); i++){
        //cout<<"!!!valor copiado a memoria "<<vector[i]<<" en la posicion " <<posicion +i<<endl;
        mem[posicion +i]= vector.at(i);  
    }
    return 0;
} 

// int mostrarMem( int *mem, int posicion, int tam){
//     cout<<"Imagen recibida "<<endl;
//     for(int i=0; i<tam; i++){
//        cout<<" "<< mem[posicion+i];
//     }

//     cout<<endl;
//     return 0;

// } 

vector<int> copyToVector( int *mem, int posicion, int tam){

    //cout<<"copiando desde las posicion: "<< posicion<<" el tamaño: "<<tam<<endl;
    vector<int> aux;
    for(int i=0; i<tam; i++){
        // cout<<"copiando en la posicion: "<< posicion +i <<" el valor: "<<mem[posicion+i]<<endl;
        aux.push_back(mem[posicion+i]);
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

     //genero una clave unica
    key_t key= ftok(".", 65);

    if ( key <= 0 ) {
        perror ("Error ftok");
       return ERROR;
    }
  

    //shmget devuelve un indentifcador unico
    /* cada imagen va estar representador por un serie de numeros enteros empezando por el ancho y alto
    ejemplo: 2 2 10 15 2 3  ;  donde representa una matriz de 2x2 con valores: |10 15 |
                                                                               | 2  3 |
    */
    int bytes_requeridos=(alto*ancho +2)* cant*sizeof(int);
    shmid= shmget(key, bytes_requeridos, 0666|IPC_CREAT);

    if ( shmid <= 0 ) {
        perror ("Error shmget");
       return ERROR;
    }


    // shamt para apuntar a la memoria compartida

   mem= (int*) shmat(shmid, 0,0);

     if ( mem==  (void*) -1 ) {
        perror ("Error shmat");
       return ERROR;
    }

    int parent_pid;
    parent_pid=getpid();
   
    int pid;
    for (int i=0; i < cant; ++i)
    {
        //genero procesos hijos
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


          

            if(logToFile){
                log= "Process id " + to_string(process_id) + " envia la imagen procesada al padre" ;
                logger->writeToLogFile(log);
            }
            
       
            int tam_imagen= sizeof(Imagen.getImagenVector());
            vector<int> vec= Imagen.getImagenVector();
         
            
            int tam_pixeles= alto*ancho +2;  //se le suma 2 para incluir el ancho y el alto

            //se copia a memoria desde la posicion i*tam_pixeles 
            copyToMem( mem, i*tam_pixeles, vec);
            //dehace el vinvulo de la memoria compartida al proceso
            shmdt(mem);

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
    if(logToFile){
        log= "Proceso id " + to_string(parent_pid) + " :esperando a los procesos hijos que terminen";
        logger->writeToLogFile(log);
    }
    //Espero  que todos los hijos terminen
    for(int i=0; i<cant; i++){
        wait(NULL);
    }

    int tam_pixeles= alto*ancho +2;

    //leo la meoria compartida y guardo las imagenes en un vector para luego procesarlas
    for(int i=0; i<cant; i++){

        vector<int> vect= copyToVector( mem, i*tam_pixeles,n*n+2);
        Imagen imagen(ancho,alto);
        imagen.getImagenFromVector(vect);
        cout<<"Imagen recibida!!!!!!1"<<endl;
        imagen.mostrarImagen();
        imagenes.push_back(imagen);
       
    }


    ProcesadorImagenes procesador(cant, ancho, alto);
    if(logToFile){
        log= "Se comienza el aplanado de imagenes ";
        logger->writeToLogFile(log);
    }

    procesador.aplanarImagenes(imagenes);
    cout<<"Imagen final"<<endl;
    procesador.devolverImagen()->mostrarImagen();

    shmdt(mem);
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
