#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "pds_functions.h"
#include <cstdlib>
#include <opencv2/core/core.hpp>
#include <string>
#include <opencv2/gpu/gpu.hpp>
using namespace cv;
using namespace std;
using namespace pdi;
//usamos norm para calcular el error cuadratico, luego se hce al cuadrado y se divide por n* m
//tamaño d ela imagen

//pdi::centre
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
void HacerHistograma(Mat img,string nombreVentana){
	imshow( "Original", img );
	
	// Initialize parameters
	int histSize = 256;    // bin size
	float range[] = { 0, 255 };
	const float *ranges[] = { range };
	
	// Calculate histogram
	MatND hist;
	calcHist( &img, 1, 0, Mat(), hist, 1, &histSize, ranges, true, false );
	
	
	double total;
	total = img.rows * img.cols;
	
	// Plot the histogram
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound( (double) hist_w/histSize );
	
	Mat histImage( hist_h, hist_w, CV_8UC1, Scalar( 0,0,0) );
	normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	
	for( int i = 1; i < histSize; i++ )
	{
		line( histImage, Point( bin_w*(i-1), hist_h - cvRound(hist.at<float>(i-1)) ) ,
			 Point( bin_w*(i), hist_h - cvRound(hist.at<float>(i)) ),
			 Scalar( 255, 0, 0), 2, 8, 0  );
	}
	
	namedWindow( nombreVentana, 1 );    imshow( nombreVentana, histImage );
	
}
Mat PatronDeGris(int filas, int columnas){
	Mat res=Mat(filas,columnas,CV_8UC1);
	for(int i=0;i<filas;i++) { for(int j=0;j<columnas;j++) { res.at<uchar>(i,j)=40; } }

	line(res,Point(0,0),Point(filas,columnas),Scalar(200),60);
	line(res,Point(0,filas),Point(columnas,0),Scalar(240),60);
return res;
}

