#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <iomanip>
#include "pdi_functions.h"
#include <vector>

using namespace cv;
using namespace pdi;
using namespace std;

void onMouse( int event, int x, int y, int, void* );

Mat imagen = cv::imread("Imagenes/pattern.tif");

void Ejercicio1(){
	//EJERCICIO 1.1
	namedWindow("Ejercicio 1.1");
	Mat img=imread("Imagenes/polaco.jpg");
	imshow("Ejercicio 1.1",img);
	
	//EJERCICIO 1.2
	cout<<"Informacion Imagen (Ejercicio 1.2):"<< endl;
	info(img);
	
	//EJECICIO 1.3
	//leer intensidad de un pixel
	Vec3b valor=img.at<cv::Vec3b>(500,958);
	cout<<"Valores BGR de un cierto pixel (Ejercicio 1.3): ";
	cout<<(int)valor.val[0]<<" ";
	cout<<(int)valor.val[1]<<" ";
	cout<<(int)valor.val[2];
	//cambiar la intensidad de un pixel
	for (int i=0;i<100;i++){
		for (int j=0;j<300;j++){
			img.at<cv::Vec3b>(i,j)[0]=0;
			img.at<cv::Vec3b>(i,j)[1]=0;
			img.at<cv::Vec3b>(i,j)[2]=0;
		}
	}
	namedWindow("Ejercicio 1.3");
	imshow("Ejercicio 1.3",img);
	
	//EJERCICIO 1.4
	namedWindow("Ejercicio 1.4");
	//	Rect corto=Rect
	Mat roi=img(Rect(500,100,200,100));
	imshow("Ejercicio 1.4",roi);
	
	//EJERCICIO 1.5
	
	Mat viruta=imread("Imagenes/viruta.jpg");
	Mat polaco=imread("Imagenes/polaco_m.jpg");
	std::vector<cv::Mat> v_img;
	v_img.push_back(viruta);
	v_img.push_back(polaco);
	v_img.push_back(polaco);
	v_img.push_back(viruta);
	Mat m=mosaic(v_img,3);
	namedWindow("Ejercicio 1.5");
	imshow("Ejercicio 1.5",m);
}

void onMouse( int event, int x, int y, int, void* )
{
	if( event != CV_EVENT_LBUTTONDOWN )
		return;
	
	Point pt = Point(x,y);
	cout<<"x="<<pt.x<<"\t y="<<pt.y;
	Vec3b valores=imagen.at<cv::Vec3b>(y,x);
	cout<<"\t B="<<(int)valores.val[0]<<" G="<<(int)valores.val[1]<<" R="<<(int)valores.val[2]<<endl;
	
}

void Ejercicio2_1(){
	//EJERCICIO 2.1
	namedWindow("Ejercicio 2.1");
	setMouseCallback( "Ejercicio 2.1", onMouse, 0 );
	imshow("Ejercicio 2.1",imagen);
	
}

