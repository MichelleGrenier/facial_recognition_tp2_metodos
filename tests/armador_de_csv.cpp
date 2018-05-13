#include <iostream>
#include <fstream>
#include <sstream>


using namespace std; 




//se compila:   g++ armador_de_csv.cpp -o armador

// se ejecuta: 	armador.exe  sujeto_min  sujeto_max  foto_min  foto_max
// ejemplo:		armador.exe 1 10 1 10

int main(int argc, char** argv) { 
 
 
	if (argc != 5){
		cout << endl<<"	Unable to run program" << endl;
		cout << "	Four parameters are expected:    armador.exe  sujeto_min  sujeto_max  foto_min  foto_max " << endl;
		return 1;
	}
	int sujeto_min = atoi(argv[1]);
	int sujeto_max = atoi(argv[2]);
	int foto_min = atoi(argv[3]);
	int foto_max = atoi(argv[4]);
	
	
	
	ostringstream filename;
    filename << "../tests/train_set_smin"<< sujeto_min << "_smax" << sujeto_max << "_fotomin" << foto_min << "_fotomax" << foto_max << ".csv";
    ofstream outputFile;
    outputFile.open(filename.str());	
	for(int i = sujeto_min; i <= sujeto_max; i++) {
		
		for(int j = foto_min; j <= foto_max; j++) {
		
			outputFile << "ImagenesCarasRed/s"<< i << "/" << j << ".pgm, " << i<< "," << endl ;	
		
		}
    }
	outputFile.close();
	return 1;
	
}

