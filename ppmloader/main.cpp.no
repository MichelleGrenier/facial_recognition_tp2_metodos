#include <iostream>
//#include <istream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include "../ppmloader/ppmloader.h"


using namespace std;


typedef vector < vector < double > > matrix;
matrix TrainImages, TestImages;


bool replace(string& str, const string& from, const string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}


string PasarAFormatoViejoEntrenamiento(string RutaEntrenamientoFormatoNuevo){
	
	string infilePath = RutaEntrenamientoFormatoNuevo;
	replace(RutaEntrenamientoFormatoNuevo, ".csv", "_viejo.csv");
	string RutaEntrenamientoFormatoViejo  = RutaEntrenamientoFormatoNuevo;
	string outfilePath = RutaEntrenamientoFormatoViejo;
	string line;
	ifstream inFile(infilePath);
	ofstream outFile;
	outFile.open(outfilePath);
	
		
	while( getline(inFile, line) ){
		istringstream linestream(line);
		string rutaImagen;
		string idImagen;

		getline(linestream,rutaImagen,',');
		getline(linestream,idImagen,',');
		
		outFile<<idImagen;
		
			uchar* data = NULL;
			int width = 0, height = 0;
			PPM_LOADER_PIXEL_TYPE pt = PPM_LOADER_PIXEL_TYPE_INVALID;

			bool ret = LoadPPMFile(&data, &width, &height, &pt, rutaImagen.c_str());
			if (!ret || width == 0|| height == 0|| pt!=PPM_LOADER_PIXEL_TYPE_GRAY_8B){
				throw runtime_error("test_load failed");
			}
			for (int h = 0; h < height; ++h){
				for (int w = 0; w < width; ++w){
						unsigned int pixel = (unsigned int)(data[h*width + w ]);
						outFile<<","<<pixel;
				}
			}
			
		outFile<<endl;
	}
	
	inFile.close();
	outFile.close();

	return RutaEntrenamientoFormatoViejo;
}




string PasarAFormatoViejoPrueba( string RutaPruebaFormatoNuevo){
	
	string infilePath = RutaPruebaFormatoNuevo;
	replace(RutaPruebaFormatoNuevo, ".csv", "_viejo.csv");
	string RutaPruebaFormatoViejo  = RutaPruebaFormatoNuevo;
	string outfilePath = RutaPruebaFormatoViejo;
	string line;
	ifstream inFile(infilePath);
	ofstream outFile;
	outFile.open(outfilePath);
	
		
	while( getline(inFile, line) ){
		istringstream linestream(line);
		string rutaImagen;
		string idImagen;

		getline(linestream,rutaImagen,',');
		getline(linestream,idImagen,',');
		
		//outFile<<idImagen;
		
			uchar* data = NULL;
			int width = 0, height = 0;
			PPM_LOADER_PIXEL_TYPE pt = PPM_LOADER_PIXEL_TYPE_INVALID;

			bool ret = LoadPPMFile(&data, &width, &height, &pt, rutaImagen.c_str());
			if (!ret || width == 0|| height == 0|| pt!=PPM_LOADER_PIXEL_TYPE_GRAY_8B){
				throw runtime_error("test_load failed");
			}
			for (int h = 0; h < height; ++h){
				for (int w = 0; w < width; ++w){
						unsigned int pixel = (unsigned int)(data[h*width + w ]);
						if (h==0 && w==0){
							outFile<<pixel;
						}else{ 
							outFile<<","<<pixel;
						}
				}
			}
			
		outFile<<endl;
	}
	
	inFile.close();
	outFile.close();

	return RutaPruebaFormatoViejo;
}



// tp2 -m 1 -i ../tests/train_10_10.csv -q ../tests/test_10_10.csv -o clasif.csv

int main(int argc, char** argv){
	
	if(argc!= 9){
				cout << endl<<"Forma de ejecucion:" << endl<<endl;
				cout <<"\t" << "./tp2"<<"\t" <<"-m <method>"<<"\t" <<"-i <train_set>"<<"\t" <<" -q <test_set>"<<"\t" <<" -o <classif>" << endl << endl;
				cout << "<method> :" <<"\t" << "0: kNN, 1: PCA + kNN." << endl;
				cout << "<train_set> :" <<"\t" << "nombre del archivo de entrada con los datos de entrenamiento." << endl;
				cout << "<test_set>  :" <<"\t" << "sera el nombre del archivo con los datos de test a clasificar." << endl;
				cout << "<classif> :" <<"\t" << "nombre del archivo de salida con la clasificacion de los datos de test." << endl;
				return 1;
	}
	string m 				= argv[1];
	string metodo 			= argv[2];
	string i 				= argv[3];
	string archivo_train_set= argv[4];
	string q 				= argv[5];
	string archivo_test_set = argv[6];
	string o 				= argv[7];
	string archivo_classif 	= argv[8];
	
	if(m != "-m" || i != "-i" || q != "-q" || o != "-o"){
		cout << endl<< "Se espera:"<<endl;
		if (m != "-m"){
			cout << endl<< "\t  \'-m\' como primer parametro" ;
		}
		if (i != "-i"){
			cout << endl<< "\t  \'-i\' como segundo parametro" ;
		}
		if (q != "-q"){
			cout << endl<< "\t  \'-q\' como tercer parametro"  ;
		}
		if (o != "-o"){
			cout << endl<< "\t  \'-o\' como cuarto parametro"  ;
		}
		cout <<"." <<  endl;
		return 1;
	}
	
	ifstream train_set, test_set, classif;
	
	//train_set.open(archivo_train_set.c_str());
	string RutaEntrenamientoFormatoNuevo= archivo_train_set;
	
	string RutaImgsEntrenamiento = PasarAFormatoViejoEntrenamiento(RutaEntrenamientoFormatoNuevo);

	string RutaPruebaFormatoNuevo= archivo_test_set;
	 
	string RutaImgsPrueba = PasarAFormatoViejoPrueba(RutaPruebaFormatoNuevo);
}


