#include "myCImg.h"

CImg<bool> Inundar( CImg<double> img, double limite ) {
	double acumulador = 0;
	img.normalize(0.0, 1.0);
	limite *= ((double)img.width()*(double)img.height()) - img.sum();
	CImg<bool> mask(img.width(), img.height(), 1, 1, false );
	
	set<Pixel> cola;
	cola.insert( Minimo( img ) );
	
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
	cola.push_back( Minimo( img ) );
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

CImg<bool> MaximosLocales( CImg<double>& img, double limite ) {
	CImg<bool> maximo(img.width(), img.height(),1,1,true);
	cimg_forXY(img,i,j) {
		for(int dx=-1; dx<=1 && maximo(i,j); dx++) {
			for(int dy=-1; dy<=1 && maximo(i,j); dy++) {
				if(dx==0 && dy==0) continue;
				if( i+dx < 0 || i+dx >= img.width() || j+dy < 0 || j+dy >= img.height() ) continue;
				maximo(i,j) = maximo(i,j) && ( img(i,j) >= img(i+dx,j+dy) ) && img(i,j) > limite; 
			}
		}
	}
	return maximo;
}
