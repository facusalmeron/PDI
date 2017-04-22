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

void onMouse( int event, int x, int y, int, void* );
//
Mat imagen = imread("Deforestacion.png");
//Mat imagen=aux(Rect(0,740,132,44));
void onMouse( int event, int x, int y, int, void* )
{
	if( event != CV_EVENT_LBUTTONDOWN )
		return;
	
	Point pt = Point(x,y);
	cout<<"x="<<pt.x<<"\t y="<<pt.y;
	Vec3b valores=imagen.at<cv::Vec3b>(y,x);
	cout<<"\t B="<<(int)valores.val[0]<<" G="<<(int)valores.val[1]<<" R="<<(int)valores.val[2]<<endl;
	
}
void Averiguar(){
	//EJERCICIO 2.1
	namedWindow("Averiguar Pixeles");
	setMouseCallback( "Averiguar Pixeles", onMouse, 0 );
	imshow("Averiguar Pixeles",imagen);
	
}

//usados para recortar una imagen a partir de otra
Rect cropRect(0, 0, 0, 0);
Point P1(0, 0);
Point P2(0, 0);

void Mouse(int event, int x, int y, int flags, void* userdata) { 
	int x_ini = 0; int y_ini = 0; int x_fin = 0; int y_fin = 0;
	
	switch (event) {
	case CV_EVENT_LBUTTONDOWN:
		P1.x = x;
		P1.y = y;
		P2.x = x;
		P2.y = y;
		cout << "Boton izquierdo presionado en las coord (x, y): " << x << " , " << y << endl;
		break;
	case CV_EVENT_LBUTTONUP:
		P2.x = x;
		P2.y = y;
		cout << "Boton izquierdo liberado en las coord (x,y): " << x << " , " << y << endl;
		break;
	default: break;
	}
}
void crop_mouse(string image) { //Funcion para armar un roi a traves de seleccion de ventana en imagen
	//	Utilizo la funcion "Mouse" definida arriba. Solo recortar desde arriba-izq hacia abajo-derecha
	Mat img = imread(image, 1);
	namedWindow("ImageDisplay", 1);
	setMouseCallback("ImageDisplay", Mouse, NULL);
	imshow("ImageDisplay", img);
	waitKey(10000);
	
	int ancho = P2.x - P1.x;
	int alto = P2.y - P1.y;
	Rect r_crop(P1.x, P1.y, ancho, alto);
	cout << P1.x << ", " << P2.x << ", " << P1.y << ", " << P2.y << endl;
	Mat img_crop = img(r_crop);
	namedWindow("cropped", 1);
	imshow("cropped", img_crop);
	waitKey(0);
	system("PAUSE");
	
}


void Ejercicio1_1(){
	Mat img_rgb=imread("patron.tif");
	//la img se forma a partir de 3 planos independientes uno para cada color primario.
	
	vector<Mat> bgr;
	split(img_rgb,bgr);
	Mat img_hsv;
	cvtColor(img_rgb, img_hsv, CV_BGR2HSV);
	vector<Mat> hsv;
	split(img_hsv,hsv);
	imshow("RGB",img_rgb);
	imshow("R",bgr[2]);
	imshow("G",bgr[1]);
	imshow("B",bgr[0]);
	//en este modelo los canales RGB brindan la informacion de los 3 planos independientes
	//de los colores primarios que forman la imagen.
	imshow("H",hsv[0]);
	imshow("S",hsv[1]);
	imshow("V",hsv[2]);
	//en este modelo HSV la H brinda informacion sobre el tono, la S sobre la saturacion
	//y la V sobre el valor (brillo?)
	
	//si quiero cambiar el patron es decir donde habia azul poner rojo y viceversa
	//simplemente debo cambiar la componente del tono (H) haciendo 120-H.
	hsv[0]=120-hsv[0]; //para cambiar matices de azul a rojo
	
//	imshow("H_m",hsv[0]);
	Mat img_hsvaux;
	Mat img_rgbaux;
	merge(hsv,img_hsvaux);
	cvtColor(img_hsvaux, img_rgbaux, CV_HSV2BGR);
	imshow("Variacion de azul a rojo",img_rgbaux); //variacion de matices de azul a rojo
	//En el otro punto que pide saturacion y brillo maximo. En cuanto a la saturacion,
	//ya se encuentra al maximo, ya que vemos en el plano S con valores de 255.
	
	hsv[2]=255; //Coloco el brillo al maximo
	merge(hsv,img_hsv);
	cvtColor(img_hsv, img_rgb, CV_HSV2BGR);
	imshow("Saturacion y brillo maximo",img_rgb); //Saturacion y brillo maximo
	waitKey(0);
}

