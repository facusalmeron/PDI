#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <iomanip>
#include "pdi_functions.h"
#include <vector>
#include <opencv2/core/cvdef.h>
#include <bitset>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iterator>
#include <opencv2/core/types.hpp>
#include <opencv2/core/matx.hpp>
#include <opencv2/imgproc.hpp>
#include <ctime>


using namespace cv;
using namespace pdi;
using namespace std;

Mat ImgEj1(){
	//Imagen que dan como referencia en el ejercicio 1 para ver los resultados al aplicar el EE.
	Mat img=cv::Mat::zeros(12,11,CV_32F);
	img.at<float>(1,1)=1;
	img.at<float>(1,2)=1;
	img.at<float>(1,3)=1;
	img.at<float>(2,3)=1;
	img.at<float>(2,8)=1;
	img.at<float>(3,3)=1;
	img.at<float>(3,7)=1;
	img.at<float>(3,8)=1;
	img.at<float>(3,9)=1;
	img.at<float>(4,2)=1;
	img.at<float>(4,3)=1;
	img.at<float>(4,4)=1;
	img.at<float>(4,7)=1;
	img.at<float>(4,8)=1;
	img.at<float>(4,9)=1;
	img.at<float>(6,1)=1;
	img.at<float>(7,2)=1;
	img.at<float>(7,7)=1;
	img.at<float>(7,8)=1;
	img.at<float>(7,9)=1;
	img.at<float>(8,3)=1;
	img.at<float>(8,9)=1;
	img.at<float>(9,6)=1;
	img.at<float>(9,7)=1;
	img.at<float>(9,8)=1;
	img.at<float>(9,9)=1;
	img.at<float>(10,1)=1;
	img.at<float>(10,7)=1;
	img.at<float>(10,8)=1;
	img.at<float>(10,9)=1;
	return img;
}

Mat ruido_sal_pimienta(Mat img, float pa, float pb ){   
	//pa y pb cuantos de sal y cuantos de pimienta agrego.
	RNG rng; // rand number generate
	
	int cantidad1=img.rows*img.cols*pa; //determino cuantos pixeles debo cambiar en total,
	//entonces cuanto menor sea la cantidad de pa y pb menos pixeles va a cambiar
	int cantidad2=img.rows*img.cols*pb;
	for(int i=0; i<cantidad1; i++){
		img.at<uchar>(rng.uniform(0,img.rows), rng.uniform(0, img.cols)) =0; //elige valores al azar de la imagen
		//y los pone en negro. Esto lo hace cantidad1 de veces que es la cantidad de pixeles que quiero poner de pimienta.
	}
	for (int i=0; i<cantidad2; i++){
		img.at<uchar>(rng.uniform(0,img.rows), rng.uniform(0,img.cols)) = 255;
	}
	return img;
}

void Ejercicio1(){
	Mat img=ImgEj1();
//	Mat img=imread("ejercicio2.tif",CV_LOAD_IMAGE_GRAYSCALE);
//	Mat img=imread("img.tif",CV_LOAD_IMAGE_GRAYSCALE);
//	img=ruido_sal_pimienta(img,0.01,0.01);
	//Si le agrego ruido me lo enfatiza, el dilatado muestra mas el ruido sal y elimina el pimienta y el de erosion al reves.
	//Es bueno para eliminar ruidos.
//	img.convertTo(img,CV_32F,1./255);
	//con getStructuringElement consigo el elemento estructurante, los parametros son los siguientes:
	//1º Forma del elemento: MORPH_RECT (rectangular), MORPH_ELLIPSE (elipse), MORPH_CROSS (cruz), CV_SHAPE_CUSTOM (personalizado)
	//2º Size (n*n) tamaño del EE.
	Mat EE=getStructuringElement(MORPH_RECT,Size(3,3));
	for(int i=0;i<EE.rows;i++) { 	for(int j=0;j<EE.cols;j++) { cout<<EE.at<bool>(i,j)<<" "; }cout<<endl;	}
	Mat erosion;
	Mat dilatado;
	dilate(img,dilatado,EE);
	erode(img,erosion,EE);
	namedWindow("Original",CV_WINDOW_KEEPRATIO);
	imshow("Original",img);
	namedWindow("Erosion",CV_WINDOW_KEEPRATIO);
	imshow("Erosion",erosion);
	namedWindow("Dilatado",CV_WINDOW_KEEPRATIO);
	imshow("Dilatado",dilatado);
	waitKey(0);
}

