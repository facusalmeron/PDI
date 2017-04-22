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

using namespace cv;
using namespace pdi;
using namespace std;

void Ejercicio1(float a, float c){
	//EJERCICIO 1_1
	Mat img=imread("earth.bmp",CV_LOAD_IMAGE_GRAYSCALE);
	Mat aux(1,256,CV_8UC(1));
	for (int i=0;i<256;i++){
		float s=a*i+c;
		if (s>255){
			aux.at<uchar>(i)=255;
		}
		else{
			if (s<0){
				aux.at<uchar>(i)=0;
			}
			else{
				aux.at<uchar>(i)=s;
			}
		}
	}
	Mat resultado;
	LUT(img,aux,resultado);
	Mat grafico(200,400,CV_8UC(1));
	grafico.setTo(Scalar(0,0,0));
	draw_graph(grafico,aux);
	imshow("Mapeo",grafico);
	imshow("Original",img);
	imshow("LUT",resultado);
}

void Ejercicio2(int tipo, float gamma){
	Mat img=imread("earth.bmp",CV_LOAD_IMAGE_GRAYSCALE);
	Mat aux(1,256,CV_8UC(1));
	if (tipo==1){
		for (int i=0;i<256;i++){
			float s=(255/(log(1+255)))*log(1+i);
//			float s=log(1+i);
			if (s>255){
				aux.at<uchar>(i)=255;
			}
			else{
				if (s<0){
					aux.at<uchar>(i)=0;
				}
				else{
					aux.at<uchar>(i)=s;
				}
			}
		}		
	}
	else{
			for (int i=0;i<256;i++){
				float s=(255/pow(255,gamma))*pow(i,gamma);
//				float s=255*pow(i/255.0,gamma);
				
				if (s>255){
					aux.at<uchar>(i)=255;
				}
				else{
					if (s<0){
						aux.at<uchar>(i)=0;
					}
					else{
						aux.at<uchar>(i)=s;
					}
				}
			}				
		}
	
	Mat resultado;
	LUT(img,aux,resultado);
	Mat grafico(256,256,CV_8UC(1));
	grafico.setTo(Scalar(0,0,0));
	draw_graph(grafico,aux);
	imshow("Mapeo 2",grafico);
	imshow("Original 2",img);
//	normalize(resultado,resultado,0,255,CV_MINMAX);
	imshow("LUT 2",resultado);
}

