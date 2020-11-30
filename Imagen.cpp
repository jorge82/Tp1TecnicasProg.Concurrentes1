#include "Imagen.h"


Imagen::Imagen(int anch, int alt){

    this->alto=alt;
    this->ancho=anch;
   
    vector_pixeles.push_back(ancho); 
    vector_pixeles.push_back(alto); 

    for (int i = 0; i < alto*ancho; i++) 
        vector_pixeles.push_back(0); 

}

vector<int> Imagen::getImagenVector(){

    return vector_pixeles;

}


int Imagen::getValueFromSerial(string &serial, int &pos){

    string num;
    string delimiter = ",";
    auto end = serial.find(delimiter,pos);
    string token = serial.substr(pos, end);
    pos = end + delimiter.length();
    return stoi(token);
}


int Imagen::generarImagenAleatoria(){
    int randomNumber;
  

    for (int i=0; i<alto*ancho; i++){
        randomNumber=randomBetween0andX(255);
        vector_pixeles[i+2]=randomNumber;
    }
    return 0;
}
int Imagen::getImagenFromVector(vector<int> &vect){

    for(int i=0; i<alto*ancho+2; i++){
        vector_pixeles[i]= vect[i];
    }
    return 0;

}



int Imagen::procesarImagen(){
    int randomSeconds=randomBetween0andX(10);
  
    for (int i=0; i<alto*ancho; i++){
        vector_pixeles[i+2]= procesarPixel(vector_pixeles[i+2]);
    }
    sleep(randomSeconds);
   
    return 0;
}

int Imagen::sumarImagen(Imagen &imagen){
    
    int suma;
    vector<int> vect=imagen.getImagenVector();
  
    
        for (int i=0; i<alto*ancho; i++){
             vector_pixeles[i+2]= sumaPixeles(vector_pixeles[i+2], vect[i+2]);
    }
  
    return 0;
}




int Imagen::procesarPixel(int pixel){
    
    
    return pixel/2 + 1;

}

//suma pixeles, devuelve un numero entre 0 y 255
int Imagen::sumaPixeles(int &pixel1, int &pixel2) {

    int suma= pixel1 + pixel2;

    if (suma< 256) {
        return suma;
    }else{
         return  suma -256;   
    }

}
int Imagen::multiplicarImagen(float valor){

     for (int i=0; i<alto*ancho; i++){
        vector_pixeles[i+2] *= valor;
    }
        return 0;

}


int Imagen::mostrarImagen(){

    cout<<"Imagen"<<endl;

    int j=0;
     for (int i = 0; i < alto*ancho; i++){
         
         if(j>=ancho){
              cout<<endl;
              j=0;

         }
           //cout<<"j vale"<<j<<endl;
            cout<<" "<<vector_pixeles[i+2]<<" "; 
            j++;
         
    }
       
    cout<<endl;
    return 0;
}

int Imagen::randomBetween0andX(int X){
    
    int random=  1 + (rand()% X);
    return random;

}

Imagen::~Imagen(){



}