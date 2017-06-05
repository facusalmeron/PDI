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
void Mostrar(Mat m){
	for(int i=0;i<m.rows;i++) { 
		for(int j=0;j<m.cols;j++) { 
			cout<<m.at<bool>(i,j)<<" ";
		}cout<<endl;
	}
}
void Ejercicio3_2(){
	Mat img=imread("Caracteres.jpeg",IMREAD_GRAYSCALE);
	img=Umbral(img,180);
	imshow("Original",img);
	Mat letras;
	Mat EE=getStructuringElement(MORPH_RECT,Size(3,3));
	Mostrar(EE);
	erode(img,letras,EE);
	dilate(letras,letras,EE);
	imshow("Letras",letras);
	Mat simbolos=img-letras;
	imshow("Simbolos",simbolos);
}

void Ejercicio3_3(){
	Mat img=imread("estrellas.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	imshow("Original",img);
	Mat umbralizada=cv::Mat::zeros(img.rows,img.cols,CV_32F);
	for(int i=0;i<img.rows;i++) { 
		for(int j=0;j<img.cols;j++) { 
			if (img.at<uchar>(i,j)>200){
				umbralizada.at<float>(i,j)=1;
			}
		}
	}
	Mat EE=getStructuringElement(MORPH_ELLIPSE,Size(3,3));
	Mat salida;
	erode(umbralizada,salida,EE);
	imshow("Umbralizada",umbralizada);
	for(int i=0;i<img.rows;i++) { 
		for(int j=0;j<img.cols;j++) { 
			if(salida.at<float>(i,j)==0){
				img.at<uchar>(i,j)=0;
			}
		}
	}
	imshow("Salida",img);
	waitKey(0);
}

void Ejercicio3_4(){
	Mat img=imread("lluviaEstrellas.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	Mat EE=getStructuringElement(MORPH_RECT,Size(7,7));
	for(int i=0;i<EE.rows;i++) { 
		for(int j=0;j<EE.cols;j++) { 
			EE.at<uchar>(i,j)=0;
		}
	}
	for(int i=0;i<EE.rows;i++) { 
		EE.at<uchar>(i,EE.cols-1-i)=1;
	}
	Mostrar(EE);
	Mat salida;
	erode(img,salida,EE);
	dilate(salida,salida,EE);
	namedWindow("Original",CV_WINDOW_KEEPRATIO);
	imshow("Original",img);
	namedWindow("Salida",CV_WINDOW_KEEPRATIO);
	imshow("Salida",salida);
	waitKey(0);
}

void Ejercicio3_5(){
	Mat img=imread("Globulos.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	Mat umbral=cv::Mat::zeros(img.size(),img.type());
	for(int i=0;i<img.rows;i++) { 
		for(int j=0;j<img.cols;j++) { 
			if (img.at<uchar>(i,j)>120){
				umbral.at<uchar>(i,j)=255;
			}
		}
	}
	for(int i=0;i<img.rows;i++) { 
		for(int j=0;j<img.cols;j++) { 
			if (umbral.at<uchar>(i,j)==255){
				umbral.at<uchar>(i,j)=0;
			}
			else{
				umbral.at<uchar>(i,j)=255;
			}
		}
	}
	Mat EE=getStructuringElement(MORPH_RECT,Size(3,3));
	erode(umbral,umbral,EE);
	dilate(umbral,umbral,EE);
	Mat aux=umbral.clone();
	vector<vector<Point> > contornos;
	vector<Vec4i> hierarchy;
	Mat salida=cv::Mat::zeros(img.size(),img.type());
	findContours(umbral, contornos, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
	for( int i = 0; i< contornos.size(); i++ )
	{
		drawContours(salida, contornos, i, Scalar(255), 1, 8, hierarchy, 0, Point() );
	}
	for(int i=0;i<contornos.size();i++) { 
		vector <Point> aux;
		aux=contornos[i];
		for(int j=0;j<aux.size();j++) { 
			if (aux[j].x==1 || aux[j].y==1 || aux[j].x==img.cols-2 || aux[j].y==img.rows-2){
				drawContours(salida,contornos,i,Scalar(0), 1, 8, hierarchy, 0, Point() );
				break;
			}
		}
//		cout<<contornos[i]<<endl;
	}
	namedWindow("Salida",CV_WINDOW_KEEPRATIO);
	imshow("Salida",salida);
	imshow("Aux",aux-salida);
	imshow("Original",img);
	imshow("Umbral",umbral);
	waitKey(0);
}

vector<Point> xy;
int cantclick = 10;
bool reset = false;

void  Click(int event, int x, int y, int flags, void* userdata){
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

Mat CrecimientoRegiones(Mat img,int x,int y){ //Recibe la imagen a segmentar y un (x,y) donde se planta la semilla.
	//RELLENA ELEMENTOS CONECTADOS CON UN VALOR DADO
	//Parámetros de floodFill:
	//1º Imagen a segmentar.
	//2º Punto x,y donde se planta la semilla.
	//3º Nuevo valor del dominio de los pixeles pintados (valores que toman regiones cercanas a la semilla).
	//4º Opcional --> NULL
	//5º Minima diferencia de brillo / color entre el pixel observado y uno de sus vecinos (tolerancia inferior)
	//6º Maxima diferencia de brillo / color entre el pixel observado y uno de sus vecinos (tolerancia superior)
	
	floodFill(img,Point(x,y),1,NULL,0.05,0.05);	
	//UMBRALIZO PARA QUE PASEN UNICAMENTE LOS ELEMENTOS CON INTENSIDAD 1
	//La funcion threshold aplica un umbral de nivel fijo a cada elemento del arreglo
	//Parametros: Origen, Destino, Valor del umbral, Maximo valor a usar con THRESH_BINARY y el INV, y por ultimo el tipo de umbral.
	threshold(img,img,1,1,THRESH_BINARY);			
	return img;
}

float Media(Mat img){
	float media=0;
	for(int i=0;i<img.rows;i++) { 
		for(int j=0;j<img.cols;j++) { 
			media+=img.at<uchar>(i,j);
		}
	}
	media/=(img.rows*img.cols);
	return media;
}

float Desvio(Mat img, int Media){
	float desvio=0;
	for(int i=0;i<img.rows;i++) { 
		for(int j=0;j<img.cols;j++) { 
			desvio+=pow(img.at<uchar>(i,j)-Media,2);
		}
	}
	desvio/=(img.rows*img.cols);
	desvio=sqrt(desvio);
	return desvio*10;
}

void Ejercicio3_6(){
	Mat img=imread("Rio.jpeg");
	//Haciendo 4 clicks obtengo los puntos para los dos ROI que quiero armar, uno el celeste del rio y otro del gris de afluentes
	cantclick=4;
	namedWindow("My Window",CV_WINDOW_KEEPRATIO);
	imshow("My Window",img);
	setMouseCallback("My Window", Click, NULL);
	waitKey(0);
	
	Mat roi=img(Rect(xy[0].x,xy[0].y,xy[1].x-xy[0].x,xy[1].y-xy[0].y));
	Mat roi2=img(Rect(xy[2].x,xy[2].y,xy[3].x-xy[2].x,xy[3].y-xy[2].y));
	
	cvtColor(roi,roi,CV_BGR2HSV); //Convierto a HSV
	cvtColor(roi2,roi2,CV_BGR2HSV); //Convierto a HSV
	vector <Mat> hsv; 	
	vector <Mat> hsv2; 	
	split(roi, hsv);
	split(roi2, hsv2);

	Mat canvasH(200,400,CV_32F);
	Mat canvasH2(200,400,CV_32F);
	Mat histoH=histogram(hsv[0],256,Mat());
	Mat histoH2=histogram(hsv2[0],256,Mat());
	normalize(histoH,histoH,0,1,CV_MINMAX);
	normalize(histoH2,histoH2,0,1,CV_MINMAX);
	draw_graph(canvasH,histoH);
	draw_graph(canvasH2,histoH2);
	//	stats(hsv[0]);
	
	//Realizo la umbralizacion.
	Mat mascara;
	Mat segmentacion= Mat::zeros(img.size(), img.type());    
	cvtColor(img,img,CV_BGR2HSV);
	cvtColor(roi,roi,CV_HSV2BGR);
	cvtColor(roi2,roi2,CV_HSV2BGR);
	//	Mat mean, std;
	//	meanStdDev(hsv[0], mean, std);
	float media=Media(hsv[0]);
	float desvio=Desvio(hsv[0],media);
	float media2=Media(hsv2[0]);
	float desvio2=Desvio(hsv2[0],media2);
	cout<<"Media: "<<media<<endl<<"Desvio: "<<desvio<<endl;
	cout<<"Media: "<<media2<<endl<<"Desvio: "<<desvio2<<endl;
	
	//	cout<<desvio;
	inRange(img,Scalar(media-desvio,0,0),Scalar(media+desvio,255,255),mascara);
	inRange(img,Scalar(media2-desvio2,0,0),Scalar(media2+desvio2,255,255),mascara);
	//	inRange(img,Scalar(170,0,0),Scalar(176,255,255),mascara);
	
	Mat kernel=filtro_promediador(7);
	mascara=convolve(mascara,kernel);
	for (int i=0;i<mascara.rows;i++){
		for (int j=0;j<mascara.cols;j++){
			if ((int)mascara.at<uchar>(i,j)>150){mascara.at<uchar>(i,j)=255;}
			else{mascara.at<uchar>(i,j)=0;}
		}
	}
	
	img.copyTo(segmentacion,mascara);
	cvtColor(img,img,CV_HSV2BGR);
	cvtColor(segmentacion,segmentacion,CV_HSV2BGR);
	
	//Para obtener los contornos (dilatacion-erosion).
	Mat EE=getStructuringElement(MORPH_RECT,Size(5,5));
	Mat contorno=cv::Mat::zeros(img.rows,img.cols,CV_32F);
	Mat aux1,aux2;
	dilate(mascara,aux1,EE);
	erode(mascara,aux2,EE);
	contorno=aux1-aux2;
	
	imshow("Original",img);
	//	imshow("ROI",roi);
//	namedWindow("Histograma H",CV_WINDOW_KEEPRATIO);
//	imshow("Histograma H",canvasH);
	namedWindow("Mascara",CV_WINDOW_KEEPRATIO);
	imshow("Mascara",mascara);
	namedWindow("Segmentacion HSV",CV_WINDOW_KEEPRATIO);
	imshow("Segmentacion HSV",segmentacion);
	imshow("Contorno",contorno);
}

void Ejercicio3_7(){
	Mat img=imread("Melanoma.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	
	imshow("Original",img);
	waitKey(0);
}

//encuentra el esqueleto de la imagen img, con tamB (tam elemento estructurante) y tamit(cant que crece)
void Ejercicio3_8(int tamB,int tamit){
	Mat img=imread("Cuerpos.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	vector<Mat> Sk(tamit);
	Mat unionSk(img.rows,img.cols,img.depth(),Scalar(0));
	for(int i=0;i<tamit;i++){
		Mat elem= getStructuringElement(MORPH_RECT,Size(tamB+i,tamB+i));
		erode(img,Sk[i],elem);
		Mat aux;
		morphologyEx(Sk[i],aux,MORPH_OPEN,getStructuringElement(MORPH_RECT,Size(tamB,tamB)));
		Sk[i]-=aux;
		bitwise_or(Sk[i],unionSk,unionSk);
	}
	Mat EE= getStructuringElement(MORPH_RECT,Size(tamB,tamB));
	dilate(unionSk,unionSk,EE);
	imshow("Original",img);
	imshow("Esqueletos",unionSk);
}

int main(int argc, char** argv) {
	//Ej1.1 El de erosion me va a ir achicando las figuras que tengo en negro. Por ej con el EE cuadrado me elimina todo directamente
	//Con la dilatacion me va a agrandar los objetos que tengo en la imagen.
//	Ejercicio1();
	
//	Ejercicio2();
	
//	Ejercicio3_1();
	
//	Ejercicio3_2();
	
//	Ejercicio3_3();
	
//	Ejercicio3_4();
	
//	Ejercicio3_5();
	
//	Ejercicio3_6();
	
//	Ejercicio3_7();

	Ejercicio3_8(3,5);
	
//	Ejercicio3_pico();
	
	waitKey(0);
	return 0;
} 
