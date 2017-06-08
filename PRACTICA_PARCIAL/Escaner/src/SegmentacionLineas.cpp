#include "myCImg.h"

CImg<double> LaplacianoGausiano(const CImg<double> &img, double sigma) {
	int size = sigma*8, ini = -(size-1)/2, fin = (size-1)/2;
	size -= size%2;
	double r2;
	CImg<double> mask(size, size, 1, 1, 0.0);
	for (int x=ini; x<fin+1; ++x){
		for (int y=ini; y<fin+1; ++y){
			r2 = x*x + y*y;
			mask(x-ini, y-ini) = -((r2 - sigma)/(sigma*sigma)) * exp( -r2 / (2*sigma) );
		}
	}
	mask -= (double(mask.sum())/double(size*size));
	return img.get_convolve(mask);
}

CImg<bool> CrucesCero(const CImg<double> &img, double lim, int size) {
	size /= 2;
	bool positivo, negativo;
	CImg<bool> res( img.width(), img.height() );
	cimg_forXY(img, x, y) {
		positivo = false;
		negativo = false;
		for(int dx=-size; dx<=size; dx++) {
			for(int dy=-size; dy<=size; dy++) {
				if( x+dx < 0 || x+dx >= img.width() || y+dy < 0 || y+dy >= img.height() ) 
					continue;
				positivo = positivo || (img(x+dx, y+dy) > lim);
				negativo = negativo || (img(x+dx, y+dy) < -lim);
			}
		}
		res(x,y) = positivo && negativo;
	}
	return res;
}

CImg<double> MagnitudGradiente(CImg<double> &img) {
	CImgList<double> g = img.get_gradient();
	g[0].pow(2); 
	g[1].pow(2);
	return (g[0]+g[1]).sqrt();
}

CImg<double> FaseGradiente(CImg<double> &img) {
	CImgList<double> g = img.get_gradient();
	return g[1].atan2(g[0]);
}


CImg<double> LineaHorizontal(CImg<double> &img) {
	static double data[] = {-1, -1, -1, 2, 2, 2, -1, -1, -1};
	static CImg<double> m(data, 3, 3, 1, 1, true);
	return img.get_convolve(m);
}

CImg<double> LineaVertical(CImg<double> &img) {
	static double data[] = {-1, 2, -1, -1, 2, -1, -1, 2, -1};
	static CImg<double> m(data, 3, 3, 1, 1, true);
	return img.get_convolve(m);
}


CImg<double> LineaDiagonal1(CImg<double> &img) {
	static double data[] = {-1, -1, 2, -1, 2, -1, 2, -1, -1};
	static CImg<double> m(data, 3, 3, 1, 1, true);
	return img.get_convolve(m);
}


CImg<double> LineaDiagonal2(CImg<double> &img) {
	static double data[] = {2, -1, -1, -1, 2, -1, -1, -1, 2};
	static CImg<double> m(data, 3, 3, 1, 1, true);
	return img.get_convolve(m);
}

