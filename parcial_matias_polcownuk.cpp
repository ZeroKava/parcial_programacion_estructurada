#include <iostream>
#include <conio.h>
#include <cstring>
#include <limits>
#include <algorithm>
#include <fstream>

using namespace std;

struct Libro {
    char titulo[20];
    char autor[20];
    int edicion;
    char editorial[20];
    char isbn[13];
    int paginas;
    bool disponible; 
    Libro* siguiente;
};

struct NodoOperacion {
    char tipo[20]; 
    Libro libro;  
    NodoOperacion* siguiente;
};

struct Lector {
    char nombre[20];
    int dni;
    char solicitud[5];
    Lector* siguiente;
};

Libro* cabeza_libro = nullptr;
NodoOperacion* pila_operaciones = nullptr;
Lector* cabeza_lector = nullptr;

Libro* crearLibro(const char* titulo, const char* autor, int edicion, const char* editorial, const char* isbn, int paginas) {
    Libro* nuevo_libro = new Libro;
    strcpy(nuevo_libro->titulo, titulo);
    strcpy(nuevo_libro->autor, autor);
    nuevo_libro->edicion = edicion;
    strcpy(nuevo_libro->editorial, editorial);
    strcpy(nuevo_libro->isbn, isbn);
    nuevo_libro->paginas = paginas;
    nuevo_libro->disponible = true; 
    nuevo_libro->siguiente = nullptr;
    return nuevo_libro;
}

void crearArchivoSiNoExiste(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo) {
        std::ofstream nuevoArchivo(nombreArchivo);
        nuevoArchivo.close(); 
        std::cout << "Archivo '" << nombreArchivo << "' creado." << std::endl;
    } else {
        std::cout << "Archivo '" << nombreArchivo << "' ya existe." << std::endl;
    }
}

void agregarLibroAlFinal(const char* titulo, const char* autor, int edicion, const char* editorial, const char* isbn, int paginas) {
    Libro* nuevo_libro = crearLibro(titulo, autor, edicion, editorial, isbn, paginas);
    
    if (cabeza_libro == nullptr) { 
        cabeza_libro = nuevo_libro;
    } else {
        Libro* actual = cabeza_libro;
        while (actual->siguiente != nullptr) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevo_libro; 
    }

    
    NodoOperacion* nueva_operacion = new NodoOperacion;
    strcpy(nueva_operacion->tipo, "Agregar");
    nueva_operacion->libro = *nuevo_libro; 
    nueva_operacion->siguiente = pila_operaciones;
    pila_operaciones = nueva_operacion; 
}

void agregarSolicitudAlFinal(const char* nombre, int dni, const char* solicitud) {
    Lector* nuevo_lector = new Lector;
    strcpy(nuevo_lector->nombre, nombre);
    nuevo_lector->dni = dni;
    strcpy(nuevo_lector->solicitud, solicitud);
    nuevo_lector->siguiente = nullptr;

    if (cabeza_lector == nullptr) {
        cabeza_lector = nuevo_lector;
    } else {
        Lector* actual = cabeza_lector;
        while (actual->siguiente != nullptr) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevo_lector; 
    }
}


int ingresarEntero(const char* mensaje) {
    int valor;
    while (true) {
        cout << mensaje;
        cin >> valor;

        if (cin.fail()) { 
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "El dato ingresado es incorrecto, por favor ingrese uno válido." << endl;
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            break; 
        }
    }
    return valor;
}

void ingresarCadena(const char* mensaje, char* destino, int longitud) {
    while (true) {
        cout << mensaje;
        cin.getline(destino, longitud);
        if (strlen(destino) == 0) {
            cout << "El dato ingresado es incorrecto, por favor ingrese uno válido." << endl;
        } else {
            break; 
        }
    }
}

