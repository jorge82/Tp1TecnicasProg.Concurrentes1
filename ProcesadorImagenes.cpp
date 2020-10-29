#include "ProcesadorImagenes.h"



 ProcesadorImagenes::ProcesadorImagenes(vector <Imagen*> &v1,  int cant , int ancho, int alto){

     this->cantidad_imagenes=cant;

    for (int i=0; i<v1.size(); i++) 
        imagenes.push_back(v1[i]); 

    this->imagen_aplanada= new Imagen( ancho, alto);


 }

    int ProcesadorImagenes::aplanarImagenes(){

        if(this->imagenes.size()==0){
            cout<<"Error, no hay imagnes para procesar"<<endl;
            return -1;
        }else{
             for (int i = 0 ;i<  imagenes.size(); i++) {
            
                 this->imagen_aplanada->sumarImagen(*imagenes[i]);
      
            }
             this->imagen_aplanada->multiplicarImagen(0.5);

        return 0;

    }



    }
    Imagen ProcesadorImagenes::devolverImagen(){

        return *imagen_aplanada;

    }

ProcesadorImagenes::~ProcesadorImagenes(){



}