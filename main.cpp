#include "Imagen.h"
#include "ProcesadorImagenes.h"


int main(int argc, const char** argv) {
    int alto=8;
    int ancho=6;

    Imagen imagen(6, 8);

    imagen.generarImagenAleatoria();
    //imagen.mostrarImagen();
    imagen.procesarImagen();
    imagen.mostrarImagen();

    Imagen imagen2(6, 8);

    imagen2.generarImagenAleatoria();
    //imagen2.mostrarImagen();
    imagen2.procesarImagen();
    imagen2.mostrarImagen();


   // imagen.sumarImagen(imagen2);
    //imagen.mostrarImagen();

    vector <Imagen*> imagenes;

    imagenes.push_back(&imagen);
    imagenes.push_back(&imagen2);

    ProcesadorImagenes procesador(imagenes,2, ancho, alto);
    procesador.aplanarImagenes();

    procesador.devolverImagen().mostrarImagen();

    return 0;
}