#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "kmp.hpp"

using namespace std;
// ---------------------------------------------------------------------
// leerArchivo: lee un archivo completo y regresa su contenido como un
// solo string, quitando los saltos de linea (asumimos que el contenido
// real son los caracteres hex, y los '\n' son solo formato del archivo).
// Si tu profe quiere que los '\n' cuenten como parte de la secuencia,
// quita el fisltro de abajo.
// ---------------------------------------------------------------------
string leerArchivo(const string& ruta) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        cerr << "ERROR: no se pudo abrir " << ruta << endl;
        exit(1);
    }
    stringstream buffer;
    string linea;
    while (getline(archivo, linea)) {
        buffer << linea; // se concatena sin el '\n'
    }
    return buffer.str();
}

int main() {
    // --- Carga de los 5 archivos ---
    string transmission1 = leerArchivo("transmission1.txt");
    string transmission2 = leerArchivo("transmission2.txt");
    string mcode1 = leerArchivo("mcode1.txt");
    string mcode2 = leerArchivo("mcode2.txt");
    string mcode3 = leerArchivo("mcode3.txt");

    vector<pair<string, string>> transmisiones = {
        {"transmission1.txt", transmission1},
        {"transmission2.txt", transmission2}
    };
    vector<pair<string, string>> mcodes = {
        {"mcode1.txt", mcode1},
        {"mcode2.txt", mcode2},
        {"mcode3.txt", mcode3}
    };

    // --- Parte 1: para cada transmision, buscar cada mcode ---
    // (opcional) vector<Evento> log; si luego quieres pasarlo a kmpBuscarPrimera
    for (auto& trans : transmisiones) {
        for (auto& mc : mcodes) {
            int pos = kmpBuscarPrimera(trans.second, mc.second,
                                        /*log=*/nullptr,
                                        trans.first, mc.first);
            if (pos == -1) {
                cout << "false" << endl;
            } else {
                // el enunciado pide posicion 1-indexada
                cout << "true " << (pos + 1) << endl;
            }
        }
    }

    return 0;
}
