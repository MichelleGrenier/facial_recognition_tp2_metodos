#include <iostream>
#include <istream>
#include <fstream>
#include <cmath>
#include <math.h>
#include <cstdio>
#include <vector>
#include <tuple>
#include <stdlib.h>
#include <iomanip>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <utility>
#include <sstream>
#include <string>
#include <algorithm>
#include "ppmloader/ppmloader.h"


using namespace std;

#define COUT if (1) cout // 0: modo silencioso, 1: modo verborrágico (debug)

// estas ya no son más constantes, vamos a contar cuando levantemos las imágenes. inicializamos en 0
int CANT_IMGS_ENTRENAMIENTO = 0; // maximo: 41*10 = 410
int CANT_PIXELS_EN_IMG = 0; //  _full: 92*112 = 10304     _reduced: 23*28 = 644
int CANT_IMGS_PRUEBA = 0; 

// estas siguen siendo constantes
const int CANT_CLASES = 41;
const int CANT_ITERS_MET_POT = 1000; // TODO: ver con qué número es suficiente // esto está un poco más suave que antes, pero sigue hardcodeado. capaz podría entrar por parámetro en la llamada al programa o via archivo.in

ifstream ArchivoEntrada;
ifstream ArchivoEntrenamientoSalida;
ifstream ArchivoPruebaSalida;

typedef vector < vector < double > > matriz; // CAMBIAR A INT ACA Y EN MAIN PARA KNN SIN REDUCCION Y COMENTAR TODO EL CODIGO QUE NO TENGA QUE VER CON EL METODO 0 (FUNCIONA MUCHO MAS RAPIDO), DOUBLE NECESARIO PARA METODOS DE REDUCCION




void escribirMatrizEnConsola(matriz& m);

bool str_terminaEn (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

bool replace(string& str, const string& from, const string& to) {
    bool result = true;
    size_t start_pos = str.find(from);
    if(start_pos == string::npos){
        result = false;
    }
    str.replace(start_pos, from.length(), to);
    return result;
}

bool todosIguales(vector<pair<int, int>> dimensiones ){
	
	bool res = true;
	pair<int, int > dim_aux = dimensiones[0];
	for(int i = 0; i < dimensiones.size(); i++){
		if(dim_aux!= dimensiones[i]){
			res = false;
		}
	}
	return res;
}



string PasarAFormatoViejoEntrenamiento(string RutaEntrenamientoFormatoNuevo){
    string infilePath = RutaEntrenamientoFormatoNuevo;	
    replace(RutaEntrenamientoFormatoNuevo, ".csv", "_viejo.csv");
    string RutaEntrenamientoFormatoViejo  = RutaEntrenamientoFormatoNuevo;
    string outfilePath = RutaEntrenamientoFormatoViejo;
    string line;
	vector<pair<int, int>> dimensiones;
    ifstream inFile(infilePath);
    if (inFile.fail()){ cout << "Fallo al intentar abrir el archivo "<<"\"" <<infilePath<<"\" " << endl; exit (1);  }
    ofstream outFile;
    outFile.open(outfilePath);
    if (outFile.fail()){ cout << "Fallo al intentar abrir el archivo "<<"\"" <<outfilePath<<"\" " << endl; exit (1); }
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
			throw runtime_error( "Fallo al intentar abrir la imagen de entrenamiento: \" "+ rutaImagen + " \" ");
		}
		dimensiones.push_back(make_pair(height, width));
		for (int h = 0; h < height; ++h){
			for (int w = 0; w < width; ++w){
				unsigned int pixel = (unsigned int)(data[h*width + w ]);
				outFile<<","<<pixel;
			}
		}
		CANT_IMGS_ENTRENAMIENTO ++;
		CANT_PIXELS_EN_IMG = height * width;
		if( not(todosIguales(dimensiones)) ){ cout << "Fallo. Las imagenes de entrenamiento deben tener todas el mismo tamaño"<< endl; exit (1);}
		outFile<<endl;
    }
    //cout<<"cant pixels: "<<CANT_PIXELS_EN_IMG<<endl;
    //cout<<"cant img entrenamiento: "<<CANT_IMGS_ENTRENAMIENTO<<endl;
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
	vector<pair<int, int>> dimensiones;
    ifstream inFile(infilePath);
    if (inFile.fail()){ cout << "Fallo al intentar abrir el archivo "<<"\"" <<infilePath<<"\" " << endl; exit (1); }
    ofstream outFile;
    outFile.open(outfilePath);
    if (outFile.fail()){ cout << "Fallo al intentar abrir el archivo "<<"\"" <<outfilePath<<"\" " << endl; exit (1); }
    while( getline(inFile, line) ){
        istringstream linestream(line);
        string rutaImagen;
        string idImagen;
        getline(linestream,rutaImagen,',');
        getline(linestream,idImagen,',');
        uchar* data = NULL;
        int width = 0, height = 0;
        PPM_LOADER_PIXEL_TYPE pt = PPM_LOADER_PIXEL_TYPE_INVALID;
        bool ret = LoadPPMFile(&data, &width, &height, &pt, rutaImagen.c_str());
        if (!ret || width == 0|| height == 0|| pt!=PPM_LOADER_PIXEL_TYPE_GRAY_8B){
            throw runtime_error( "Fallo al intentar abrir la imagen de prueba: \" "+ rutaImagen + " \" ");
        }
		dimensiones.push_back(make_pair(height, width));
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
        CANT_IMGS_PRUEBA ++;
		if( not(todosIguales(dimensiones)) ){ cout << "Fallo. Las imagenes de prueba deben tener todas el mismo tamaño"<< endl; exit (1);}
		if( not(height * width == CANT_PIXELS_EN_IMG) ){ cout << "Fallo. Las imagenes de prueba deben tener el mismo tamaño que las de entrenamiento."<< endl; exit (1);}
        
		outFile<<endl;
    }
    //cout<<"cant img prueba: "<<CANT_IMGS_PRUEBA<<endl;
    inFile.close();
    outFile.close();
    return RutaPruebaFormatoViejo;
}