Mat MediaAritmetica(int n){
//	if (n%2==0) return NULL;
	Mat ker = (Mat_<float>(3,3) <<  1./9.,1./9.,1./9.,
			   1./9.,  1./9., 1./9.,
			   1./9., 1./9., 1./9.);
	Mat aux=Mat(n,n,ker.type());
	float nn=n*n;
	for(int i=0;i<n;i++) { 
		for(int j=0;j<n;j++) { 
			aux.at<float>(i,j)=1/(nn);
		}
	}
	return aux;
}
Mat MediaAlphaRecortado(Mat img, float alpha){
	Mat aux=img.clone();
	//alpha par!!!
	for(int i=1;i<img.rows-1;i++) { 
		for(int j=1;j<img.cols-1;j++) { 
			vector<uchar>v(10);
			
			// (i-1,j-1)   (i-1,j)  (i-1,j+1)
			// (i,j-1)     (i,j)    (i,j+1)
			// (i+1,j-1)   (i+1,j)  (i+1,j+1)
			v[0]=img.at<uchar>(i,j-1);v[3]=img.at<uchar>(i-1,j-1); v[6]=img.at<uchar>(i+1,j-1); 
			v[1]=img.at<uchar>(i,j); v[4]=img.at<uchar>(i-1,j); v[7]=img.at<uchar>(i+1,j);
			v[2]=img.at<uchar>(i,j+1);v[5]=img.at<uchar>(i-1,j+1);v[8]=img.at<uchar>(i+1,j+1);  
			sort(v.begin(),v.end());
			v.erase(v.end()-(alpha/2)-1,v.end());
			v.erase(v.begin(),v.begin()+(alpha/2));
	
		int sum=accumulate(v.begin(),v.end(),0);
			aux.at<uchar>(i,j)=sum/v.size();
				
			
		}
	}
	return aux;
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
Mat FiltroMediana(Mat img){
	Mat aux=img.clone();
	for(int i=1;i<img.rows-1;i++) { 
		for(int j=1;j<img.cols-1;j++) { 
			vector<uchar>v(10);
			
			// (i-1,j-1)   (i-1,j)  (i-1,j+1)
			// (i,j-1)     (i,j)    (i,j+1)
			// (i+1,j-1)   (i+1,j)  (i+1,j+1)
			v[0]=img.at<uchar>(i,j-1);v[3]=img.at<uchar>(i-1,j-1); v[6]=img.at<uchar>(i+1,j-1); 
			v[1]=img.at<uchar>(i,j); v[4]=img.at<uchar>(i-1,j); v[7]=img.at<uchar>(i+1,j);
			v[2]=img.at<uchar>(i,j+1);v[5]=img.at<uchar>(i-1,j+1);v[8]=img.at<uchar>(i+1,j+1);  
			sort(v.begin(),v.end());
			aux.at<uchar>(i,j)=v[5];
		}
	}
	return aux;
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

Mat FiltroPuntoMedio(Mat img){
	Mat aux=img.clone();
	for(int i=1;i<img.rows-1;i++) { 
		for(int j=1;j<img.cols-1;j++) { 
			vector<uchar>v(10);
			
			// (i-1,j-1)   (i-1,j)  (i-1,j+1)
			// (i,j-1)     (i,j)    (i,j+1)
			// (i+1,j-1)   (i+1,j)  (i+1,j+1)
			v[0]=img.at<uchar>(i,j-1);v[3]=img.at<uchar>(i-1,j-1); v[6]=img.at<uchar>(i+1,j-1); 
			v[1]=img.at<uchar>(i,j); v[4]=img.at<uchar>(i-1,j); v[7]=img.at<uchar>(i+1,j);
			v[2]=img.at<uchar>(i,j+1);v[5]=img.at<uchar>(i-1,j+1);v[8]=img.at<uchar>(i+1,j+1);  
			sort(v.begin(),v.end());
			
			aux.at<uchar>(i,j)=.5*(v[0]+v[7]);
		}
	}
	return aux;
	
}

void Ejercicio1(){
	//	Mat img=imread("cameraman.tif",CV_LOAD_IMAGE_GRAYSCALE);
	//	//////	img.convertTo(img,CV_32F,1./255);
	//	Mat asd=img.clone();
	//	
	//	asd=ensuciasSalyPimienta(asd,10.0/255,10.0/255);
	//	imshow("Sal y Pimienta",asd);
	//	HacerHistograma(asd,"histograma SalyPi");
	//	HacerHistograma(img,"histograma original");
	//	
	
	//	asd=EnsuciaGauss(asd,0,0.3*255);
	//	imshow("ensucia Gauss",asd);
	//	
	//	HacerHistograma(asd,"histograma SalyPi");
	//	HacerHistograma(img,"histograma original");
	//	
	//	
	//	Mat img=imread("casilla.tif",IMREAD_GRAYSCALE);
	//	Mat img=imread("camaleon.tif",IMREAD_GRAYSCALE);
	//	/freelance
	//	/upworks
	//	ejercicio 1.2
	///PREGUNTARRR ; NO DA LOS PALITOS GAUSEANOS
	Mat res=PatronDeGris(256,256);
	imshow("original",res);
	
	//		HacerHistograma(res,"limpita");
	
	Mat su=EnsuciaGauss(res,0,170);
	imshow("sucia",su);
	HacerHistograma(su,"sucia");
	
	
}


void Ejercicio2(){

	Mat img=imread("sangre.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	imshow("Limpia",img);
	Mat ss=img.clone();
	ss=EnsuciaGauss(ss,0,100);
	ss=ensuciasSalyPimienta(ss,.1,.1);
	imshow("sucia",ss);
	
	//// filtrando armónica
	Mat fma=MediaContraArmonica(ss,3,-1);//-3 por tener sal, pimienta positivo
	imshow("Filtrada Media Armónica ",fma);
	////filtrando geométrica
	Mat fmg=MediaGeometrica(ss,5);//aparecen puntos negros(la caga)
	imshow("Filtrada Media Geometrica",ss);//bueno para sal, malo para pimienta
	
	cout<<"ECM entre la imagen limpia y sucia: "<<ECM(img,ss)<<endl;
	cout<<"ECM entre la imagen limpia y FMA: "<<ECM(img,fma)<<endl;
	cout<<"ECM entre la imagen limpia y FMG: "<<ECM(img,fmg)<<endl;
}
void Ejercicio3(){
	Mat img=imread("sangre.jpg",CV_LOAD_IMAGE_GRAYSCALE);
//	Mat img=imread("cameraman.tif",CV_LOAD_IMAGE_GRAYSCALE);
	imshow("Limpia",img);
	Mat ss=img.clone();
	ss=EnsuciaGauss(ss,0,0.1*255);
	ss=ensuciasSalyPimienta(ss,.1,.1);
	imshow("sucia",ss);
//	Mat fMediana=FiltroMediana(img);
//	imshow("Filtrada",fMediana);

	
//	Mat fPmedio=FiltroPuntoMedio(img);
//	imshow("Filtrada",fPmedio);
	//con alpha = 8 da la mediana, y con alpha =0 la aritmética
	Mat alphaRecortao=MediaAlphaRecortado(ss,0);
	imshow("alpha Recortao",alphaRecortao);
	
}
void FuncionMouse(int event, int x, int y, int flags, void* userdata){
	if  ( event == EVENT_LBUTTONDOWN )//Boton izquierdo
		cout<<x<<" "<<y<<" ";
}
Mat FiltroNoitch(size_t rows, size_t cols, double corte){
cv::Mat
	magnitud = cv::Mat::zeros(rows, cols, CV_32F);

corte *= rows;
//corte *= corte;
for(size_t K=0; K<rows; ++K)
	for(size_t L=0; L<cols; ++L){
	double distance = distance2(K+.5, L+.5, rows/2., cols/2.);
	magnitud.at<float>(K,L) = std::exp(-distance/(2*corte*corte));
}
	
	centre(magnitud);
}
void Ejercicio4(){
	Mat img=imread("img_degradada.tif",CV_LOAD_IMAGE_GRAYSCALE);
//	imshow("Sucia",img);
	img.convertTo(img,CV_32F,1./255);
	Mat fft=spectrum(img);
	imshow("Transformada",fft);
//	setMouseCallback("Transformada",FuncionMouse, NULL);
//		Mat pasaAlto= filter_butterworth(img.rows,img.cols,.3,5);
//		Mat pasaBajo= 1- filter_butterworth(img.rows,img.cols,.45,5);
//		Mat pasaBanda=pasaAlto+pasaBajo;
//		imshow("Pasa ",pasaBanda);
		
	
	
	
}





int main(int argc, char** argv) {
Mat a=filter_gaussian(256,256,0.1);
imshow("asd",a);

//Ejercicio1();
//Ejercicio2();
//Ejercicio3();
//Ejercicio4();
waitKey(0);


	return 0;
} 
