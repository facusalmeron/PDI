#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/gpu/gpu.hpp>
#include "pds_functions.h"
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <vector>
#include <algorithm>

using namespace cv;
using namespace std;
using namespace pdi;

void FuncionMouse(int event, int x, int y, int flags, void* userdata)
{
		if  ( event == EVENT_LBUTTONDOWN )//Boton izquierdo
			cout<<x<<" "<<y<<endl;
	//	if ( event == EVENT_RBUTTONDOWN )//Boton Derecho
	//  if  ( event == EVENT_MBUTTONDOWN )//Boton del diome
	// if ( event == EVENT_MOUSEMOVE )//posicion del mouse dentro de la ventana
	
}

Mat ensuciaSalyPimienta(Mat img, float pa, float pb){
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
Mat BordePrewitt(Mat img,int op){
	Mat aux=img.clone();
	Mat ker;
	switch(op){
		
	case 1: {
	 ker = (Mat_<float>(3,3) <<  -1.,0.,1.,
			   -1,  0, 1,
			   -1., 0, 1.);break;}
	case 2: {
			ker = (Mat_<float>(3,3) <<  -1.,-1.,-1.,
					 			   0,  0, 0,
								   1., 1., 1.); break;}
	case 3:			
    {ker = (Mat_<float>(3,3) <<  0.,1.,1.,
	  		  					 -1,  0, 1.,
								   -1., -1., 0); break;}
	
	case 4: 
			{ker = (Mat_<float>(3,3) <<  -1.,-1.,0,
			   -1.,  0, 1.,
			   0, 1., 1.); break;}

	}
	aux=convolve(img,ker);
	
	return aux;
	
}
Mat BordeSobel(Mat img){
	Mat aux=img.clone();
	Mat ker = (Mat_<float>(3,3) <<  -1.,-2.,-1,
									0,  0, 0,
			                        1., 2., 1.);
//	Mat ker = (Mat_<float>(3,3) <<  1.,0,1,
//									-2,  0, 2.,
//			                        1., 0, 1);
//	Mat ker = (Mat_<float>(3,3) <<  0,1.,2,
//									-1,  0, 1,
//			                        -2, -1, 0);
//	Mat ker = (Mat_<float>(3,3) <<  -2.,-1.,0,
//									-1.,  0, 1,
//			                        0, 1, 2.);
	aux=convolve(img,ker);
	
	return aux;
	
}

Mat BordeRoberts(Mat img,int op){
	Mat aux=img.clone();
	Mat ker;
	if (op){
	ker = (Mat_<float>(3,3) <<  0.,0,0,
			   0,  -1., 0,
			   0, 0, 1);}
	else{
	ker = (Mat_<float>(3,3) <<  0.,0,0,
			   0,  0., -1.,
			   0, 1, 0);
	}
	aux=convolve(img,ker);
	
	return aux;
	
}

Mat BordeLaplaciano(Mat img, int op){
	Mat aux=img.clone();Mat ker;
if (op==1){
//	n4
	 ker = (Mat_<float>(3,3) <<  0.,-1,0,
			   -1,  4., -1,
			   0, -1, 0);}
else
//n8
	{ ker = (Mat_<float>(3,3) <<  -1.,-1,-1,
			   -1,  8., -1,
			   -1, -1, -1);}
	aux=convolve(img,ker);
	
	return aux;
	
}

Mat BordeLoG(Mat img){
	Mat aux=img.clone();
	Mat ker = (Mat_<float>(5,5) <<  0.,0,-1,0,0,
			   0,  -1., -1.,-1.,0,
			   -1, -2, 16.,-2,-1,
			   0,-1.,-2.,-1.,0,
			   0,0,-1.,0,0);
	aux=convolve(img,ker);
	
	return aux;
	
}
void DeteccionDeBordes(Mat img){
	
	Mat res,res2,res3,res4;
	///robert|
		res=BordeRoberts(img,1);//el uno o cero dan exactamente lo mismo
	//	res2=BordeRoberts(img,0);
		imshow("Borede Roberts A",res);
	//	imshow("Borede Roberts B",res2);
	
	
	
	///prewitt	
		res=BordePrewitt(img,1);//1 vertital 2 horizontal 3 y 4 diagonales
	//	res2=BordePrewitt(img,2);
	//	res3=BordePrewitt(img,3);
	//	res4=BordePrewitt(img,4);
	//	
		imshow("Borde prewitt a",res);
	//	imshow("Borde prewitt b",res2);
	//	imshow("Borde prewitt c",res3);
	//	imshow("Borde prewitt d",res4);
	
	
	///laplaciano
		res=BordeLaplaciano(img,0);//1 n4 y  0 n8
		imshow("Borde laplaciano n4", res);
//		res2=BordeLaplaciano(img,1);//1 n4
//		imshow("Borde laplaciano n8", res2);
	
	
	///Gaussiano
		res=BordeLoG(img);
		imshow("Borde Log",res);
	
}
Mat Umbralizar(uchar umbral,Mat img){
	Mat res=Mat::zeros(img.rows,img.cols,CV_8UC1);
//	Mat res=binaryMat(img.rows,img.cols,CV_8UC1);
	for(int i=0;i<img.rows;i++) { 
		for(int j=0;j<img.cols;j++) { 
			if(img.at<uchar>(i,j)>umbral)
			{res.at<uchar>(i,j)=255;}
			
				
		}
	}
	return res;
}

void Ejercicio1(){
//	Mat img=imread("estanbul.tif",CV_LOAD_IMAGE_GRAYSCALE);
//	DeteccionDeBordes(img);
//Mat img=imread("mosquito.jpg",CV_LOAD_IMAGE_GRAYSCALE);
//imshow("Original",img);
//Mat sg=EnsuciaGauss(img,0,110);
//imshow("Sucia gauss",sg);
//DeteccionDeBordes(img);
//DeteccionDeBordes(sg);

}
void Ejercicio2(){
	Mat img=imread("letras1.tif",CV_LOAD_IMAGE_GRAYSCALE);
//	Mat res=BordeRoberts(img,1);
//	Mat res2=Umbralizar(50,res);Mat src=img.clone();
//	imshow("Binaria",res2);
	
	Mat src=img.clone();
	Mat dst, cdst;
	
	Canny(src, dst, 50, 200, 3);
	imshow("asdasd",dst);
	cvtColor(dst, cdst, CV_GRAY2BGR);
	
	vector<Vec2f> lines;

	HoughLines(dst, lines, 1, CV_PI/180, 50, 0, 0 );
	cout<<lines.size();
	
	for( size_t i = 0; i < lines.size(); i++ )
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000*(-b));
		pt1.y = cvRound(y0 + 1000*(a));
		pt2.x = cvRound(x0 - 1000*(-b));
		pt2.y = cvRound(y0 - 1000*(a));
		line( cdst, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
	}
	imshow("source", src);
	imshow("detected lines", cdst);
//	imshow("Transformaa",res2);

}




