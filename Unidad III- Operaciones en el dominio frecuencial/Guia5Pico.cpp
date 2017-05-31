#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include "pds_functions.h"

using namespace cv;
using namespace pdi;
using namespace std;

//optinum_size
//	spectrumdft
//	idftimg.conveTo(img,cv. o _ 32f, 1./255)
//	filter_ideal()
//	filter_buteerwordl()
//	filter_gausian()
//	filter()
//guia 7
//	findContours();
//	drawContours();
//	ContourArea()

vector<Mat> HacerFiguras(){
	//FALTA CIRCLEEEEE (antogila);
	vector<Mat>x(6);
	int ancho=256;
	int alto=256;
	x[0]=Mat(ancho,alto,CV_8UC1);
	x[1]=Mat(ancho,alto,CV_8UC1);
	x[2]=Mat(ancho,alto,CV_8UC1);
	x[3]=Mat(ancho,alto,CV_8UC1);
	x[4]=Mat(ancho,alto,CV_8UC1);
	//se vacian las imaqgenes
	for(int i=0;i<ancho;i++) { for(int j=0;j<alto;j++) { x[0].at<uchar>(i,j)=0;
	x[1].at<uchar>(i,j)=0;x[2].at<uchar>(i,j)=0;x[3].at<uchar>(i,j)=0;x[4].at<uchar>(i,j)=0;} }
	
/*	for(int i=0;i<ancho;i++) { 	x[0].at<uchar>(i,128)=255;x[1].at<uchar>(128,i)=255;}*/
line(x[0],Point(128,1),Point(128,255),Scalar(255,255,255),1);	
	for(int i=108;i<148;i++) { 
		x[2].at<uchar>(i,108)=255;
		x[2].at<uchar>(i,148)=255;
		
	} 
	for(int i=108;i<148;i++) { 
		x[2].at<uchar>(108,i)=255;
		x[2].at<uchar>(148,i)=255;
		x[3].at<uchar>(88,i)=255;
		x[3].at<uchar>(148,i)=255;
	} 

	
	for(int i=88;i<148;i++) { 
		x[3].at<uchar>(i,108)=255;
		x[3].at<uchar>(i,148)=255;
	} 
	 circle(x[4],Point(128,128),20,Scalar(255));
//	imshow("linea",x[4]);
	return x;
}
int main(int argc, char** argv) {
	vector<Mat>x=HacerFiguras();
	Mat fftLinea(256,256,CV_32F);
	int cual=2;//1 linea 2 cuad 3 rec 4 cir
	x[cual].convertTo(x[cual],CV_32F,1./256);
//////	dft(x[cual],fftLinea);
	fftLinea=spectrum(x[cual]);
	imshow("Imagen",x[cual]);
	imshow("Transformada",fftLinea);
////	

//	
	
	//problema 2
//	Mat img=imread("cameraman.tif",IMREAD_GRAYSCALE);
//	img.convertTo(img,CV_32F,1./256);
//	Mat ff=spectrum(img);
//	imshow("Spectrum",ff);
////	Mat filtro=filter_ideal(256,256,.1);
	
	// 2)
//	Mat filtro=filter_butterworth(256,256,.4,10);
//	imshow("Filtro",filtro);
//	Mat res=filter(img,filtro);
//	imshow("filtraa",res);

	//3)
//	double corte=.3;
//	Mat img=imread("cameraman.tif",IMREAD_GRAYSCALE);
//	size_t rows=img.rows, cols=img.cols;
//	Mat filtro=filter_gaussian(rows,cols,corte);
//		img.convertTo(img,CV_32F,1./256);
//	imshow("Filtro",filtro);
	
//	//pasa alto
//	filtro=1-filtro;
//	imshow("pasa alto",filtro);
//	Mat res=filter(img,filtro);
//	imshow("Filtra á",res);
//	
//	
//	Mat res;
//	GaussianBlur(img,res,Size(3,3),2);
//	imshow("filtrada",res);
//	imshow("Originial",img);
	
	//ejercicio 3
	//1)
//	Mat img=imread("cameraman.tif",IMREAD_GRAYSCALE);
//	img.convertTo(img,CV_32F,1./256);
	
//		Mat kern = (Mat_<float>(256,256) <<  1.,1.,1.,
//					1.,  -1., 1,
//					-1., -1., -1.);
//	Mat kern;
//	kern.at<float>(0,0)=1.;
//	kern.at<float>(0,1)=1.;
//	kern.at<float>(0,2)=1.;
//	kern.at<float>(1,0)=1.;
//	kern.at<float>(1,1)=-1.;
//	kern.at<float>(1,2)=1;
//	kern.at<float>(2,0)=-1.;
//	kern.at<float>(2,1)=-1;
//	kern.at<float>(2,2)=-1;
	
//		for(int i=3;i<256;i++) { 
//			for(int j=3;j<256;j++) { kern.at<float>(i,j)=0; }
//		}
//		imshow("asd",kern);
//		Mat filtro=spectrum(kern);

//		Mat res=convolve(img,kern);
//		imshow("Resultado",res);	
	
		
		waitKey(0);
	return 0;
} 