Libro* dividir(Libro* cabeza) {
    if (cabeza == nullptr || cabeza->siguiente == nullptr) {
        return cabeza;
    }
    
    Libro* lento = cabeza;
    Libro* rapido = cabeza->siguiente;

    while (rapido != nullptr && rapido->siguiente != nullptr) {
        lento = lento->siguiente;
        rapido = rapido->siguiente->siguiente;
    }

    Libro* mitad = lento->siguiente;
    lento->siguiente = nullptr;
    return mitad;
}

Libro* fusionar(Libro* izquierda, Libro* derecha) {
    if (izquierda == nullptr) return derecha;
    if (derecha == nullptr) return izquierda;

    Libro* resultado;
    
    if (strcmp(izquierda->titulo, derecha->titulo) <= 0) {
        resultado = izquierda;
        resultado->siguiente = fusionar(izquierda->siguiente, derecha);
    } else {
        resultado = derecha;
        resultado->siguiente = fusionar(izquierda, derecha->siguiente);
    }
    return resultado;
}

Libro* mergeSort(Libro* cabeza) {
    if (cabeza == nullptr || cabeza->siguiente == nullptr) {
        return cabeza;
    }

    Libro* mitad = dividir(cabeza);
    
    cabeza = mergeSort(cabeza);
    mitad = mergeSort(mitad);
    
    return fusionar(cabeza, mitad);
}

void ingresarDatosLibro() {
    char titulo[20], autor[20], editorial[20], isbn[13];
    int edicion, paginas;

    ingresarCadena("Ingrese el título del libro: ", titulo, 20);
    ingresarCadena("Ingrese el autor del libro: ", autor, 20);
    edicion = ingresarEntero("Ingrese la edición: ");
    ingresarCadena("Ingrese la editorial: ", editorial, 20);
    ingresarCadena("Ingrese el ISBN: ", isbn, 13);
    paginas = ingresarEntero("Ingrese el número de páginas: ");

    agregarLibroAlFinal(titulo, autor, edicion, editorial, isbn, paginas);
    cout << "Libro agregado correctamente.\n";
}

void mostrarLibros(Libro* cabeza_libro) {
    if (cabeza_libro == nullptr) {
        cout << "No hay libros en la lista.\n";
        return;
    }
    
    Libro* actual = cabeza_libro;
    while (actual != nullptr) {
        cout << "Titulo: " << actual->titulo << ", Autor: " << actual->autor
             << ", Edicion: " << actual->edicion << ", Editorial: " << actual->editorial
             << ", ISBN: " << actual->isbn << ", Paginas: " << actual->paginas 
             << ", Disponible: " << (actual->disponible ? "Sí" : "No") << endl;
        actual = actual->siguiente;
    }
}

void solicitarLibro() {
    char titulo[20];
    ingresarCadena("Ingrese el título del libro que desea solicitar: ", titulo, 20);

    Libro* actual = cabeza_libro;
    while (actual != nullptr) {
        if (strcmp(actual->titulo, titulo) == 0) {
            if (actual->disponible) {
                cout << "Libro '" << actual->titulo << "' ha sido solicitado.\n";
                actual->disponible = false; 

                NodoOperacion* nueva_operacion = new NodoOperacion;
                strcpy(nueva_operacion->tipo, "Solicitar");
                nueva_operacion->libro = *actual; 
                nueva_operacion->siguiente = pila_operaciones;
                pila_operaciones = nueva_operacion; 
                return;
            } else {
                cout << "El libro '" << actual->titulo << "' no está disponible.\n";
                return;
            }
        }
        actual = actual->siguiente;
    }
    cout << "El libro no se encontró.\n";
}

void devolverLibro() {
    char titulo[20];
    ingresarCadena("Ingrese el título del libro que desea devolver: ", titulo, 20);

    Libro* actual = cabeza_libro;
    while (actual != nullptr) {
        if (strcmp(actual->titulo, titulo) == 0) {
            if (!actual->disponible) {
                cout << "Libro '" << actual->titulo << "' ha sido devuelto.\n";
                actual->disponible = true;

                NodoOperacion* nueva_operacion = new NodoOperacion;
                strcpy(nueva_operacion->tipo, "Devolver");
                nueva_operacion->libro = *actual; 
                nueva_operacion->siguiente = pila_operaciones;
                pila_operaciones = nueva_operacion; 

                return;
            } else {
                cout << "El libro '" << actual->titulo << "' no está prestado.\n";
                return;
            }
        }
        actual = actual->siguiente;
    }
    cout << "El libro no se encontró.\n";
}

