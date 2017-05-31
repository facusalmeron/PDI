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

vector<Mat> TransformadaHough(Mat img){
	//Paso 1 del algoritmo: Calcular el gradiente de la imagen y umbralizar el resultado
	Mat Gradiente;
	Canny(img, Gradiente, 50, 200, 5);
	
	//Paso 2 del algoritmo: Generar el espacio de parametros mediante valores equiespaciados de ro y sigma,
	//inicializandolo en cero al acumulador.
	//Para cada punto de borde de la imagen, calcular la curva correspondiente en el espacio de parametros
	//para cada sigma, despejando el valor de ro. Incrementar el acumulador en la posicion (ro,sigma).
	Mat acumulador = Mat::zeros(400,400,CV_8U);
	vector<Vec2i> coordenadas;
	for(int i=0;i<img.rows;i++){
		for(int j=0;j<img.cols;j++){
			if(Gradiente.at<uchar>(i,j) == 255){ //Quiere decir que corresponde a un borde
				float angulo = atan(i/j*1.0);
				float ro = i*cos(angulo) + j*sin(angulo); //Curva en el espacio de parametro
				acumulador.at<uchar>(ro,angulo*180.0) += 1; //Incremento el acumulador en la posicion de ro-sigma
				Vec2i aux;
				aux[0] = i;
				aux[1] = j;
				coordenadas.push_back(aux);
			}
		}
	}
	
	//Paso 3: La interseccion de curvas en el espacio ro-sigma (altos valores en el acumulador) identifica 
	//la colinearidad de los puntos a los cuales corresponden tales curvas.
	//Conectar los puntos colineales cercanos (adyacentes o cuya distancia entre si sea menor a un umbral).
	Mat resultado = Mat::zeros(img.rows,img.cols,img.type());
	for(int i=0;i<coordenadas.size();i++){ //Recorro por coordenadas para conectar los puntos
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
	vector <Mat> Solucion;
	Solucion.push_back(Gradiente);
	Solucion.push_back(acumulador*255);
	Solucion.push_back(resultado);
	return Solucion;
}

void Ejercicio2_1(){
	Mat img = Mat::zeros(300,300, CV_8U);
	line(img,Point(100,100),Point(190,190),Scalar(255));
	line(img,Point(10,190),Point(100,100),Scalar(255));
	line(img,Point(150,0),Point(150,300),Scalar(255));
	imshow("Original",img);
	vector <Mat> Solucion;
	Solucion=TransformadaHough(img);
	imshow("Gradiente",Solucion[0]);
	imshow("Acumulador",Solucion[1]);
	imshow("Resultado",Solucion[2]);
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
	
	floodFill(img,Point(x,y),1.01,NULL,0.05,0.05);	
	//UMBRALIZO PARA QUE PASEN UNICAMENTE LOS ELEMENTOS CON INTENSIDAD 1
	//La funcion threshold aplica un umbral de nivel fijo a cada elemento del arreglo
	//Parametros: Origen, Destino, Valor del umbral, Maximo valor a usar con THRESH_BINARY y el INV, y por ultimo el tipo de umbral.
	threshold(img,img,1,1,THRESH_BINARY);			
	return img;
}

void Ejercicio3(){
	Mat img = imread("bone.tif",CV_LOAD_IMAGE_GRAYSCALE);
	img.convertTo(img,CV_32F,1./255);

	//OBTENGO LAS COORDENADAS X e Y DE DONDE HICE CLICK PARA PLANTAR LA SEMILLA.
	cantclick = 1;
	namedWindow("My Window",WINDOW_NORMAL);
	imshow("My Window",img);
	//Despues de llamar a Click, en el vector xy tengo guardadas las coordenadas donde hice click
	setMouseCallback("My Window", Click, NULL);
	waitKey(0);
	imshow("Original",img);
	
	//HAGO EL CRECIMIENTO A PARTIR DE DONDE HICE CLICK
	Mat resultado = CrecimientoRegiones(img,xy[0].x,xy[0].y);
	imshow("Resultado",resultado);
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

void Ejercicio4(){
	Mat img=imread("rosas.jpg");
	//Haciendo 2 clicks obtengo los puntos para el ROI que quiero armar
	cantclick=2;
	namedWindow("My Window",CV_WINDOW_KEEPRATIO);
	imshow("My Window",img);
	setMouseCallback("My Window", Click, NULL);
	waitKey(0);
	
	//muestra representativa del color de una rosa (ROI)
	Mat roi=img(Rect(xy[0].x,xy[0].y,xy[1].x-xy[0].x,xy[1].y-xy[0].y));
	
	cvtColor(roi,roi,CV_BGR2HSV); //Convierto a HSV
	vector <Mat> hsv; 	
	split(roi, hsv);
	
	//Si muestro las imagenes del H, S y V de la img original, voy a ver que la que influye en el color de
	//las rosas es solamente el canal de H, entonces trabajo solamente en él, descartando el S y V.
	
	//Hago los histogramas para saber que region utilizar luego.
	Mat canvasH(200,400,CV_32F);
	Mat histoH=histogram(hsv[0],256,Mat());
	normalize(histoH,histoH,0,1,CV_MINMAX);
	draw_graph(canvasH,histoH);
//	stats(hsv[0]);
	
	//Realizo la umbralizacion.
	Mat mascara;
	Mat segmentacion= Mat::zeros(img.size(), img.type());    
	cvtColor(img,img,CV_BGR2HSV);
	cvtColor(roi,roi,CV_HSV2BGR);
//	Mat mean, std;
//	meanStdDev(hsv[0], mean, std);
	float media=Media(hsv[0]);
	float desvio=Desvio(hsv[0],media);
	cout<<"Media: "<<media<<endl<<"Desvio: "<<desvio<<endl;

//	cout<<desvio;
	inRange(img,Scalar(media-desvio,0,0),Scalar(media+desvio,255,255),mascara);
//	inRange(img,Scalar(170,0,0),Scalar(176,255,255),mascara);
	
	//Aplico filtro a la mascara para sacar los puntitos que aparecen que no son de rosas
	Mat kernel=filtro_promediador(5);
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
	
	imshow("Original",img);
//	imshow("ROI",roi);
	namedWindow("Histograma H",CV_WINDOW_KEEPRATIO);
	imshow("Histograma H",canvasH);
	imshow("Mascara",mascara);
	namedWindow("Segmentacion HSV",CV_WINDOW_KEEPRATIO);
	imshow("Segmentacion HSV",segmentacion);
	waitKey(0);
	
}

int main(int argc, char** argv) {
	//EJERCICIO 1:
	//TIPO 0: ROBERTS
	//TIPO 1: PREWITT
	//TIPO 2: SOBEL
	//TIPO 3: LAPLACIANO
	//TIPO 4: LoG
//	Ejercicio1(2);

//	Ejercicio2_1();
	
//	Ejercicio3();
	
	Ejercicio4();
	waitKey(0);
	return 0;
} 