void imagenes_A_Vectores(matriz& a, matriz& b, ifstream& TestEntrada, int NoHayTest, int o, string RutaImgsEntrenamiento) // capaz a "o" la llamaría "indice"
{
   // COUT << "PASANDO IMAGENES A VECTORES  imagenes_A_Vectores" << o + 1 << endl<<endl;
    ArchivoEntrada.open(RutaImgsEntrenamiento.c_str());
    if (ArchivoEntrada.fail()){ cout << "Fallo al intentar abrir el archivo "<<"\"" <<RutaImgsEntrenamiento<<"\" " << endl; exit (1); } 
    int v = 0; // v: índice imágenes entrenamiento
    int i = 0;
    int w = 0;
    int j, h, t; // h es un píxel // t indica si la imagen se usa para entrenar o para pr0bar // j: índice píxeles
    char m; // m es un separador (una coma o un espacio) que se lee para avanzar un puntero
    while(v < CANT_IMGS_ENTRENAMIENTO){ 
        TestEntrada >> t;
        j = 0;
        if(t == 1 || NoHayTest == 1){// si "NoHayTest" está activado, no se particiona "train"
            a.resize(i + 1);
            a[i].resize(CANT_PIXELS_EN_IMG + 1);
        }else{
            b.resize(w + 1);
            b[w].resize(CANT_PIXELS_EN_IMG + 1);
        }

        while(j<CANT_PIXELS_EN_IMG + 1)
        {
            if(j == CANT_PIXELS_EN_IMG ){
                ArchivoEntrada >> h;
                assert(0 <= h && h <= 255);
            }else{
                ArchivoEntrada >> h >> m;
                assert(0 <= h && h <= 255);
            }
            if(t == 1 || NoHayTest == 1){
                a[i][j] = h;
                j++;
            }else{
                b[w][j] = h;
                j++;
            }
        }
        if(t == 1 || NoHayTest == 1){
            i++;
        }else{
            w++;
        }
        v++;
    }

   // COUT << "dimension de b = " << b.size() << " por " << b[0].size() << endl;
   // COUT << "dimension de a = " << a.size() << " por " << a[0].size() << endl;
    ArchivoEntrada.close();
}



void imagenes_A_Vectores_Salida(matriz& m_imgsEntrenamiento, matriz& m_imgsPrueba, string RutaImgsEntrenamiento, string RutaImgsPrueba){

    //COUT << "PASANDO IMAGENES A VECTORES imagenes_A_Vectores_Salida" << endl << endl;

    int pixel;
    char separador; //separa los pixeles con coma
    ArchivoEntrenamientoSalida.open(RutaImgsEntrenamiento.c_str());
    if (ArchivoEntrenamientoSalida.fail()){ cout << "Fallo al intentar abrir el archivo "<<"\"" <<RutaImgsEntrenamiento<<"\" " << endl; exit (1); } 
    for(int idx_entrenamiento = 0; idx_entrenamiento< CANT_IMGS_ENTRENAMIENTO; idx_entrenamiento++){
        m_imgsEntrenamiento.resize(idx_entrenamiento + 1); //redimensiona a para agregar la imagen de entrenamiento iesima
        m_imgsEntrenamiento[idx_entrenamiento].resize(CANT_PIXELS_EN_IMG + 1); //la primer coordenada de la imagen es la etiqueta y las restantes son los pixeles
        for(int idx_pixel = 0; idx_pixel < CANT_PIXELS_EN_IMG + 1 ; idx_pixel++){
            if(idx_pixel == CANT_PIXELS_EN_IMG){
                ArchivoEntrenamientoSalida >> pixel;
                assert(0 <= pixel && pixel <= 255);
            } else {
                ArchivoEntrenamientoSalida >> pixel >> separador;
                assert(0 <= pixel && pixel <= 255);
            }
            m_imgsEntrenamiento[idx_entrenamiento][idx_pixel] = pixel;
        }
    }
    ArchivoEntrenamientoSalida.close();

    ArchivoPruebaSalida.open(RutaImgsPrueba.c_str());
    if (ArchivoPruebaSalida.fail()){ cout << "Fallo al intentar abrir el archivo "<<"\"" <<RutaImgsPrueba<<"\" " << endl; exit (1); }
    for (int idx_pruebas = 0; idx_pruebas < CANT_IMGS_PRUEBA; idx_pruebas++){
        m_imgsPrueba.resize(idx_pruebas + 1); //redimensiona a para agregar la imagen de entrenamiento iesima
        m_imgsPrueba[idx_pruebas].resize(CANT_PIXELS_EN_IMG); //la primer coordenada de la imagen es la etiqueta y las restantes son los pixeles
        for(int idx_pixeles = 0 ; idx_pixeles < CANT_PIXELS_EN_IMG; idx_pixeles++){ // acá el +1 mepa que está de más. arriba tmb
            // COUT << "indice_pixeles: " << indice_pixeles << endl;
            if(idx_pixeles == CANT_PIXELS_EN_IMG){
                ArchivoPruebaSalida >> pixel;
                assert(0 <= pixel && pixel <= 255);
            } else {
                ArchivoPruebaSalida >> pixel >> separador;
                assert(0 <= pixel && pixel <= 255);
            }
            m_imgsPrueba[idx_pruebas][idx_pixeles] = pixel;
        }
    }
    ArchivoPruebaSalida.close();
    return;
}



