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

using namespace cv;
using namespace pdi;
using namespace std;

void Ejercicio1_1(){
	Mat img=imread("huang1.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	
	Mat canvas(img.rows,img.cols,CV_32F);
	Mat histo = histogram(img,255);
	normalize(histo,histo,0,1,CV_MINMAX);
	draw_graph(canvas,histo);
	
	Mat ecualizado;
	equalizeHist(img,ecualizado);
	Mat canvas2(ecualizado.rows,ecualizado.cols,CV_32F);
	Mat histo2 = histogram(ecualizado,255);
	normalize(histo2,histo2,0,1,CV_MINMAX);
	draw_graph(canvas2,histo2);
	imshow("Original",img);
	imshow("Histograma original",canvas);
	imshow("Imagen ecualizada",ecualizado);
	imshow("Histograma ecualizado",canvas2);
	
	waitKey(0);
}

void Ejercicio1_2(){
	Mat img1=imread("imagenA.tif",CV_LOAD_IMAGE_GRAYSCALE);
	Mat img2=imread("imagenB.tif",CV_LOAD_IMAGE_GRAYSCALE);
	Mat img3=imread("imagenC.tif",CV_LOAD_IMAGE_GRAYSCALE);
	Mat img4=imread("imagenD.tif",CV_LOAD_IMAGE_GRAYSCALE);
	Mat img5=imread("imagenE.tif",CV_LOAD_IMAGE_GRAYSCALE);
	Mat canvas1(img1.rows,img1.cols,CV_32F);
	Mat histo1 = histogram(img1,255);
	normalize(histo1,histo1,0,1,CV_MINMAX);
	draw_graph(canvas1,histo1);
	Mat canvas2(img2.rows,img2.cols,CV_32F);
	Mat histo2 = histogram(img2,255);
	normalize(histo2,histo2,0,1,CV_MINMAX);
	draw_graph(canvas2,histo2);
	Mat canvas3(img3.rows,img3.cols,CV_32F);
	Mat histo3 = histogram(img3,255);
	normalize(histo3,histo3,0,1,CV_MINMAX);
	draw_graph(canvas3,histo3);
	Mat canvas4(img4.rows,img4.cols,CV_32F);
	Mat histo4 = histogram(img4,255);
	normalize(histo4,histo4,0,1,CV_MINMAX);
	draw_graph(canvas4,histo4);
	Mat canvas5(img5.rows,img5.cols,CV_32F);
	Mat histo5 = histogram(img5,255);
	normalize(histo5,histo5,0,1,CV_MINMAX);
	draw_graph(canvas5,histo5);
	imshow("Histograma Imagen A:",canvas1);
	imshow("Histograma Imagen B:",canvas2);
	imshow("Histograma Imagen C:",canvas3);
	imshow("Histograma Imagen D:",canvas4);
	imshow("Histograma Imagen E:",canvas5);
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

void Ejercicio2_1(){
	Mat img=imread("huang2.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	Mat k3,k5,k11,k15;
	k3=filtro_promediador(3);
	k5=filtro_promediador(5);
	k11=filtro_promediador(11);
	k15=filtro_promediador(15);
	Mat f3,f5,f11,f15;
	f3=convolve(img,k3);
	f5=convolve(img,k5);
	f11=convolve(img,k11);
	f15=convolve(img,k15);
	vector<Mat> images;
	images.push_back(img);
	images.push_back(f3);
	images.push_back(f5);
	images.push_back(f11);
	images.push_back(f15);
	Mat m=mosaic(images,1);
	putText(m,"Original",cvPoint(0,240),FONT_HERSHEY_PLAIN,2,cvScalar(255,255,255),4);
	putText(m,"Kernel 3",cvPoint(257,240),FONT_HERSHEY_PLAIN,2,cvScalar(255,255,255),4);
	putText(m,"Kernel 5",cvPoint(513,240),FONT_HERSHEY_PLAIN,2,cvScalar(255,255,255),4);
	putText(m,"Kernel 11",cvPoint(767,240),FONT_HERSHEY_PLAIN,2,cvScalar(255,255,255),4);
	putText(m,"Kernel 15",cvPoint(1025,240),FONT_HERSHEY_PLAIN,2,cvScalar(255,255,255),4);
//	putText(output,"Hello World :)",cvPoint(15,70),	FONT_HERSHEY_PLAIN,	3,cvScalar(0,255,0),4);
	imshow("Filtro Promediador",m);
	waitKey(0);
}
Mat filtro_gaussiano(int tam,double sigma){
	int tamreal=tam;
	tam=tam/2;
	Mat kernel(tamreal,tamreal,CV_32F);
	// set standard deviation to 1.0
	double r, s = 2.0 * sigma * sigma;
	// sum is for normalization
	double sum = 0.0;
	// generate 3x3 kernel
	for (int x = -tam; x <= tam; x++)
	{
		for(int y = -tam; y <= tam; y++)
		{
			r = sqrt(x*x + y*y);
			kernel.at<float>(x + tam,y + tam) = (exp(-(r*r)/s))/(M_PI * s);
			sum += kernel.at<float>(x + tam,y + tam);
		}
	}

	// normalize the Kernel
	for(int i = 0; i < 3; ++i)
		for(int j = 0; j < 3; ++j)
		kernel.at<float>(i,j) /= sum;
	
	return kernel;
}

void Ejercicio2_2(){
	Mat img=imread("huang2.jpg",CV_LOAD_IMAGE_GRAYSCALE);
//	Mat k3=filtro_gaussiano(3,1);
//	Mat k5=filtro_gaussiano(5,1);
//	Mat k33=filtro_gaussiano(3,10);
//	Mat k55=filtro_gaussiano(5,10);
//	Mat f3=convolve(img,k3);
//	Mat f5=convolve(img,k5);
//	Mat f33=convolve(img,k33);
//	Mat f55=convolve(img,k55);
	Mat f3,f5,f33,f55;
	//Filtro gaussiano implementado por opencv.
	GaussianBlur(img,f3,Size(3,3),1,1);
	GaussianBlur(img,f5,Size(5,5),1,1);
	GaussianBlur(img,f33,Size(3,3),10,10);
	GaussianBlur(img,f55,Size(5,5),10,10);
	vector<Mat> images;
	images.push_back(img);
	images.push_back(f3);
	images.push_back(f5);
	images.push_back(f33);
	images.push_back(f55);
	Mat m=mosaic(images,1);
	putText(m,"Original",cvPoint(0,240),FONT_HERSHEY_PLAIN,1.4,cvScalar(255,255,255),2);
	putText(m,"Tam=3,Sigma=1",cvPoint(257,240),FONT_HERSHEY_PLAIN,1.4,cvScalar(255,255,255),2);
	putText(m,"Tam=5,Sigma=1",cvPoint(514,240),FONT_HERSHEY_PLAIN,1.4,cvScalar(255,255,255),2);
	putText(m,"Tam=3,Sigma=10",cvPoint(769,240),FONT_HERSHEY_PLAIN,1.4,cvScalar(255,255,255),2);
	putText(m,"Tam=5,Sigma=10",cvPoint(1026,240),FONT_HERSHEY_PLAIN,1.4,cvScalar(255,255,255),2);
	imshow("Filtro Gaussiano",m);
	
	waitKey(0);
}

void Ejercicio2_3(){
	Mat img=imread("hubble.tif",CV_LOAD_IMAGE_GRAYSCALE);
	Mat kernel=filtro_promediador(7);
	Mat filtro=convolve(img,kernel);
	Mat umbral(filtro.size(),CV_8UC(1));
	for (int i=0;i<filtro.rows;i++){
		for (int j=0;j<filtro.cols;j++){
			if ((int)filtro.at<uchar>(i,j)>150){umbral.at<uchar>(i,j)=255;}
			else{umbral.at<uchar>(i,j)=0;}
		}
	}
	imshow("Original",img);
	Mat multiplicacion= Mat::zeros(img.size(), img.type());    
	img.copyTo(multiplicacion,umbral);
	imshow("Umbral binario",multiplicacion);
	imshow("Filtrado",filtro);
	imshow("mascara",umbral);
	waitKey(0);
}

Mat filtro_pasa_alto_suma1(int tam){
	Mat kernel(tam,tam,CV_32F);
	for (int i=0;i<kernel.rows;i++){
		for (int j=0;j<kernel.cols;j++){
			kernel.at<float>(i,j)=-1;
		}
	}
	kernel.at<float>(tam/2,tam/2)=tam*tam;
	return kernel;
}

void Ejercicio3_1(){
	Mat img=imread("camaleon.tif",CV_LOAD_IMAGE_GRAYSCALE);
	Mat kernel=filtro_pasa_alto_suma1(3);
	Mat k5=filtro_pasa_alto_suma1(5);
	Mat k7=filtro_pasa_alto_suma1(7);
	Mat filtro=convolve(img,kernel);
	Mat filtro5=convolve(img,k5);
	Mat filtro7=convolve(img,k7);
	vector<Mat> images;
	images.push_back(img);
	images.push_back(filtro);
	images.push_back(filtro5);
	images.push_back(filtro7);
	Mat m=mosaic(images,1);
	imshow("Filtro Pasa-Altos Suma 1",m);
	waitKey(0);
}

Mat filtro_pasa_alto_suma0(int tam){
	Mat kernel(tam,tam,CV_32F);
	for (int i=0;i<kernel.rows;i++){
		for (int j=0;j<kernel.cols;j++){
			kernel.at<float>(i,j)=-1;
		}
	}
	kernel.at<float>(tam/2,tam/2)=tam*tam-1;
	return kernel;
}
void Ejercicio3_2(){
	Mat img=imread("camaleon.tif",CV_LOAD_IMAGE_GRAYSCALE);
	Mat kernel=filtro_pasa_alto_suma0(3);
	Mat k5=filtro_pasa_alto_suma0(5);
	Mat k7=filtro_pasa_alto_suma0(7);
	Mat filtro=convolve(img,kernel);
	Mat filtro5=convolve(img,k5);
	Mat filtro7=convolve(img,k7);
	vector<Mat> images;
	images.push_back(img);
	images.push_back(filtro);
	images.push_back(filtro5);
	images.push_back(filtro7);
	Mat m=mosaic(images,1);
	imshow("Filtro Pasa-Altos Suma 0",m);
	waitKey(0);
}

void Ejercicio4_1(){
	Mat img=imread("camaleon.tif",CV_LOAD_IMAGE_GRAYSCALE);
	Mat kernel=filtro_promediador(5);
	Mat filtro=convolve(img,kernel);
	Mat difusa=(img-filtro+255)/2;
	imshow("Original",img);
	imshow("Pasa-Bajos",filtro);
	imshow("Mascara Difusa",difusa);
	
	waitKey(0);
};

void Ejercicio4_2(float A){
	Mat img=imread("camaleon.tif",CV_LOAD_IMAGE_GRAYSCALE);
	Mat kernel=filtro_promediador(5);
	Mat filtro=convolve(img,kernel);
	Mat alta_potencia=(A*img-filtro+255)/2;
	imshow("Original",img);
	imshow("Pasa-Bajos",filtro);
	imshow("Filtrado de Alta Potencia",alta_potencia);
	waitKey(0);
}

void Ejercicio5_1(){
	//Lectura e histograma de la imagen original
	Mat img=imread("cuadros.tif",CV_LOAD_IMAGE_GRAYSCALE);
	imshow("Original",img);
	//Ecualizo globalmente la imagen original y se puede ver que al haber la 
	//mayor parte de la imagen en blanco, se ecualiza de mal forma obteniendo 
	//una imagen como si tuviera ruido.
	Mat img2;
	equalizeHist(img,img2);
	imshow("Ecualizacion Global",img2);
	//Ecualizo por partes (localmente):
	Mat cuadro1=img(Rect(12,12,124,124));
	equalizeHist(cuadro1,cuadro1);
	Mat cuadro2=img(Rect(374,12,124,124));
	equalizeHist(cuadro2,cuadro2);
	Mat cuadro3=img(Rect(194,194,124,124));
	equalizeHist(cuadro3,cuadro3);	
	Mat cuadro4=img(Rect(12,374,124,124));
	equalizeHist(cuadro4,cuadro4);
	Mat cuadro5=img(Rect(374,374,124,124));
	equalizeHist(cuadro5,cuadro5);
	imshow("Ecualizacion Local",img);
	waitKey(0);
}

void Ejercicio5_2(){
	Mat img=imread("esqueleto.tif",CV_LOAD_IMAGE_GRAYSCALE);
//	Mat kernel=filtro_promediador(7);
	Mat filtro;
	GaussianBlur(img,filtro,Size(5,5),15,15);
//	Mat kernel=filtro_pasa_alto_suma1(3);
//	Mat filtro=convolve(img,kernel);
//		Mat img_ecualizada;
//	equalizeHist(filtro,img_ecualizada);
	imshow("Original",img);
	imshow("Filtro",filtro);
//	imshow("Ecualizada",img_ecualizada);
	waitKey(0);
}

void Ejercicio5_3(string nombre){
	//Genero el histograma base para las banderas
	Mat banderas=imread("Busqueda_histograma/Bandera01.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	Mat histo_banderas = histogram(banderas,255);
	normalize(histo_banderas,histo_banderas,0,1,CV_MINMAX);
	
	//Genero el histograma base para las caricaturas 
	Mat caricaturas=imread("Busqueda_histograma/Caricaturas01.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	Mat histo_caricaturas = histogram(caricaturas,255);
	normalize(histo_caricaturas,histo_caricaturas,0,1,CV_MINMAX);
	
	//Genero el histograma base para los paisajes
	Mat paisajes=imread("Busqueda_histograma/Paisaje01.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	Mat histo_paisajes= histogram(paisajes,255);
	normalize(histo_paisajes,histo_paisajes,0,1,CV_MINMAX);
	
	//Genero el histograma base para los personajes
	Mat personajes=imread("Busqueda_histograma/Personaje01.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	Mat histo_personajes= histogram(personajes,255);
	normalize(histo_personajes,histo_personajes,0,1,CV_MINMAX);
	
	//Genero el histograma para la imagen a clasificar
	Mat original=imread(nombre,CV_LOAD_IMAGE_GRAYSCALE);
	Mat histo_original= histogram(original,255);
	normalize(histo_original,histo_original,0,1,CV_MINMAX);
	
	Mat canvas_banderas(200,400,CV_32F);
	draw_graph(canvas_banderas,histo_banderas);
	
	Mat canvas_caricaturas(200,400,CV_32F);
	draw_graph(canvas_caricaturas,histo_caricaturas);
	
	Mat canvas_paisajes(200,400,CV_32F);
	draw_graph(canvas_paisajes,histo_paisajes);
	
	Mat canvas_personajes(200,400,CV_32F);
	draw_graph(canvas_personajes,histo_personajes);
	
	Mat canvas_original(200,400,CV_32F);
	draw_graph(canvas_original,histo_original);
	
	imshow("bandera",canvas_banderas);
	imshow("caricaturas",canvas_caricaturas);
	imshow("paisajes",canvas_paisajes);
	imshow("personajes",canvas_personajes);
	imshow("original",canvas_original);
	
	double cpbanderas=compareHist(histo_banderas,histo_original,CV_COMP_CORREL);
	double cpcaricaturas=compareHist(histo_caricaturas,histo_original,CV_COMP_CORREL);
	double cppaisajes=compareHist(histo_paisajes,histo_original,CV_COMP_CORREL);
	double cppersonajes=compareHist(histo_personajes,histo_original,CV_COMP_CORREL);	
	cout<<cpbanderas<<endl<<cpcaricaturas<<endl<<cppaisajes<<endl<<cppersonajes<<endl;
	
	if ((cpbanderas >= cpcaricaturas) && (cpbanderas >= cppaisajes) && (cpbanderas >= cppersonajes)){
		cout<<"La imagen ingresada pertenece al grupo de las BANDERAS"<<endl;
	}
	else{
		if ((cpcaricaturas >= cpbanderas) && (cpcaricaturas>= cppaisajes) && (cpcaricaturas>= cppersonajes)){
			cout<<"La imagen ingresada pertenece al grupo de las CARICATURAS"<<endl;
		}
		else{
			if ((cppaisajes>= cpbanderas) && (cppaisajes>= cpcaricaturas) && (cppaisajes>= cppersonajes)){
				cout<<"La imagen ingresada pertenece al grupo de los PAISAJES"<<endl;
			}
			else{
				if ((cppersonajes>= cpbanderas) && (cppersonajes>= cpcaricaturas) && (cppersonajes>= cpcaricaturas)){
					cout<<"La imagen ingresada pertenece al grupo de los PERSONAJES"<<endl;
				}
			}
		}
	}
	
	waitKey(0);
}

int main(int argc, char** argv) {
//	Ejercicio1_1();
	
//	EJERCICIO 1.2.a:
	//*HISTO1: MAS OSCURA, BUEN CONTRASTE
	//*HISTO2: CLARA, GRISES MEDIOS, POCO CONTRASTE
	//*HISTO3: MUY OSCURA, MALO CONTRASTE
	//*HISTO4: MUY CLARA, POCO CONTRASTE
	//*HISTO5: MAS CLARA Y BUEN CONTRASTE.
	
	//EJERCICIO 1.2.b
	//*IMAGEN A: HISTOGRAMA 2
	//*IMAGEN B: HISTOGRAMA 4 ES MUY CLARA, MAL CONTRASTE
	//*IMAGEN C: HISTOGRAMA 1
	//*IMAGEN D: HISTOGRAMA 5
	//*IMAGEN E: HISTOGRAMA 3 MUY OSCURA
	
//	Ejercicio1_2();
	
//	Ejercicio2_1();
	
//	Ejercicio2_2();
	
//	Ejercicio2_3(); //para este ejercicio basta solamente con aplicar un filtro
	//promediador por ejemplo y aplico un umbral binario, es decir los de cierto
	//rango de grises se pintan en blanco, los otros no pasan. Entonces si voy 
	//aumentando el tamaño del kernel pasaran menos objetos, llegando a pasar 
	//solo los mas grandes, porque los mas chicos se desenfocan mas.

//	Ejercicio3_1();
	
//	Ejercicio3_2();
	
//	Ejercicio4_1();
	
//	Ejercicio4_2(2);
	
	Ejercicio5_1();
	
//	Ejercicio5_2();  //PAG 169 LIBRO
	
//	Ejercicio5_3("Busqueda_histograma/Bandera03.jpg");
	waitKey(0);
	return 0;
} 
