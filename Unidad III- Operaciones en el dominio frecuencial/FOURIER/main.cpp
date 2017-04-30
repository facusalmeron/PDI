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

int main(int argc, char** argv) {
	//EJERCICIO 1:
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
	Mat figura=Ejercicio1_1(5);
	imshow("Ejercicio1_1",figura);
	Ejercicio1_2(figura);
	
	waitKey();
	return 0;
} 
