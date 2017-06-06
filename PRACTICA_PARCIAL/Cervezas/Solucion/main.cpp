#include<iostream>
#include "myCImg.h"

int main (int argc, char *argv[]) {
	char filename[32];
	
	for( int i=1; i<12; i++ ) {
		sprintf(filename, "%i.jpg", i);
		CImg<double> img( filename );
		
		img.blur( 2 );
		enfocarImagen( img, 250 );
		
		CImg<double> hsi = img.get_RGBtoHSI();
		CImg<double> hsi1(img.width(), img.height());
		CImg<double> hsi2(img.width(), img.height());
		CImg<double> hsi3(img.width(), img.height());
		CImg<bool> mask;
		SepararCanales( hsi, hsi1, hsi2, hsi3 );
		
		double v1 = hsi2.mean();
		double v2 = sqrt(hsi2.variance(0));
		
		CImg<double> perfil;
		if( v1*2.5 + v2*2.5 < 1 ) {
			cout<<"La cerveza es Stout"<<endl;
			perfil = PerfilY( hsi3 );
			mask = Crecimiento( perfil, 0.5 );
		}
		else {
			cout<<"La cerveza es Lagger"<<endl;
			perfil = PerfilY( hsi2 );
			mask = CrecimientoInverso( perfil, 0.75 );
		}
		int inicio=0;
		while( inicio < mask.width() && !mask(inicio) ) inicio++;
		int fin = inicio;
		while( fin < mask.width() && mask(fin) ) fin++;
		
		int porcentajeEspuma = 100*((double)inicio)/((double)fin);
		cout<<"Inicio: "<<inicio<<"            Fin: "<<fin<<endl;
		cout<<"Porcentaje de espuma: % "<<porcentajeEspuma<<endl;
		cout<<"El liso esta "<<(( abs(porcentajeEspuma-20) <= 10 )? "Bien" : "Mal")<<" Servido"<<endl<<endl;
		
		img.draw_line(0, inicio, img.width(), inicio, Rojo);
		img.draw_line(0, fin, img.width(), fin, Verde);
		CImg<double> grafico = Graficar(perfil, img.height());
		grafico.draw_line(inicio, 0, inicio, grafico.height(), Gris1);
		grafico.draw_line(fin, 0, fin, grafico.height(), Gris2);
		
		CImgList<double> lista( img, grafico );
		lista.display("cerveza", false);
	}
	
	return 0;
}

