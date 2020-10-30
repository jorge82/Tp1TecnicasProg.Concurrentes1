#include "Imagen.h"
#include "ProcesadorImagenes.h"


int main(int argc, const char** argv) {
    int alto=8;
    int ancho=8;
    
    if (argc!=2){
        cout<<"Falta ingresar el número de cámaras"<<endl;
        return -1;
    }

    vector <Imagen> imagenes;
    int cant= atoi(argv[1]);

    for (int i; i<cant; i++){
        
        cout<<"Generando la imagen "<<i<<endl;
        // Imagen *imagen= new Imagen(ancho, alto);
        // imagen->generarImagenAleatoria();
        
        //imagenes.push_back(*(new Imagen(ancho, alto)));
          imagenes.emplace_back(ancho,alto);

        
    }
    for (int i; i<imagenes.size(); i++){
        cout<<"Procesando la imagen "<<i<<endl;
        imagenes[i].procesarImagen();
        imagenes[i].mostrarImagen();

    }

//     Imagen imagen(6, 8);

//     imagen.generarImagenAleatoria();
//     //imagen.mostrarImagen();
//     imagen.procesarImagen();
//     imagen.mostrarImagen();

//     Imagen imagen2(6, 8);

//     imagen2.generarImagenAleatoria();
//     //imagen2.mostrarImagen();
//     imagen2.procesarImagen();
//     imagen2.mostrarImagen();


//    // imagen.sumarImagen(imagen2);
//     //imagen.mostrarImagen();

    

  
//     imagenes.push_back(&imagen2);

    // ProcesadorImagenes procesador(imagenes,cant, ancho, alto);
    // procesador.aplanarImagenes();

    // procesador.devolverImagen().mostrarImagen();

    return 0;
}