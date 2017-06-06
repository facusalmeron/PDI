#include "myCImg.h"

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

CImg<double> PerfilY( CImg<double>& img ) {
	CImg<double> perfil( img.height(), 1 );
	perfil.fill(0);
	cimg_forXY(img, i, j) {
		perfil(j) += img(i,j);
	}
	return perfil;
}

CImg<double> PerfilX( CImg<double>& img ) {
	CImg<double> perfil = CImg<double> ( img.width(), 1 );
	perfil.fill(0);
	cimg_forXY(img, i, j) {
		perfil(i) += img(i,j);
	}
	return perfil;
}

Pixel Minimo( CImg<double>& img ) {
	int xmin=0, ymin=0;
	double minimo = img(0,0);
	cimg_forXY(img, i, j) {
		if( img(i,j) < minimo ) {
			minimo = img(i,j);
			xmin = i;
			ymin = j;
		}
	}
	return Pixel(xmin, ymin, &img);
}

Pixel Maximo( CImg<double>& img ) {
	int xmax=0, ymax=0;
	double maximo = img(0,0);
	cimg_forXY(img, i, j) {
		if( img(i,j) > maximo ) {
			maximo = img(i,j);
			xmax = i;
			ymax = j;
		}
	}
	return Pixel(xmax, ymax, &img);
}

CImg<double> MedialAxis( CImg<double> img ) {
	bool hayCambios=true;
	CImg<double> copia = img;
	while( hayCambios ) {
		hayCambios = false;
		cimg_forXY(img,i,j) {
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
		}
		img = copia;
	}
	return img;
}
