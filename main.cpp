#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "kmp.hpp"
#include "manacher.hpp"
#include "lcs.hpp"

using namespace std;

string leerArchivo(const string& ruta) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        cerr << "ERROR: no se pudo abrir " << ruta << endl;
        exit(1);
    }
    stringstream buffer;
    string linea;
    while (getline(archivo, linea)) {
        buffer << linea; 
    }
    return buffer.str();
}

int main() {

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

    for (auto& trans : transmisiones) {
        for (auto& mc : mcodes) {
            int pos = kmpBuscarPrimera(trans.second, mc.second,
                                        nullptr,
                                        trans.first, mc.first);
            if (pos == -1) {
                cout << "false" << endl;
            } else {
                cout << "true " << (pos + 1) << endl;
            }
        }
    }

    manacher(transmission1);
    manacher(transmission2);
    
    // Parte 3: Subcadena común más larga entre transmission1 y transmission2
    ResultadoLCS lcsRes = longestCommonSubstring(transmission1, transmission2);
    if (lcsRes.longitud > 0) {
        cout << lcsRes.inicioTexto1 << " " << lcsRes.finTexto1 << endl;
    } else {
        cout << "0 0" << endl;
    }

    return 0;
}
