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



void Ejercicio3_2(){
	Mat img=imread("Caracteres.jpeg",CV_LOAD_IMAGE_GRAYSCALE);
	Mat EE=getStructuringElement(MORPH_RECT,Size(3,3));
	Mat cierre;
	dilate(img,cierre,EE);
	erode(cierre,cierre,EE);
	
	Mat kernel=filtro_promediador(3);
	cierre=convolve(cierre,kernel);
	
	//	namedWindow("Original",CV_WINDOW_KEEPRATIO);
	imshow("Original",img);
	//	namedWindow("Cierre",CV_WINDOW_KEEPRATIO);
	imshow("Letras",cierre);
	waitKey(0);
}


int main(int argc, char** argv) {
	//Ej1.1 El de erosion me va a ir achicando las figuras que tengo en negro. Por ej con el EE cuadrado me elimina todo directamente
	//Con la dilatacion me va a agrandar los objetos que tengo en la imagen.
//	Ejercicio1();
	
//	Ejercicio2();
	
//	Ejercicio3_1();
	
//	Ejercicio3_2();
	
	waitKey(0);
	return 0;
} 
