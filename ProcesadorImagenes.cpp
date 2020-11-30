#include "ProcesadorImagenes.h"



 ProcesadorImagenes::ProcesadorImagenes(  int cant , int ancho, int alto){
    this->cantidad_imagenes=cant;
    this->imagen_aplanada= new Imagen( ancho, alto);
 }

int ProcesadorImagenes::aplanarImagenes(vector <Imagen> &v1){
    if(v1.size()==0){
        cout<<"Error, no hay imagnes para procesar"<<endl;
        return -1;
    }else{
         
            for (int i = 0 ;i<  v1.size(); i++) {
               
            (*imagen_aplanada).sumarImagen(v1[i]);

        }
            float num=1/(float) cantidad_imagenes; 
            this->imagen_aplanada->multiplicarImagen(num);
    return 0;
    }   
}

Imagen* ProcesadorImagenes::devolverImagen(){
    return imagen_aplanada;
}

ProcesadorImagenes::~ProcesadorImagenes(){
    delete(imagen_aplanada);
}