#include "Imagen.h"


int main(int argc, const char** argv) {

    Imagen imagen(6, 8);

    imagen.generarImagenAleatoria();
    imagen.mostrarImagen();
    imagen.procesarImagen();
    imagen.mostrarImagen();

    Imagen imagen2(6, 8);

    imagen2.generarImagenAleatoria();
    imagen2.mostrarImagen();
    imagen2.procesarImagen();
    imagen2.mostrarImagen();
    return 0;
}