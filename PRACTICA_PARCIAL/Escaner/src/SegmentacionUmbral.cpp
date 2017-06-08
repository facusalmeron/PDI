#include "myCImg.h"

CImg<bool> SegmentacioColor(const CImg<double> &img, double R, double G, double B, double tol) {
	CImg<bool> res(img.width(), img.height(), 1, 1, false);
	tol *= tol;
	cimg_forXY(img, x, y) {
		res(x, y) = ((pow(img(x, y, 0, 1) - R, 2) + pow(img(x, y, 0, 1) - G, 2) + pow(img(x, y, 0, 2) - B, 2)) < tol); 
	}
	return res;
}
