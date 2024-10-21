#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Persona {
public:
    int id;
    string nombre;
    string apellido;
    string correo;

    Persona() : id(0), nombre(""), apellido(""), correo("") {}
    Persona(int i, string n, string a, string c) : id(i), nombre(n), apellido(a), correo(c) {}

    bool operator<(const Persona& otra) const {
        return id < otra.id;  // Compara por ID
    }

    bool operator==(const Persona& otra) const {
        return id == otra.id;  // Compara por ID
    }

    friend ostream& operator<<(ostream& os, const Persona& p) {
        os << p.id << "," << p.nombre << "," << p.apellido << "," << p.correo;
        return os;
    }
};

// Nodo simple circular
template <typename T>
class Nodo {
public:
    T dato;
    Nodo* siguiente;

    Nodo(T d) : dato(d), siguiente(nullptr) {}
};

// Nodo doblemente enlazado
template <typename T>
class NodoDoble {
public:
    T dato;
    NodoDoble* siguiente;
    NodoDoble* anterior;

    NodoDoble(T d) : dato(d), siguiente(nullptr), anterior(nullptr) {}
};

// Lista circular simple
template <typename T>
class ListaCircular {
    Nodo<T>* ultimo;
public:
    ListaCircular() : ultimo(nullptr) {}

    bool estaVacia() {
        return ultimo == nullptr;
    }

    void agregar(T dato) {
        Nodo<T>* nuevo = new Nodo<T>(dato);
        if (estaVacia()) {
            ultimo = nuevo;
            ultimo->siguiente = nuevo;
        } else {
            nuevo->siguiente = ultimo->siguiente;
            ultimo->siguiente = nuevo;
            ultimo = nuevo;
        }
    }

    Nodo<T>* getUltimo() {  // Método para acceder a 'ultimo'
        return ultimo;
    }

    void buscar(int id) {
        if (estaVacia()) {
            cout << "La lista está vacía." << endl;
            return;
        }

        Nodo<T>* actual = ultimo->siguiente;
        do {
            if (actual->dato.id == id) {  // Buscar por ID
                cout << "Encontrado: " << actual->dato << endl;
                return;
            }
            actual = actual->siguiente;
        } while (actual != ultimo->siguiente);

        cout << "No se encontró." << endl;
    }

    void eliminar(int id) {
        if (estaVacia()) return;

        Nodo<T>* actual = ultimo->siguiente;
        Nodo<T>* previo = ultimo;

        do {
            if (actual->dato.id == id) {  // Eliminar por ID
                if (actual == ultimo->siguiente && actual == ultimo) {
                    // Solo un elemento en la lista
                    delete actual;
                    ultimo = nullptr;
                } else {
                    previo->siguiente = actual->siguiente;
                    if (actual == ultimo) {
                        ultimo = previo;
                    }
                    delete actual;
                }
                return;
            }
            previo = actual;
            actual = actual->siguiente;
        } while (actual != ultimo->siguiente);
    }

    void guardarEnCSV(const string& archivo) {
        ofstream file(archivo);
        if (file.is_open()) {
            Nodo<T>* actual = ultimo->siguiente;
            do {
                file << actual->dato << endl;
                actual = actual->siguiente;
            } while (actual != ultimo->siguiente);
            file.close();
        }
    }

    void cargarDesdeCSV(const string& archivo) {
        ifstream file(archivo);
        string linea, nombre, apellido, correo;
        int id;

        while (getline(file, linea)) {
            size_t coma1 = linea.find(",");
            size_t coma2 = linea.find(",", coma1 + 1);
            size_t coma3 = linea.find(",", coma2 + 1);
            id = stoi(linea.substr(0, coma1));
            nombre = linea.substr(coma1 + 1, coma2 - coma1 - 1);
            apellido = linea.substr(coma2 + 1, coma3 - coma2 - 1);
            correo = linea.substr(coma3 + 1);
            agregar(Persona(id, nombre, apellido, correo));
        }

        file.close();
    }
};

// Lista doblemente enlazada
template <typename T>
class ListaDoble {
    NodoDoble<T>* cabeza;
public:
    ListaDoble() : cabeza(nullptr) {}

    bool estaVacia() {
        return cabeza == nullptr;
    }

    void agregarOrdenado(T dato) {
        NodoDoble<T>* nuevo = new NodoDoble<T>(dato);
        if (estaVacia()) {
            cabeza = nuevo;
        } else {
            NodoDoble<T>* actual = cabeza;
            NodoDoble<T>* anterior = nullptr;
            while (actual != nullptr && actual->dato < dato) {
                anterior = actual;
                actual = actual->siguiente;
            }
            if (anterior == nullptr) {
                nuevo->siguiente = cabeza;
                cabeza->anterior = nuevo;
                cabeza = nuevo;
            } else {
                nuevo->siguiente = anterior->siguiente;
                nuevo->anterior = anterior;
                if (anterior->siguiente != nullptr) {
                    anterior->siguiente->anterior = nuevo;
                }
                anterior->siguiente = nuevo;
            }
        }
    }