void Ejercicio3_1ab(){
	//SUMA
	Mat img=imread("micro.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	Mat img2=imread("mascara.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	Mat m=mosaic(img,img2,0);
	Mat suma=(img+img2)/2;
	
	//RESTA
//	Mat resta;
	Mat resta=(img-img2+255)/2;
//	subtract(img,img2,resta);
	
	
	imshow("Imagenes",m);
	imshow("Suma",suma);
	imshow("Resta",resta);
}
void Ejercicio3_1c(string nombre,string ventana){
	
	Mat img=imread(nombre,CV_LOAD_IMAGE_GRAYSCALE);
	Mat mascara= Mat::zeros(img.size(), img.type());
	Mat multiplicacion= Mat::zeros(img.size(), img.type());    
	//SI LA MASCARA BINARIA ES UN CIRCULO
//	circle(mascara, Point(53,54), 16, Scalar(255, 0, 0), -1, 8, 0);
//	circle(mascara, Point(102,53), 16, Scalar(255, 0, 0), -1, 8, 0);
//	circle(mascara, Point(152,53), 16, Scalar(255, 0, 0), -1, 8, 0);
//	circle(mascara, Point(202,53), 16, Scalar(255, 0, 0), -1, 8, 0);
//	circle(mascara, Point(252,51), 16, Scalar(255, 0, 0), -1, 8, 0);
//	
//	circle(mascara, Point(53,101), 16, Scalar(255, 0, 0), -1, 8, 0);
//	circle(mascara, Point(102,102), 16, Scalar(255, 0, 0), -1, 8, 0);
//	circle(mascara, Point(152,101), 16, Scalar(255, 0, 0), -1, 8, 0);
//	circle(mascara, Point(202,100), 16, Scalar(255, 0, 0), -1, 8, 0);
//	circle(mascara, Point(252,100), 16, Scalar(255, 0, 0), -1, 8, 0);
	//SI LA MASCARA BINARIA ES UN RECTANGULO
	info(img);
	rectangle(mascara,Point(190,90),Point(250,155),Scalar(255, 0, 0),-1,8,0);
	//Now you can copy your source image to destination image with masking
	img.copyTo(multiplicacion, mascara);
	
//	imshow("Imagen",img);
	imshow("Mascara",mascara);
	imshow(ventana,multiplicacion);

}
	
void Ejercicio3_2(){
	Mat img=imread("huang1.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	img.convertTo(img,CV_32F,1/255.0);
	vector <Mat> imagenes;
	for(int i=0;i<50;i++) { 
		Mat ruido(img.size(),img.type());
		randn(ruido,0,0.05);
		Mat aux=(ruido+img);
		imagenes.push_back(aux);
	}
	Mat img_final(img.size(),CV_32F);
	int tam=imagenes.size();
	for(int i=1;i<tam;i++) { 
		img_final+=imagenes[i];
	}
	img_final=(img_final/tam);
	imshow("Original",img);
	imshow("Img con ruido",imagenes[0]);
	imshow("Promedio",img_final);
}
float ECM(Mat img1,Mat img2){
	float error=0;
	if(img1.depth() != CV_32F || img2.depth() != CV_32F){
		cout<<endl<<"TIENE Q SER CV32F"<<endl;
	}else{
		if(img1.rows==img2.rows && img1.cols==img2.cols){
			for(int i=0;i<img1.rows;i++) { 
				for(int j=0;j<img1.cols;j++) { 
					error+=pow(img1.at<float>(i,j)-img2.at<float>(i,j),2);
				}
			}
			error=sqrt(error/(img1.rows*img1.cols));
		}else{
			cout<<endl<<"TIENEN QUE TENER EL MISMO TAMANO"<<endl;
		}}
	return error;
}
void Ejercicio4(){
	Mat img=imread("huang2.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	Mat plano0(img.size(),img.type());
	Mat plano1(img.size(),img.type());
	Mat plano2(img.size(),img.type());
	Mat plano3(img.size(),img.type());
	Mat plano4(img.size(),img.type());
	Mat plano5(img.size(),img.type());
	Mat plano6(img.size(),img.type());
	Mat plano7(img.size(),img.type());
	for (int i=0;i<img.rows;i++){
		for (int j=0;j<img.cols;j++){
			string binary = std::bitset<8>(img.at<uchar>(i,j)).to_string();
			string p0, p1, p2, p3, p4, p5, p6, p7;
			p0=binary[7];
			p1=binary[6];
			p2=binary[5];
			p3=binary[4];
			p4=binary[3];
			p5=binary[2];
			p6=binary[1];
			p7=binary[0]; 
			plano0.at<uchar>(i,j)=atoi(p0.c_str());
			plano1.at<uchar>(i,j)=atoi(p1.c_str());
			plano2.at<uchar>(i,j)=atoi(p2.c_str());
			plano3.at<uchar>(i,j)=atoi(p3.c_str());
			plano4.at<uchar>(i,j)=atoi(p4.c_str());
			plano5.at<uchar>(i,j)=atoi(p5.c_str());
			plano6.at<uchar>(i,j)=atoi(p6.c_str());
			plano7.at<uchar>(i,j)=atoi(p7.c_str());
		}
	}
	Mat imgfinal(img.size(),img.type());
	for (int i=0;i<plano7.rows;i++){
		for (int j=0;j<plano7.cols;j++){
			imgfinal.at<uchar>(i,j)=plano7.at<uchar>(i,j)*pow(2,7)
				+plano6.at<uchar>(i,j)*pow(2,6);
//				+plano5.at<uchar>(i,j)*pow(2,5)
//				+plano4.at<uchar>(i,j)*pow(2,4)
//				+plano3.at<uchar>(i,j)*pow(2,3)
//				+plano2.at<uchar>(i,j)*pow(2,2)
//				+plano1.at<uchar>(i,j)*pow(2,1)
//				+plano0.at<uchar>(i,j)*pow(2,0);
		}
	}
	cout<<"El error cuadratico medio entre las dos imagenes es: "<<ECM(img,imgfinal);
	for (int i=0;i<plano0.rows;i++){
		for (int j=0;j<plano0.cols;j++){
			if (plano0.at<uchar>(i,j)==1) plano0.at<uchar>(i,j)=255;
			else plano0.at<uchar>(i,j)=0;
		}
	}
	for (int i=0;i<plano1.rows;i++){
		for (int j=0;j<plano1.cols;j++){
			if (plano1.at<uchar>(i,j)==1) plano1.at<uchar>(i,j)=255;
			else plano1.at<uchar>(i,j)=0;
		}
	}
	for (int i=0;i<plano2.rows;i++){
		for (int j=0;j<plano2.cols;j++){
			if (plano2.at<uchar>(i,j)==1) plano2.at<uchar>(i,j)=255;
			else plano2.at<uchar>(i,j)=0;
		}
	}
	for (int i=0;i<plano3.rows;i++){
		for (int j=0;j<plano3.cols;j++){
			if (plano3.at<uchar>(i,j)==1) plano3.at<uchar>(i,j)=255;
			else plano3.at<uchar>(i,j)=0;
		}
	}
	for (int i=0;i<plano4.rows;i++){
		for (int j=0;j<plano4.cols;j++){
			if (plano4.at<uchar>(i,j)==1) plano4.at<uchar>(i,j)=255;
			else plano4.at<uchar>(i,j)=0;
		}
	}
	for (int i=0;i<plano5.rows;i++){
		for (int j=0;j<plano5.cols;j++){
			if (plano5.at<uchar>(i,j)==1) plano5.at<uchar>(i,j)=255;
			else plano5.at<uchar>(i,j)=0;
		}
	}
	for (int i=0;i<plano6.rows;i++){
		for (int j=0;j<plano6.cols;j++){
			if (plano6.at<uchar>(i,j)==1) plano6.at<uchar>(i,j)=255;
			else plano6.at<uchar>(i,j)=0;
		}
	}
	for (int i=0;i<plano7.rows;i++){
		for (int j=0;j<plano7.cols;j++){
			if (plano7.at<uchar>(i,j)==1) plano7.at<uchar>(i,j)=255;
			else plano7.at<uchar>(i,j)=0;
		}
	}

	
	imshow("Imagen original",img);
	imshow("Plano 0",plano0);
	imshow("Plano 1",plano1);
	imshow("Plano 2",plano2);
	imshow("Plano 3",plano3);
	imshow("Plano 4",plano4);
	imshow("Plano 5",plano5);
	imshow("Plano 6",plano6);
	imshow("Plano 7",plano7);
	imshow("Suma de planos",imgfinal);
}

void Ejercicio5_2(){
	//PARTE GENERICA DEL EJERCICIO PARA VER DONDE PONER LAS MASCARAS
	//LA RESOLUCION DEL EJERCICIO ES EL VOID TIPO_PLACA(NOMBRE);
	Mat img=imread("a7v600-SE.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	Mat img2=imread("a7v600-X.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	
	Mat mascara1= Mat::zeros(img.size(), img.type());
	Mat mascara2= Mat::zeros(img2.size(), img2.type());
	Mat multiplicacion1= Mat::zeros(img.size(), img.type());    
	Mat multiplicacion2= Mat::zeros(img2.size(), img2.type());    
	//SI LA MASCARA BINARIA ES UN CIRCULO
	//SI LA MASCARA BINARIA ES UN RECTANGULO
	info(img);
	info(img2);
	
	rectangle(mascara1,Point(195,90),Point(245,155),Scalar(255, 0, 0),-1,8,0);
	rectangle(mascara2,Point(195,90),Point(245,155),Scalar(255, 0, 0),-1,8,0);
	//Now you can copy your source image to destination image with masking
	img.copyTo(multiplicacion1, mascara1);
	img2.copyTo(multiplicacion2, mascara2);
	cout<<"Modelo SE"<<endl;
	float SE=0.0;
	float X=0.0;
	for (int i=195;i<245;i++){
//		multiplicacion1.at<uchar>(110,i)=255;
		cout<<(int)multiplicacion1.at<uchar>(110,i)<<"-";
		SE+=(int)multiplicacion1.at<uchar>(110,i);
	}
	cout<<endl<<endl<<endl<<"Modelo X"<<endl;
	for (int i=195;i<245;i++){
//		multiplicacion2.at<uchar>(110,i)=255;
		cout<<(int)multiplicacion2.at<uchar>(110,i)<<"-";
		X+=(int)multiplicacion2.at<uchar>(110,i);
	}
	SE=SE/(245-195);
	X=X/(245-195);
	cout<<endl<<endl<<"Promedio de SE="<<SE<<endl<<"Promedio de X="<<X<<endl;
	//	imshow("Imagen",img);
//	Mat img3=imread("a7v600-SE(RImpulsivo).jpg",CV_LOAD_IMAGE_GRAYSCALE);
//	Mat img4=imread("a7v600-X(RImpulsivo).jpg",CV_LOAD_IMAGE_GRAYSCALE);
	imshow("SE",img);
	imshow("X",img2);
//	imshow("Mascara 1",mascara1);
//	imshow("Mascara 2",mascara2);
	imshow("SE-Mascara",multiplicacion1);
	imshow("X-Mascara",multiplicacion2);
//	imshow("SE-Ruido",img3);
//	imshow("X-Ruido",img4);
}

void tipo_placa(string nombre){
	Mat img=imread(nombre,CV_LOAD_IMAGE_GRAYSCALE);
	Mat mascara= Mat::zeros(img.size(), img.type());
	Mat multiplicacion= Mat::zeros(img.size(), img.type());    
	rectangle(mascara,Point(195,90),Point(245,155),Scalar(255, 0, 0),-1,8,0);
	img.copyTo(multiplicacion, mascara);
	float ruido=0.0;
	float promedio=0.0;
	for (int i=195;i<245;i++){
		promedio+=(int)multiplicacion.at<uchar>(110,i);
	}
	promedio=promedio/(245-195);
	int c=0;
	for (int i=0;i<15;i++){
		for (int j=0;j<40;j++){
			ruido+=img.at<uchar>(i,j);
			c++;
		}
	}
	ruido=ruido/c;
	cout<<promedio<<endl;
	if (promedio>50) cout<<"La placa es del modelo A7V600-SE"<<endl;
	else cout<<"La placa es del modelo A7V600-x"<<endl;
	cout<<ruido<<endl;
	if (ruido<253) cout<<"La imagen esta afectada por ruido"<<endl;
	else cout<<"La imagen no esta afectada por ruido"<<endl;
	
	imshow("Placa",img);
	
}

void Ejercicio5_3(string nombre){
	Mat img=imread(nombre,CV_LOAD_IMAGE_GRAYSCALE);
	Mat mascara= Mat::zeros(img.size(), img.type());
	Mat multiplicacion= Mat::zeros(img.size(), img.type());    
	//SI LA MASCARA BINARIA ES UN CIRCULO
	circle(mascara, Point(53,54), 16, Scalar(255, 0, 0), -1, 8, 0);
	circle(mascara, Point(102,53), 16, Scalar(255, 0, 0), -1, 8, 0);
	circle(mascara, Point(152,53), 16, Scalar(255, 0, 0), -1, 8, 0);
	circle(mascara, Point(202,53), 16, Scalar(255, 0, 0), -1, 8, 0);
	circle(mascara, Point(252,51), 16, Scalar(255, 0, 0), -1, 8, 0);
	
	circle(mascara, Point(53,101), 16, Scalar(255, 0, 0), -1, 8, 0);
	circle(mascara, Point(102,102), 16, Scalar(255, 0, 0), -1, 8, 0);
	circle(mascara, Point(152,101), 16, Scalar(255, 0, 0), -1, 8, 0);
	circle(mascara, Point(202,100), 16, Scalar(255, 0, 0), -1, 8, 0);
	circle(mascara, Point(252,100), 16, Scalar(255, 0, 0), -1, 8, 0);
	//SI LA MASCARA BINARIA ES UN RECTANGULO
	info(img);
	//	rectangle(mascara,Point(190,90),Point(250,155),Scalar(255, 0, 0),-1,8,0);
	//Now you can copy your source image to destination image with masking
	img.copyTo(multiplicacion, mascara);
	
	if ((int)img.at<uchar>(Point(53,54))<100) cout<<"El blister esta incompleto. Faltante en: "<<Point(53,54)<<endl;
	if ((int)img.at<uchar>(Point(102,53))<100) cout<<"El blister esta incompleto. Faltante en: "<<Point(102,53)<<endl;
	if ((int)img.at<uchar>(Point(152,53))<100) cout<<"El blister esta incompleto. Faltante en: "<<Point(152,53)<<endl;
	if ((int)img.at<uchar>(Point(202,53))<100) cout<<"El blister esta incompleto. Faltante en: "<<Point(202,53)<<endl;
	if ((int)img.at<uchar>(Point(252,51))<100) cout<<"El blister esta incompleto. Faltante en: "<<Point(252,51)<<endl;
	if ((int)img.at<uchar>(Point(53,101))<100) cout<<"El blister esta incompleto. Faltante en: "<<Point(53,101)<<endl;
	if((int)img.at<uchar>(Point(102,102))<100) cout<<"El blister esta incompleto. Faltante en: "<<Point(102,102)<<endl;
	if ((int)img.at<uchar>(Point(152,101))<100) cout<<"El blister esta incompleto. Faltante en: "<<Point(152,101)<<endl;
	if ((int)img.at<uchar>(Point(202,100))<100) cout<<"El blister esta incompleto. Faltante en: "<<Point(202,100)<<endl;
	if ((int)img.at<uchar>(Point(252,100))<100) cout<<"El blister esta incompleto. Faltante en: "<<Point(252,100)<<endl;

	
	
	//	imshow("Imagen",img);
	imshow("Mascara",mascara);
	imshow("Zona de interes",multiplicacion);
	imshow("Blister",img);
}


int main(int argc, char** argv) {
//	Ejercicio1(1,0); //Lineal
	//A MEDIDA QUE VOY AUMENTANDO LA VARIABLE INDEPENDIENTE ES DECIR EL C, EL 
	//GRAFICO DE MAPEO SE ENCUENTRA DESPLAZADO, DONDE TODO LO MAYOR A 255 ES 255
	//Y TODO LO MENOR A 0 ES 0. AHORA SI VARIO LA PENDIENTE, ES DECIR EL 'A',
	//EL GRAFICO DEL MAPEO ME VA A CAMBIAR LA RECTA DE ESTAR MAS INCLINADA O NO.
//	Ejercicio1(-1,255);
	//EJERCICIO 1.3
	//PARA IMPLEMENTAR EL NEGATIVO DE LA IMAGEN SIMPLEMENTE DEBO PONER EN NEGATIVO
	//LA PENDIENTE Y EN 255 LA VARIABLE INDEPENDIENTE (-1,255);
	//EJERCICIO 1.4
	//SI PRODUZCO UN ESTIRAMIENTO, ES DECIR LA PENDIENTE ESTARÀ ENTRE 0 Y 1, LA
	//IMAGEN ORIGINAL SE VA OSCURECIENDO. SI COMPRIMO, ES DECIR LA PENDIENTE CADA
	//VEZ MAYOR, SE IRA VIENDO CADA VEZ MÁS BRILLOSA LA IMAGEN A PUNTO DE QUEDAR
	//COMPLETAMENTE BLANCA
	//EJERCICIO 5.1 
	//APLICANDO CUALQUIERA DEL EJERCICIO 1 O 2 SE PUEDE VER LOS OBJETOS QUE NO SE
	//PERCIBEN A SIMPLE VISTA Ejercicio2(2,0.4) por ejemplo.
//	Ejercicio5_2();
//	tipo_placa("a7v600-X(RImpulsivo).jpg");
//	tipo_placa("a7v600-SE(RImpulsivo).jpg");
//	tipo_placa("a7v600-X.jpg");
//	tipo_placa("a7v600-SE.jpg");
	Ejercicio3_2();
//	Ejercicio4();
	//5.4 ESCONDER LA IMG EN EL PLANO MENOS SIGNIFICATIVO YA Q ES DE 0,1
	waitKey(0);
//	cout<<aux;
	return 0;
} 