void Ejercicio2_2(){
	//EJERCICIO 2.2
	Mat messi=imread("Imagenes/huang1.jpg");
	namedWindow("Ejercicio 2.2");
	imshow("Ejercicio 2.2",messi);
	
	char foc;
	int n;
	
	cout<<"Inserte f por fila o c por columna: "; cin>>foc;
	cout<<endl<<"Inserte la fila o columna deseada: "; cin>>n;
	vector<int> B, G, R;
	
	if (foc=='f'){
		for (int i=0;i<messi.cols;i++){
			Vec3b valor=messi.at<cv::Vec3b>(n,i);
			B.push_back((int)valor.val[0]);
			G.push_back((int)valor.val[1]);
			R.push_back((int)valor.val[2]);
			cout<<(int)valor.val[0]<<" ";
			cout<<(int)valor.val[1]<<" ";
			cout<<(int)valor.val[2]<<endl;
		}
	}
	else{
		for (int i=0;i<messi.rows;i++){
			Vec3b valor=messi.at<cv::Vec3b>(i,n);
			B.push_back((int)valor.val[0]);
			G.push_back((int)valor.val[1]);
			R.push_back((int)valor.val[2]);
			cout<<(int)valor.val[0]<<" ";
			cout<<(int)valor.val[1]<<" ";
			cout<<(int)valor.val[2]<<endl;	
		}
	}
	cout<<B.size();
	Mat aux_B(1,B.size(),CV_8U);
	
	for(int i=0;i<aux_B.cols;i++){
		aux_B.at<uchar>(i)=B[i];
	}
	
	Mat grafico1(200,400,CV_8U);
	grafico1.setTo(Scalar(0,0,0));
	draw_graph(grafico1,aux_B);
	
	Mat aux_G(1,G.size(),CV_8U);
	
	for(int i=0;i<aux_G.cols;i++){
		aux_G.at<uchar>(i)=G[i];
	}
	
	Mat grafico2(200,400,CV_8U);
	grafico2.setTo(Scalar(0,0,0));
	draw_graph(grafico2,aux_G);
	
	Mat aux_R(1,R.size(),CV_8U);
	
	for(int i=0;i<aux_R.cols;i++){
		aux_R.at<uchar>(i)=R[i];
	}
	
	Mat grafico3(200,400,CV_8U);
	grafico3.setTo(Scalar(0,0,0));
	draw_graph(grafico3,aux_R);
	
	
	std::vector<cv::Mat> v_img;
	v_img.push_back(grafico1);
	v_img.push_back(grafico2);
	v_img.push_back(grafico3);
	Mat m=mosaic(v_img,3);
	
	namedWindow("Grafico intensidad");
	imshow("Grafico intensidad",m);
}
void Ejercicio3(){
	Mat img=imread("Imagenes/botellas.tif",CV_LOAD_IMAGE_GRAYSCALE);
	Mat aux(1,img.cols,CV_8U);
	for (int i=0;i<img.cols;i++){
		aux.at<uchar> (i)=img.at<uchar>(60,i);
	}
	Mat grafico(img.rows,img.cols,CV_8U);
	grafico.setTo(Scalar(0,0,0));
	draw_graph(grafico,aux);
	
	//2da parte para saber posicion de la botella vacia
	Mat img2=imread("Imagenes/botellas.tif",CV_LOAD_IMAGE_GRAYSCALE);
	vector <int> a;
	for (int i=0;i<aux.cols;i++){
		if ((int)aux.at<uchar>(i)>200) a.push_back(i);
	}
	int b=a.size()-1;
	for (int i=0;i<img2.rows;i++){
		img2.at<uchar>(i,(int)a[0]-5)=255;
		img2.at<uchar>(i,(int)a[0]-4)=255;
		img2.at<uchar>(i,(int)a[0]-3)=255;
		img2.at<uchar>(i,(int)a[b]+5)=255;
		img2.at<uchar>(i,(int)a[b]+4)=255;
		img2.at<uchar>(i,(int)a[b]+3)=255;
	}
	for (int i=1;i<a.size();i++){
		img2.at<uchar>(0,a[i])=255;
		img2.at<uchar>(1,a[i])=255;
		img2.at<uchar>(2,a[i])=255;
		img2.at<uchar>(193,a[i])=255;
		img2.at<uchar>(193,a[i])=255;
		img2.at<uchar>(192,a[i])=255;
	}
	
	//Parte 3 porcentaje de llenado.
	int contadorbv=0;
	int contadorbl=0;
	for (int i=0;i<img.rows;i++){
//		cout<<((int)img.at<uchar>(i,a[a.size()/2]))<<" ";
		if (img.at<uchar>(i,a[a.size()/2])<200) contadorbv++;
	}
	for (int i=0;i<img.rows;i++){
//		img.at<uchar>(i,5)=255;
		if (img.at<uchar>(i,5)<200) contadorbl++;
	}
	double resultado=contadorbv*100/contadorbl;
	cout<<"El porcentaje de llenado que tiene la botella no llena es de "<<resultado<<"%";
	imshow("Imagen original",img);
	imshow("Detectar botella vacia",grafico);
	imshow("Posicion", img2);
}
void Ejercicio3_3(){
	Mat img=imread("Imagenes/botellas.tif",CV_LOAD_IMAGE_GRAYSCALE);
	info(img);
	Mat aux(1,img.rows,CV_8U);
	int c=0;
	for (int i=0;i<img.rows;i++){
		aux.at<uchar> (i)=img.at<uchar>(i,5);
		if ((int)aux.at<uchar> (i)>220)
			c++;
		
	}
	int c2=0;
	for (int i=0;i<img.rows;i++){
		aux.at<uchar> (i)=img.at<uchar>(i,img.cols/2);
		if ((int)aux.at<uchar> (i)>220)
			c2++;
		
	}
	cout<<(float)c2/img.rows*100;
	Mat grafico(img.rows,img.cols,CV_8U);
	grafico.setTo(Scalar(0,0,0));
	draw_graph(grafico,aux);
	imshow("Ventana",grafico);
	imshow("Ventana2",img);
}

int main(int argc, char** argv) {
	Ejercicio3();
	waitKey(0);
	return 0;
	
} 
