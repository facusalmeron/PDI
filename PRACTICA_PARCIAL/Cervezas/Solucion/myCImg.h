#ifndef MYCIMG_H
#define MYCIMG_H
#include<CImg.h>

#include <vector>
#include <deque>
#include <algorithm>
#include <set>
#include <cassert>
#include <complex>
#include <limits>

using namespace std;
using namespace cimg_library;

#define byte unsigned char
#define intensidad(img,i,j) (img(i,j,0,0)+img(i,j,0,1)+img(i,j,0,2))/764
#define distancia( x1, y1, x2, y2) sqrt( pow(x1-x2,2) + pow(y1-y2,2) )

struct Pixel {
	int x;
	int y;
	CImg<double>* img;
	
	Pixel() : x(0), y(0), img(NULL) {};
	Pixel(const Pixel& p) : x(p.x), y(p.y), img(p.img) {};
	Pixel(int x, int y, CImg<double>* img) : x(x), y(y), img(img) {};
	
	bool operator<( const Pixel& p ) const {
		return this->value() < p.value();
	}
	double value() const {
		return img->operator()(x,y);
	}
	bool AdentroImagen( ) {
		return x>=0 && y>=0 && x<img->width() && y<img->height();
	}
};

// Colores
const byte Blanco[] = {255};
const byte Negro[] = {0};
const double Rojo[] = {255, 0, 0};
const double Verde[] = {0, 255, 0};
const double Azul[] = {0, 0, 255};
const double Amarillo[] = {255, 255, 0};

// Varios
CImg<double> MedialAxis( CImg<double> img );
void enfocarImagen( CImg<double> &img, double lim );
CImg<double> Graficar( CImg<double> &img, int h );
CImg<double> PerfilY( CImg<double>& img );
CImg<double> PerfilX( CImg<double>& img );
Pixel Minimo( CImg<double>& img );
Pixel Maximo( CImg<double>& img );

// Segmentacion por umbral
CImg<bool> SegmentacioColor(const CImg<double> &img, double R, double G, double B, double tol);

// Segmentacion de regiones
CImg<bool> MaximosLocales( CImg<double>& img, double limite=0 );
CImg<bool> Inundar( CImg<double> img, double limite );
CImg<bool> InundarInverso( CImg<double> img, double limite );
CImg<bool> Crecimiento( CImg<double> img, double limite );
CImg<bool> CrecimientoInverso( CImg<double> img, double limite );

// Segmentacion de bordes
CImg<double> LaplacianoGausiano(const CImg<double> &img, double sigma);
CImg<bool> CrucesCero(const CImg<double> &img, double lim, int size);
CImg<double> MagnitudGradiente(CImg<double> &img);
CImg<double> FaseGradiente(CImg<double> &img);
CImg<double> LineaHorizontal(CImg<double> &img);
CImg<double> LineaVertical(CImg<double> &img);
CImg<double> LineaDiagonal1(CImg<double> &img);
CImg<double> LineaDiagonal2(CImg<double> &img);

// Para eliminar ruido
CImg<double> AlfaRecortado(CImg<double>& img, int mx, int my, int d);
CImg<double> MediaGeometrica(const CImg<double> &img, int ancho_filtro = 3, int alto_filtro = 3);
CImg<double> ContraArmonica(const CImg<double> &img, int Q, int ancho_filtro = 3, int alto_filtro = 3);
CImg<double> RealceLocal(CImg<double> &img, double MG, double DG, double k0, double k1, double k2, double E, int size = 3);

// Transformada de Fourier
void Cartesianas2Polares(CImgList<double>& imagen);
void Polares2Cartesianas(CImgList<double>& imagen);
CImg<double> BonitaTF(CImgList<double> fft);
void VerTF(const CImg<double> &img, const char *title = "Sin nombre");

// Transformadas de Hough
CImg<double> TransformadaHough( CImg<bool>& img );
CImg<double> InversaHough(CImg<double> img, int nLineas);
CImg<double> TransformadaHoughCirculos(CImg<bool> &img, int r);

#endif
