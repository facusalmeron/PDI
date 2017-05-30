#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "pdi_functions.h"
#include <opencv2/core/core.hpp>
//#include <opencv2/gpu/gpu.hpp>
#include <cmath>

using namespace cv;
using namespace pdi;
using namespace std;

Mat Ejercicio1(Mat img,int tipo){
	Mat Gx,Gy;
	Mat magnitud;
	switch(tipo){
		case 0:		//Deteccion de bordes por Roberts
			Gx = (Mat_<double>(3,3) << 
					0, 0, 0,
					0,-1, 0,
					0, 0, 1);
			Gy = (Mat_<double>(3,3) << 
					0, 0, 0,
					0, 0,-1,
					0, 1, 0);
			break;
		case 1:		//Deteccion de bordes por Prewitt
			Gx = (Mat_<double>(3,3) << 
					-1,-1,-1,
					 0, 0, 0,
					 1, 1, 1);
			Gy = (Mat_<double>(3,3) << 
					-1, 0, 1,
					-1, 0, 1,
					-1, 0, 1);
			break;
		case 2:		//Deteccion de bordes por Sobel
			Gx = (Mat_<double>(3,3) << 
					-1,-2,-1,
					 0, 0, 0,
					 1, 2, 1);
			Gy = (Mat_<double>(3,3) << 
					-1, 0, 1,
					-2, 0, 2,
					-1, 0, 1);
			break;
		case 3:		//Deteccion de bordes por Laplaciano
			Gx = (Mat_<double>(3,3) << 
					-1,-1,-1,
					-1, 8,-1,
					-1,-1,-1);
			break;
		case 4:
	//		Gx = (Mat_<double>(5,5) << 
	//			  0, 0,-1, 0, 0,
	//			  0,-1,-2,-1, 0,
	//			  -1,-2,16,-2,-1,
	//			  0,-1,-2,-1, 0,
	//			  0, 0,-1, 0, 0,);
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
	return magnitud;
}

void Ejercicio2(){
	Mat img = Mat::zeros(200,200, CV_8U);
//	img.at<uchar>(Point(10,10)) = 255;
//	img.at<uchar>(Point(190,10)) = 255;
//	img.at<uchar>(Point(10,190)) = 255;
//	img.at<uchar>(Point(190,190)) = 255;
//	img.at<uchar>(Point(100,100)) = 255;
	line(img,Point(100,100),Point(190,190),Scalar(255));
	line(img,Point(10,190),Point(100,100),Scalar(255));
	imshow("Original",img);
	
	//Calculo Gradiente y Umbralizo
	Mat img_grad;
	Canny(img, img_grad, 50, 200, 5);
	imshow("Gradiente y Umbralizo",img_grad);
//	img_grad = img;
	Mat acumulador = Mat::zeros(400,400,CV_8U);
	vector<Vec2i> coordenadas;
	for(int i=0;i<img.rows;i++){
		for(int j=0;j<img.cols;j++){
			if(img_grad.at<uchar>(i,j) == 255){
				float angulo = atan(i/j*1.0);
				float ro = i*cos(angulo) + j*sin(angulo);
				acumulador.at<uchar>(ro,angulo*180.0) += 1;
				Vec2i aux;
				aux[0] = i;
				aux[1] = j;
				coordenadas.push_back(aux);
//				cout<<angulo*180.0<<"-"<<ro<<endl;
//				cout<<aux<<endl;
			}
		}
	}
	imshow("Acumulador",acumulador*255);
	Mat resultado = Mat::zeros(img.rows,img.cols,img.type());
	for(int i=0;i<coordenadas.size();i++){
		Vec2i aux1 = coordenadas[i];
		Vec2i aux2 = coordenadas[i+1];
		float angulo1 = atan2(aux1[1],aux1[0]);
		float angulo2 = atan2(aux2[1],aux2[0]);
		float ro1 = aux1[0]*cos(angulo1) + aux1[1]*sin(angulo1);
		float ro2 = aux2[0]*cos(angulo2) + aux2[1]*sin(angulo2);
		
		if( sqrt(((angulo2-angulo1)*(angulo2-angulo1))+((ro2-ro1)*(ro2-ro1))) < 3){
			line(resultado,Point(aux1[1],aux1[0]),Point(aux2[1],aux2[0]),Scalar(255));
		}
	}
	imshow("Resultado",resultado);
	
	waitKey(0);
}

/////////////////CLICK EVENTS///////////////////////////////
vector<Point> xy;
int cantclick = 10;
bool reset = false;
void  manejorosas(int event, int x, int y, int flags, void* userdata){
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

Mat crecimientoregiones(Mat img,int x,int y){
	//RELLENA ELEMENTOS CONECTADOS CON UN VALOR DADO
	//imagen, punto donde pone semilla, valores que toman las regiones cercanas
	//a la semilla, null, tolerancia inferior, tolerancia superior
	floodFill(img,Point(x,y),1.01,NULL,0.05,0.05);	
	//UMBRALIZO PARA QUE PASEN UNICAMENTE LOS ELEMENTOS CON INTENSIDAD 1
	threshold(img,img,1,1,THRESH_BINARY);			
	return img;
}

void Ejercicio3(){
	Mat img = imread("Imagenes/bone.tif",CV_LOAD_IMAGE_GRAYSCALE);
	img.convertTo(img,CV_32F,1./255);
	//OBTENGO LAS COORDENADAS X e Y DE DONDE HICE CLICK
	cantclick = 1;
	namedWindow("My Window",WINDOW_NORMAL);
	imshow("My Window",img);
	//Despues de llamar a madejorosas, en el vector xy tengo guardadas las coordenadas donde hice click
	setMouseCallback("My Window", manejorosas, NULL);
	waitKey(0);
	
	//HAGO EL CRECIMIENTO A PARTIR DE DONDE HICE CLICK
	Mat resultado = crecimientoregiones(img,xy[0].x,xy[0].y);
	imshow("Resultado",resultado);
	waitKey(0);
}
int main(int argc, char** argv) {
//	Mat img = imread("Imagenes/estanbul.tif",CV_LOAD_IMAGE_GRAYSCALE);
//	img.convertTo(img,CV_32F,1./255);
//	Mat magnitud = Ejercicio1(img,2);
//	imshow("Original",img);
//	imshow("Resultado",magnitud);
//	waitKey(0);
	
//	Ejercicio2();
	Ejercicio3();
	
	return 0;
} 
