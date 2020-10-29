#if !defined PROCESADORIMAGENES_H
#define PROCESADORIMAGENES_H




#include <iostream>
#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h>
#include <time.h> 
#include "Imagen.h"
#include <vector>

using namespace std;


class ProcesadorImagenes {




    public:

    vector <Imagen*> imagenes;

    int cantidad_imagenes;

    Imagen *imagen_aplanada;



    ProcesadorImagenes(vector <Imagen*> &imagenes, int cant, int ancho, int alto);

    int aplanarImagenes();
    Imagen devolverImagen();

    ~ProcesadorImagenes();


  
   



};
#endif // PROCESADORIMAGENES_H