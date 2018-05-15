#include <iostream>
#include <istream>
#include <fstream>
#include <cmath>
#include "tp2.h"
#include <cstdio>
#include <vector>
#include <stdlib.h>
#include <iomanip>
#include <time.h>
#include <ctime>

using namespace std;



/*
 se compila:  g++ tp2.cpp -o tp2 ppmloader/ppmloader.cpp                                                                         
 
 se corre: 
 
 forma 1:  ./tp -m <method> -i <train_set> -q <test_set> -o <classif>
 ejemplo:  ./tp -m 0 -i ../data/train.csv -q ../data/test.csv -o salida.csv
 forma 2 : ./tp <archivo con par├ímetros de entrada> <archivo de salida> <method>
 ejemplo:  ./tp test1.in test1.out 1  

 forma 3:   python2 metnum.py test   
 */

typedef vector <vector <double> > matriz;
matriz ImagenesEntrenamiento, ImagenesTest, matrizCovarianzas, autovectoresTraspuestos, ImagenesEntrenamientoCentradas;
matriz ImagenesTestCentradas, ImagenesEntrenamientoPLS, ImagenesTestPLS, ImagenesEntrenamientoPCA, ImagenesTestPCA;

vector<int> resultados;
vector<double> autovalores, media;

string RutaArchivoEntrada = "", RutaArchivoSalida = "", RutaArchivoSalidaReporte = "";
string RutaImgsEntrenamiento = "", RutaImgsPrueba = "";
string RutaEntrenamientoFormatoNuevo = "", RutaPruebaFormatoNuevo = "";
string RutaImgs;
 
ifstream TestEntrada;

int k, alfa, metodo, K, NoHayTest;