/*
vector<pair<int,double> > ordenarPrimeraskDistancias(vector<pair<int,double> >& distancias, int k)
{
    vector<pair<int,double> > k_vecinos;
    k_vecinos.resize(k);
    vector<pair<int,double> > aux;
    aux.resize(1);
    int min;    

    for(int i = 0; i < k; i++){
        min = i;
        for(int j = i + 1; j < distancias.size(); j++){
            if(distancias[min].second > distancias[j].second) {
                min = j;
                aux[0] = make_pair(distancias[min].first,distancias[min].second);
                distancias[min] = distancias[j];
                distancias[j] = aux[0] ;
            }
        }
        k_vecinos[i] = distancias[i];
    }
    return k_vecinos;
}
*/






std::ostream& operator<<(std::ostream& o, const vector<pair<int, double>> & v){ 
    for(int i = 0 ; i< v.size(); i++){
            o << v[i].first <<  '\t' << v[i].second << endl;
    }
    return o;
}



bool pairCompare( const pair<int, double>& x, const pair<int, double>& y ){
    return x.second < y.second;
}


vector<pair<int,double> > ordenarPrimeraskDistancias(vector<pair<int,double> >& distancias, int k){
    vector<pair<int, double> > res = distancias;
    sort(res.begin(), res.end(), pairCompare);
    vector<pair<int,double> > res_k(res.begin(), res.begin() + k);
    return res_k;
}



/*
int vecinoGanador(vector<pair<int,double> >& k_vecinos, int f)// f es el numero de imagen
{
    int i;
    int j = 0;
    int cantRepeticiones;
    int sonIguales = -1;
    int mayoriaAbsoluta = 1;
    int respuesta = -1;

    while(j < k_vecinos.size()){

        i = j + 1;
        cantRepeticiones = 1;

        while(i < k_vecinos.size()){
            if(k_vecinos[j].first == k_vecinos[i].first){
                cantRepeticiones++;
            }
            i++;
        }
        if(cantRepeticiones > mayoriaAbsoluta){
            mayoriaAbsoluta = cantRepeticiones;
            respuesta = k_vecinos[j].first;
        }else{
            if(cantRepeticiones == mayoriaAbsoluta){
                sonIguales = mayoriaAbsoluta;
            }
        }
        j++;
    }
    if(mayoriaAbsoluta == sonIguales){
        //COUT << "NO HAY VECINO DOMINANTE PARA LA IMAGEN NUMERO " << f << endl;
        respuesta = k_vecinos[0].first;
    }
    return respuesta;
}

*/



vector< tuple<int, int, double> > VectorDeIdRepeticionesYMinDistancia( vector<pair<int,double> >& k_vecinos){
    vector<tuple<int, int, double>> IdRepsDist; 
    for(int i =0; i <= CANT_CLASES; i++){
		IdRepsDist.push_back(tuple<int, int, double> (0,0,9999999.9));
	}	
    for(int i =0; i < k_vecinos.size(); i++){	
		int id = k_vecinos[i].first;
        double dist = k_vecinos[i].second;
        std::get<0>(IdRepsDist[id]) = id; //id
        std::get<1>(IdRepsDist[id]) ++; //repeticiones
        if( dist < std::get<2>(IdRepsDist[id]) ) {
            std::get<2>(IdRepsDist[id]) = dist; //distancia
        }
	}
    return IdRepsDist;
}



