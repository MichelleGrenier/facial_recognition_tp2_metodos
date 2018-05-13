#include <iostream>
//#include <istream>
#include <fstream>
#include <vector>
#include "../ppmloader/ppmloader.h"


using namespace std;


typedef vector < vector < double > > matrix;
matrix TrainImages, TestImages;




}


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
	
	train_set.open(archivo_train_set.c_str());
	
	imagesToMatrices(TrainImages, TestImages, train_set);
	
	//test_set.open(archivo_test_set.c_str());
	
}


