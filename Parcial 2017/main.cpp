#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <iomanip>
#include "pdi_functions.h"
#include "funciones_FS.h"
#include "utils.h"
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
#include <sstream>


using namespace cv;
using namespace pdi;
using namespace std;
using namespace fs;
using namespace utils;

//void Brujula(Mat img){
//	Mat aux=img.clone();
////	Mat roi=aux(Rect(167,208,6,5));
//
//	
//	vector <Mat> bgr; 	
//	split(aux, bgr);
////	
//	Mat mascara= Mat::zeros(aux.size(), aux.type());
//	inRange(aux,Scalar(0,0,200),Scalar(30,30,255),mascara); //regiones aproximadas sacadas de los histogramas
////	
//	Mat EE=getStructuringElement(MORPH_RECT,Size(3,3));
//	erode(mascara,mascara,EE);
//	dilate(mascara,mascara,EE);
////	
//	mascara.convertTo(mascara,CV_32F,1./255);
////	
//	Mat espectro=spectrum(mascara);
//	
//	namedWindow("Original",CV_WINDOW_KEEPRATIO);
//	imshow("Original",img);
//	
//	namedWindow("Mascara",CV_WINDOW_KEEPRATIO);
//	imshow("Mascara",mascara);
////	
//	namedWindow("Espectro Mascara",CV_WINDOW_KEEPRATIO);
//	imshow("Espectro Mascara",espectro);
//	
//	Mat umbral(espectro.size(),espectro.type());
//	for(int i=0;i<espectro.rows;i++) { 
//		for(int j=0;j<espectro.cols;j++) { 
//			if (espectro.at<float>(i,j)> 200/255.0){
//				umbral.at<float>(i,j)=1;
//			}
//			else umbral.at<float>(i,j)=0;
//		}
//	}
//	
//	Mat kernel=Filtro_Promediador(5);
//	umbral=convolve(umbral,kernel);
//	for(int i=0;i<umbral.rows;i++) { 
//		for(int j=0;j<umbral.cols;j++) { 
//			if (umbral.at<float>(i,j)>120/255.0){
//				umbral.at<float>(i,j)=1;
//			}
//			else{
//				umbral.at<float>(i,j)=0;
//			}
//		}
//	}
//	Mat EE2=getStructuringElement(MORPH_RECT,Size(3,3));
//	erode(umbral,umbral,EE2);
//
//	int ii=0;
//	int jj=0;
//	for(int i=0;i<umbral.rows;i++) { 
//		for(int j=0;j<umbral.cols;j++) { 
//			if (umbral.at<float>(i,j)==1 && ii==0 && jj==0){
//				ii=i;
//				jj=j;
//			}
//			
//		}
//	}
//
//	double angulo=0;
//	
//	if (jj<(int)umbral.cols/2){
//		double a=abs(umbral.cols/2-jj);
//		double b=abs(umbral.rows/2-ii);
//		double division=a/b;
//		angulo=atan(division)*180/M_PI;
//		angulo=atan(division)*180/M_PI;
//	}
//	else{
//		double a=abs(jj-umbral.cols/2);
//		double b=abs(umbral.rows/2-ii);
//		double division=a/b;
//		angulo=atan(division)*180/M_PI;
//	}
//	
//	
//	cout<<"Cantidad filas:"<<espectro.rows/2<<" Cantidad columnas:"<<espectro.cols/2<<endl<<" I:"<<ii<<" J:"<<jj<<" Angulo:"<<angulo<<endl;
//	
//	namedWindow("Umbral",CV_WINDOW_KEEPRATIO);
//	imshow("Umbral",umbral);
//	
////	Mat canvas=cv::Mat::zeros(hsv[0].rows,aux.cols,CV_32F);
////	Mat histo = histogram(hsv[0],255);
////	normalize(histo,histo,0,1,CV_MINMAX);
////	draw_graph(canvas,histo);
//	
//
//
//	
//	
//	waitKey(0);
//}


