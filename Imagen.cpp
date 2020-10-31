#include "Imagen.h"


Imagen::Imagen(int anch, int alt){

    this->alto=alt;
    this->ancho=anch;
    pixeles= new int*[alt];
     /* initialize random seed: */
    //srand (time(NULL));

	// dynamically allocate memory of size N for each row
	for (int i = 0; i < alt; i++)
		pixeles[i] = new int[anch];
    //inicializo la matriz 
    for (int i = 0; i < alto; i++)
        for (int j = 0; j < ancho; j++){
            pixeles[i][j] = 0;
        }
 
}

int** Imagen::getImagen(){

    return this->pixeles;

}

string Imagen::serializeImagen(){

    string resp;

        resp.append(to_string(ancho)+","+ to_string(alto)+",");

     for (int i = 0; i < alto; i++)
        for (int j = 0; j < ancho; j++){
            resp.append(to_string(pixeles[i][j])+",");
            
        }
        return resp;
}



int Imagen::desSerializeImagen(string &serial){

    int pos=0;
    int anch= getValueFromSerial(serial, pos);
    int alt= getValueFromSerial(serial, pos);
 
    for (int i = 0; i < alt; i++)
    for (int j = 0; j < anch; j++){
        pixeles[i][j] = getValueFromSerial(serial, pos);
    }
    return 0;      
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
    for (int i = 0; i < alto; i++)
        for (int j = 0; j < ancho; j++){
            randomNumber=randomBetween0andX(255);
            pixeles[i][j] = randomNumber;
        }
    return 0;
}

int Imagen::procesarImagen(){
    int randomSeconds=randomBetween0andX(10);
    cout<<"Comenzo a procesar "<< randomSeconds<<" segundos"<<endl;
    for (int i = 0; i < alto; i++)
        for (int j = 0; j < ancho; j++){
            pixeles[i][j] = procesarPixel(pixeles[i][j]);
        }
    sleep(randomSeconds);
    cout<<"Termino de procesar"<<endl;
    return 0;
}

int Imagen::sumarImagen(Imagen &imagen){
    
    int suma;
    cout<<"Comienza la suma"<<endl;
    for (int i = 0; i < alto; i++)
        for (int j = 0; j < ancho; j++){
          //  suma= pixeles[i][j] +imagen.getImagen()[i][j];

            //cout<<"sumando"<< pixeles[i][j]<<" con "<<imagen.getImagen()[i][j]<<" da: "<< suma <<endl;
            pixeles[i][j] = sumaPixeles(pixeles[i][j] ,imagen.getImagen()[i][j]);
        }
  
    cout<<"Termina la suma"<<endl;
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

     for (int i = 0; i < alto; i++)
        for (int j = 0; j < ancho; j++){
            pixeles[i][j] *= valor;
        }
        return 0;

}


int Imagen::mostrarImagen(){

    cout<<"Imagen"<<endl;
    
    for (int i = 0; i < alto; i++){
     //cout<<"i vale"<<i<<endl;
        for (int j = 0; j < ancho; j++){
         //   cout<<"j vale"<<j<<endl;
            cout<<" "<<pixeles[i][j]<<" "; 
        }
        cout<<endl;
    }
    return 0;
}

int Imagen::randomBetween0andX(int X){
    
    int random=  1 + (rand()% X);
    return random;

}

Imagen::~Imagen(){

       for (int i = 0; i < alto; i++)
		delete[] pixeles[i];

	    delete[] pixeles;
        
        cout<<"Destructor called!!!!!"<<endl;


}