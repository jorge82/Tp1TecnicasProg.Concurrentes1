#include "Imagen.h"
#include "ProcesadorImagenes.h"

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

using namespace std;

struct memory { 
    char buff[1000]; 
    int status, pid, parent_pid;  
};

struct memory* shmptr; 

string data_in;

vector <Imagen> imagenes;

Imagen imagen(8,8);

void handler(int signum) 
{ 
    if (signum == SIGUSR1) { 
        sleep(1);
        //cout<<" data recieved from child: "<<shmptr->buff<<endl<<endl;
        data_in=shmptr->buff;
        
        imagen.desSerializeImagen(data_in);
        imagen.mostrarImagen();
        imagenes.push_back(imagen);
        shmptr->status = Recieved;     
    } 
    if (signum == SIGUSR2) { 
        cout<<endl<<"here!!"<<endl;
         shmptr->status = Recieved; 
         cout<<" changing status  "<<shmptr->status<<endl;
    }
}

int main(int argc, const char** argv) {
    int alto=8;
    int ancho=8;
    
    if (argc!=2){
        cout<<"Falta ingresar el número de cámaras"<<endl;
        return -1;
    }


    int cant= atoi(argv[1]);   

    printf("--beginning of program\n");

     //genero una clave unica
    key_t key= ftok("sharedMem", 65);

    //shmget devuelve un indentiifcador unico
    int shmid= shmget(key, 10000, 0666|IPC_CREAT);

    // shamt para apuntar a la memoria compartida
    shmptr= (struct memory*) shmat(shmid, NULL,0);

    
    signal(SIGUSR1, handler); 
    //signal(SIGUSR2, handler); 
    
 
  
    shmptr->status = NotReady; 

    int parent;


     for (int i=0; i < cant; ++i)
    {
        if(fork()==0){

             shmptr->pid = getpid(); 
             shmptr->parent_pid = getppid(); 
            //signal(SIGUSR1, handler); 
            //signal(SIGUSR2, handler); 
            srand (time(NULL)+i);
            Imagen Imagen(ancho, alto);
            Imagen.generarImagenAleatoria();
            Imagen.procesarImagen();
            //Imagen.mostrarImagen();
 
            string serial=Imagen.serializeImagen();
            int n = serial.length();
 
            // declaring character array
            char char_array[n + 1];
        
            // copying the contents of the
            // string to char array
            strcpy(char_array, serial.c_str());


            //cout<<"iamgen a envia "<<serial.c_str()<<endl;
            //cout<<"iamgen a envia 22222"<<char_array<<endl;
            
            
         
            //memset(shmptr->buff,' ', strlen(shmptr->buff));
     
            memcpy(shmptr->buff, char_array, sizeof(char_array));


            shmptr->status = Ready; 
            
            kill(shmptr->parent_pid, SIGUSR1);

            cout<<"finsihing process "<<i<<endl;
            shmptr->status = NotReady; 
            shmdt(shmptr);
            return 1;
        }    
            
    }






       cout << "Waiting childs to finishg" << endl;
        // Need to wait for all
        for(int i=0; i<cant; i++){
            wait(NULL);
            cout << "Got " << i+1 << " done" << endl;
        }

         ProcesadorImagenes procesador(cant, ancho, alto);
         procesador.aplanarImagenes(imagenes);

         procesador.devolverImagen()->mostrarImagen();

    cout<<" terminating"<<endl;
     shmdt(shmptr);
     shmctl(shmid, IPC_RMID, NULL);
    // vector <Imagen> imagenes;
    // int cant= atoi(argv[1]);

    // for (int i; i<cant; i++){
        
    //     cout<<"Generando la imagen "<<i<<endl;
    //     // Imagen *imagen= new Imagen(ancho, alto);
    //     // imagen->generarImagenAleatoria();
        
    //     //imagenes.push_back(*(new Imagen(ancho, alto)));
    //       imagenes.emplace_back(ancho,alto);

        
    // }
    // for (int i; i<imagenes.size(); i++){
    //     cout<<"Procesando la imagen "<<i<<endl;
    //     imagenes[i].procesarImagen();
    //     imagenes[i].mostrarImagen();

    // }

    // Imagen imagen(6, 8);

    // imagen.generarImagenAleatoria();
    // //imagen.mostrarImagen();
    // //imagen.procesarImagen();
    // imagen.mostrarImagen();
    // string serial=imagen.serializeImagen();
    // cout<<endl<<"imagen serializada"<<serial<<endl;

    // imagen.desSerializeImagen(serial);
    // imagen.mostrarImagen();

    // Imagen imagen2(6, 8);

    // imagen2.generarImagenAleatoria();
    // //imagen2.mostrarImagen();
    // imagen2.procesarImagen();
    // imagen2.mostrarImagen();


//    // imagen.sumarImagen(imagen2);
//     //imagen.mostrarImagen();

    

  
//     imagenes.push_back(&imagen2);

    // ProcesadorImagenes procesador(imagenes,cant, ancho, alto);
    // procesador.aplanarImagenes();

    // procesador.devolverImagen().mostrarImagen();

    return 0;
}