void Brujula(Mat img){
//	Mat Gradiente, transformada;
//	vector <vector <Point> > &pt
////	HoughComun(img,transformada,50,pt);
//	HoughComun(img, Gradiente, transformada, 100, pt);
//	namedWindow("Original",CV_WINDOW_KEEPRATIO);
//	imshow("Original",img);
//	namedWindow("gradiente",CV_WINDOW_KEEPRATIO);
//	imshow("gradiente",gradiente);
//	namedWindow("transformada",CV_WINDOW_KEEPRATIO);
//	imshow("transformada",transformada);

	
	
	Mat aux2=img.clone();
	//	Mat roi=aux(Rect(167,208,6,5));
	
	
		
		vector <Mat> bgr; 	
		split(aux2, bgr);
	//	
		Mat mascara= Mat::zeros(aux2.size(), aux2.type());
		inRange(aux2,Scalar(0,0,200),Scalar(30,30,255),mascara); //regiones aproximadas sacadas de los histogramas
	//	
		Mat EE=getStructuringElement(MORPH_RECT,Size(3,3));
		erode(mascara,mascara,EE);
		dilate(mascara,mascara,EE);
		
		//Saber cuadrante
		float media1=0;
		float media2=0;
		float media3=0;
		float media4=0;
		
		for(int i=0;i<mascara.rows/2;i++) { 
			for(int j=0;j<mascara.cols/2;j++) { 
				media1+=mascara.at<uchar>(i,j);
			}
		}
		
		for(int i=0;i<mascara.rows/2;i++) { 
			for(int j=mascara.cols/2;j<mascara.cols;j++) { 
				media2+=mascara.at<uchar>(i,j);
			}
		}
		
		for(int i=mascara.rows/2;i<mascara.rows;i++) { 
			for(int j=0;j<mascara.cols/2;j++) { 
				media3+=mascara.at<uchar>(i,j);
//				mascara.at<uchar>(i,j)=255;
			}
		}
		
		for(int i=mascara.rows/2;i<mascara.rows;i++) { 
			for(int j=mascara.cols/2;j<mascara.cols;j++) { 
				media4+=mascara.at<uchar>(i,j);
			}
		}
		float multi=(img.cols/2)*(img.rows/2);
		media1=media1/multi;
		media1=media2/multi;
		media1=media3/multi;
		media1=media4/multi;
		
		Mat gradiente,transformada;
		
		vector <vector <Point> > pt;
		HoughComun(img,gradiente,transformada,110,pt);
		//	HoughComun(img,gradiente,transformada,120,pt);
			namedWindow("Original",CV_WINDOW_KEEPRATIO);
			imshow("Original",img);
		//	namedWindow("Gradiente",CV_WINDOW_KEEPRATIO);
		//	imshow("Gradiente",gradiente);
			namedWindow("Transformada",CV_WINDOW_KEEPRATIO);
			imshow("Transformada",transformada);
		
		vector <Point> aux=pt[0]; //Puntos que forman la linea
		Point p0=aux[0]; //punto 2 de la linea
		Point p1=aux[1]; //punto 2 de la linea
		double angulo;
		
		RecorrerVectorVectoresPoint(pt);
		
		if (media1>media2 && media1>media3 && media1>media4){ //Esta en el primer cuadrante
			double a=abs(img.cols/2+p0.y);
			double b=abs(img.rows/2+p0.x);
			double division=a/b;
			angulo=atan(division)*180/M_PI;
			angulo=360-angulo;
			cout<<"PRIMERO"<<endl;
		}
		else{
			if (media2>media1 && media2>media3 && media2>media4){ //Esta en el segundo cuadrante
				double a=abs(p1.y-img.cols/2);
				double b=abs(img.rows/2-p1.x);
				double division=a/b;
				angulo=atan(division)*180/M_PI;
				cout<<"SEGUNGO"<<endl;
			}
			else{
				if (media3>media1 && media3>media2 && media3>media4){ //Esta en el tercer cuadrante
					double a=abs(p0.y-img.cols/2);
					double b=abs(p0.x+img.rows/2);
					double division=a/b;
					angulo=atan(division)*180/M_PI;
					angulo=180+(90-angulo);
					cout<<"TERCERO"<<endl;
				}
				else{
					double a=abs(p1.x-img.rows/2); //4to cuadrante
					double b=abs(p1.y-img.cols/2);
					double division=a/b;
					angulo=atan(division)*180/M_PI;
					angulo=180-angulo;
					cout<<"CUARTO"<<endl;
				}
			}
		}
		cout<<"ANGULO REAL "<<angulo<<endl;
		
//		cout<<"Media1: "<<media1<<" Media2: "<<media2<<" Media3: "<<media3<<" Media4: "<<media4<<endl;
//		namedWindow("Mascara",CV_WINDOW_KEEPRATIO);
//		imshow("Mascara",mascara);
	
	waitKey(0);
}
int main(int argc, char** argv) {
	

	
	//IMPLEMENTACION DEL PARCIAL DEL ESCANER
	for(int i=1;i<5;i++) { 
		string aux="Brujula/";
		string nombre;
		stringstream c;
		c<<i;
		nombre=c.str();
		aux=aux+nombre+".png";
		Mat img=imread(aux);
		Brujula(img);
	}	
	
//	Mat img=imread("Brujula/1.png");
//	Averiguar();
	
	
//	waitKey(0);
	return 0;
} 
