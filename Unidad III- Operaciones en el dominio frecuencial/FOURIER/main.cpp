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
	imshow( "image" , image ) ; //imagen original
	imshow( "image espectro",spectrum(image)); //Espectro (Intensidad) de la imagen original
	imshow("filtrada",result); //resultado de aplicarle dicho filtro a la imagen original
	imshow("filtrada espectro",spectrum(result)); //espectro que se utilizara para filtrar al ser 
	//pasa-bajo solo usa el centro que es el q contiene a las mismas
	waitKey(0);
	
}

Mat Ejercicio1_1(int tipo){
	Mat figura(512,512,CV_32F);
	switch (tipo){
	case 1:{ //linea horizontal
//		line(figura,Point(0,figura.rows/2-100),Point(figura.cols,figura.rows/2-100),Scalar(1));
		//Este for si quiero hacer una linea con mayor grosor
//		for(int i=(figura.rows/2)-2;i<(figura.rows/2)+2;i++) { 
//			for(int j=0;j<figura.cols;j++) { 
//				figura.at<float>(i,j)=1;
//			}
//		}
		line(figura,Point(0,figura.rows/2),Point(figura.cols,figura.rows/2),Scalar(1));
//		line(figura,Point(0,figura.rows/2+100),Point(figura.cols,figura.rows/2+100),Scalar(1));
		break;}
	case 2:{ //linea vertical
		line(figura,Point(figura.cols/2,0),Point(figura.cols/2,figura.rows),Scalar(1));
		break;}
	case 3:{ //cuadrado centrado
		rectangle(figura,Point((figura.cols/2)-20,(figura.rows/2)-20),Point((figura.cols/2)+20,(figura.rows/2)+20),Scalar(1));
		//for para hacer cuadrado relleno
//		for(int i=figura.rows/2-20;i<figura.rows/2+20;i++) { 
//			for(int j=figura.cols/2-20;j<figura.cols/2+20;j++) { 
//				figura.at<float>(i,j)=1;
//			}
//		}
		break;}
	case 4:{ //rectangulo centrado
		rectangle(figura,Point((figura.cols/2)-40,(figura.rows/2)-20),Point((figura.cols/2)+40,(figura.rows/2)+20),Scalar(1));	
		break;}
	case 5:{ //circulo centrado
		circle(figura,Point(figura.cols/2,figura.rows/2),50,Scalar(1));
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
	transformada=spectrum(img);
	namedWindow("Espectro de Intensidad",CV_WINDOW_KEEPRATIO);
	imshow("Espectro de Intensidad",transformada);
	waitKey();
}

void rotate(cv::Mat& src, double angle, cv::Mat& dst){ //Funcion para rotar una imagen.
	cv::Point2f ptCp(src.cols*0.5, src.rows*0.5);
	cv::Mat M = cv::getRotationMatrix2D(ptCp, angle, 1.0);
	cv::warpAffine(src, dst, M, src.size(), cv::INTER_CUBIC); //Nearest is too rough, 
}

void Ejercicio1_3(){
	Mat img(512,512,CV_32F);
	line(img,Point(img.cols/2,0),Point(img.cols/2,img.rows),Scalar(1));
	Mat roi1=img(Rect(img.cols/2-128,img.rows/2-128,256,256));
	imshow("Imagen Original",roi1);
	imshow("Espectro Img Original",spectrum(roi1));
	rotate(img,20,img);
	Mat roi2=img(Rect(img.cols/2-128,img.rows/2-128,256,256));
	imshow("Imagen Rotada",roi2);
	imshow("Espectro Img Rotada",spectrum(roi2));
	waitKey(0);
}

void Ejercicio1_4(string nombre){
	Mat img=imread(nombre,CV_LOAD_IMAGE_GRAYSCALE);
	img.convertTo(img,CV_32F,1./255);
	imshow("Original",img);
	imshow("Magnitud",spectrum(img));
	waitKey(0);
}

void Ejercicio2(string nombre,string pasa,int tipo,double D0,int orden){
	Mat img=imread(nombre,CV_LOAD_IMAGE_GRAYSCALE);
	img.convertTo(img,CV_32F,1./255);
	int filas=img.rows;
	int columnas=img.cols;
	img=optimum_size(img); //Hago el rellenado
	Mat filtro;
	switch (tipo){
	case 1:{
		filtro=filter_ideal(img.rows,img.cols,D0); //filtro ideal
		break;}
	case 2:{
		filtro=filter_butterworth(img.rows,img.cols,D0,orden); //filtro de Butterworth
		break;}
	case 3:{
		filtro=filter_gaussian(img.rows,img.cols,D0); //Filtro Gaussiano
		break;}
	}
	if (pasa=="Pasa Alto"){
		filtro=1-filtro; //Si es pasa alto se invierte todo, ya que ahora lo que no se filtra es el circulo
		//central que es donde estan las frecuencias bajas y se dejan pasar el resto que son las altas
	}
	Mat filtrada=filter(img,filtro);
	filtrada=filtrada(Range(0,filas),Range(0,columnas));
	img=img(Range(0,filas),Range(0,columnas));
	imshow("Original",img);
	imshow("Espectro Original",spectrum(img));
	namedWindow("Imagen Filtrada",CV_WINDOW_KEEPRATIO);
	imshow("Imagen Filtrada",filtrada);
	imshow("Espectro filtro",spectrum(filtrada));
	waitKey(0);
}

void Ejercicio3(float A, float a, float b){
	Mat img=imread("camaleon.tif",CV_LOAD_IMAGE_GRAYSCALE);
	img.convertTo(img,CV_32F,1./255);
	int filas=img.rows;
	int columnas=img.cols;
	img=optimum_size(img);
	//Filtrado de alta potencia (high boost)
	Mat high_boost=filter_butterworth(img.rows,img.cols,50/255.0,2);
	high_boost=1-high_boost;
	high_boost=(A-1)+high_boost;
	Mat filtrado_AltaPotencia=filter(img,high_boost);
	filtrado_AltaPotencia=filtrado_AltaPotencia(Range(0,filas),Range(0,columnas));
	img=img(Range(0,filas),Range(0,columnas));
	
	//Filtrado de Alta Frecuencia
	img=optimum_size(img);
	//Filtrado de alta potencia (high boost)
	Mat enfasis=filter_butterworth(img.rows,img.cols,50/255.0,2);
	enfasis=1-enfasis;
	enfasis=a+b*enfasis;
	Mat filtrado_AltaFrecuencia=filter(img,enfasis);
	filtrado_AltaFrecuencia=filtrado_AltaFrecuencia(Range(0,filas),Range(0,columnas));
	img=img(Range(0,filas),Range(0,columnas));
	
	imshow("Original",img);
	imshow("Espectro Original",spectrum(img));
	namedWindow("Filtrado de Alta Potencia",CV_WINDOW_KEEPRATIO);
	imshow("Filtrado de Alta Potencia",filtrado_AltaPotencia);
	imshow("Espectro Filtro Alta Potencia",spectrum(filtrado_AltaPotencia));
	namedWindow("Filtrado de Alta Frecuencia",CV_WINDOW_KEEPRATIO);
	imshow("Filtrado de Alta Frecuencia",filtrado_AltaFrecuencia);
	imshow("Espectro Filtro Alta Frecuencia",spectrum(filtrado_AltaFrecuencia));
	
	waitKey(0);
}
void prueba(){
	Mat img(512,512,CV_32F);
	line(img,Point(img.cols/2,0),Point(img.cols/2,img.rows),Scalar(1));
	Mat roi1=img(Rect(img.cols/2-128,img.rows/2-128,256,256));
	imshow("Imagen Original",roi1);
	imshow("Espectro Img Original",spectrum(roi1));
	rotate(img,20,img);
	Mat roi2=img(Rect(img.cols/2-128,img.rows/2-128,256,256));
	imshow("Imagen Rotada",roi2);
	Mat espectro=spectrum(roi2);
	imshow("Espectro Img Rotada",spectrum(roi2));
	Mat umbral(roi1.size(),roi1.type());
	for(int i=0;i<umbral.rows;i++) { 
		for(int j=0;j<umbral.cols;j++) { 
			if (espectro.at<float>(i,j)> 220/255.0){
				umbral.at<float>(i,j)=1;
			}
			else umbral.at<float>(i,j)=0;
		}
	}
	int i=128;
	while (umbral.at<float>(i,168)!=1){
		i--;
	}
	double angulo=atan((128-i)/40.0)*180/M_PI;
	cout<<angulo;
	rotate(roi1,-angulo,roi1);
	imshow("Espectro",roi1);
	waitKey(0);
}

void Ejercicio4(double D0){
	Mat img=imread("reunion.tif",CV_LOAD_IMAGE_GRAYSCALE);
	img.convertTo(img,CV_32F,1./255);
	int filas=img.rows;
	int columnas=img.cols;
	img=optimum_size(img); //Hago el rellenado
	Mat salida;
	log(img,salida);
	Mat filtro=1-filter_butterworth(img.rows,img.cols,D0,2);
	salida=filter(salida,filtro);
	salida=salida(Range(0,filas),Range(0,columnas));
	img=img(Range(0,filas),Range(0,columnas));
	exp(salida,salida);
	imshow("Original",img);
	imshow("Filtrado homomorfico",salida);
	
	waitKey(0);
}

int main(int argc, char** argv) {
	//EJERCICIO 1:
	//el espectro no cambia si se mueve la linea
	//tipo 1: linea horizontal --> El espectro que obtengo es el de una linea vertical solamente
	//si voy agregando mas lineas horizontales, a la unica linea vertical que obtengo en el espectro
	//se le ve como un punteado. Si tuerzo un poquito la linea horizontal ahora obtengo un espectro
	//completo, siguiendo la tendencia del angulo de rotacion, siempre teniendo como linea blanca en
	//el centro que muestra la linea principal. Lo mismo sucede con una linea de mayor grosor

	//tipo 2: linea vertical --> En este caso el espectro que obtengo es una linea horizontal solamente,
	//el resto de las caracteristicas es lo mismo que se dijo para la linea horizontal.
	
	//tipo 3: cuadrado centrado --> El espectro aca es completo y obtengo una clara tendencia vertical
	//y horizontal (lineas gruesas blancas por el centro), ya que esta figura posee componentes tanto verticales
	//como horizontales. Si el cuadrado lo hago mas grande, las lineas horizontales y verticales en el espectro
	//se hacen mas angostas ambas. Si el cuadrado se achica las lineas se van haciendo mas gruesas. Si el cuadrado
	//esta relleno se puede ver en las diagonales como se representa mejor que en el vacio.
	
	//tipo 4: rectangulo centrado --> Es mas o menos lo mismo que el cuadrado, ahora el lado que es mas
	//largo que el otro en el espectro la linea es mas fina. Ejemplo si los lados horizontales son mas
	//largos que los verticales, en el espectro las lineas verticales van a ser mas finas que las horizontales.

	//tipo 5: circulo centrado --> Mientras menor sea el circulo (menor radio) obtengo en el espectro
	//circulos coaxiales mas grandes y mas separados. A mayor radio circulos mas pequeños mas juntos.
	
//	Mat figura=Ejercicio1_1(5);
//	imshow("Ejercicio1_1",figura);
	
//	Ejercicio1_2(figura);
	
//	Ejercicio1_3(); //como se explico mas arriba, al rotar 20 grados el espectro tambien lo hace.
	//Al tratarse de una linea vertical el espectro aparece horizontal y cuando lo roto 20 grados,
	//tambien lo hace en el espectro
	
//	Ejercicio1_4("huang3.jpg"); //Si pruebo con la img de ejemplo de huang1 o huang 2 observamos que
	//hay muchos componentes de baja frecuencia que son los que se encuentran en el centro del espectro.
	//Mientras que si probamos con huang3 que tiene gran cantidad de bordes y por lo tanto muchas frecuencias
	//podemos ver en el espectro que en el centro practicamente no hay nada y se visualizan las altas frecuencias.
	
	//EJERCICIO 2:
//	Ejercicio2("huang3.jpg","Pasa Alto",3,25/255.0,5);
//	TIPO 1 --> IDEAL: Primero en los pasa-bajos mientras menor sea el radio D0 mas borrosa sera la img, 
	//y el fenomeno de Gibbs se puede ver por ejemplo cuando agrandamos la imagen filtrada y se ve que 
	//las ondulaciones se ven repetidas.
	//Ahora en los pasa-altos si el radio es mayor van quedando solamente los bordes de la imagen.
	
//	TIPO 2 --> BUTTERWORTH: En este caso probando con un mismo radio que el filtro ideal veo como se borronea
	//la img pero no aparece el fenomeno de Gibbs, mientras que si incremento el orden, las ondulaciones
	//en el filtro empiezan a aparecer. Cuando crece el orden n crece Gibbs. El radio en este caso solamente
	//lo hace mas o menos borrosa a la img. Si el orden se incrementa el filtro se va pareciendo al 
	//filtro ideal, mientras que si el orden es inferior se parece al filtro de Gauss.
	
//	TIPO 3 --> GAUSSIANO: El filtro gaussiano no presenta el fenomeno de Gibbs
	
//	EJERCICIO 3:
//	Ejercicio3(2.5,0,1);
//	prueba();
	
	Ejercicio4(100/255.0);
	
	waitKey();
	return 0;
} 
