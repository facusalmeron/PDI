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

void Parcial_Cervezas(Mat img){
	//Primero saco las transformada Hough para obtener las lineas del vaso.
	
	Mat Gradiente2;
	Canny(img,Gradiente2,50,200,3); //Detecto los bordes de la imagen
	namedWindow("Original",CV_WINDOW_KEEPRATIO);
	imshow("Original",img);
	namedWindow("Bordes",CV_WINDOW_KEEPRATIO);
	imshow("Bordes",Gradiente2);

	//HoughLinesP me da como salida los extremos de las lineas detectadas. (x0,y0),(x1,y1).
	vector <Vec4i> lines2;
	//	HoughLinesP(Gradiente2,lines2,1,CV_PI/180, 30, 15, 10  ); //Parametros para "letras1.tif"
	HoughLinesP(Gradiente2,lines2,1,CV_PI/180, 10, 10, 10  ); //Parametros para "snowman.png"
	Mat transformadaP;
	cvtColor(Gradiente2, transformadaP, CV_GRAY2BGR);
	//Los parametros son los mismos, pero los ultimos dos son:
	//* El numero minimo de puntos que se puede formar una linea, las lineas con menos de estos puntos no se tienen en cuenta
	//* Separacion maxima entre dos puntos a considerar en la misma recta
	for(size_t i=0; i<lines2.size(); i++) 
	{ 
		Vec4i  l=lines2[i];
		line(transformadaP ,Point(l[0],l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA); 
	}
	
	//Una vez obtenidas las lineas de los contornos, de los mismos obtengo el menor xy y mayor xy de cada vaso para hacer un roi de solo el vaso
	int minx=9999;
	int miny=9999;
	int maxx=-1;
	int maxy=-1;
	for(int i=0;i<lines2.size();i++) { 
		Vec4i  v=lines2[i];
		if (v[0]<minx){
			minx=v[0];
		}
		if (v[1]<miny){
			miny=v[1];
		}
		if (v[0]>maxx){
			maxx=v[0];
		}
		if (v[1]>maxy){
			maxy=v[1];
		}
		if (v[2]<minx){
			minx=v[2];
		}
		if (v[3]<miny){
			miny=v[3];
		}
		if (v[2]>maxx){
			maxx=v[2];
		}
		if (v[3]>maxy){
			maxy=v[3];
		}
	}
//	cout<<"Minimo de X: "<<minx<<endl<<"Minimo de Y: "<<miny<<endl<<"Maximo de X: "<<maxx<<endl<<"Maximo de Y: "<<maxy<<endl;
	namedWindow("Transformada HoughP",CV_WINDOW_KEEPRATIO);
	imshow("Transformada HoughP",transformadaP);
	Mat auxiliar=img.clone();
	Mat ROI=auxiliar(Rect(minx,miny,maxx-minx,maxy-miny));
	namedWindow("ROI",CV_WINDOW_KEEPRATIO);
	imshow("ROI",ROI);
	
	//Armo un segundo roi del roi que ya tenia con un cuadrado central y le saco la media (si es rubia la media da 150 mas o menos
	//si es negra la media da entre 50 y 90 depende la iluminacion, entonces con preguntar de la media nomas ya se cual es).
	Mat roi2=ROI(Rect(ROI.cols/2-10,ROI.rows/2-10,20,20));
//	imshow("2do ROI",roi2);
	float media=Media(roi2);
	if (media>100){
		cout<<"Es una Cerveza RUBIA"<<endl;
	}
	else{
		cout<<"Es una Cerveza NEGRA"<<endl;
	}
	
	//Con el canal B es el mejor para determinar la espuma, recorro por la linea central y si es mayor a 110 voy sumando espuma
	//como todos los vasos tienen una base esta la cuenta como espuma tmb, entonces pregunto si el valos de B es blanco y sus
	//anteriores no lo eran, entonces corto. Y luego para sacar el promedio de espuma lo hago hasta el valor donde encontro que 
	//empieza la base
	vector <Mat> bgr; 	
	split(ROI, bgr);
	int espuma=0;
	int aux2;
	for(int i=0;i<ROI.rows;i++) { 
		aux2=i;
		if ((int)bgr[0].at<uchar>(i,ROI.cols/2)>110){
			espuma++;
			if (i>6 && (int)bgr[0].at<uchar>(i-5,ROI.cols/2)<60){
				break;
			}
		}
//		cout<<i<<endl;
		cout<<(int)bgr[0].at<uchar>(i,ROI.cols/2)<<endl;
	}
	for(int i=0;i<ROI.cols;i++) { 
		ROI.at<cv::Vec3b>(aux2,i)[0]=0;
		ROI.at<cv::Vec3b>(aux2,i)[1]=0;
		ROI.at<cv::Vec3b>(aux2,i)[2]=255;
	}
	float porcentaje=espuma*100/aux2;
	cout<<"La cerveza tiene un porcentaje de espuma del: "<<porcentaje<<"%"<<endl;
	namedWindow("Finaly",CV_WINDOW_KEEPRATIO);
	imshow("Finaly",ROI);
	waitKey(0);
}
//
void Parcial_Billetes(Mat img){
	
	//Primero verifico si la imagen esta derecha o rotada 180º. para esto con perfiles de intensidad
	//elijo cuatro lineas fijas y saco el promedio de gris de cada una y un promedio general de los
	//cuatro promedios elegidos, por eso si la img tiene un promedio menor a 200 hay que rotar 180º
	Mat aux=img.clone();
	cvtColor(aux,aux,CV_BGR2GRAY);
	float promedio=0.0;
	float promedio1=0.0;
	float promedio2=0.0;
	float promedio3=0.0;
	for(int i=0;i<aux.rows;i++) { 
		promedio+=(int)aux.at<uchar>(i,110);
		promedio1+=(int)aux.at<uchar>(i,115);
		promedio2+=(int)aux.at<uchar>(i,120);
		promedio3+=(int)aux.at<uchar>(i,125);
//		aux.at<uchar>(i,110)=0;
//		aux.at<uchar>(i,115)=0;
//		aux.at<uchar>(i,120)=0;
//		aux.at<uchar>(i,125)=0;
	}
	promedio/=aux.rows;
	promedio1/=aux.rows;
	promedio2/=aux.rows;
	promedio3/=aux.rows;
	float PromedioGeneral=(promedio+promedio1+promedio2+promedio3)/4;
	cout<<"Promedio: "<<PromedioGeneral<<endl;
	if (PromedioGeneral<200){
		aux=rotate(aux,180);
	}
	
	//Para saber que billete es saco los triangulitos, y como todas las imagenes van a ser siempre del mismo tamaño
	//y van a estar en la misma direccion (ya fueron rotadas) los triangulos van a estar en el mismo lugar.
	//Entonces saco un roi que comprenda todos los triangulos de cada billete
	
	Mat roi=aux(Rect(132,13,50,75));
	
	//Umbralizo
	Mat Mascara=cv::Mat::zeros(roi.size(),roi.type());
	for(int i=0;i<roi.rows;i++) { 
		for(int j=0;j<roi.cols;j++) { 
			if (roi.at<uchar>(i,j)<120){
				Mascara.at<uchar>(i,j)=255;
			}
		}
	}
	
	//Con el dilate directamente tapo los huecos
	Mat EE=getStructuringElement(MORPH_RECT,Size(3,3));
	dilate(Mascara,Mascara,EE);
	
//	
//	//Filtro para sacar los huecos y vuelvo a binarizar
//	Mat kernel=Filtro_Promediador(3);
//	Mascara=convolve(Mascara,kernel);
//	for(int i=0;i<roi.rows;i++) { 
//		for(int j=0;j<roi.cols;j++) { 
//			if (Mascara.at<uchar>(i,j)<70){
//				Mascara.at<uchar>(i,j)=0;
//			}
//		}
//	}
	
	//Ahora uso la funcion findContours para contar los triangulitos.
	vector<vector<Point> > contornos;
	vector<Vec4i> hierarchy;
	Mat mask=Mascara.clone();
	findContours(mask, contornos, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	int triangulos=hierarchy.size();
	switch (triangulos){
	case 1:
		cout<<"El billete presentado es de $100"<<endl;
		break;
	case 2:
		cout<<"El billete presentado es de $50"<<endl;
		break;
	case 3:
		cout<<"El billete presentado es de $20"<<endl;
		break;
	case 4:
		cout<<"El billete presentado es de $10"<<endl;
		break;
	case 5:
		cout<<"El billete presentado es de $5"<<endl;
		break;
	case 6:
		cout<<"El billete presentado es de $2"<<endl;
		break;
	default: 
		cout<<"ERROR"<<endl;
		return;
	}
	
//	imshow("Original",img);
	namedWindow("Original",CV_WINDOW_KEEPRATIO);
	imshow("Original",aux);
	namedWindow("ROI",CV_WINDOW_KEEPRATIO);
	imshow("ROI",roi);
	namedWindow("Umbral",CV_WINDOW_KEEPRATIO);
	imshow("Umbral",Mascara);
	waitKey(0);
};

void Copa_America(Mat img){
	Mat fich=imread("CopaAmerica/fich.jpg");
	Mat unl=imread("CopaAmerica/unl.jpg");
	Mat sinc=imread("CopaAmerica/sinc.png");
	Mat aux=img.clone();
	//Aplico la funcion de transformada de Hough, en lines obtengo el gradiente y lines2 las rectas dibujadas.
	Mat lines, lines2;
	int cx=0,cy=0,cy2; //en cx y cy obtengo donde se van a graficar las rectas que le pido (vertical u horizontal).
	Hough(img,lines,lines2,220,cx,cy,1,0); //vertical
	Hough(img,lines,lines2,500,cx,cy2,0,1);//horizomntal
	
	//Armo un roi distinto de cada logo a insertar, sabiendo las coordenadas x, y por la transformada hough y el tamaño de los logos
	Mat roifich=img(Rect(cy-67,cx+1,65,65));
	namedWindow("FICH",CV_WINDOW_KEEPRATIO);
	imshow("FICH",roifich);
	
	vector <Mat> bgrfich; 	
	split(roifich, bgrfich);
	
	Mat auxfich=roifich.clone();
	Mat mascarafich;
	
	float mediafich=Media(bgrfich[0]);
	float desviofich=Desvio(bgrfich[0],mediafich);
	float mediafich2=Media(bgrfich[1]);
	float desviofich2=Desvio(bgrfich[1],mediafich2);
	float mediafich3=Media(bgrfich[2]);
	float desviofich3=Desvio(bgrfich[2],mediafich3);
	inRange(auxfich,Scalar(mediafich-desviofich-10,mediafich2-desviofich2-10,mediafich3-desviofich3-10),Scalar(mediafich+desviofich+10,mediafich2+desviofich2+10,mediafich3+desviofich3+10),mascarafich);
//	Mat EE=getStructuringElement(MORPH_ELLIPSE,Size(2,2));
//	dilate(mascarafich,mascarafich,EE);
//	Mat kernelfich=Filtro_Promediador(3);
//	mascarafich=convolve(mascarafich,kernelfich);
//	for (int i=0;i<mascarafich.rows;i++){
//		for (int j=0;j<mascarafich.cols;j++){
//			if ((int)mascarafich.at<uchar>(i,j)>190){mascarafich.at<uchar>(i,j)=255;}
//			else{mascarafich.at<uchar>(i,j)=0;}
//		}
//	}
	
	for(int i=0;i<roifich.rows;i++) { 
		for(int j=0;j<roifich.cols;j++) { 
			if(mascarafich.at<uchar>(i,j)==255){
				roifich.at<Vec3b>(i,j)[0]=fich.at<Vec3b>(i,j)[0];
				roifich.at<Vec3b>(i,j)[1]=fich.at<Vec3b>(i,j)[1];
				roifich.at<Vec3b>(i,j)[2]=fich.at<Vec3b>(i,j)[2];
			}
		}
	}
	namedWindow("FICHf",CV_WINDOW_KEEPRATIO);
	imshow("FICHf",roifich);
	Mat roiunl=img(Rect(cy+2,cx+1,65,65));
	namedWindow("UNL",CV_WINDOW_KEEPRATIO);
	imshow("UNL",roiunl);

	vector <Mat> bgrunl; 	
	split(roiunl, bgrunl);
	
	Mat auxunl=roiunl.clone();
	Mat mascaraunl;
	
	float mediaunl=Media(bgrunl[0]);
	float desviounl=Desvio(bgrunl[0],mediaunl);
	float mediaunl2=Media(bgrunl[1]);
	float desviounl2=Desvio(bgrunl[1],mediaunl2);
	float mediaunl3=Media(bgrunl[2]);
	float desviounl3=Desvio(bgrunl[2],mediaunl3);
	inRange(auxunl,Scalar(mediaunl-desviounl-10,mediaunl2-desviounl2-10,mediaunl3-desviounl3-10),Scalar(mediaunl+desviounl+10,mediaunl2+desviounl2+10,mediaunl3+desviounl3+10),mascaraunl);
//	Mat EEu=getStructuringElement(MORPH_ELLIPSE,Size(2,2));
//	dilate(mascaraunl,mascaraunl,EEu);
//		Mat kernelunl=Filtro_Promediador(3);
//		mascaraunl=convolve(mascaraunl,kernelunl);
//		for (int i=0;i<mascaraunl.rows;i++){
//			for (int j=0;j<mascaraunl.cols;j++){
//				if ((int)mascaraunl.at<uchar>(i,j)>190){mascaraunl.at<uchar>(i,j)=255;}
//				else{mascaraunl.at<uchar>(i,j)=0;}
//			}
//		}
	
	for(int i=0;i<roiunl.rows;i++) { 
		for(int j=0;j<roiunl.cols;j++) { 
			if(mascaraunl.at<uchar>(i,j)==255){
				roiunl.at<Vec3b>(i,j)[0]=unl.at<Vec3b>(i,j)[0];
				roiunl.at<Vec3b>(i,j)[1]=unl.at<Vec3b>(i,j)[1];
				roiunl.at<Vec3b>(i,j)[2]=unl.at<Vec3b>(i,j)[2];
			}
		}
	}
	namedWindow("UNLr",CV_WINDOW_KEEPRATIO);
	imshow("UNLr",roiunl);
//
	Mat roisinc=img(Rect(cy-198,img.rows-63,396,58));
	namedWindow("SINC",CV_WINDOW_KEEPRATIO);
	imshow("SINC",roisinc);
//	
	vector <Mat> bgrsinc; 	
	split(roisinc, bgrsinc);
	
	Mat auxsinc=roisinc.clone();
	Mat mascarasinc;
	
	float mediasinc=Media(bgrsinc[0]);
	float desviosinc=Desvio(bgrsinc[0],mediasinc);
	float mediasinc2=Media(bgrsinc[1]);
	float desviosinc2=Desvio(bgrsinc[1],desviosinc2);
	float mediasinc3=Media(bgrsinc[2]);
	float desviosinc3=Desvio(bgrsinc[2],desviosinc3);
	inRange(auxsinc,Scalar(mediasinc-desviosinc-5,mediasinc2-desviosinc2-5,mediasinc3-desviosinc3-5),Scalar(mediasinc+desviosinc+5,mediasinc2+desviosinc2+5,mediasinc3+desviosinc3+5),mascarasinc);
	Mat EEs=getStructuringElement(MORPH_CROSS,Size(3,3));
	erode(mascarasinc,mascarasinc,EEs);
//		Mat kernelsinc=Filtro_Promediador(3);
//		mascarasinc=convolve(mascarasinc,kernelsinc);
//		for (int i=0;i<mascarasinc.rows;i++){
//			for (int j=0;j<mascarasinc.cols;j++){
//				if ((int)mascarasinc.at<uchar>(i,j)>190){mascarasinc.at<uchar>(i,j)=255;}
//				else{mascarasinc.at<uchar>(i,j)=0;}
//			}
//		}
	
	for(int i=0;i<roisinc.rows;i++) { 
		for(int j=0;j<roisinc.cols;j++) { 
			if(mascarasinc.at<uchar>(i,j)==255){
				roisinc.at<Vec3b>(i,j)[0]=sinc.at<Vec3b>(i,j)[0];
				roisinc.at<Vec3b>(i,j)[1]=sinc.at<Vec3b>(i,j)[1];
				roisinc.at<Vec3b>(i,j)[2]=sinc.at<Vec3b>(i,j)[2];
			}
		}
	}
	
	namedWindow("SINCr",CV_WINDOW_KEEPRATIO);
	imshow("SINCr",roisinc);
	
	
	
	namedWindow("Original",CV_WINDOW_KEEPRATIO);
	imshow("Original",img);
	
	
	waitKey(0);
}

int main(int argc, char** argv) {
	
	//IMPLEMENTACION DEL PARCIAL DE CERVEZAS:
//	for(int i=1;i<12;i++) { 
//		string aux="Cervezas/";
//		string nombre;
//		stringstream c;
//		c<<i;
//		nombre=c.str();
//		aux=aux+nombre+".jpg";
//		Mat img=imread(aux);
//		Parcial_Cervezas(img);
//	}
	
	//IMPLEMENTACION DEL PARCIAL DE LOS BILLETES
//		for(int i=1;i<13;i++) { 
//			string aux="Billetes/";
//			string nombre;
//			stringstream c;
//			c<<i;
//			nombre=c.str();
//			aux=aux+nombre+".jpg";
//			Mat img=imread(aux);
//			Parcial_Billetes(img);
//		}	
		
	//IMPLEMENTACION DEL RECUPERATORIO 2016 COPA AMERICA
		for(int i=1;i<7;i++) { 
			string aux="CopaAmerica/";
			string nombre;
			stringstream c;
			c<<i;
			nombre=c.str();
			aux=aux+nombre+".png";
			Mat img=imread(aux);
			Copa_America(img);
		}	
	
	
	
//	cv::Mat im=imread("Fut02_3.png"),output,mask,segment,mask_inv,im_hsv,lines,lines2,jugadores,mcolor,jugadores_orig,jugadores_sinc,jugadores_grande,salida;
//	//Fut01_1,Fut01_3,Fut02_1
//	cv::Mat fich=imread("fich.jpg"),unl=imread("unl.jpg"),sinc=imread("Logo03.png");
//	
//	//view_coordinates(im);//261 225
//	//segmentator(im,segment,mask,96,132);//15 87
//	int h1,s1,anc1,al1;
//	//HOUGH
//	Huang(im,lines,lines2,220,cx,cy,1,0); //vertical
//	//std::cout<<cy<<std::endl;
//	Huang(im,lines,lines2,500,cx,cy2,0,1);//horizomntal
	return 0;
} 