void Ejercicio2(){
	//Apertura --> Primero erosiona y despues dilata (suaviza contorno de un objeto), rompe istmos (conexiones pequeñas) y salientes delgadas
	//Cierre --> Primero dilata y despues erosiona (suaviza contorno de un objeto), elimina agujeros pequeños, discontinuidades estrechas
	//las fusiona y golfos largos y finos.
	Mat img=imread("ejercicio2.tif",CV_LOAD_IMAGE_GRAYSCALE);
	img.convertTo(img,CV_32F,1./255);
	Mat EE=getStructuringElement(MORPH_RECT,Size(3,3));
	Mat apertura;
	erode(img,apertura,EE);
	dilate(apertura,apertura,EE);
	
	Mat cierre;
	dilate(img,cierre,EE);
	erode(cierre,cierre,EE);
	
	namedWindow("Original",CV_WINDOW_KEEPRATIO);
	imshow("Original",img);
	namedWindow("Apertura",CV_WINDOW_KEEPRATIO);
	imshow("Apertura",apertura);
	namedWindow("Cierre",CV_WINDOW_KEEPRATIO);
	imshow("Cierre",cierre);
	waitKey(0);
}

Mat filtro_promediador(int tam){
	Mat kernel(tam,tam,CV_32F);
	float aux=1/pow(tam,2);
	for (int i=0;i<kernel.rows;i++){
		for (int j=0;j<kernel.cols;j++){
			kernel.at<float>(i,j)=aux;
		}
	}
	return kernel;
}

void Ejercicio3_1(){
	Mat img=imread("Tarjeta.jpeg",CV_LOAD_IMAGE_GRAYSCALE);
//	for(int i=0;i<img.rows;i++) { 
//		for(int j=0;j<img.cols;j++) { 
//			if (img.at<uchar>(i,j)<130){
//				img.at<uchar>(i,j)=0;
//			}
//		}
//	}
	Mat EE=getStructuringElement(MORPH_CROSS,Size(3,3));
	Mat cierre;
	dilate(img,cierre,EE);
	erode(cierre,cierre,EE);
	for(int i=0;i<cierre.rows;i++) { 
		for(int j=0;j<cierre.cols;j++) { 
			if (cierre.at<uchar>(i,j)>130){
				cierre.at<uchar>(i,j)=255;
			}
		}
	}
//	namedWindow("Original",CV_WINDOW_KEEPRATIO);
	imshow("Original",img);
//	namedWindow("Cierre",CV_WINDOW_KEEPRATIO);
	imshow("Nombre y Titulo",cierre);
	
	
	waitKey(0);
}

Mat Umbral(Mat img,int tol){
	Mat aux=Mat::zeros(img.rows,img.cols,CV_8UC(1));
	for(int i=0;i<img.rows;i++) { 
		for(int j=0;j<img.cols;j++) { 
			if(img.at<uchar>(i,j)<tol){
				aux.at<uchar>(i,j)=255;
			}
		}
	}
	return aux;
}
void Mostrar(Mat m){
	for(int i=0;i<m.rows;i++) { 
		for(int j=0;j<m.cols;j++) { 
			cout<<m.at<bool>(i,j)<<" ";
		}cout<<endl;
	}
}
void Ejercicio3_2(){
	Mat img=imread("Caracteres.jpeg",IMREAD_GRAYSCALE);
	img=Umbral(img,180);
	imshow("Original",img);
	Mat letras;
	Mat EE=getStructuringElement(MORPH_RECT,Size(3,3));
	Mostrar(EE);
	erode(img,letras,EE);
	dilate(letras,letras,EE);
	imshow("Letras",letras);
	Mat simbolos=img-letras;
	imshow("Simbolos",simbolos);
}