bool maxReps( const tuple<int, int, double>& x, const tuple<int, int, double>& y ){
    return get<1>(x) > get<1>(y);
}



int vecinoGanador(vector<pair<int,double> >& k_vecinos, int f){// f es el numero de imagen
    vector<tuple<int, int, double>> IdRepsDist= VectorDeIdRepeticionesYMinDistancia(k_vecinos);
    sort(IdRepsDist.begin(), IdRepsDist.end(), maxReps); //ordeno por repeticiones de mayor a menor
    int mejor_id= get<0>(IdRepsDist[0]);
    int max_reps= get<1>(IdRepsDist[0]);
    double min_dist= get<2>(IdRepsDist[0]);
    for (int i = 1; get<1>(IdRepsDist[i]) == max_reps && i < k_vecinos.size(); i++){
        if( get<2>(IdRepsDist[i]) < min_dist ){
            min_dist = get<2>(IdRepsDist[i]);
            mejor_id = get<0>(IdRepsDist[i]);
        }
    }
    return mejor_id;
}






void mostrarVectorOrdenado(vector<pair<int,double> >& distancias){
    int i = 0;
    while(i < distancias.size()){
        COUT << "Vector de distancias[" << i << "] = " << distancias[i].first << " , " << distancias[i].second << endl;
        i++;
    }
}




void mostrarVector(vector<int>& a){
    int i = 0;
    while(i < a.size()){
        cout << "vector[" << i << "] = " << a[i] << endl;
        i++;
    }
}

void mostrarVector(vector<double>& a){
    int i = 0;
    while(i < a.size()){
        cout << "vector[" << i << "] = " << a[i] << endl;
        i++;
    }
}


vector<int> Knn(matriz& ImagenesEntrenamiento, matriz& ImagenesTest, int k, int alfa, int metodo){
    
    if (k > ImagenesEntrenamiento.size()-1){ cout <<endl<< "	Fallo parametro k.  En esta corrida k debe ser menor a "<< ImagenesEntrenamiento.size() <<". Ahora vale "<<k<<endl; exit (1); }
    vector<pair<int,double> > k_vecinos;
    vector<int> respuestas;
    double distanciaImagen, distanciaCoordendas;
    alfa = alfa + 1; 
    respuestas.resize(ImagenesTest.size());
    cout <<endl<<  "imagenes entrenamiento: " << ImagenesEntrenamiento.size() ;
    cout << " -  imagenes test: " << ImagenesTest.size();
    if(metodo == 0){
        alfa = CANT_PIXELS_EN_IMG; //ImagenesTest[0].size();  // si solo hacemos Knn no reducimos dimensiones
        cout << " -  alfa met0: " << alfa << endl;
    }
    for(int f = 0; f < ImagenesTest.size(); f++){
        vector<pair<int,double> > distancias;
        distancias.resize(ImagenesEntrenamiento.size());
        for(int i = 0; i < ImagenesEntrenamiento.size(); i++){
            distanciaImagen = 0;
            distanciaCoordendas = 0;
            for(int j= 1; j < alfa; j++){
                distanciaCoordendas = distanciaCoordendas + pow(ImagenesEntrenamiento[i][j] - ImagenesTest[f][j], 2);
            }
            distanciaImagen = sqrt(distanciaCoordendas);
            distancias[i] = (make_pair(ImagenesEntrenamiento[i][0],distanciaImagen)); // par(id_sujeto, distancia)
        }
        k_vecinos = ordenarPrimeraskDistancias(distancias, k);
        respuestas[f] = vecinoGanador(k_vecinos, f);
        //COUT <<"respuesta[" << f << "] = " << respuestas[f] << endl;
    }
    //mostrarVector(respuestas);
    return respuestas;
}




float Precision(matriz& ImagenesTest, vector<int>& resultados, int j, FILE* ArchivoSalidaReporte){

    vector<float> tpi;
    for (int h=0; h<CANT_CLASES; ++h){
        tpi.push_back(0.0);
    }

    vector<float> fpi;
    for (int k=0; k<CANT_CLASES; ++k){
        fpi.push_back(0.0);
    }

    vector<float> precClases;
    for (int k=0; k<CANT_CLASES; ++k){
        precClases.push_back(0.0);
    }

    int i = 0;
    while(i < resultados.size()){

        for (int m=0; m<CANT_CLASES; ++m){

            if( m == resultados[i] ){

                if (ImagenesTest[i][0] == m){
                    //COUT << "ENTRA A TPI " << endl;
                    tpi[m] = tpi[m] + 1.0;
                }else{
                    //COUT << "ENTRA A FPI " << endl;
                    fpi[m] = fpi[m] + 1.0;
                }
            }
        }
        i++;
    }

    float divisor;
    for (int i=0; i<CANT_CLASES; ++i){
        //COUT << "FPI " << fpi[i] << endl;
        //COUT << "TPI " << tpi[i] << endl;
        float t = tpi[i];
        float f = fpi[i];
        divisor = t+f;
        //COUT << "t " << t << endl;
        //COUT << "f " << f << endl;


        float pre = t/(t+f);
        //COUT << "precison " << pre << endl;

        divisor = tpi[i]+fpi[i];
        //COUT << "DIVISOR " << divisor << endl;

        precClases[i] = pre;
        //COUT << "prec clase i " << precClases[i] << endl;
        fprintf(ArchivoSalidaReporte, "Precision clase %d :  %4.6f\n", i, precClases[i]);
        //COUT << "Precision clase  " << i << " :" << precClases[i] << endl;
    }

    float precision = 0;
    for (int i=0; i<CANT_CLASES; ++i){
        precision = precision + precClases[i];
    }

    precision = precision/CANT_CLASES;
    return precision;
}





