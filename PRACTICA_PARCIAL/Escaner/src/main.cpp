#include<iostream>
#include "myCImg.h"

#define nLineas 150
#define pesoBlur 12
#define umbralTexto 0.7
#define umbralBinario 0.4

int main (int argc, char *argv[]) {
	char filename[32];
	
	for( int i=1; i<4; i++ ) {
		sprintf(filename, "%i.png", i);
		CImg<double> original( filename );
		
		// Primero elimino las imagenes de la pagina y otros objetos grandes que me puedan molestas
		// Utilizo solamente la intensidad de la imagen
		CImg<double> intensidad = ((original.get_channel(0)+original.get_channel(1)+original.get_channel(2))).normalize(0.0,1.0);
		// Utilizo un pasa bajos para eliminar el texto
		CImg<double> blur = intensidad.get_blur(pesoBlur);
		// Umbralizo la imagen para detectar donde no hay imagenes
		CImg<bool> textoMask = blur.get_threshold( umbralTexto );
		// En las regiones donde hay imagen ( textoMask es true ) coloco intensidad 1 (color de fondo)
		CImg<double> texto = intensidad;
		// Utilizo un umbral para detectar el texto
		CImg<bool> binarizada(texto.width(), texto.height(), 1, 1, false);
		
		cimg_forXY(texto,x,y) {
			if( !textoMask(x,y) )
				texto(x,y)=1;
			binarizada(x,y) = texto(x,y) < umbralBinario;
		}
		CImgList<double> lista1( original, intensidad, blur, textoMask, texto, binarizada);
		lista1.display("Eliminar Imagenes", false);
		
		// Para calcular el angulo calculo la transformada de Hough y la multiplico por una imagen para pesar los angulos
		// (Es mas probable que la imagen este girada unos pocos grados)
		CImg<double> hough = TransformadaHough( binarizada );
		CImg<double> peso(hough.width(),hough.height(),1,1,"1 / (1 + 2 * (((x/w)*2)-1) * (((x/w)*2)-1))",false);
		hough.mul( peso );
		
		CImgList<double> lista2( hough.get_div(peso), peso, hough, original-InversaHough( hough, nLineas ) );
		lista2.display("Calcular angulo - Hough", false);
		
		// Para calcular el angulo de giro de la imagen tomo los "nLineas" maximos de la imagen y los promedio 
		double angulo = 0;
		for(int i=0; i<nLineas; i++) {
			Pixel p = Maximo( hough );
			angulo += p.x;
			hough( p.x, p.y ) = 0;
		}
		angulo /= (double)nLineas;
		angulo -= (double)(hough.width()/2);
		angulo *= 90.0 / (double)(hough.width()/2);
		cout<<"Angulo de giro: "<<angulo<<endl;
		
		// Giro la imagen y aplico un poco de zoom
		int cx = original.width()/2;
		int cy = original.height()/2;
		float zoom = fabs(1 / cos( (M_PI/180.0)*angulo )) + 0.05;
		CImg<double> final = original.get_rotate(angulo, cx, cy, zoom, 1, 2);
		CImgList<double> lista3( original, final );
		lista3.display("Resultado", false);
	}
	
	return 0;
}

