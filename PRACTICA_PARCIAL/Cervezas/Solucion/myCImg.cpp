#include "myCImg.h"
#include <algorithm>
#include <set>
using namespace std;

void enfocarImagen( CImg<double> &img, double lim ) {
	int xmin=img.width(), xmax=0, ymin=img.height(), ymax=0;
	for( int j=0; j<img.height(); j++ ) { 
		for( int i=0; i<img.width(); i++ ) {
			if( img(i,j,0,0)+img(i,j,0,1)+img(i,j,0,2) < lim*3 ) {
				if( xmin > i ) xmin=i;
				if( xmax < i ) xmax=i;
				if( ymin > j ) ymin=j;
				if( ymax < j ) ymax=j;
			}
		}
	}
	img.crop( xmin,ymin,xmax,ymax );
}

CImg<double> Graficar( CImg<double> &img, int h ) {
	static const byte tc[] = { 255 };
	CImg<double> grafico( img.width(), h );
	grafico.fill(0);
	grafico.draw_graph( img, tc );
	return grafico;
}

void SepararCanales( CImg<double>& img, CImg<double>& img1, CImg<double>& img2, CImg<double>& img3 ) {
	img1 = CImg<double>( img.width(), img.height() );
	img2 = CImg<double>( img.width(), img.height() );
	img3 = CImg<double>( img.width(), img.height() );
	forpixels(img) {
		img1(i,j) = img(i,j,0,0);
		img2(i,j) = img(i,j,0,1);
		img3(i,j) = img(i,j,0,2);
		endfor();
	}
}

CImg<double> PerfilY( CImg<double>& img ) {
	CImg<double> perfil( img.height(), 1 );
	perfil.fill(0);
	forpixels( img ) {
		perfil(j) += img(i,j);
		endfor();
	}
	return perfil;
}

CImg<double> PerfilX( CImg<double>& img ) {
	CImg<double> perfil = CImg<double> ( img.width(), 1 );
	perfil.fill(0);
	forpixels( img ) {
		perfil(i) += img(i,j);
		endfor();
	}
	return perfil;
}

Pixel PosicionMinimo( CImg<double> img ) {
	int xmin, ymin;
	double minimo = 1000000;
	forpixels(img) {
		if( img(i,j) < minimo ) {
			minimo = img(i,j);
			xmin = i;
			ymin = j;
		}
		endfor();
	}
	return Pixel(xmin, ymin, &img);
}

CImg<bool> Inundar( CImg<double> img, double limite ) {
	double acumulador = 0;
	img.normalize(0.0, 1.0);
	limite *= ((double)img.width()*(double)img.height()) - img.sum();
	CImg<bool> mask(img.width(), img.height(), 1, 1, false );
	
	set<Pixel> cola;
	cola.insert( PosicionMinimo( img ) );
	
	while( acumulador < limite ) {
		//agrego el pixel de menor valor (el primero de la cola)
		Pixel p = (*cola.begin()); cola.erase( cola.begin() );
		acumulador += 1.0 - p.value();
		mask(p.x,p.y) = true;
		//Agrego los vecinos a la cola
		Pixel pix = p;
		pix.x++;
		if( pix.AdentroImagen() && !mask(pix.x, pix.y) ) cola.insert( pix );
		pix.x -= 2;
		if( pix.AdentroImagen() && !mask(pix.x, pix.y) ) cola.insert( pix );
		pix.x++; pix.y++;
		if( pix.AdentroImagen() && !mask(pix.x, pix.y) ) cola.insert( pix );
		pix.y -= 2;
		if( pix.AdentroImagen() && !mask(pix.x, pix.y) ) cola.insert( pix );
	}
	return mask;
}

CImg<bool> InundarInverso( CImg<double> img, double limite ) {
	return Inundar( img.max() - img, limite);
}