float Recall(matriz& ImagenesTest, vector<int>& resultados, int j, FILE* ArchivoSalidaReporte){

    float recall = 0;
    int i = 0;
    vector<float> tpi;
    for (int h=0; h<CANT_CLASES; h++){
    tpi.push_back(0.0);
    }
    vector<float> fni;
    for (int k=0; k<CANT_CLASES; k++){
    fni.push_back(0.0);
    }
    vector<float> recallClases;
    for (int k=0; k<CANT_CLASES; k++){
    recallClases.push_back(0.0);
    }
    //cout << endl;
    //cout << "TEST " << j+1 << endl;

    while(i < resultados.size()){
        for (int m=0; m<CANT_CLASES; m++){
			if( m == resultados[i] ){
                if (ImagenesTest[i][0] == m){
                    //cout << "ENTRA A TPI " << endl;
                    tpi[m] = tpi[m] + 1.0;
                }
            }
            if ((ImagenesTest[i][0] == m && ImagenesTest[i][0] != resultados[i] ))
            {
                fni[m] = fni[m] + 1.0;
            }
		}
        i++;
    }
    float divisor;
    for (int i=0; i<CANT_CLASES; ++i){
        //cout << "FNI " << fni[i] << endl;
        //cout << "TPI " << tpi[i] << endl;
        float t = tpi[i];
        float f = fni[i];
        divisor = t+f;
        //cout << "t " << t << endl;
        //cout << "f " << f << endl;


        float pre = t/(t+f);
        //cout << "precison " << pre << endl;

        divisor = tpi[i]+fni[i];
        //cout << "DIVISOR " << divisor << endl;

        recallClases[i] = pre;
        //cout << "recall clase i " << recallClases[i] << endl;
        fprintf(ArchivoSalidaReporte, "Recall clase %d :  %4.6f\n", i, recallClases[i]);
        //cout << "Recall clase  " << i << " :" << recallClases[i] << endl;
    }
    for (int i=0; i<CANT_CLASES; ++i){
    recall = recall + recallClases[i];
    }
    recall = recall/CANT_CLASES;
    return recall;
}




float resultadosCorrectos(matriz& ImagenesTest, vector<int>& resultados, int j, FILE* ArchivoSalidaReporte)
{
    float resultadosCorrectos = 0;
    float resultadosIncorrectos = 0;
    float porcentajeAciertos;
    int i = 0;

    cout <<endl;
    cout << "TEST " << j+1 << endl;

    if(!(resultados.size() == ImagenesTest.size()))
    {
        cout << "ALGO ANDA MAL" << endl;
    }

    while(i < resultados.size())
    {
        if(ImagenesTest[i][0] == resultados[i])
        {
            resultadosCorrectos++;
        }else
        {
            resultadosIncorrectos++;
        }

        i++;
    }

    if(!(resultadosIncorrectos + resultadosCorrectos == resultados.size())){
        cout << "ALGO ANDA MAL" << endl;
    }

    porcentajeAciertos = (resultadosCorrectos/resultados.size())*100;


    // acá se muestra por pantalla
    cout << "Resultados correctos: " << resultadosCorrectos << endl;
    cout << "Resultados incorrectos: " << resultadosIncorrectos << endl;
    cout.setf(ios::fixed);
    cout <<"Porcentaje de aciertos: " << setprecision(3) << porcentajeAciertos << endl;
    cout << setprecision(0) << endl <<endl;

    // acá se escribe en un archivo
    fprintf(ArchivoSalidaReporte, "Resultados correctos: %4.6f\n", resultadosCorrectos);
    fprintf(ArchivoSalidaReporte, "Resultados incorrectos: %4.6f\n", resultadosIncorrectos);
    fprintf(ArchivoSalidaReporte, "Porcentaje de aciertos: %4.6f\n", porcentajeAciertos);
    return porcentajeAciertos;

}


// de acá para abajo comentás si querés hacer solo Knn usando matriz de ints

