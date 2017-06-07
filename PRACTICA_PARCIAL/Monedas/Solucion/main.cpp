#include<iostream>
#include "myCImg.h"

const int nTiposMonedas = 8;
const double tamanioMoneda[] = { 34.2, 37.8, 43.0, 39.6, 44.1, 47.7, 45.9, 50.2 };
const char* nombreMoneda[] = { "1 Centavo", "2 Centavos", "5 Centavos", "10 Centavos", "20 Centavos", "50 Centavos", "1 Euro", "2 Euros"};

int main (int argc, char *argv[]) {
	char filename[32];
	for( int i=1; i<3; i++ ) {
		sprintf(filename, "%i.jpg", i);
		CImg<double> img( filename );
		img.blur(2);
		CImg<double> img2( filename );
		CImg<double> mask(img.width(), img.height(), 1, 1, 0.0);
		CImg<double> medial(img.width(), img.height(), 1, 1, 0.0);
		CImg<double> maximos(img.width(), img.height(), 1, 1, 0.0);
		CImg<double> maximos2(img.width(), img.height(), 1, 1, 0.0);
		cimg_forXY(img, i, j) {
			if( intensidad(img,i,j) > 0.99 )
				mask(i,j) = 1.0;
		}
		medial = MedialAxis(mask);
		maximos = MaximosLocales( medial, 25 );
		
		vector<double> moneda;
		maximos2 = maximos;
		cimg_forXY(maximos, i, j) {
			if( maximos(i,j) > 0.0 ) {
				moneda.push_back( medial(i,j)*0.6 + medial(i,j)*0.1 + medial(i,j)*0.1 + medial(i,j)*0.1);
				
//				char buf[32];
//				sprintf( buf, "%f", medial(i,j)*0.6 + medial(i,j)*0.1 + medial(i,j)*0.1 + medial(i,j)*0.1);
//				img2.draw_text( i, j, buf, Rojo, Verde, 0.8, 13);
				
				for(int dx=-20; dx<=20; dx++) {
					if( i+dx < 0 || i+dx >= maximos.width() ) continue;
					for(int dy=-20; dy<=20; dy++) {
						if( j+dy < 0 || j+dy >= maximos.height() ) continue;
						maximos(i+dx,j+dy) = 0.0;
					}
				}
			}
		}
		
		vector<int> nMonedas( nTiposMonedas, 0 );
		for( int j=0; j<moneda.size(); j++ ) {
			double minDist = 100000;
			int masCercano;
			for( int i=0; i<nTiposMonedas; i++) {
				double dist = (( moneda[j] > tamanioMoneda[i])? moneda[j] - tamanioMoneda[i] : tamanioMoneda[i] -  moneda[j]);
				if( dist < minDist ) {
					minDist = dist;
					masCercano = i;
				}
			}
			nMonedas[masCercano]++;
		}
		
		cout<<"Se encontraron "<<moneda.size()<<" monedas"<<endl;	
		for( int i=0; i<nTiposMonedas; i++) {
			cout<<"  -> "<<nombreMoneda[i]<<" ........................ "<<nMonedas[i]<<endl;
		}
		
		CImgList<double> lista( img2, mask, medial, maximos2 );
		lista.display("Monedas", false);
		
	}
	
	return 0;
}