void Ejercicio3_3(){
	Mat img=imread("estrellas.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	imshow("Original",img);
	Mat umbralizada=cv::Mat::zeros(img.rows,img.cols,CV_32F);
	for(int i=0;i<img.rows;i++) { 
		for(int j=0;j<img.cols;j++) { 
			if (img.at<uchar>(i,j)>200){
				umbralizada.at<float>(i,j)=1;
			}
		}
	}
	Mat EE=getStructuringElement(MORPH_ELLIPSE,Size(3,3));
	Mat salida;
	erode(umbralizada,salida,EE);
	imshow("Umbralizada",umbralizada);
	for(int i=0;i<img.rows;i++) { 
		for(int j=0;j<img.cols;j++) { 
			if(salida.at<float>(i,j)==0){
				img.at<uchar>(i,j)=0;
			}
		}
	}
	imshow("Salida",img);
	waitKey(0);
}

void Ejercicio3_4(){
	Mat img=imread("lluviaEstrellas.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	Mat EE=getStructuringElement(MORPH_RECT,Size(7,7));
	for(int i=0;i<EE.rows;i++) { 
		for(int j=0;j<EE.cols;j++) { 
			EE.at<uchar>(i,j)=0;
		}
	}
	for(int i=0;i<EE.rows;i++) { 
		EE.at<uchar>(i,EE.cols-1-i)=1;
	}
	Mostrar(EE);
	Mat salida;
	erode(img,salida,EE);
	dilate(salida,salida,EE);
	namedWindow("Original",CV_WINDOW_KEEPRATIO);
	imshow("Original",img);
	namedWindow("Salida",CV_WINDOW_KEEPRATIO);
	imshow("Salida",salida);
	waitKey(0);
}

void Ejercicio3_5(){
	Mat img=imread("Globulos.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	Mat umbral=cv::Mat::zeros(img.size(),img.type());
	for(int i=0;i<img.rows;i++) { 
		for(int j=0;j<img.cols;j++) { 
			if (img.at<uchar>(i,j)>120){
				umbral.at<uchar>(i,j)=255;
			}
		}
	}
	for(int i=0;i<img.rows;i++) { 
		for(int j=0;j<img.cols;j++) { 
			if (umbral.at<uchar>(i,j)==255){
				umbral.at<uchar>(i,j)=0;
			}
			else{
				umbral.at<uchar>(i,j)=255;
			}
		}
	}
	Mat EE=getStructuringElement(MORPH_RECT,Size(3,3));
	erode(umbral,umbral,EE);
	dilate(umbral,umbral,EE);
	Mat aux=umbral.clone();
	vector<vector<Point> > contornos;
	vector<Vec4i> hierarchy;
	Mat salida=cv::Mat::zeros(img.size(),img.type());
	findContours(umbral, contornos, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
	for( int i = 0; i< contornos.size(); i++ )
	{
		drawContours(salida, contornos, i, Scalar(255), 1, 8, hierarchy, 0, Point() );
	}
	for(int i=0;i<contornos.size();i++) { 
		vector <Point> aux;
		aux=contornos[i];
		for(int j=0;j<aux.size();j++) { 
			if (aux[j].x==1 || aux[j].y==1 || aux[j].x==img.cols-2 || aux[j].y==img.rows-2){
				drawContours(salida,contornos,i,Scalar(0), 1, 8, hierarchy, 0, Point() );
				break;
			}
		}
//		cout<<contornos[i]<<endl;
	}
	namedWindow("Salida",CV_WINDOW_KEEPRATIO);
	imshow("Salida",salida);
	imshow("Aux",aux-salida);
	imshow("Original",img);
	imshow("Umbral",umbral);
	waitKey(0);
}

vector<Point> xy;
int cantclick = 10;
bool reset = false;

void  Click(int event, int x, int y, int flags, void* userdata){
	if(reset){
		xy.clear();
		reset = false;
	}
	if(event == EVENT_LBUTTONDOWN){
		cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
		xy.push_back(Point(x,y));
		if(xy.size()==cantclick){
			reset = true;
			destroyWindow("My Window");
		}
	}
}

Mat CrecimientoRegiones(Mat img,int x,int y){ //Recibe la imagen a segmentar y un (x,y) donde se planta la semilla.
	//RELLENA ELEMENTOS CONECTADOS CON UN VALOR DADO
	//Parámetros de floodFill:
	//1º Imagen a segmentar.
	//2º Punto x,y donde se planta la semilla.
	//3º Nuevo valor del dominio de los pixeles pintados (valores que toman regiones cercanas a la semilla).
	//4º Opcional --> NULL
	//5º Minima diferencia de brillo / color entre el pixel observado y uno de sus vecinos (tolerancia inferior)
	//6º Maxima diferencia de brillo / color entre el pixel observado y uno de sus vecinos (tolerancia superior)
	
	floodFill(img,Point(x,y),1,NULL,0.05,0.05);	
	//UMBRALIZO PARA QUE PASEN UNICAMENTE LOS ELEMENTOS CON INTENSIDAD 1
	//La funcion threshold aplica un umbral de nivel fijo a cada elemento del arreglo
	//Parametros: Origen, Destino, Valor del umbral, Maximo valor a usar con THRESH_BINARY y el INV, y por ultimo el tipo de umbral.
	threshold(img,img,1,1,THRESH_BINARY);			
	return img;
}

void Ejercicio3_6(){
	Mat img = imread("Rio.jpeg",CV_LOAD_IMAGE_GRAYSCALE);
	//OBTENGO LAS COORDENADAS X e Y DE DONDE HICE CLICK PARA PLANTAR LA SEMILLA.
	cantclick = 1;
	namedWindow("My Window",WINDOW_NORMAL);
	imshow("My Window",img);
	//Despues de llamar a Click, en el vector xy tengo guardadas las coordenadas donde hice click
	setMouseCallback("My Window", Click, NULL);
	waitKey(0);
	imshow("Original",img);
	
	//HAGO EL CRECIMIENTO A PARTIR DE DONDE HICE CLICK
	Mat resultado = CrecimientoRegiones(img,xy[0].x,xy[0].y);
	imshow("Resultado",resultado);
	waitKey(0);
}

int main(int argc, char** argv) {
	//Ej1.1 El de erosion me va a ir achicando las figuras que tengo en negro. Por ej con el EE cuadrado me elimina todo directamente
	//Con la dilatacion me va a agrandar los objetos que tengo en la imagen.
//	Ejercicio1();
	
//	Ejercicio2();
	
//	Ejercicio3_1();
	
//	Ejercicio3_2();
	
//	Ejercicio3_3();
	
//	Ejercicio3_4();
	
//	Ejercicio3_5();
	
	Ejercicio3_6();
	
//	Ejercicio3_pico();
	
	waitKey(0);
	return 0;
} 