/*
vector <double> calculoVectorMedias(matriz& ImagenesEntrenamiento)
{
    COUT << "CALCULANDO VECTOR DE MEDIAS" << endl;
    COUT << endl;

    vector <double> media;
    int i = 0;
    int j = 1;
    media.resize(ImagenesEntrenamiento[0].size());

    while(j < ImagenesEntrenamiento[0].size())
    {
        i = 0;
        media[j - 1] = 0;

        while(i < ImagenesEntrenamiento.size())
        {
            media[j - 1] = (media[j - 1] + ImagenesEntrenamiento[i][j]);
            i++;
        }

        media[j - 1] = media[j - 1]/(ImagenesEntrenamiento.size());
        j++;
    }

    return media;
}
*/
std::ostream& operator<<(std::ostream& o, const vector< double> & v){ 
    for(int i = 0 ; i< v.size(); i++){
            o << v[i] <<'\t' ;
    }
    cout<<endl;
    return o;
}



vector <double> calculoVectorMedias(matriz& ImagenesEntrenamiento)
{	
cout<<"CANT_IMGS_ENTRENAMIENTO: "<<CANT_IMGS_ENTRENAMIENTO<<endl;
cout<<"CANT_PIXELS_EN_IMG: "<< CANT_PIXELS_EN_IMG<<endl;
   vector <double> medias (CANT_PIXELS_EN_IMG);
	for(int j = 0; j< CANT_PIXELS_EN_IMG; j++){	
		for(int i = 0; i< CANT_IMGS_ENTRENAMIENTO; i++){
			cout<<"i "<<i<<" j "<<j<<endl;			
			medias[j] += ImagenesEntrenamiento[i][j+1];
		}
		medias[j] = medias[j]/ CANT_IMGS_ENTRENAMIENTO;
	}
	cout<<"medias: "<<medias<<endl;
	return medias;
}
   




matriz armarMatrizX(matriz& Imagenes, vector<double>& media, int n)// n es la cantidad de imagenes de entrenamiento
{
    matriz b;
    b.resize(Imagenes.size());
    for(int i = 0; i < b.size(); i++){
        b[i].resize(CANT_PIXELS_EN_IMG);
    }
    for(int j = 0; j <  CANT_PIXELS_EN_IMG; j++){
        for(int i = 0; i < CANT_IMGS_ENTRENAMIENTO; i++){
            b[i][j] = (Imagenes[i][j + 1] - media[j])/(sqrt(n - 1));
        }
    }
    return b;
}





matriz armarMatrizY(matriz& Imagenes, vector<double>& media, int n)// n es la cantidad de imagenes de entrenamiento
{
    int i = 0;
    int j = 0;

    while(j < Imagenes[0].size())
    {
        i = 0;

        while(i < Imagenes.size())
        {
            Imagenes[i][j] = (Imagenes[i][j] - media[j])/(sqrt(n - 1));
            i++;
        }

        j++;
    }

    return Imagenes;
}





void centrar(matriz& Imagenes, vector<double>& media, int n)
{
    int i = 0;
    int j = 1;
    while(j < Imagenes[0].size())
    {
        i = 0;

        while(i < Imagenes.size())
        {
            Imagenes[i][j] = (Imagenes[i][j] - media[j - 1])/(sqrt(n - 1));
            i++;

        }
        j++;
    }
}





matriz Trasponer(matriz& a)
{

    matriz Traspuesta;
    int i = 0;
    int j;

    Traspuesta.resize(a[0].size());

    while(i < a[0].size())
    {
        Traspuesta[i].resize(a.size());
        j = 0;

        while(j < a.size())
        {
            Traspuesta[i][j] = a[j][i];
            j++;
        }
        i++;

    }


    return Traspuesta;
}






vector<double> matrizPorVector(matriz& A, vector<double>& x)
{
    vector<double> solucion;
    int i = 0;
    int j;
    solucion.resize(A.size());

    while(i < A.size())
    {
        j = 0;
        solucion[i] = 0;

        while(j < A[i].size())
        {
            solucion[i] = solucion[i] + A[i][j] * x[j];
            j++;
        }

        i++;
    }

    return solucion;
}





std::ostream& operator<<(std::ostream& o, const matriz & v){ 
    for(int i = 0 ; i < v.size(); i++){
		for(int j= 0; j < v[0].size(); j++){
            o << v[i][j] << '\t' ;
        }
        o << endl;
    }
    cout<<endl;
    return o;
}



