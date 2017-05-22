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

Mat ruido_gaussiano(Mat img,double mean,double sigma){
	Mat ruido = img.clone();
//	img.convertTo(img,CV_32F,1./255);
//	img.convertTo(ruido,CV_32F,1./255);
	RNG rng;
	rng.fill(ruido, RNG::NORMAL, mean,sigma); 
	add(img, ruido, img);
	return img;
}

void Ejercicio1(){
	Mat img=imread("circulo.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	Mat canvas_original(img.rows,img.cols,CV_32F);
	Mat histo_original=histogram(img,255);
	normalize(histo_original,histo_original,0,1,CV_MINMAX);
	draw_graph(canvas_original,histo_original);
	
	//Ruido sal y pimienta
	Mat syp=img.clone();
	syp=ruido_sal_pimienta(syp,10/255.0,10/255.0);
	Mat canvas_syp(syp.rows,syp.cols,CV_32F);
	Mat histo_syp=histogram(syp,255);
	normalize(histo_syp,histo_syp,0,1,CV_MINMAX);
	draw_graph(canvas_syp,histo_syp);
	
	//Ruido Gaussiano
	Mat rg=img.clone();
	rg=ruido_gaussiano(rg,0,0.10*255);
	Mat canvas_rg(rg.rows,rg.cols,CV_32F);
	Mat histo_rg=histogram(rg,255);
	normalize(histo_rg,histo_rg,0,1,CV_MINMAX);
	draw_graph(canvas_rg,histo_rg);
	
	imshow("Original",img);
	imshow("Histograma Original",canvas_original);
	imshow("Ruido Sal y Pimienta",syp);
	imshow("Histograma Sal y Pimienta",canvas_syp);
	imshow("Ruido Gaussiano",rg);
	imshow("Histograma Ruido Gaussiano",canvas_rg);
	waitKey(0);
}

Mat MediaGeometrica(Mat img,int tam){
	img.convertTo(img,CV_32F,1./255);
	Mat img2=img.clone();
	int m=tam/2;
	for(int i=m;i<img.rows-m;i++) { 
		for(int j=m;j<img.cols-m;j++) { 
			for(int tt=0;tt<3;tt++){
				float aux=1;
				for(int k=-m;k<=m;k++) { 
					for(int l=-m;l<=m;l++) { 
						aux*=img.at<float>(i+k,j+l);
					}
				}
				img2.at<float>(i,j)=pow(aux,1./(tam*tam));
			}
		}
	}
	img=img2;
	return img;
}

Mat MediaContraArmonica(Mat img,int tam,float Q){
	img.convertTo(img,CV_32F,1./255);
	Mat img2=img.clone();
	int m=tam/2;
	for(int i=m;i<img.rows-m;i++) { 
		for(int j=m;j<img.cols-m;j++) { 
			for(int tt=0;tt<3;tt++){
				float aux1=0;
				float aux2=0;
				for(int k=-m;k<=m;k++) { 
					for(int l=-m;l<=m;l++) { 
						aux1+=pow(img.at<float>(i+k,j+l),Q+1);
						aux2+=pow(img.at<float>(i+k,j+l),Q);
					}
				}
				img2.at<float>(i,j)=aux1/aux2;
			}
		}
	}
	img=img2;
	return img;
}
float ECM(Mat img1,Mat img2){
	img1.convertTo(img1,CV_32F,1./255);
	img2.convertTo(img2,CV_32F,1./255);
	float error=0;
	if(img1.rows==img2.rows && img1.cols==img2.cols){
		for(int i=0;i<img1.rows;i++) { 
			for(int j=0;j<img1.cols;j++) { 
				error+=pow(img1.at<float>(i,j)-img2.at<float>(i,j),2);
			}
		}
		error=sqrt(error/(img1.rows*img1.cols));
	}else{
		cout<<endl<<"TIENEN QUE TENER EL MISMO TAMANO"<<endl;
	}
	return error;
}

void Ejercicio2(){
	Mat img=imread("sangre.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	Mat ruido=img.clone();
	ruido=ruido_sal_pimienta(ruido,0.0,0.1); 
	ruido=ruido_gaussiano(ruido,0,0.10*255);
	Mat fmg=ruido.clone(); 
	fmg=MediaGeometrica(fmg,3); //bueno para ruido sal, malo para ruido pimienta
	Mat fca=ruido.clone();
	fca=MediaContraArmonica(fca,3,-3); //-3 por tener ruido sal, si fuera pimienta tengo que poner positivo
	imshow("Original",img);
	imshow("Con ruido",ruido);
	imshow("Filtro Media Geometrica",fmg);
	imshow("Filtro Media Contra Armonica",fca);
	cout<<"El Error Cuadratico Medio (ECM) entre la imagen original y el ruido es: "<<ECM(img,ruido)<<endl;
	cout<<"El Error Cuadratico Medio (ECM) entre la imagen original y filtrada por Media Geometrica es: "<<ECM(img,fmg)<<endl;
	cout<<"El Error Cuadratico Medio (ECM) entre la imagen original y filtrada por Media Contra Armonica es: "<<ECM(img,fca)<<endl;
	waitKey(0);
}

Mat OrdenMediana(Mat img,int tam){
	img.convertTo(img,CV_32F,1./255);
	Mat img2=img.clone();
	int m=tam/2;
	for(int i=m;i<img.rows-m;i++) { 
		for(int j=m;j<img.cols-m;j++) { 
			for(int tt=0;tt<3;tt++){
				vector<float> aux;
				for(int k=-m;k<=m;k++) { 
					for(int l=-m;l<=m;l++) { 
						aux.push_back(img.at<float>(i+k,j+l));
					}
				}
				sort(aux.begin(),aux.end());
				img2.at<float>(i,j)=aux[((tam*tam)/2)+1];
			}
		}
	}
		img=img2;
	return img;
}

Mat OrdenPuntoMedio(Mat img,int tam){
	img.convertTo(img,CV_32F,1./255);
	Mat img2=img.clone();
	int m=tam/2;
	for(int i=m;i<img.rows-m;i++) { 
		for(int j=m;j<img.cols-m;j++) { 
			for(int tt=0;tt<3;tt++){
				float aux1=0;
				float aux2=1;
				for(int k=-m;k<=m;k++) { 
					for(int l=-m;l<=m;l++) { 
						if(aux1<img.at<float>(i+k,j+l))
							aux1=img.at<float>(i+k,j+l);
						if(aux2>img.at<float>(i+k,j+l))
							aux2=img.at<float>(i+k,j+l);
					}
				}
				img2.at<float>(i,j)=(aux1+aux2)/2;
			}
		}
		}
		img=img2;
	return img;
}

Mat OrdenMinimo(Mat img,int tam){
	img.convertTo(img,CV_32F,1./255);	
	Mat img2=img.clone();
	int m=tam/2;
	for(int i=m;i<img.rows-m;i++) { 
		for(int j=m;j<img.cols-m;j++) { 
			for(int tt=0;tt<3;tt++){
				float aux=1;
				for(int k=-m;k<=m;k++) { 
					for(int l=-m;l<=m;l++) { 
						if(aux>img.at<float>(i+k,j+l))
							aux=img.at<float>(i+k,j+l);
					}
				}
				img2.at<float>(i,j)=aux;
			}
		}
	}
	img=img2;
	return img;
}

Mat OrdenMaximo(Mat img,int tam){
	img.convertTo(img,CV_32F,1./255);
	Mat img2=img.clone();
	int m=tam/2;
	for(int i=m;i<img.rows-m;i++) { 
		for(int j=m;j<img.cols-m;j++) { 
			for(int tt=0;tt<3;tt++){
				float aux=0;
				for(int k=-m;k<=m;k++) { 
					for(int l=-m;l<=m;l++) { 
						if(aux<img.at<float>(i+k,j+l))
							aux=img.at<float>(i+k,j+l);
					}
				}
				img2.at<float>(i,j)=aux;
			}
		}
	}
		img=img2;
	return img;
}
Mat OrdenAlfaRecortado(Mat img,int tam,int d){
	img.convertTo(img,CV_32F,1./255);
	Mat img2=img.clone();
	int m=tam/2;
	for(int i=m;i<img.rows-m;i++) { 
		for(int j=m;j<img.cols-m;j++) { 
			for(int tt=0;tt<3;tt++){
				vector<float> aux;
				for(int k=-m;k<=m;k++) { 
					for(int l=-m;l<=m;l++) { 
						aux.push_back(img.at<float>(i+k,j+l));
					}
				}
				sort(aux.begin(),aux.end());
				aux.resize(aux.size()-d/2);
				float aux2=0;
				for(int k=d/2;k<aux.size();k++){ 
					aux2+=aux[k];
				}
				img2.at<float>(i,j)=aux2/(tam*tam-d);
			}
		}
	}
	img=img2;
	return img;
}

void Ejercicio3(){
	Mat img=imread("huang1.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	Mat ruido=img.clone();
	ruido=ruido_sal_pimienta(ruido,0.01,0.01);
	Mat filtro_mediana=ruido.clone();
	filtro_mediana=OrdenMediana(filtro_mediana,3);
	Mat filtro_ptomedio=ruido.clone();
	filtro_ptomedio=OrdenPuntoMedio(filtro_ptomedio,3);
	Mat filtro_ordenmin=ruido.clone();
	filtro_ordenmin=OrdenMinimo(filtro_ordenmin,3); //para ruido sal
	Mat filtro_ordenmax=ruido.clone();
	filtro_ordenmax=OrdenMaximo(filtro_ordenmax,3); //para ruido pimienta
	Mat filtro_alfarecortado=ruido.clone();
	filtro_alfarecortado=OrdenAlfaRecortado(filtro_alfarecortado,3,5); //para ruido pimienta
	imshow("Original",img);
	imshow("Ruido",ruido);
	imshow("Filtro Mediana",filtro_mediana);
	imshow("Filtro Punto Medio",filtro_ptomedio);
	imshow("Filtro Orden Maximo",filtro_ordenmax);
	imshow("Filtro Orden Minimo",filtro_ordenmin);
	imshow("Filtro Orden Alfa Recortado",filtro_alfarecortado);
	waitKey(0);
}

int main(int argc, char** argv) {
//	Ejercicio1();

//	Ejercicio2();
	
	Ejercicio3();
	waitKey(0);
	return 0;
} 
