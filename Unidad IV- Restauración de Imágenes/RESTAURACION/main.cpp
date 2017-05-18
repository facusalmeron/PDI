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

Mat ruido_gaussiano(Mat img,double mean,double sigma){
	Mat ruido = img.clone();
	img.convertTo(img,CV_32F,1./255);
	img.convertTo(ruido,CV_32F,1./255);
	RNG rng;
	rng.fill(ruido, RNG::NORMAL, mean,sigma); 
	add(img, ruido, img);
	return img;
}

void Ejercicio1(){
	Mat img=imread("circulo.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	Mat canvas_original(img.rows,img.cols,CV_32F);
	Mat histo_original=histogram(img,255);
	normalize(histo_original,histo_original,0,1,CV_MINMAX);
	draw_graph(canvas_original,histo_original);
	
	//Ruido sal y pimienta
	Mat syp=img.clone();
	syp=ruido_sal_pimienta(syp,10/255.0,10/255.0);
	Mat canvas_syp(syp.rows,syp.cols,CV_32F);
	Mat histo_syp=histogram(syp,255);
	normalize(histo_syp,histo_syp,0,1,CV_MINMAX);
	draw_graph(canvas_syp,histo_syp);
	
	//Ruido Gaussiano
	Mat rg=img.clone();
	rg=ruido_gaussiano(rg,0,0.10);
	Mat canvas_rg(rg.rows,rg.cols,CV_32F);
	info(canvas_rg);
	Mat histo_rg=histogram(rg,255);
	normalize(histo_rg,histo_rg,0,1,CV_MINMAX);
	draw_graph(canvas_rg,histo_rg);
	
	imshow("Original",img);
	imshow("Histograma Original",canvas_original);
	imshow("Ruido Sal y Pimienta",syp);
	imshow("Histograma Sal y Pimienta",canvas_syp);
	imshow("Ruido Gaussiano",rg);
	imshow("Histograma Ruido Gaussiano",histo_rg);
	waitKey(0);
}




int main(int argc, char** argv) {
	Ejercicio1();

	waitKey(0);
	return 0;
} 