void Ejercicio3(){
	Mat img=imread("bone.tif",CV_LOAD_IMAGE_GRAYSCALE);
	int x=128,y=128;
	
	imshow("Original",img);
	Mat asd=img.clone();
	floodFill(asd,Point(x,y),Scalar(255),0,Scalar(10),Scalar(100));
	
//	img=img-asd;
	
//	setMouseCallback("Original",FuncionMouse, NULL);
	
	
//	imshow("Original",img);
	imshow("Unundaa",asd);


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

void SegmentarPorColor(Mat img, int x, int y, int ancho, int alto,
					   bool op,int tol0=0, int tol1=0, int tol2=0){
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
		inRange(RGBimg,Scalar(m0i-tol0,m1i-tol1,m2i-tol2),Scalar(m0f+tol0,m1f+tol1,m2f+tol2),RGBaux);

		RGBimg.copyTo(segmentado,RGBaux);
		
		imshow("Segmentada RGB",segmentado);
		
	}
	else {
		
		Mat HSVimg;cvtColor(img,HSVimg,CV_RGB2HSV);
		Mat HSVaux;cvtColor(aux,HSVaux,CV_RGB2HSV);
		
		int m0i,m0f,m1i,m1f,m2i,m2f;
		SacarDatos(HSVaux,m0i,m0f,m1i,m1f,m2i,m2f);
		
		Mat segmentado=Mat::zeros(HSVimg.size(),HSVimg.type());
		inRange(HSVimg,Scalar(m0i-tol0,m1i-tol0,0),Scalar(m0f+tol0,m1f+tol1,255),HSVaux);
		
		
		HSVimg.copyTo(segmentado,HSVaux);
		cvtColor(segmentado,segmentado,CV_HSV2RGB);
		imshow("Segmentado HSV",segmentado);
		
	vector<vector<Point> > contornos;
		vector<Vec4i> hierarchy;
	findContours(HSVaux,contornos,hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	cout<<"Las rosas son: "<<hierarchy.size();///ME CUENTA UNA MAS
	}
}
Mat MascaraPromediado(int n){
	///tiene que ser impar N!
	Mat ker = (Mat_<float>(3,3) <<  1./9.,1./9.,1./9.,
			   1./9.,  1./9., 1./9.,
			   1./9., 1./9., 1./9.);
	Mat kern=Mat(n,n,CV_32F);
	float nn=n*n;
	for(int i=0;i<n;i++) { 
		for(int j=0;j<n;j++) { 
			kern.at<float>(i,j)=(1./nn);
		}
	}
	return kern;
}

void Ejercicio4(){
	Mat img=imread("rosas.jpg");
	imshow("Original",img);
//	setMouseCallback("Original",FuncionMouse, NULL);=> (194,148)
//		SegmentarPorColor(img,194,148,10,10,1,20,20,10);//Rosas Rojas
	img=convolve(img,MascaraPromediado(11));
		SegmentarPorColor(img,194,148,10,10,0,20,20,0);//Rosas Rojas
	
	
}
int main(int argc, char** argv) {
//	Ejercicio2();
	
//	Ejercicio3();
	Ejercicio4();

	
	
	/// Convert image to gray and blur it
	//	cvtColor( img, src_gray, CV_BGR2GRAY );
	//	blur( src_gray, src_gray, Size(3,3) );
////	Mat canny_output;
////	vector<vector<Point> > contours;
////	vector<Vec4i> hierarchy;
////	RNG rng(12345);
////	
////	/// Detect edges using canny
////	Canny( src_gray, canny_output, 100,200, 3 );
////	/// Find contours
////	findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
////	
////	/// Draw contours
////	Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
////	for( int i = 0; i< contours.size(); i++ )
////	{
////		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
////		drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
////	}
////	
////	
////	
////	namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
////	imshow( "Contours", drawing );
////	
	waitKey(0);
	return 0;
} 
