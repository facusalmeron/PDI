// The contents of this file are in the public domain. See LICENSE_FOR_EXAMPLE_PROGRAMS.txt
/*

    This example program shows how to find frontal human faces in an image and
    estimate their pose.  The pose takes the form of 68 landmarks.  These are
    points on the face such as the corners of the mouth, along the eyebrows, on
    the eyes, and so forth.  
    


    This face detector is made using the classic Histogram of Oriented
    Gradients (HOG) feature combined with a linear classifier, an image pyramid,
    and sliding window detection scheme.  The pose estimator was created by
    using dlib's implementation of the paper:
        One Millisecond Face Alignment with an Ensemble of Regression Trees by
        Vahid Kazemi and Josephine Sullivan, CVPR 2014
    and was trained on the iBUG 300-W face landmark dataset.  

    Also, note that you can train your own models using dlib's machine learning
    tools.  See train_shape_predictor_ex.cpp to see an example.

    


    Finally, note that the face detector is fastest when compiled with at least
    SSE2 instructions enabled.  So if you are using a PC with an Intel or AMD
    chip then you should enable at least SSE2 instructions.  If you are using
    cmake to compile this program you can enable them by using one of the
    following commands when you create the build project:
        cmake path_to_dlib_root/examples -DUSE_SSE2_INSTRUCTIONS=ON
        cmake path_to_dlib_root/examples -DUSE_SSE4_INSTRUCTIONS=ON
        cmake path_to_dlib_root/examples -DUSE_AVX_INSTRUCTIONS=ON
    This will set the appropriate compiler options for GCC, clang, Visual
    Studio, or the Intel compiler.  If you are using another compiler then you
    need to consult your compiler's manual to determine how to enable these
    instructions.  Note that AVX is the fastest but requires a CPU from at least
    2011.  SSE4 is the next fastest and is supported by most current machines.  
*/


#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <iostream>

using namespace dlib;
using namespace std;

// ----------------------------------------------------------------------------------------