matriz matrizCovarianza(matriz ImagenesEntrenamiento, vector<double>& media)
{	
	//cout<<"ENTRENAMIENTO: "<<endl<<ImagenesEntrenamiento<<endl;
    COUT << "CALCULANDO MATRIZ DE COVARIANZAS" << endl << endl;

    matriz matrizCovarianza, matrizX, Traspuesta;
    media = calculoVectorMedias(ImagenesEntrenamiento); // promedio (de cada dimension), μ
    cout<<"MEDIAS"<<endl;
    //cout<< media<<endl;
    COUT << "ARMANDO MATRIZ X" << endl;
    COUT << endl;

    matrizX = armarMatrizX(ImagenesEntrenamiento, media, ImagenesEntrenamiento.size()); // Matriz ImagenesEntrenamiento donde a cada coordenada se le resta μ
    Traspuesta = Trasponer(matrizX);

    int i = 0;
    int j;

    COUT << "CALCULANDO MATRIZ DE COVARIANZAS COORDENADA A COORDENADA" << endl;
    COUT << endl;

    matrizCovarianza.resize(Traspuesta.size());

    while(i < Traspuesta.size())//La matriz resultante esta traspuesta pero no importa porque es simetrica
    {
        matrizCovarianza[i].resize(Traspuesta.size());
        matrizCovarianza[i] = matrizPorVector(Traspuesta, Traspuesta[i]);
        i++;
       // COUT << i << " de"<< CANT_PIXELS_EN_IMG <<" vectores terminados" << endl;
    }

    i = 0;

    while(i < matrizCovarianza.size())
    {
        j = 0;

        while(j < matrizCovarianza.size())
        {
            matrizCovarianza[i][j] = matrizCovarianza[i][j];//(Traspuesta[i].size() - 1);
            j++;
        }

        i++;
    }

    COUT << endl;

    return matrizCovarianza;
}





void testCovarianzaSimetrica(matriz& a) // por qué "a" y no "m"? :P
{
    cout << "TESTEANDO SIMETRIA DE MATRIZ DE COVARIANZAS" << endl << endl;

    int i = 0; // indice_filas
    int j; // indice_columnas
    int contador;

    if (a.size() != a[i].size()) {cout << "ERROR, LA MATRIZ DE COVARIANZAS NO ES CUADRADA" << endl;}

    while(i < a.size())
    {
        j = i + 1;

        while(j < a.size())
        {
            //cout << "a[" << i << "][" << j << "] vale: " << a[i][j] << endl;
            if(a[i][j] == 0)
            {
                contador++;
            }

            if(a[i][j] != a[j][i])
            {
                cout << "ERROR, LA MATRIZ DE COVARIANZAS NO ES SIMETRICA" << endl;
                break;
            }else{j++;}
        }

        if((j != a.size()) && (a[i][j] != a[j][i])){break;}
        else{i++;}
    }

    if(contador == a.size() * a.size())
    {
        cout << "LA MATRIZ DE COVARIANZAS ES NULA" << endl << endl;
    }

    cout << "BIEN! LA MATRIZ DE COVARIANZAS ES SIMETRICA!" << endl;
    cout << endl;
}






double norma(vector <double>& a)
{
    double norma_a = 0;
    int i = 0;

    while(i < a.size())
    {
        norma_a = norma_a + a[i]*a[i];
        i++;
    }

    norma_a = sqrt(norma_a);

    return norma_a;
}





matriz deflacion(matriz& matrizCovarianzas, double& autovalor, vector<double>& autovector)
{
    int i = 0;
    int j;

    while(i < matrizCovarianzas.size())
    {
        j = 0;

        while(j < matrizCovarianzas.size())
        {
            matrizCovarianzas[i][j] = matrizCovarianzas[i][j] - autovalor * autovector[i] * autovector[j];
            j++;
        }

        i++;
    }

    return matrizCovarianzas;
}





vector<double> normalizoX(vector<double>& x)
{
    int i = 0;
    double norm;

    norm = norma(x);

    while(i < x.size())
    {
        x[i] = x[i]/norm;
        i++;
    }

    return x;
}





vector<double> metodoDeLaPotencia(matriz& matrizCovarianzas, int alfa, matriz& autovectoresTraspuestos, const int CANT_ITERS_MET_POT) // devuelve un vector<double> c/los avals. vamos a necesitar los avects también, para hacer el cambio de base de los datos. devuelve matriz de avects traspuestos
{

    COUT << "CALCULANDO AUTOVALORES Y AUTOVECTORES" << endl;
    COUT << endl;

    vector<double> autovalores, x, K_MasUno; // K_MasUno es el vector X en la iteracion k + 1. Una vez que se tiene X = autovector, al multiplicarlo una vez mas por A tenemos un vector multiplo, a partir de ese vector y del anterior se obtiene el lambda autovalor
    srand(time(NULL)); // para que los numeros random no sean siempre los mismos. Con esto van a depender de la hora del reloj de la computadora
    int k;
    double y, z;
    int i = 0;

    autovalores.resize(alfa);
    autovectoresTraspuestos.resize(alfa);
    x.resize(matrizCovarianzas.size());

    while(i < alfa)
    {

        for(int u = 0; u < x.size(); u++) //GENERAR UN VECTOR X DE TAMAÑO MATRIZCOVARIANZAS.SIZE() Y VALORES RANDOM
        {
            x[u] = 0 + rand();
        }

        x = normalizoX(x);

        autovectoresTraspuestos[i].resize(x.size());
        autovalores[i] = 0;
        k = 0;

        while(k < CANT_ITERS_MET_POT)
        {
                x = matrizPorVector(matrizCovarianzas, x);
                x = normalizoX(x);
                k++;
        }

        K_MasUno = matrizPorVector(matrizCovarianzas,x);

        autovalores[i] = norma(K_MasUno);
        autovectoresTraspuestos[i] = x;
        if(alfa > 1)matrizCovarianzas = deflacion(matrizCovarianzas, autovalores[i], autovectoresTraspuestos[i]);
        i++;
    }

    return autovalores;
}





