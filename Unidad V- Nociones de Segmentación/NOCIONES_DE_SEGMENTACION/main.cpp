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

using namespace cv;
using namespace pdi;
using namespace std;


Mat ruido_gaussiano(Mat img,double mean,double sigma){
	Mat ruido = img.clone();
	//	img.convertTo(img,CV_32F,1./255);
	//	img.convertTo(ruido,CV_32F,1./255);
	RNG rng;
	rng.fill(ruido, RNG::NORMAL, mean,sigma); 
	add(img, ruido, img);
	return img;
}

void Ejercicio1(int tipo){
	Mat img=imread("mosquito.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	img.convertTo(img,CV_32F,1./255);
	Mat Gx,Gy;
	Mat magnitud;
	
	//Para el ejercicio 1.3
	img=ruido_gaussiano(img,0,0.05);
	
	switch(tipo){
	case 0:		//Deteccion de bordes por Roberts
		Gx = (Mat_<double>(3,3) << 0, 0, 0, 0,-1, 0, 0, 0, 1);
		Gy = (Mat_<double>(3,3) << 0, 0, 0, 0, 0,-1, 0, 1, 0);
		break;
	case 1:		//Deteccion de bordes por Prewitt
		//Respuesta nula en zona de gris constante
		Gx = (Mat_<double>(3,3) << -1,-1,-1, 0, 0, 0, 1, 1, 1);
		Gy = (Mat_<double>(3,3) << -1, 0, 1,-1, 0, 1,-1, 0, 1);
		break;
	case 2:		//Deteccion de bordes por Sobel
		//Respuesta nula en zona de gris constante.
		//Enfatiza los pixeles mas cercanos al centro consiguiendo una mejor rta en presencia de ruido gaussiano.
		Gx = (Mat_<double>(3,3) << -1,-2,-1, 0, 0, 0, 1, 2, 1);
		Gy = (Mat_<double>(3,3) << -1, 0, 1,-2, 0, 2,-1, 0, 1);
		break;
	case 3:		//Deteccion de bordes por Laplaciano
		//Muy sensible al ruido, produce bordes dobles, no detecta direccion de bordes
		//Utilidad para clasificar ptos que pertenecen a zona clara y oscura a cada lado del borde.
		Gx = (Mat_<double>(3,3) << -1,-1,-1,-1, 8,-1,-1,-1,-1);
		break;
	case 4: 	//Deteccion de bordes por LoG (Laplaciano del Gaussiano).
		img.convertTo(img,CV_8U,255);
		Canny(img, magnitud, 50, 200, 5);
		break;
	}
	if(tipo!=4){
		if(tipo<3){
			//APLICO LAS MASCARAS A LA IMAGEN
			Mat x[2];
			x[0] = convolve(img,Gx);
			x[1] = convolve(img,Gy);
			//CALCULO LA MAGNITUD
			Mat derivada;
			merge(x,2,derivada);
			magnitud = magnitude(derivada);
		}else{
			magnitud = convolve(img,Gx);
		}
		//UMBRALIZO
		//entrada,salida,umbral,valor maximo q agarra,tipo de umbral.
		threshold(magnitud,magnitud,0.5,1,0);
	}
	imshow("Original",img);
	imshow("Bordes",magnitud);
	waitKey(0);
}


int main(int argc, char** argv) {
	//EJERCICIO 1:
	//TIPO 0: ROBERTS
	//TIPO 1: PREWITT
	//TIPO 2: SOBEL
	//TIPO 3: LAPLACIANO
	//TIPO 4: LoG
	Ejercicio1(2);
	waitKey(0);
	return 0;
} 