CImg<bool> Crecimiento( CImg<double> img, double limite ) {
	img.normalize(0.0, 1.0);
	CImg<bool> mask(img.width(), img.height(), 1, 1, false );
	CImg<bool> procesado(img.width(), img.height(), 1, 1, false );
	deque<Pixel> cola;
	cola.push_back( PosicionMinimo( img ) );
	procesado(cola.front().x, cola.front().y) = true;
	
	while( !cola.empty() ) {
		Pixel p = cola.front(); cola.pop_front();
		if( p.value() < limite ) {
			mask(p.x,p.y) = true;
			//agrego a los vecinos para procesarlos
			Pixel pix = p;
			pix.x++;
			if( pix.AdentroImagen() && !procesado(pix.x, pix.y) ) {
				cola.push_back( pix );
				procesado(pix.x, pix.y) = true;
			}
			pix.x -= 2;
			if( pix.AdentroImagen() && !procesado(pix.x, pix.y) ) {
				cola.push_back( pix );
				procesado(pix.x, pix.y) = true;
			}
			pix.x++; pix.y++;
			if( pix.AdentroImagen() && !procesado(pix.x, pix.y) ) {
				cola.push_back( pix );
				procesado(pix.x, pix.y) = true;
			}
			pix.y -= 2;
			if( pix.AdentroImagen() && !procesado(pix.x, pix.y) ) {
				cola.push_back( pix );
				procesado(pix.x, pix.y) = true;
			}
		}
	}
	return mask;
}

CImg<bool> CrecimientoInverso( CImg<double> img, double limite ) {
	return Crecimiento( img.max() - img, limite);
}

CImg<double> MedialAxis( CImg<double> img ) {
	bool hayCambios=true;
	CImg<double> copia = img;
	while( hayCambios ) {
		hayCambios = false;
		forpixels(img) {
			
			if(img(i,j) == 0) {
				double max = 0;
				for(int dx=-1; dx<=1; dx++) {
					for(int dy=-1; dy<=1; dy++) {
						if( abs(dx)+abs(dy)>1 ) continue;
						if(img(i+dx,j+dy) > max)
							max = img(i+dx,j+dy);
					}
				}
				if( max != 0 ) {
					hayCambios = true;
					copia(i,j) = max+1;
				}
			}
			endfor();
		}
		img = copia;
	}
	return img;
}

CImg<bool> MaximosLocales( CImg<double>& img, double limite ) {
	CImg<bool> maximo(img.width(), img.height(),1,1,true);
	forpixels(img) {
		for(int dx=-1; dx<=1 && maximo(i,j); dx++) {
			for(int dy=-1; dy<=1 && maximo(i,j); dy++) {
				if(dx==0 && dy==0) continue;
				if( i+dx < 0 || i+dx >= img.width() || j+dy < 0 || j+dy >= img.height() ) continue;
				maximo(i,j) = maximo(i,j) && ( img(i,j) >= img(i+dx,j+dy) ) && img(i,j) > limite; 
			}
		}
		endfor();
	}
	return maximo;
}


CImg<double> AlfaRecortado(CImg<double>& img, unsigned int mx, unsigned int my, unsigned int d) {
	int dx = (mx-1)/2;
	int dy = (my-1)/2;
	CImg<double> resultado( img.width(), img.height() );
	int upperLimit, lowerLimit, leftLimit, rightLimit;
	vector<double> vecino;
	forpixels( img ) {
		upperLimit = max( 0, j - dy );
		lowerLimit = min( img.height()-1, j + dy );
		leftLimit = max( 0, i - dx );
		rightLimit = min( img.width()-1, i + dx );
		vecino.clear();
		vecino.reserve( (lowerLimit-upperLimit+1)*(rightLimit-leftLimit+1) );
		for(int x=leftLimit; x<=rightLimit; x++) {
			for(int y=upperLimit; y<=lowerLimit; y++) {
				vecino.push_back( img(x,y) );
			}
		}
		sort( vecino.begin(), vecino.end() );
		for( int k=d; k<vecino.size()-d; k++ )
			resultado(i,j) += vecino[k];
		resultado(i,j) /= (double)(vecino.size()-2*d);
		endfor();
	}
	return resultado;
}