vector<double> transformacionCaracteristica(vector<double>& Imagen_i, matriz& autovectoresTraspuestos)
{
    vector<double> solucion;
    int i = 1;
    int j;
    solucion.resize(autovectoresTraspuestos.size() + 1);//La primera coordenada es la etiqueta
    solucion[0] = Imagen_i[0];
    while(i < solucion.size())
    {
        j = 1;
        solucion[i] = 0;
        while(j < Imagen_i.size())
        {
            solucion[i] = solucion[i] + autovectoresTraspuestos[i - 1][j - 1] * Imagen_i[j];
            j++;
        }
        i++;
    }
    return solucion;
}





void escribirMatrizEnConsola(matriz& m){

    int i = 0;
    int j;
    while( i < m.size() ){

        j = 0;
        while( j < m[i].size() ){

            if(j == m[i].size() - 1){
                printf("%.0f", m[i][j]);
            }else{
                printf("%.0f,", m[i][j]);
            }

            j++;
        }
        cout << endl;
        i++;
    }
    cout << endl;
}





void escribirMatrizEnArchivo(matriz& m, FILE* archivo){
    int i = 0;
    int j;

    while(i < m.size())
    {
    j = 0;

    while(j < m[i].size())
    {

        if(j == m[i].size() - 1)
        {
            fprintf(archivo, "%.0f", m[i][j]);
        }else
            {
                fprintf(archivo, "%.0f,", m[i][j]);
            }

        j++;
    }

    fprintf(archivo, "\n");
    i++;
    }
}





matriz generarMatrizPreY(matriz& ImagenesEntrenamiento){

    matriz PreY;
    PreY.resize(ImagenesEntrenamiento.size());
    int i = 0;
    int j;
    while(i < PreY.size())
    {
        PreY[i].resize(CANT_CLASES);
        j = 0;

        while(j < CANT_CLASES)
        {
            if (ImagenesEntrenamiento[i][0] == j)
            {
                PreY[i][j] = 1;
            }
            else
            {
                PreY[i][j] = -1;
            }

            j++;
        }

        i++;
    }

    return PreY;
}





void generarMatriz_Mi(matriz& matrizX, matriz& matrizY, matriz& matriz_Mi)
{
    matriz TraspuestaX, TraspuestaY, aux, aux2;

    TraspuestaX = Trasponer(matrizX);
    TraspuestaY = Trasponer(matrizY);

    int i = 0;

    aux.resize(TraspuestaY.size());

    while(i < TraspuestaY.size()) // X traspuesta por Y. El resultado queda traspuesto
    {
        aux[i].resize(TraspuestaX.size());
        aux[i] = matrizPorVector(TraspuestaX, TraspuestaY[i]);
        i++;
    }

    aux = Trasponer(aux);

    i = 0;

    aux2.resize(TraspuestaX.size());

    while(i < TraspuestaX.size())
    {
        aux2[i].resize(TraspuestaY.size());
        aux2[i] = matrizPorVector(TraspuestaY, TraspuestaX[i]);
        i++;
    }

    i = 0;

    matriz_Mi.resize(aux2.size());

    while(i < aux2.size())
    {
        matriz_Mi[i].resize(aux.size());
        matriz_Mi[i] = matrizPorVector(aux, aux2[i]);
        i++;
    }

    matriz_Mi = Trasponer(matriz_Mi);

    /*aux = Trasponer(aux); // Lo traspongo para que quede bien
    aux2.resize(matrizY.size());

    while(i < matrizY.size()) // Lo de antes por Y traspuesta. Es decir X traspuesta por Y por Y traspuesta
    {
        aux2[i].resize(aux.size());
        aux2[i] = matrizPorVector(aux, matrizY[i]);
        i++;
    }

    i = 0;

    aux2 = Trasponer(aux2);
    matriz_Mi.resize(TraspuestaX.size());

    while(i < TraspuestaX.size()) // Lo de antes por X
    {
        matriz_Mi[i].resize(aux2.size());
        matriz_Mi[i] = matrizPorVector(aux2, TraspuestaX[i]);
        i++;
    }

    matriz_Mi = Trasponer(matriz_Mi);*/

}





matriz actualizoMatriz(matriz& a, vector<double>& Ti)
{
    matriz aTraspuesta;
    vector<double> b;
    int i = 0;
    int j;
    aTraspuesta = Trasponer(a);
    b = matrizPorVector(aTraspuesta, Ti);

    while(i < a.size())
    {
        j = 0;

        while(j < a[i].size())
        {
            a[i][j] = a[i][j] - Ti[i] * b[j];
            j++;
        }

        i++;
    }

    return a;
}