    NodoDoble<T>* getCabeza() {  // Método para acceder a 'cabeza'
        return cabeza;
    }

    void buscar(int id) {
        NodoDoble<T>* actual = cabeza;
        while (actual != nullptr) {
            if (actual->dato.id == id) {  // Buscar por ID
                cout << "Encontrado: " << actual->dato << endl;
                return;
            }
            actual = actual->siguiente;
        }
        cout << "No se encontró." << endl;
    }

    void eliminar(int id) {
        NodoDoble<T>* actual = cabeza;
        while (actual != nullptr) {
            if (actual->dato.id == id) {  // Eliminar por ID
                if (actual->anterior != nullptr) {
                    actual->anterior->siguiente = actual->siguiente;
                } else {
                    cabeza = actual->siguiente;
                }
                if (actual->siguiente != nullptr) {
                    actual->siguiente->anterior = actual->anterior;
                }
                delete actual;
                return;
            }
            actual = actual->siguiente;
        }
    }

    void guardarEnCSV(const string& archivo) {
        ofstream file(archivo);
        if (file.is_open()) {
            NodoDoble<T>* actual = cabeza;
            while (actual != nullptr) {
                file << actual->dato << endl;
                actual = actual->siguiente;
            }
            file.close();
        }
    }

    void cargarDesdeCSV(const string& archivo) {
        ifstream file(archivo);
        string linea, nombre, apellido, correo;
        int id;

        while (getline(file, linea)) {
            size_t coma1 = linea.find(",");
            size_t coma2 = linea.find(",", coma1 + 1);
            size_t coma3 = linea.find(",", coma2 + 1);
            id = stoi(linea.substr(0, coma1));
            nombre = linea.substr(coma1 + 1, coma2 - coma1 - 1);
            apellido = linea.substr(coma2 + 1, coma3 - coma2 - 1);
            correo = linea.substr(coma3 + 1);
            agregarOrdenado(Persona(id, nombre, apellido, correo));
        }

        file.close();
    }
};

void menu(ListaCircular<Persona>& listaCircular, ListaDoble<Persona>& listaDoble) {
    int opcion;
    string archivoCSV = "personas.csv";

    // Cargar datos desde el archivo CSV
    listaCircular.cargarDesdeCSV(archivoCSV);
    listaDoble.cargarDesdeCSV(archivoCSV);

    do {
        cout << "1. Agregar persona" << endl;
        cout << "2. Buscar persona" << endl;
        cout << "3. Eliminar persona" << endl;
        cout << "4. Guardar en CSV" << endl;
        cout << "0. Salir" << endl;
        cin >> opcion;

        switch (opcion) {
            case 1: {
                string nombre, apellido, correo;
                int nuevoId = listaCircular.getUltimo() ? listaCircular.getUltimo()->dato.id + 1 : 1; // Asignar ID

                cout << "Ingrese nombre: ";
                cin >> nombre;
                cout << "Ingrese apellido: ";
                cin >> apellido;
                cout << "Ingrese correo: ";
                cin >> correo;

                listaCircular.agregar(Persona(nuevoId, nombre, apellido, correo));
                listaDoble.agregarOrdenado(Persona(nuevoId, nombre, apellido, correo));
                break;
            }
            case 2: {
                int id;
                cout << "Ingrese ID a buscar: ";
                cin >> id;
                listaCircular.buscar(id);
                listaDoble.buscar(id);
                break;
            }
            case 3: {
                int id;
                cout << "Ingrese ID a eliminar: ";
                cin >> id;
                listaCircular.eliminar(id);
                listaDoble.eliminar(id);
                // Guardar después de eliminar
                listaCircular.guardarEnCSV(archivoCSV);
                listaDoble.guardarEnCSV(archivoCSV);
                cout << "Datos actualizados en " << archivoCSV << endl;
                break;
            }
            case 4:
                listaCircular.guardarEnCSV(archivoCSV);
                listaDoble.guardarEnCSV(archivoCSV);
                cout << "Datos guardados en " << archivoCSV << endl;
                break;
            case 0:
                break;
            default:
                cout << "Opción no válida." << endl;
        }
    } while (opcion != 0);
}

int main() {
    ListaCircular<Persona> listaCircular;
    ListaDoble<Persona> listaDoble;
    menu(listaCircular, listaDoble);
    return 0;
}