int main(int argc, char** argv){

	if (argc == 3) { // para correr metnum.py test

		RutaArchivoEntrada = argv[1];
		RutaArchivoSalida = argv[2];
		metodo = 1;
		TestEntrada.open(RutaArchivoEntrada.c_str());

		TestEntrada >> RutaImgs >> k >> alfa >> K;

	} else if (argc == 4) { // para buscar los valores óptimos/correr tests propios

		RutaArchivoEntrada = argv[1];
		RutaArchivoSalida = argv[2];
		metodo = atoi(argv[3]); // 0: kNN, 1: PCA + kNN

		TestEntrada.open(RutaArchivoEntrada.c_str());
		TestEntrada >> RutaImgs >> k >> alfa >> K;

	} else if (argc == 9) { // para usar el data de caras y sacar el csv que pide la catedra

		// estas 2 variables son auxiliares al control del flujo del programa:
		NoHayTest = 1;
		K = 0;

                assert( strcmp(argv[1], "-m") == 0 );
                metodo = atoi(argv[2]);

                assert( strcmp(argv[3], "-i") == 0 );
                RutaEntrenamientoFormatoNuevo = argv[4];
                RutaImgsEntrenamiento = PasarAFormatoViejoEntrenamiento(RutaEntrenamientoFormatoNuevo); //pasamos a formato viejo

                assert( strcmp(argv[5], "-q") == 0 );
                RutaPruebaFormatoNuevo= argv[6];
                RutaImgsPrueba = PasarAFormatoViejoPrueba(RutaPruebaFormatoNuevo);

                assert( strcmp(argv[7], "-o") == 0 );
                RutaArchivoSalida = argv[8];

		//acá van los parámetros para competir:
		k = 3;
		alfa = 15;


        }else{ // instrucciones de uso

                cout << endl << "Cantidad de argumentos pasados incorrecta." << endl << endl;
		cout << "Uso:" << endl;
                cout << "\t$ ./tp -m <method> -i <train_set> -q <test_set> -o <classif>" << endl;
                cout << "donde:" << endl;
                cout << "<method> el método a ejecutar (0: kNN , 1: PCA + kNN)" << endl;
                cout << "<train_set> será el nombre del archivo de entrada con los datos de entrenamiento." << endl;
                cout << "<test_set> será el nombre del archivo con los datos de test a clasificar" << endl;
                cout << "<classif> el nombre del archivo de salida con la clasificación de los datos de test de <test_set>" << endl;
		cout << "Ej.: $ ./tp -m 0 -i ../data/train.csv -q ../data/test.csv -o salida.csv" << endl << endl;
		cout << "O bien así:" << endl;
		cout << "\t$ ./tp <archivo con parámetros de entrada> <archivo de salida> <method>" << endl;
		cout << "Ej.: $ ./tp test1.in test1.out 1" << endl;
		cout << "Ver src/tests/catedra/LEEME.txt" << endl << endl;
		cout << "O así:" << endl;
		cout << "\t $ python2 metnum.py test" << endl;
		cout << "que corre los tests de la cátedra y verifica los resultados" << endl << endl;
                return 1;
        }


	RutaArchivoSalidaReporte = RutaArchivoSalida + ".report";

	FILE* ArchivoSalidaReporte 	= fopen(RutaArchivoSalidaReporte.c_str(),	 "w");
	FILE* ArchivoSalida 		= fopen(RutaArchivoSalida.c_str(),		 "w");

	string met;
	std::clock_t start;
	double duration = 0;
	double tiempo 	= 0;

	float promedio 		= 0;
	float aux 		= 0;
	float precision 	= 0;
	float auxprecision 	= 0;
	float recall 		= 0;
	float auxrecall 	= 0;




	for(int i = 0; i < K; i++){

        imagenes_A_Vectores(ImagenesEntrenamiento, ImagenesTest, TestEntrada, NoHayTest, i, RutaImgs);

		if(metodo == 0){

			met = "kNN";
			start = std::clock();

			resultados = Knn(ImagenesEntrenamiento, ImagenesTest, k, alfa, metodo);

			duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
			fprintf(ArchivoSalidaReporte, "Tiempo de esta corrida: %4.6f\n", duration);
			tiempo = tiempo + duration;
			aux = resultadosCorrectos(ImagenesTest, resultados, i, ArchivoSalidaReporte);
			promedio = promedio + aux;
			auxprecision = Precision(ImagenesTest, resultados, i, ArchivoSalidaReporte);
			precision = precision + auxprecision;
			auxrecall = Recall(ImagenesTest, resultados, i, ArchivoSalidaReporte);
			recall = recall + auxrecall;
		}

		if(metodo == 1)	{

			met = "PCA + kNN";
			start = std::clock();
		

			matrizCovarianzas = matrizCovarianza(ImagenesEntrenamiento, media);
			autovalores = metodoDeLaPotencia(matrizCovarianzas, alfa, autovectoresTraspuestos);
			mostrarVector(autovalores);

			COUT << "REALIZANDO TRANSFORMACION CARACTERISTICA" << endl << endl;

			ImagenesEntrenamientoPCA = ImagenesEntrenamiento;
			centrar(ImagenesEntrenamientoPCA, media, ImagenesEntrenamiento.size());

			int j = 0;
			while(j < ImagenesEntrenamiento.size()) {

				ImagenesEntrenamientoPCA[j] = transformacionCaracteristica(ImagenesEntrenamiento[j], autovectoresTraspuestos);
				j++;
			}

			ImagenesTestPCA = ImagenesTest;
			centrar(ImagenesTestPCA, media, ImagenesEntrenamiento.size());

			j = 0;
			while(j < ImagenesTest.size()) {

				ImagenesTestPCA[j] = transformacionCaracteristica(ImagenesTest[j], autovectoresTraspuestos);
				j++;
			}
				
			resultados = Knn(ImagenesEntrenamientoPCA, ImagenesTestPCA, k, alfa, metodo);


			duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
			fprintf(ArchivoSalidaReporte, "Tiempo de esta corrida: %4.6f\n", duration);
			tiempo = tiempo + duration;
			aux = resultadosCorrectos(ImagenesTest, resultados, i, ArchivoSalidaReporte);
			promedio = promedio + aux;
			auxprecision = Precision(ImagenesTest, resultados, i, ArchivoSalidaReporte);
			precision = precision + auxprecision;
			auxrecall = Recall(ImagenesTest, resultados, i, ArchivoSalidaReporte);
			recall = recall + auxrecall;
		
			if (argc == 3 || argc == 4){ // metnum.py test o comparando con .expected a mano

				j = 0;
				while (j < alfa) {

					fprintf(ArchivoSalida, "%.2f\n", autovalores[j]);
					j++;
				}
			}


		} // si metodo es 1 (PCA)

	} // para cada K-pliegue

	if (argc == 9) { // voy a escribir el .csv

		imagenes_A_Vectores_Salida(ImagenesEntrenamiento, ImagenesTest, RutaImgsEntrenamiento, RutaImgsPrueba);

		if (metodo == 0){

			resultados = Knn(ImagenesEntrenamiento, ImagenesTest, k, alfa, metodo);
		}


		if (metodo == 1){

			matrizCovarianzas = matrizCovarianza(ImagenesEntrenamiento, media);
			autovalores = metodoDeLaPotencia(matrizCovarianzas, alfa, autovectoresTraspuestos);
			//mostrarVector(autovalores);

			ImagenesEntrenamientoPCA = ImagenesEntrenamiento;
			centrar(ImagenesEntrenamientoPCA, media, ImagenesEntrenamiento.size());

			int j = 0;
			while(j < ImagenesEntrenamiento.size()) {

				ImagenesEntrenamientoPCA[j] = transformacionCaracteristica(ImagenesEntrenamiento[j], autovectoresTraspuestos);
				j++;
			}

			ImagenesTestPCA = ImagenesTest;
			centrar(ImagenesTestPCA, media, ImagenesTest.size());

			j = 0;
			while(j < ImagenesTest.size()) {

				ImagenesTestPCA[j] = transformacionCaracteristica(ImagenesTest[j], autovectoresTraspuestos);
				j++;
			}
				
			resultados = Knn(ImagenesEntrenamientoPCA, ImagenesTestPCA, k, alfa, metodo);

		} // si knn + pca

		
		fprintf(ArchivoSalida, "ImageId,Label\n");

		int i = 0;
		while ( i < resultados.size() ){

			fprintf(ArchivoSalida, "%d,%d\n", i+1, resultados[i]);
			i++;
		}

	}   
    

	if (argc == 3 || argc == 4) {
		fprintf(ArchivoSalidaReporte, "***RESULTADOS***\n");
		fprintf(ArchivoSalidaReporte, "PARAMETROS: k: %d, alfa: %d, K: %d, metodo: %s\n", k,alfa,K,met.c_str());


		fprintf(ArchivoSalidaReporte, "Tiempo: %4.6f\n", tiempo);

		promedio = promedio/K;
		fprintf(ArchivoSalidaReporte, "Promedio de aciertos: %4.6f\n", promedio);

		precision = precision/K;
		fprintf(ArchivoSalidaReporte, "Precision: %4.6f\n", precision);

		recall = recall/K;
		fprintf(ArchivoSalidaReporte, "Recall: %4.6f\n", recall);
	}

	fclose(ArchivoSalidaReporte);
	fclose(ArchivoSalida);

	return 0;
}

