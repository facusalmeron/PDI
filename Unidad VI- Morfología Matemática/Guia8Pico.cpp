#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "pds_functions.h"

using namespace cv;
using namespace pdi;
using namespace std;

Mat ensuciasSalyPimienta(Mat img, float pa, float pb){
	RNG rgn;//generar aleatorio
	//	int rgn=rand()%255;
	int cantidad1=img.rows*img.cols*pa;//determino cuantos pixeles tento que 
	//cambiar en total
	//cuanto menos sea la cantidad de pa y pb menos pixeles va a cambiar
	int cantidad2=img.rows*img.cols*pb;
	for(int i=0;i<cantidad1;i++) { 	
		img.at<uchar>(rgn.uniform(0,img.rows),rgn.uniform(0,img.cols))=0;
		//elige valores al azar de la img para ponerlos negros
		//esto lo hace cantidad1 veces, la cantidad de pixel que quiero poner pimienta
	}
	for(int i=0;i<cantidad2;i++) { 	
		img.at<uchar>(rgn.uniform(0,img.rows),rgn.uniform(0,img.cols))=255;
		//elige valores al azar de la img para ponerlos negros
		//esto lo hace cantidad1 veces, la cantidad de pixel que quiero poner sal
	}
	return img;
}

Mat EnsuciaGauss(Mat img, double media, double desviacion){
	Mat ruido=img.clone();
	RNG rgn;
	rgn.fill(ruido,RNG::NORMAL,media,desviacion);
	add(img,ruido,img);
	return img;
}
void Ejercicio1(){
	Mat img=imread("img.tif",IMREAD_GRAYSCALE);
	
	
//	img=EnsuciaGauss(img,0,.5);
//	img=ensuciasSalyPimienta(img,.1,.1);
	
	
	Canny(img,img,50,50);
	imshow("Umbralizaa",img);
	Mat delate,eroate;int n=4;
	Mat m=getStructuringElement(MORPH_CROSS,Size(n,n));
	//Mascarita ;)
	for(int i=0;i<n;i++) { 	for(int j=0;j<n;j++) { cout<<m.at<bool>(i,j)<<" "; }cout<<endl;	}
	//dilatacion
	dilate(img,delate,m);
	imshow("Dilatada",delate);
	//erosion
	erode(img,eroate,m);
	imshow("Erosionada",eroate);
	
	
	
	
}
Mat Apertura(Mat img){
	Mat aux=img.clone();
	
	Canny(img,img,50,50);
	imshow("Umbralizaa",img);
	Mat delate,eroate;int n=3;
	Mat m=getStructuringElement(MORPH_RECT,Size(n,n));
	
	
	for(int i=0;i<n;i++) { 	for(int j=0;j<n;j++) { cout<<m.at<bool>(i,j)<<" "; }cout<<endl;	}
	
	//erosion
	erode(img,eroate,m);
	
	//dilatacion
	dilate(eroate,aux,m);
	
	
	return aux;
}
Mat Cierre(Mat img){
	Mat aux=img.clone();
	
	Canny(img,img,50,50);
	imshow("Umbralizaa",img);
	Mat delate,eroate;int n=5;
	Mat m=getStructuringElement(MORPH_RECT,Size(n,n));
	
	
	for(int i=0;i<n;i++) { 	for(int j=0;j<n;j++) { cout<<m.at<bool>(i,j)<<" "; }cout<<endl;	}
	
	//dilatacion
	dilate(img,delate,m);
	//erosion
	erode(delate,aux,m);
	
	
	
	return aux;
}
void Ejercicio2(){
	Mat img=imread("img.tif",IMREAD_GRAYSCALE);
	
//	img = Apertura(img);
//	imshow("Apertura",img);
	
	
	
	img = Cierre(img);
	imshow("Cierre",img);
	
	
	
	
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
void Ejercicio3a(){
	Mat img=imread("Tarjeta.jpeg",IMREAD_GRAYSCALE);
	Mat res=Umbral(img,100);
	imshow("Haber..",res);
	int n=3;
	Mat res1,res2,res3;
	//Para las letras grandes, erocionamos ;)
	Mat m=getStructuringElement(MORPH_CROSS,Size(n,n),Point(2,2));
	erode(res,res1,m);//sale palitos horizontales, faltarian Verticales
	imshow(" Primer Erode",res1);
	
	Mat m1=getStructuringElement(MORPH_RECT,Size(2,4));
	erode(res,res3,m1);//sale palitos horizontales, faltarian Verticales
	imshow("dsps del erode y ants de la suma!",res3);
	
	Mat m2=getStructuringElement(MORPH_RECT,Size(4,2));
	erode(res,res2,m2);
	imshow("Tercer Erode",res2);
	res=res1+res2+res3;
//	for(int i=0;i<n;i++) { 	for(int j=0;j<n;j++) { cout<<m.at<bool>(i,j)<<" "; }cout<<endl;	}
	Mat m3=getStructuringElement(MORPH_RECT,Size(2,2));
	dilate(res,res,m3);
	imshow("Resultado",res);
}
void Mostrar(Mat m){
	for(int i=0;i<m.rows;i++) { 
		for(int j=0;j<m.cols;j++) { 
			cout<<m.at<bool>(i,j)<<" ";
		}cout<<endl;
	}
}
void Ejercicio3b(){
	Mat img=imread("Caracteres.jpeg",IMREAD_GRAYSCALE);
	img=Umbral(img,180);
	imshow("Original",img);
	
	
	Mat res;
//	//extrayendo letras
//	Mat m=getStructuringElement(MORPH_CROSS,Size(3,3),Point(1,1));
//	Mostrar(m);
//	erode(img,res,m);
//	imshow("Morfologia",res);
	
	//extrayendo peques
	
	Mat m=getStructuringElement(MORPH_CROSS,Size(2,1));
	Mostrar(m);
	
	erode(img,res,m);
	
	imshow("Morfoligia",res);
	
	
}
int main(int argc, char** argv) {
//	Ejercicio1();
//	Ejercicio2();
//	Ejercicio3a();
	Ejercicio3b();
	
	
	
	waitKey(0);
	return 0;
} 