int main(int argc, char** argv)
{  
    try
    {
        // This example takes in a shape model file and then a list of images to
        // process.  We will take these filenames in as command line arguments.
        // Dlib comes with example images in the examples/faces folder so give
        // those as arguments to this program.
        if (argc == 1)
        {
            cout << "Call this program like this:" << endl;
            cout << "./face_landmark_detection_ex shape_predictor_68_face_landmarks.dat faces/*.jpg" << endl;
            cout << "\nYou can get the shape_predictor_68_face_landmarks.dat file from:\n";
            cout << "http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << endl;
            return 0;
        }

        // We need a face detector.  We will use this to get bounding boxes for
        // each face in an image.

	// Necesitamos un detector de caras. Entonces usaremos esto para obtener el bounding box de cada cara en la imagen.
	
	//La funcion get_frontal_face_detector devuelve un object_detector (del tipo frontal_face_detector), o sea los rostros frontales.
        frontal_face_detector detector = get_frontal_face_detector();  //detector tiene las caras frontales en la imagen

        // And we also need a shape_predictor.  This is the tool that will predict face
        // landmark positions given an image and face bounding box.  Here we are just
        // loading the model from the shape_predictor_68_face_landmarks.dat file you gave
        // as a command line argument.

	// Tambien necesitamos el shape_predictor, este es el que va a encontrar los puntos de interes (landmarks) 
	// en la cara y delimitarlos en un bounding box. Aca solo cargamos el modelo desde el archivo .dat que se pasa como parametro.

	//shape_predictor toma una region de img que contiene algun objeto y emite un conjunto de puntos. El ejemplo es el de tomar
	//una cara humana y se identifican los landmarks en la boca, ojos, nariz, etc.
        shape_predictor sp; //sp va a contener entonces los puntos de interes de la cara.
        deserialize(argv[1]) >> sp;


        image_window win, win_faces; //creo dos ventanas para mostrar las imagenes.

        // Loop over all the images provided on the command line.

	// El for hace un loop sobre todas las imagenes ingresadas en el comando y a cada una busca las caras y calcula los ptos de interes.

        for (int i = 2; i < argc; ++i)
        {
            cout << "processing image " << argv[i] << endl; //Escribo el nombre de la imagen que se procesa.
            array2d<rgb_pixel> img; //forma de definir una imagen en dlib del tipo RGB
            load_image(img, argv[i]); //a img le cargo la imagen que le paso cuando ejecuto el comando en la terminal.
		//esta funcion toma un nombre de archivo mira su extension y la carga en una matriz2d (array2d)
            
	// Make the image larger so we can detect small faces.
	// Agranda la imagen para poder detectar las caras mas pequeñas.

            pyramid_up(img); 

        // Now tell the face detector to give us a list of bounding boxes around all the faces in the image.
	// Ahora el detector de caras nos da una lista de bounding box de cada una de las imagenes de la cara.

            std::vector<rectangle> dets = detector(img); //coloco en bounding box (rectangulos) las caras encontradas.

            cout << "Number of faces detected: " << dets.size() << endl; //Escribo el numero total de caras detectadas.

        // Now we will go ask the shape_predictor to tell us the pose of each face we detected.
	// Ahora shape_predictor nos va a decir la postura de cada cara que detectamos.

            std::vector<full_object_detection> shapes; //el tipo full_object_detection es un objeto que representa la ubicacion
		//de un objeto en una imagen junto con las posiciones de cada una de sus partes constitutivas.

            for (unsigned long j = 0; j < dets.size(); ++j) //recorro por cada cara detectada (dets.size)
            {
                full_object_detection shape = sp(img, dets[j]); //creo un nuevo objeto full_object_detection llamado shape que
			//es el que contiene los puntos de interes en la cara, por eso para acceder a cada uno de ellos debo hacerlos
			//usando el shape.part(punto a acceder)(0 es el x, 1 es el y).

                cout << "number of parts: "<< shape.num_parts() << endl; //Devuelvo el numero de partes, que deberia de ser 68 (landmarks)

                cout << "pixel position of first part:  " << shape.part(0) << endl; //Posicion del primer (0) punto de interes 
			//que es el de mas a la izquierda, cerca de la oreja (contorno).
 
                cout << "pixel position of second part: " << shape.part(1) << endl; //Posicion del segundo (1) punto de interes
			//que es a la izquierda abajo del primero, por lo que en x deberia ser parecido y en Y ser mayor q el anterior.

                // You get the idea, you can get all the face part locations if you want them.  Here we just store 
                // them in shapes so we can put them on the screen.
		//Esto lo hace para mostrar solamente la idea, ahora podemos conseguir todas las localizaciones de la cara
		//que se necesiten analizar con el shape.part.

                shapes.push_back(shape); //en shapes el general, guarda los puntos de interes de cada cara encontrada.
			//por lo que shapes(0) tiene los puntos de la primer cara y shapes(1) los puntos de la segunda cara
			//por si se quisieran hacer comparaciones entre las caras que se encuentran usando los puntos de interes.
            }

            // Now let's view our face poses on the screen.
		//Ahora se muestran las caras encontradas en la pantalla.

            win.clear_overlay();
            win.set_image(img);
            win.add_overlay(render_face_detections(shapes)); //toma un full_object_detections (cara humana) y la convierte para mostrar 
		//en una ventana, tomando la salida del shape_predictor (predictor de puntos) y mostrara los puntos dibujados.

            // We can also extract copies of each face that are cropped, rotated upright, and scaled to a standard size as shown here:
		//Se extraen copias de los rostros encontrados recortados, girados y escalados.

            dlib::array<array2d<rgb_pixel> > face_chips;
            extract_image_chips(img, get_face_chip_details(shapes), face_chips);
            win_faces.set_image(tile_images(face_chips));

            cout << "Hit enter to process the next image..." << endl; //si hay otra imagen cargada presionar enter para que la muestre.
            cin.get();
        }
    }
    catch (exception& e)
    {
        cout << "\nexception thrown!" << endl;
        cout << e.what() << endl;
    }
}

// ----------------------------------------------------------------------------------------