void Ejercicio1_2(){
	Mat img_rgb=imread("rosas.jpg");
	Mat img_hsv;
	cvtColor(img_rgb,img_hsv,CV_BGR2HSV);
	vector <Mat> hsv;
	split(img_hsv,hsv);
	imshow("Imagen Original",img_rgb);
	hsv[0]=hsv[0]+90; //complemento del tono H -  Averiguar complemento
//	hsv[1]=255-hsv[1]; //complemento de la saturacion S
	hsv[2]=255-hsv[2]; //complemento del brillo V
//	cv::print(hsv[2]);
	merge(hsv,img_hsv);
	cvtColor(img_hsv,img_rgb,CV_HSV2BGR);
	imshow("Colores complementarios",img_rgb);
	waitKey(0);
}

void Ejercicio2(){
	Mat img=imread("rio.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	//Realizo el histograma
	Mat canvas(200,400,CV_32F);
	Mat histo=histogram(img,255);
	normalize(histo,histo,0,1,CV_MINMAX);
	draw_graph(canvas,histo);
	//con el histograma de la imagen, puedo ver la distribucion de grises que tiene
	//la misma. Para obtener el contenido del agua se supone que los niveles minimo
	//y maximo aproximadamente de los mismos son de 0 a 20 respectivamente.
	Mat img_color;
	cvtColor(img,img_color,COLOR_GRAY2BGR); //convierto img de gris a color para poder pintar luego en amarillo
	for (int i=0;i<img.rows;i++){
		for (int j=0;j<img.cols;j++){
			if (img.at<uchar>(i,j)<20){
				img_color.at<Vec3b>(i,j)[0]=0;
				img_color.at<Vec3b>(i,j)[1]=255;
				img_color.at<Vec3b>(i,j)[2]=255;
			}
		}
	}
	imshow("Imagen",img);
	imshow("Histograma",canvas);
	imshow("Imagen a color",img_color);
	
	waitKey(0);
}

void Ejercicio3_1(){
	Mat img=imread("Deforestacion.png");
//	Mat imgO=imread("chairs.jpg");
	//Primero genero histograma para los 3 planos de RGB y luego ecualizo.
	vector <Mat> bgr; 	
	split(img, bgr);
	equalizeHist(bgr[0],bgr[0]);
	equalizeHist(bgr[1],bgr[1]);
	equalizeHist(bgr[2],bgr[2]);
	Mat img_eqRGB;
	merge(bgr,img_eqRGB);
	
	//Ecualizo la imagen en HSV
	Mat img_HSV;
	cvtColor(img, img_HSV, CV_BGR2HSV);
	vector <Mat> hsv; 	
	split(img_HSV, hsv);
	equalizeHist(hsv[2],hsv[2]); //en estas imagenes solo basta ecualizar el canal V.
	merge(hsv,img_HSV);
	cvtColor(img_HSV, img_HSV, CV_HSV2BGR);
	//a simple vista la ecualizacion en RGB o HSV se ven iguales.
	//comparando con la imagen original "chairs.jpg" la ecualizacion parece darle
	//mayor brillo a la imagen, se nota sobre todo en el reflejo de la puerta del auto
	
	//Si pruebo con "flowers_oscura.tif" se nota la diferencia entre ecualizar en RGB y HSV
	//al ser una img con mayor contraste, dando mejores resultados la ecualizacion por HSV.
	imshow("Imagen Oscura",img);
//	imshow("Imagen Original",imgO);
	imshow("Imagen Ecualizada RGB",img_eqRGB);
	imshow("Imagen Ecualizada HSV",img_HSV);
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

void Ejercicio3_2(){
	Mat img=imread("camino.tif");
	//Primero filtro pasa altos para imagen en RGB
	vector <Mat> bgr; 	
	split(img, bgr);
	Mat kernel=filtro_pasa_alto_suma1(3);
	vector<Mat> aux;
	bgr[0]=convolve(bgr[0],kernel);
	bgr[1]=convolve(bgr[1],kernel);
	bgr[2]=convolve(bgr[2],kernel);
	Mat imgRGB;
	merge(bgr,imgRGB);
	
	//Ahora filtro pasa altos para imagen en HSV
	Mat img_HSV;
	cvtColor(img, img_HSV, CV_BGR2HSV);
	vector<Mat> hsv;
	split(img_HSV,hsv);
//	hsv[0]=convolve(hsv[0],kernel);
//	hsv[1]=convolve(hsv[1],kernel);
	hsv[2]=convolve(hsv[2],kernel);
	merge(hsv,img_HSV);
	cvtColor(img_HSV, img_HSV, CV_HSV2BGR);
	//el filtrado en HSV parece darle mayor claridad a la imagen y contraste que el RGB
	imshow("Imagen Original",img);
	imshow("Filtro Pasa Alto RGB",imgRGB);
	imshow("Filtro Pasa Alto HSV",img_HSV);
	waitKey(0);
}

void Ejercicio4_1(){
	Mat img=imread("futbol.jpg");
	//muestra representativa del color azul
	//PARA SABER PREVIAMENTE QUE PIXELES LLAMAR PARA HACER EL ROI, LLAMAR A LA FUNCION
	//AVERIGUAR() CAMBIANDO PREVIAMENTE LA IMAGEN DE INTERES, Y AHI SE SACA EL X,Y Q SE NECESITA.
	Mat roi=img(Rect(135,155,52,80));
	vector <Mat> bgr; 	
	split(roi, bgr);
	Mat canvasB(200,400,CV_32F);
	Mat canvasG(200,400,CV_32F);
	Mat canvasR(200,400,CV_32F);
	//Hago los histogramas para saber que region utilizar luego.
	Mat histoB=histogram(bgr[0],256,Mat());
	Mat histoG=histogram(bgr[1],256,Mat());
	Mat histoR=histogram(bgr[2],256,Mat());
	normalize(histoB,histoB,0,1,CV_MINMAX);
	normalize(histoG,histoG,0,1,CV_MINMAX);
	normalize(histoR,histoR,0,1,CV_MINMAX);
	draw_graph(canvasB,histoB);
	draw_graph(canvasG,histoG);
	draw_graph(canvasR,histoR);
	imshow("Imagen Original",img);	
//	cv::print(bgr[0]);
	Mat mascara= Mat::zeros(img.size(), img.type());
	Mat segmentacionRGB= Mat::zeros(img.size(), img.type());    
	inRange(img,Scalar(100,17,0),Scalar(212,113,60),mascara); //regiones aproximadas sacadas de los histogramas
	img.copyTo(segmentacionRGB, mascara);
	imshow("ROI",roi);
	imshow("Histograma B",canvasB);
	imshow("Histograma G",canvasG);
	imshow("Histograma R",canvasR);
	imshow("Segmentacion RGB",segmentacionRGB);
	waitKey(0);
}
void Ejercicio4_2(){
	Mat img=imread("futbol.jpg");
	//muestra representativa del color azul
	Mat roi=img(Rect(135,155,52,80));
	cvtColor(roi,roi,CV_BGR2HSV);
	vector <Mat> hsv; 	
	split(roi, hsv);
	Mat canvasH(200,400,CV_32F);
	Mat canvasS(200,400,CV_32F);
	//Hago los histogramas para saber que region utilizar luego.
	Mat histoH=histogram(hsv[0],256,Mat());
	Mat histoS=histogram(hsv[1],256,Mat());
	
	normalize(histoH,histoH,0,1,CV_MINMAX);
	normalize(histoS,histoS,0,1,CV_MINMAX);
	
	draw_graph(canvasH,histoH);
	draw_graph(canvasS,histoS);
	
	Mat mascara;
	Mat segmentacion= Mat::zeros(img.size(), img.type());    
	cvtColor(img,img,CV_BGR2HSV);
	
	cvtColor(roi,roi,CV_HSV2BGR);
	inRange(img,Scalar(100,170,0),Scalar(120,255,255),mascara);
	img.copyTo(segmentacion,mascara);
	cvtColor(img,img,CV_HSV2BGR);
	cvtColor(segmentacion,segmentacion,CV_HSV2BGR);
	
	imshow("Imagen Original",img);
	imshow("Histograma H",canvasH);
	imshow("Histograma S",canvasS);
	imshow("Roi",roi);
//	imshow("Mascara",mascara);
	imshow("Segmentacion HSV",segmentacion);
	waitKey(0);
}

void Rostros(){
	Mat img=imread("rostro10.png");
//	Mat roi=img(Rect(136,111,50,70)); //Rostro de cesar
	Mat roi=img(Rect(150,57,60,100)); //Rostro de Enrique
	cvtColor(roi,roi,CV_BGR2HSV);
	vector <Mat> hsv; 	
	split(roi, hsv);
	Mat canvasH(200,400,CV_32F);
	Mat canvasS(200,400,CV_32F);
	//Hago los histogramas para saber que region utilizar luego.
	Mat histoH=histogram(hsv[0],256,Mat());
	Mat histoS=histogram(hsv[1],256,Mat());
//	cv::print(hsv[0]);
	normalize(histoH,histoH,0,1,CV_MINMAX);
	normalize(histoS,histoS,0,1,CV_MINMAX);
	
	draw_graph(canvasH,histoH);
	draw_graph(canvasS,histoS);
	
	Mat mascara;
	Mat segmentacion= Mat::zeros(img.size(), img.type());    
	cvtColor(img,img,CV_BGR2HSV);
	
	cvtColor(roi,roi,CV_HSV2BGR);
//	inRange(img,Scalar(0,14,0),Scalar(45,75,255),mascara); //Rostro de Cesar
	inRange(img,Scalar(8,0,0),Scalar(30,100,255),mascara); //Rostro de Enrique
	img.copyTo(segmentacion,mascara);
	cvtColor(img,img,CV_HSV2BGR);
	cvtColor(segmentacion,segmentacion,CV_HSV2BGR);
	
	imshow("Imagen Original",img);
	imshow("Histograma H",canvasH);
	imshow("Histograma S",canvasS);
	imshow("Roi",roi);
	//	imshow("Mascara",mascara);
	imshow("Segmentacion HSV",segmentacion);
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

void Ejercicio5(){
	Mat img=imread("Deforestacion.png");
	//	Mat zona_escala(Rect(
	//	Mat roi=img(Rect(267,517,116,92)); //zona de interes, para ver escala colores
	Mat roi=img(Rect(169,600,33,86)); //zona de interes, para ver escala colores
	//uso dos ROI distintos para abarcar toda la zona, cuando abajo hago el inRange es de los datos obtenidos
	//con cada ROI, entonces comento uno y descomento el otro cuando lo obtengo.
	Mat zona_interes=img(Rect(152,276,577,428)); //zona de interes
	cvtColor(zona_interes,zona_interes,CV_BGR2HSV);
	vector<Mat> ver;
	split(zona_interes,ver);
	cvtColor(zona_interes,zona_interes,CV_HSV2BGR);
	cvtColor(roi,roi,CV_BGR2HSV);
	vector <Mat> hsv; 	
	split(roi, hsv);
	Mat canvasH(200,400,CV_32F);
	Mat canvasS(200,400,CV_32F);
	Mat canvasV(200,400,CV_32F);
	//Hago los histogramas para saber que region utilizar luego.
	Mat histoH=histogram(hsv[0],256,Mat());
	Mat histoS=histogram(hsv[1],256,Mat());
	Mat histoV=histogram(hsv[2],256,Mat());
	
	normalize(histoH,histoH,0,1,CV_MINMAX);
	normalize(histoS,histoS,0,1,CV_MINMAX);
	normalize(histoV,histoV,0,1,CV_MINMAX);
	
	draw_graph(canvasH,histoH);
	draw_graph(canvasS,histoS);
	draw_graph(canvasV,histoV);
	
	Mat mascara;
	Mat mascara2;
	Mat segmentacion= Mat::zeros(zona_interes.size(), zona_interes.type());    
	cvtColor(zona_interes,zona_interes,CV_BGR2HSV);
	
	cvtColor(roi,roi,CV_HSV2BGR);
	//Si graficamos el hsv[0], hsv[1] y hsv[2] vemos que el H y V no aportan nada a la imagen, ya que hay muchas
	//variaciones, mientras que el S si muestra bien la diferencia entre zonas deforestadas y sembradas, por eso
	//en el inRange solo uso los S que obtengo de los histogramas, ya que los otros no aportan nada.
	inRange(zona_interes,Scalar(0,22,0),Scalar(255,53,255),mascara); 
	inRange(zona_interes,Scalar(0,36,0),Scalar(255,59,255),mascara2); 
	//aplico filtros pasa bajos para eliminar los puntos que muestra arriba en la img que no son zonas deforestadas.
	Mat kernel=filtro_promediador(55);
	mascara=convolve(mascara,kernel);
	mascara2=convolve(mascara2,kernel);
	for (int i=0;i<mascara.rows;i++){
		for (int j=0;j<mascara.cols;j++){
			if ((int)mascara.at<uchar>(i,j)>150){mascara.at<uchar>(i,j)=255;}
			else{mascara.at<uchar>(i,j)=0;}
		}
	}
	for (int i=0;i<mascara2.rows;i++){
		for (int j=0;j<mascara2.cols;j++){
			if ((int)mascara2.at<uchar>(i,j)>150){mascara2.at<uchar>(i,j)=255;}
			else{mascara2.at<uchar>(i,j)=0;}
		}
	}
	
	zona_interes.copyTo(segmentacion,mascara);
	zona_interes.copyTo(segmentacion,mascara2);
	cvtColor(zona_interes,zona_interes,CV_HSV2BGR);
	cvtColor(segmentacion,segmentacion,CV_HSV2BGR);
	
	//una vez hecha la segmentacion, con estos for lo que hago es pintar de rojo todas las zonas deforestadas
	//hay algunos puntos de arriba que los pinta rojo por error, por tener el mism rango de color q el definido
	for (int i=0;i<zona_interes.rows;i++){
		for (int j=0;j<zona_interes.cols;j++){
			if ((segmentacion.at<Vec3b>(i,j)[0]!=0) && (segmentacion.at<Vec3b>(i,j)[1]!=0) && (segmentacion.at<Vec3b>(i,j)[2]!=0)){
				zona_interes.at<Vec3b>(i,j)[0]=0;
				zona_interes.at<Vec3b>(i,j)[1]=0;
				zona_interes.at<Vec3b>(i,j)[2]=255;
			}
		}
	}
	//HASTA ACA EL 5.A ("Segmente y resalte en algun tono de rojo el area deforestada.)
	
	//PARTE DEL EJERCICIO 5.B (Calcule el area total (hectareas) de la zona delimitada, el area de la zona que
	//tiene monte y el area de la zona deforestada.
	//Usando las escalas que aparecen en la parte inferior izquierda de la imagen:
	//Usando la funcion averiguar veo q la escala es de 6-99, entonces 93 pixeles de largo
	//y usando la funcion info(zona_interes) veo que la img es de 428x577)
	//Con regla de 3 obtengo que el area es de [920mx1240m]=1141591 metros cuadrados
	//o de otra forma viendo la img y haciendo zoom, determino que si 2,5cm son 200 m, entonces 12,3cm(alto) son 984m.
	//de la otra forma viendo que el ancho son 16.3cm, entonces es igual a 1304m.
	//por lo tanto el area total es de [984x1304]=1283136 metros cuadrados. que es aproximado a lo calculado con pixeles.
	double area_total=((428*200/93)*(577*200/93))/10000; //divido por 10mil para pasar a hectareas
	cout<<"El area total de la region de interes es de "<<area_total<<" hectareas"<<endl;
	
	int contador=0;
	//Cuento todos los pixeles en rojo, es decir las zonas deforestadas.
	for (int i=0;i<segmentacion.rows;i++){
		for (int j=0;j<segmentacion.cols;j++){
			if ((zona_interes.at<Vec3b>(i,j)[0]==0) && (zona_interes.at<Vec3b>(i,j)[1]==0) && (zona_interes.at<Vec3b>(i,j)[2]==255)){
				contador++;
			}
		}
	}
	//como se que la img es de 428x577=264266 pixeles en total, que corresponden al area de 1141591 metros cuadrados
	//entonces hago regla de 3 tambien y obtengo el area deforestada y el area que tiene monte la resto de la deforestada
	double area_deforestada=((contador*area_total)/(478*577));
	double area_monte=area_total-area_deforestada;
	cout<<"El area de la zona con montes es de "<<area_monte<<" hectareas"<<endl;
	cout<<"El area de la zona deforestada es de "<<area_deforestada<<" hectareas"<<endl;
	cout<<"Con lo obtenido podemos mencionar que el "<<area_deforestada*100/area_total<<"% esta deforestada del area total"<<endl;
	imshow("Imagen Original",zona_interes);
	imshow("Histograma H",canvasH);
	imshow("Histograma S",canvasS);
	imshow("Histograma V",canvasV);
	imshow("H",ver[0]);
	imshow("S",ver[1]);
	imshow("V",ver[2]);
	imshow("Segmentacion HSV",segmentacion);
	waitKey(0);
}

//Segun Omar se podia hacer un ROI de toda la region de interes, aca hay que implementarlo, el otro anda
//void Ejercicio5(){
//	Mat img=imread("Deforestacion.png");
//	Mat zona_interes=img(Rect(152,276,577,428)); //zona de interes
//	cvtColor(zona_interes,zona_interes,CV_BGR2HSV);
//	vector <Mat> hsv; 	
//	split(zona_interes, hsv);
//	Mat canvasH(200,400,CV_32F);
//	Mat canvasS(200,400,CV_32F);
//	Mat canvasV(200,400,CV_32F);
//	//Hago los histogramas para saber que region utilizar luego.
//	Mat histoH=histogram(hsv[0],256,Mat());
//	Mat histoS=histogram(hsv[1],256,Mat());
//	Mat histoV=histogram(hsv[2],256,Mat());
//	
//	normalize(histoH,histoH,0,1,CV_MINMAX);
//	normalize(histoS,histoS,0,1,CV_MINMAX);
//	normalize(histoV,histoV,0,1,CV_MINMAX);
//	imshow("H",hsv[0]);
//	imshow("S",hsv[1]);
//	imshow("V",hsv[2]);
//	for (int i=0;i<histoS.rows;i++){
//		cout<<i<<"-"<<histoS.at<float>(i)<<endl;
//	}
//	draw_graph(canvasH,histoH);
//	draw_graph(canvasS,histoS);
//	draw_graph(canvasV,histoV);
//	
//	cvtColor(zona_interes,zona_interes,CV_HSV2BGR);
//
//	imshow("Imagen Original",zona_interes);
//	imshow("Histograma H",canvasH);
//	imshow("Histograma S",canvasS);
//	imshow("Histograma V",canvasV);
//	waitKey(0);
//}	
int main(int argc, char** argv) {
//	Ejercicio1_1();
	
//	Ejercicio1_2();
	
//	Ejercicio2();
	
//	Ejercicio3_1();
	
//	Ejercicio3_2();
	
//	Ejercicio4_1();
	
//	Ejercicio4_2();
	
//	Rostros(); //Probando con el Roi y los rangos de enrique medianamente sirve para los 3 rostros, aunque no se ven muy bien.
	
	Ejercicio5();
	
//	Averiguar();
	waitKey(0);
	return 0;
} 
