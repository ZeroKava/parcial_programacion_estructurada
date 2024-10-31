import os

class Libro:
    def __init__(self, titulo, autor, edicion, editorial, isbn, paginas):
        self.titulo = titulo
        self.autor = autor
        self.edicion = edicion
        self.editorial = editorial
        self.isbn = isbn
        self.paginas = paginas
        self.disponible = True
        self.siguiente = None

class NodoOperacion:
    def __init__(self, tipo, libro):
        self.tipo = tipo
        self.libro = libro
        self.siguiente = None

class Lector:
    def __init__(self, nombre, dni, solicitud):
        self.nombre = nombre
        self.dni = dni
        self.solicitud = solicitud
        self.siguiente = None

cabeza_libro = None
pila_operaciones = None
cabeza_lector = None

def crearLibro(titulo, autor, edicion, editorial, isbn, paginas):
    nuevo_libro = Libro(titulo, autor, edicion, editorial, isbn, paginas)
    return nuevo_libro

def crearArchivoSiNoExiste(nombreArchivo):
    if not os.path.exists(nombreArchivo):
        with open(nombreArchivo, 'w') as archivo:
            pass
        print(f"Archivo '{nombreArchivo}' creado.")
    else:
        print(f"Archivo '{nombreArchivo}' ya existe.")

def agregarLibroAlFinal(titulo, autor, edicion, editorial, isbn, paginas):
    nuevo_libro = crearLibro(titulo, autor, edicion, editorial, isbn, paginas)

    global cabeza_libro
    if cabeza_libro is None:
        cabeza_libro = nuevo_libro
    else:
        actual = cabeza_libro
        while actual.siguiente is not None:
            actual = actual.siguiente
        actual.siguiente = nuevo_libro

    nueva_operacion = NodoOperacion("Agregar", nuevo_libro)
    nueva_operacion.siguiente = pila_operaciones
    pila_operaciones = nueva_operacion

def agregarSolicitudAlFinal(nombre, dni, solicitud):
    nuevo_lector = Lector(nombre, dni, solicitud)

    global cabeza_lector
    if cabeza_lector is None:
        cabeza_lector = nuevo_lector
    else:
        actual = cabeza_lector
        while actual.siguiente is not None:
            actual = actual.siguiente
        actual.siguiente = nuevo_lector

def ingresarEntero(mensaje):
    while True:
        try:
            valor = int(input(mensaje))
            return valor
        except ValueError:
            print("El dato ingresado es incorrecto, por favor ingrese uno válido.")

def ingresarCadena(mensaje, longitud):
    while True:
        cadena = input(mensaje)
        if len(cadena) == 0 or len(cadena) > longitud:
            print("El dato ingresado es incorrecto, por favor ingrese uno válido.")
        else:
            return cadena

def dividir(cabeza):
    if cabeza is None or cabeza.siguiente is None:
        return cabeza

    lento = cabeza
    rapido = cabeza.siguiente

    while rapido is not None and rapido.siguiente is not None:
        lento = lento.siguiente
        rapido = rapido.siguiente.siguiente

    mitad = lento.siguiente
    lento.siguiente = None
    return mitad

def fusionar(izquierda, derecha):
    if izquierda is None:
        return derecha
    if derecha is None:
        return izquierda

    if izquierda.titulo <= derecha.titulo:
        resultado = izquierda
        resultado.siguiente = fusionar(izquierda.siguiente, derecha)
    else:
        resultado = derecha
        resultado.siguiente = fusionar(izquierda, derecha.siguiente)
    return resultado

def mergeSort(cabeza):
    if cabeza is None or cabeza.siguiente is None:
        return cabeza

    mitad = dividir(cabeza)

    cabeza = mergeSort(cabeza)
    mitad = mergeSort(mitad)

    return fusionar(cabeza, mitad)

def ingresarDatosLibro():
    titulo = ingresarCadena("Ingrese el título del libro: ", 20)
    autor = ingresarCadena("Ingrese el autor del libro: ", 20)
    edicion = ingresarEntero("Ingrese la edición: ")
    editorial = ingresarCadena("Ingrese la editorial: ", 20)
    isbn = ingresarCadena("Ingrese el ISBN: ", 13)
    paginas = ingresarEntero("Ingrese el número de páginas: ")

    agregarLibroAlFinal(titulo, autor, edicion, editorial, isbn, paginas)
    print("Libro agregado correctamente.\n")

def mostrarLibros(cabeza_libro):
    if cabeza_libro is None:
        print("No hay libros en la lista.\n")
        return

    actual = cabeza_libro
    while actual is not None:
        print(f"Titulo: {actual.titulo}, Autor: {actual.autor}, Edicion: {actual.edicion}, Editorial: {actual.editorial}, ISBN: {actual.isbn}, Paginas: {actual.paginas}, Disponible: {'Sí' if actual.disponible else 'No'}")
        actual = actual.siguiente

def solicitarLibro():
    titulo = ingresarCadena("Ingrese el título del libro que desea solicitar: ", 20)

    actual = cabeza_libro
    while actual is not None:
        if actual.titulo == titulo:
            if actual.disponible:
                print(f"Libro '{actual.titulo}' ha sido solicitado.\n")
                actual.disponible = False

                nueva_operacion = NodoOperacion("Solicitar", actual)
                nueva_operacion.siguiente = pila_operaciones
                pila_operaciones = nueva_operacion
                return
            else:
                print(f"El libro '{actual.titulo}' no está disponible.\n")
                return
        actual = actual.siguiente
    print("El libro no se encontró.\n")

