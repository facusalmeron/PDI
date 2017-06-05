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
void Ejercicio3_1(){
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
void Ejercicio3_2(){
	Mat img=imread("Caracteres.jpeg",IMREAD_GRAYSCALE);
	img=Umbral(img,180);
	imshow("Original",img);
	
	
	Mat res;
//	//extrayendo letras
	Mat m=getStructuringElement(MORPH_RECT,Size(3,3),Point(2,2));
	Mostrar(m);
	erode(img,res,m);
	dilate(res,res,m);
	imshow("Letras",res);
	
	//extrayendo peques
	
	res=img-res;

	imshow("Simpboliyos",res);
	
	
}
void Ejercicio3_3(){
	Mat img=imread("estrellas.jpg",IMREAD_GRAYSCALE);
	imshow("Original",img);
	img=Umbral(img,100);
	imshow("Estrellas",img);
	Mat m=getStructuringElement(MORPH_ELLIPSE,Size(6,6),Point(2,2));
	Mostrar(m);
	Mat res;
	dilate(img,res,m);
	imshow("Dilatada",res);
	
	
	
}
void Ejercicio3_4(){
	Mat img=imread("lluviaEstrellas.jpg",IMREAD_GRAYSCALE);
	img=Umbral(img,100);
	imshow("Umbralizada",img);
	int n=10;
	Mat m=Mat::zeros(n,n,CV_8UC1);
	for(int i=0;i<n;i++) { m.at<uchar>(i,n-1-i)=1; }
	Mat res;
	dilate(img,res,m);
	Mostrar(m);

	imshow("Resultao",res);
}

void Ejercicio3_5(){//NO SALIOOOOOOOOOOOOO
	Mat img=imread("Globulos Rojos.jpg",IMREAD_GRAYSCALE);
	img=Umbral(img,100);
	imshow("Umbralizada",img);
	int n=10;//es cuadrada
	int rr=4;//es el recuadro
	Mat m=Mat::zeros(n,n,CV_8UC1);
	for(int i=0;i<16;i++) { m.at<uchar>((i/rr)+3,(i%rr)+3)=1; }
	Mat res;
	dilate(img,res,m);
	Mostrar(m);

	imshow("Resultao",res);
}
int Media(Mat img){
	int sum=0;
	for(int i=0;i<img.rows;i++) { for(int j=0;j<img.cols;j++) { sum+=int(img.at<uchar>(i,j)); } }
	return sum/(img.rows*img.cols);
}
void SacarDatos(Mat img,int &m0i, int &m0f,int &m1i, int &m1f,int &m2i, int &m2f   ){
	vector<Mat>x;
	split(img,x);
	int m0=Media(x[0]);
	int m1=Media(x[1]);
	int m2=Media(x[2]);
	int std0=0,std1=0,std2=0;
	for(int i=0;i<img.rows;i++) { 
		for(int j=0;j<img.cols;j++) { 
			std0+=pow((m0-x[0].at<uchar>(i,j)),2);
			std1+=pow((m1-x[1].at<uchar>(i,j)),2);
			std2+=pow((m2-x[2].at<uchar>(i,j)),2);
			
		}
	}
	int nn=img.rows*img.cols;
	std0=sqrt(std0/nn);
	std1=sqrt(std1/nn);
	std2=sqrt(std2/nn);
	m0i=m0-std0;m1i=m1-std1;m2i=m2-std2;
	m0f=std0+m0;m1f=std1+m1;m2f=std2+m2;
	//	cout<<"Esta entre "<<m0-std0<<" y  "<<std0+m0<<endl;
	//	cout<<"Esta entre "<<m1-std1<<" y  "<<std1+m1<<endl;
	//	cout<<"Esta entre "<<m2-std2<<" y  "<<std2+m2<<endl;
	
	
}
Mat MascaraPromediado(int n){
	///tiene que ser impar N!
	Mat kern=Mat(n,n,CV_32F);
	float nn=n*n;
	for(int i=0;i<n;i++) { 
		for(int j=0;j<n;j++) { 
			kern.at<float>(i,j)=(1./nn);
		}
	}
	return kern;
}

Mat SegmentarPorColor(Mat img, int x, int y, int ancho, int alto,
					   bool op,int tol0=0, int tol1=0, int tol2=0){
Mat res;
	Mat aux=Mat(img,Rect(x,y,ancho,alto));
	//		imshow("Muestra Del Color",aux);
	if (op){//en RGB
		//	Mat RGBimg;cvtColor(img,RGBimg,CV_HSV2RGB);
		//	Mat RGBaux;cvtColor(aux,RGBaux,CV_HSV2RGB);
		Mat RGBimg=img.clone();
		Mat RGBaux=aux.clone();
		
		int m0i,m0f,m1i,m1f,m2i,m2f;
		SacarDatos(RGBaux,m0i,m0f,m1i,m1f,m2i,m2f);
		
		Mat segmentado=Mat::zeros(RGBimg.size(),RGBimg.type());
		inRange(RGBimg,Scalar(m0i-tol0,m1i-tol1,m2i-tol2),Scalar(m0f+tol0,m1f+tol1,m2f+tol2),res);
		
//		RGBimg.copyTo(segmentado,RGBaux);
		
//		imshow("Segmentada RGB",segmentado);
		return res;
	}
	else {
		
		Mat HSVimg;cvtColor(img,HSVimg,CV_RGB2HSV);
		Mat HSVaux;cvtColor(aux,HSVaux,CV_RGB2HSV);
		
		int m0i,m0f,m1i,m1f,m2i,m2f;
		SacarDatos(HSVaux,m0i,m0f,m1i,m1f,m2i,m2f);
		
		Mat segmentado=Mat::zeros(HSVimg.size(),HSVimg.type());
		inRange(HSVimg,Scalar(m0i-tol0,m1i-tol0,0),Scalar(m0f+tol0,m1f+tol1,255),res);
		
		
//		HSVimg.copyTo(segmentado,HSVaux);
//		cvtColor(segmentado,segmentado,CV_HSV2RGB);
//		imshow("Segmentado HSV",segmentado);
		
return res;
	}
}
void FuncionMouse(int event, int x, int y, int flags, void* userdata){
	if  ( event == EVENT_LBUTTONDOWN )//Boton izquierdo
		cout<<x<<" "<<y<<" ";
}
void Ejercicio3_6(){//AHI NOMAS; NO SE TERMINO
		Mat img=imread("Rio.jpeg");
		imshow("Original",img);
//		setMouseCallback("Original",FuncionMouse,NULL);
//		(519,294)
		Mat mascara=SegmentarPorColor(img,519,294,10,10,0,15,15,15);
		mascara=convolve(mascara,MascaraPromediado(5));
		
	
		Mat m=getStructuringElement(MORPH_RECT,Size(10,10));
		erode(mascara,mascara,m);
		
		mascara=convolve(mascara,MascaraPromediado(5));
		
		imshow("Mascara",mascara);
	
}
int main(int argc, char** argv) {
//	Ejercicio1();
//	Ejercicio2();
//	Ejercicio3_1();
//	Ejercicio3_2();
//	Ejercicio3_3();
//	Ejercicio3_3();
//	Ejercicio3_5();//NO SALIOOOO
//	Ejercicio3_6();//AHI NOMAS; NO SE TERMINO
	
	
	
	
	waitKey(0);
	return 0;
} 
