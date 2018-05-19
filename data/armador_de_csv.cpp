#include <iostream>
#include <fstream>
#include <sstream>


using namespace std;




//se compila:   g++ armador_de_csv.cpp -o armador
//o bien:       make

// se ejecuta:  armador.exe  sujeto_min  sujeto_max  foto_min  foto_max usar_imgs_reducidas
// ejemplo:     armador.exe 1 10 1 10 1

int main(int argc, char** argv) {

    if (argc != 6){
        cout << endl<<" Unable to run program" << endl;
        cout << "   5 parameters are expected:    armador.exe  sujeto_min  sujeto_max  foto_min  foto_max usar_imgs_reducidas" << endl;
        return 1;
    }
    int sujeto_min = atoi(argv[1]);
    int sujeto_max = atoi(argv[2]);
    int foto_min = atoi(argv[3]);
    int foto_max = atoi(argv[4]);
    int usar_imgs_reducidas = atoi(argv[5]);

    string tamanio_imgs = "";
    if (usar_imgs_reducidas == 0){

        tamanio_imgs = "full";
    } else {
        tamanio_imgs = "reduced";
    }

    ostringstream filename;
    filename << "train_set_smin"<< sujeto_min << "_smax" << sujeto_max << "_fotomin" << foto_min << "_fotomax" << foto_max << "_" << tamanio_imgs << ".csv";
    ofstream outputFile;
    outputFile.open(filename.str());

    for(int i = sujeto_min; i <= sujeto_max; i++) {
        for(int j = foto_min; j <= foto_max; j++) {
            outputFile << "../data/" << tamanio_imgs << "/s" << i << "/" << j << ".pgm, " << i<< "," << endl ;
        }
    }
    outputFile.close();
    return 0;
}