void buscarLibro() {
    char criterio[20], valor[20];
    bool criterioValido = false;

    do {
        cout << "Coloque el campo por el que desea buscar el libro 'titulo','autor' o 'isbn' ";
        cin.getline(criterio, 20); 

        for (int i = 0; criterio[i]; ++i) {
            criterio[i] = tolower(criterio[i]);
        }

        criterioValido = (strcmp(criterio, "titulo") == 0) ||
                         (strcmp(criterio, "autor") == 0) ||
                         (strcmp(criterio, "isbn") == 0);

        if (!criterioValido) {
            cout << "Criterio inválido. Por favor, ingrese 'titulo', 'autor' o 'isbn'." << endl;
        }
    } while (!criterioValido);

    cout << "Ingrese el valor a buscar: ";
    cin.getline(valor, 20); 

    Libro* actual = cabeza_libro;
    bool encontrado = false;

    while (actual != nullptr) {
        bool coincide = false;

        if (strcmp(criterio, "titulo") == 0 && strstr(actual->titulo, valor) != nullptr) {
            coincide = true;
        } else if (strcmp(criterio, "autor") == 0 && strstr(actual->autor, valor) != nullptr) {
            coincide = true;
        } else if (strcmp(criterio, "isbn") == 0 && strcmp(actual->isbn, valor) == 0) {
            coincide = true;
        }

        if (coincide) {
            cout << "Libro encontrado:\n";
            cout << "Título: " << actual->titulo << endl;
            cout << "Autor: " << actual->autor << endl;
            cout << "ISBN: " << actual->isbn << endl;
            encontrado = true; 
        }
        actual = actual->siguiente;
    }

    if (!encontrado) {
        cout << "Libro no encontrado." << endl;
    }
}

void deshacerOperacion() {
    if (pila_operaciones == nullptr) {
        cout << "No hay operaciones para deshacer.\n";
        return;
    }

    NodoOperacion* operacion = pila_operaciones;
    pila_operaciones = pila_operaciones->siguiente;

    if (strcmp(operacion->tipo, "Agregar") == 0) {
        Libro* actual = cabeza_libro;
        Libro* anterior = nullptr;
        while (actual != nullptr) {
            if (strcmp(actual->isbn, operacion->libro.isbn) == 0) {
                if (anterior == nullptr) {
                    cabeza_libro = actual->siguiente; 
                } else {
                    anterior->siguiente = actual->siguiente; 
                }
                delete actual; 
                break;
            }
            anterior = actual;
            actual = actual->siguiente;
        }
        cout << "Operación 'Agregar' deshecha: " << operacion->libro.titulo << " eliminado.\n";
    } else if (strcmp(operacion->tipo, "Solicitar") == 0) {
        
        Libro* actual = cabeza_libro;
        while (actual != nullptr) {
            if (strcmp(actual->isbn, operacion->libro.isbn) == 0) {
                actual->disponible = true; 
                break;
            }
            actual = actual->siguiente;
        }
        cout << "Operación 'Solicitar' deshecha: " << operacion->libro.titulo << " devuelto.\n";
    } else if (strcmp(operacion->tipo, "Devolver") == 0) {

        Libro* actual = cabeza_libro;
        while (actual != nullptr) {
            if (strcmp(actual->isbn, operacion->libro.isbn) == 0) {
                actual->disponible = false;
                break;
            }
            actual = actual->siguiente;
        }
        cout << "Operación 'Devolver' deshecha: " << operacion->libro.titulo << " solicitado nuevamente.\n";
    }

    delete operacion; 
}


