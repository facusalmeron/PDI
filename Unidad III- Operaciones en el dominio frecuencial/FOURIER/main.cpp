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

void EjWalter(){
	Mat image=imread("huang1.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	image.convertTo(image,CV_32F,1./255);
	int	original_rows=image.rows, original_cols=image.cols;
	image=optimum_size(image);
	Mat ideal=filter_ideal(image.rows,image.cols,0.1);
//	Mat ideal=filter_butterworth( image.rows , image.cols, 0.1,1 );
	Mat result=filter(image,ideal);
	result=result(Range(0,original_rows),Range(0,original_cols)) ;
	image = image (Range(0,original_rows),Range( 0 , original_cols));
	namedWindow("image",CV_WINDOW_KEEPRATIO);
	namedWindow("image espectro",CV_WINDOW_KEEPRATIO);
	namedWindow("filtrada",CV_WINDOW_KEEPRATIO);
	namedWindow("filtrada espectro",CV_WINDOW_KEEPRATIO);
	imshow( "image" , image ) ;
	imshow( "image espectro",spectrum(image));
	imshow("filtrada",result);
	imshow("filtrada espectro",spectrum(result));
	waitKey(0);
	
}

Mat Ejercicio1_1(int tipo){
	Mat figura(512,512,CV_32F);
	switch (tipo){
	case 1:{ //linea horizontal
		line(figura,Point(0,figura.rows/2),Point(figura.cols,figura.rows/2),Scalar(1));
		break;}
	case 2:{ //linea vertical
		line(figura,Point(figura.cols/2-5,0),Point(figura.cols/2-5,figura.rows),Scalar(1));
		line(figura,Point(figura.cols/2+5,0),Point(figura.cols/2+5,figura.rows),Scalar(1));
//		line(figura,Point(figura.cols/2-25,0),Point(figura.cols/2+25,figura.rows),Scalar(1));
		break;}
	case 3:{ //cuadrado centrado
		rectangle(figura,Point((figura.cols/2)-20,(figura.rows/2)-20),Point((figura.cols/2)+20,(figura.rows/2)+20),Scalar(1));
		break;}
	case 4:{ //rectangulo centrado
		rectangle(figura,Point((figura.cols/2)-40,(figura.rows/2)-20),Point((figura.cols/2)+40,(figura.rows/2)+20),Scalar(1));	
		break;}
	case 5:{ //circulo centrado
		circle(figura,Point(figura.cols/2,figura.rows/2),30,Scalar(1));
		break;}
	default:{
		cout<<"Figura Invalida"<<endl;
		break;}
	}
	return figura;
	waitKey(0);
}

void Ejercicio1_2(Mat img){
	Mat transformada(img.size(),img.type());
//	dft(img,transformada);
	transformada=spectrum(img);
	namedWindow("Transformada",CV_WINDOW_KEEPRATIO);
	imshow("Transformada",transformada);
	
	waitKey();
}

int main(int argc, char** argv) {
	//tipo 1: linea horizontal, tipo 2: linea vertical, tipo 3: cuadrado centrado
	//tipo 4: rectangulo centrado, tipo 5: circulo centrado
	Mat figura=Ejercicio1_1(2);
	imshow("Ejercicio1_1",figura);
	Ejercicio1_2(figura);
	waitKey();
	return 0;
} 