def devolverLibro():
    titulo = ingresarCadena("Ingrese el título del libro que desea devolver: ", 20)

    actual = cabeza_libro
    while actual is not None:
        if actual.titulo == titulo:
            if not actual.disponible:
                print(f"Libro '{actual.titulo}' ha sido devuelto.\n")
                actual.disponible = True

                nueva_operacion = NodoOperacion("Devolver", actual)
                nueva_operacion.siguiente = pila_operaciones
                pila_operaciones = nueva_operacion
                return
            else:
                print(f"El libro '{actual.titulo}' no está prestado.\n")
                return
        actual = actual.siguiente
    print("El libro no se encontró.\n")

def buscarLibro():
    criterio = input("Coloque el campo por el que desea buscar el libro ('titulo', 'autor' o 'isbn'): ").lower()
    if criterio not in ["titulo", "autor", "isbn"]:
        print("Criterio inválido. Por favor, ingrese 'titulo', 'autor' o 'isbn'.")
        return

    valor = input("Ingrese el valor a buscar: ")

    actual = cabeza_libro
    encontrado = False

    while actual is not None:
        coincide = False

        if criterio == "titulo" and valor.lower() in actual.titulo.lower():
            coincide = True
        elif criterio == "autor" and valor.lower() in actual.autor.lower():
            coincide = True
        elif criterio == "isbn" and valor == actual.isbn:
            coincide = True

        if coincide:
            print("Libro encontrado:\n")
            print(f"Título: {actual.titulo}")
            print(f"Autor: {actual.autor}")
            print(f"ISBN: {actual.isbn}")
            encontrado = True
        actual = actual.siguiente

    if not encontrado:
        print("Libro no encontrado.")

def deshacerOperacion():
    global pila_operaciones, cabeza_libro
    if pila_operaciones is None:
        print("No hay operaciones para deshacer.\n")
        return

    operacion = pila_operaciones
    pila_operaciones = pila_operaciones.siguiente

    if operacion.tipo == "Agregar":
        actual = cabeza_libro
        anterior = None
        while actual is not None:
            if actual.isbn == operacion.libro.isbn:
                if anterior is None:
                    cabeza_libro = actual.siguiente
                else:
                    anterior.siguiente = actual.siguiente
                del actual
                break
            anterior = actual
            actual = actual.siguiente
        print(f"Operación 'Agregar' deshecha: {operacion.libro.titulo} eliminado.\n")
    elif operacion.tipo == "Solicitar":
        actual = cabeza_libro
        while actual is not None:
            if actual.isbn == operacion.libro.isbn:
                actual.disponible = True
                break
            actual = actual.siguiente
        print(f"Operación 'Solicitar' deshecha: {operacion.libro.titulo} devuelto.\n")
    elif operacion.tipo == "Devolver":
        actual = cabeza_libro
        while actual is not None:
            if actual.isbn == operacion.libro.isbn:
                actual.disponible = False
                break
            actual = actual.siguiente
        print(f"Operación 'Devolver' deshecha: {operacion.libro.titulo} solicitado nuevamente.\n")

    del operacion

def cargarLibrosDesdeArchivo():
    try:
        with open("biblioteca.txt", "r") as archivo:
            for linea in archivo:
                datos = linea.strip().split()
                if len(datos) == 7:
                    titulo, autor, edicion, editorial, isbn, paginas, disponible = datos
                    agregarLibroAlFinal(titulo, autor, int(edicion), editorial, isbn, int(paginas))
                    libro = cabeza_libro
                    while libro is not None:
                        if libro.isbn == isbn:
                            libro.disponible = disponible == 'True'
                            break
    except FileNotFoundError:
        print("No se pudo abrir el archivo biblioteca.txt\n")

def guardarLibrosEnArchivo():
    with open("biblioteca.txt", "w") as archivo:
        actual = cabeza_libro
        while actual is not None:
            archivo.write(f"{actual.titulo} {actual.autor} {actual.edicion} {actual.editorial} {actual.isbn} {actual.paginas} {actual.disponible}\n")
            actual = actual.siguiente
    print("Libros guardados correctamente en 'biblioteca.txt'.\n")

def menu():
    crearArchivoSiNoExiste("biblioteca.txt")
    cargarLibrosDesdeArchivo()

    while True:
        print("Menu:")
        print("1. Agregar libro")
        print("2. Mostrar libros")
        print("3. Solicitar libro")
        print("4. Devolver libro")
        print("5. Buscar libro")
        print("6. Deshacer última operación")
        print("7. Guardar cambios y salir")
        print("8. Salir sin guardar")
        opcion = ingresarEntero("Seleccione una opción: ")

        if opcion == 1:
            ingresarDatosLibro()
        elif opcion == 2:
            mostrarLibros(cabeza_libro)
        elif opcion == 3:
            solicitarLibro()
        elif opcion == 4:
            devolverLibro()
        elif opcion == 5:
            buscarLibro()
        elif opcion == 6:
            deshacerOperacion()
        elif opcion == 7:
            guardarLibrosEnArchivo()
            break
        elif opcion == 8:
            print("Saliendo sin guardar los cambios...")
            break
        else:
            print("Opción inválida. Por favor intente de nuevo.")


menu()