void cargarLibrosDesdeArchivo() {
    ifstream archivo("biblioteca.txt");
    if (!archivo) {
        cout << "No se pudo abrir el archivo biblioteca.txt\n";
        return;
    }
    
    while (!archivo.eof()) {
        Libro libro;
        archivo >> libro.titulo >> libro.autor >> libro.edicion >> libro.editorial >> libro.isbn >> libro.paginas >> libro.disponible;
        if (archivo) { 
            agregarLibroAlFinal(libro.titulo, libro.autor, libro.edicion, libro.editorial, libro.isbn, libro.paginas);
        }
    }
    archivo.close();
}

void cargarSolicitudesDesdeArchivo() {
    ifstream archivo("solicitudes.txt");
    if (!archivo) {
        cout << "No se pudo abrir el archivo solicitudes.txt\n";
        return;
    }
    
    while (!archivo.eof()) {
        Lector lector;
        archivo >> lector.nombre >> lector.dni >> lector.solicitud;
        if (archivo) { 
            agregarSolicitudAlFinal(lector.nombre, lector.dni, lector.solicitud);
        }
    }
    archivo.close();
}

void guardarLibrosEnArchivo() {
    ofstream archivo("biblioteca.txt");
    if (!archivo) {
        cout << "No se pudo abrir el archivo biblioteca.txt para guardar.\n";
        return;
    }

    Libro* actual = cabeza_libro;
    while (actual != nullptr) {
        archivo << actual->titulo << " "
                << actual->autor << " "
                << actual->edicion << " "
                << actual->editorial << " "
                << actual->isbn << " "
                << actual->paginas << " "
                << actual->disponible << "\n";
        actual = actual->siguiente;
    }
    archivo.close();
}

void guardarSolicitudesEnArchivo() {
    ofstream archivo("solicitudes.txt");
    if (!archivo) {
        cout << "No se pudo abrir el archivo solicitudes.txt para guardar.\n";
        return;
    }

    Lector* actual = cabeza_lector;
    while (actual != nullptr) {
        archivo << actual->nombre << " "
                << actual->dni << " "
                << actual->solicitud << "\n";
        actual = actual->siguiente;
    }
    archivo.close();
}

void menu() {
    int opcion;

    do {
        cout << "\n---- Menú ----\n";
        cout << "1. Agregar libro\n";
        cout << "2. Mostrar libros\n";
        cout << "3. Ordenar libros\n";
        cout << "4. Solicitar libro\n";
        cout << "5. Devolver libro\n";
        cout << "6. Buscar libro\n";
        cout << "7. Deshacer última operación\n";
        cout << "0. Salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcion) {
            case 1:
                ingresarDatosLibro();
                break;
            case 2:
                mostrarLibros(cabeza_libro);
                break;
            case 3:
                cabeza_libro = mergeSort(cabeza_libro);
                cout << "Libros ordenados correctamente.\n";
                break;
            case 4:
                solicitarLibro();
                break;
            case 5:
                devolverLibro();
                break;
            case 6:
                buscarLibro();
                break;
            case 7:
                deshacerOperacion();
                break;
            case 0:
                cout << "Guardando datos y saliendo...\n";
                guardarLibrosEnArchivo();
                guardarSolicitudesEnArchivo();
                break;
            default:
                cout << "Opción no válida. Inténtelo de nuevo.\n";
        }
    } while (opcion != 0);
}

int main() {
    crearArchivoSiNoExiste("biblioteca.txt");
    crearArchivoSiNoExiste("solicitudes.txt");
    
    menu();

    while (cabeza_libro != nullptr) {
        Libro* temp = cabeza_libro;
        cabeza_libro = cabeza_libro->siguiente;
        delete temp;
    }
    while (pila_operaciones != nullptr) {
        NodoOperacion* temp = pila_operaciones;
        pila_operaciones = pila_operaciones->siguiente;
        delete temp;
    }

    return 0;
}
