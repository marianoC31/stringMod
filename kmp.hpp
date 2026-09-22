#ifndef KMP_HPP
#define KMP_HPP

#include <string>
#include <vector>
#include <iostream>

using namespace std;

struct Evento {
    string parte;          
    string archivoTexto;   
    string archivoPatron;  
    int posTexto;               
    int posPatron;              
    string tipo;           
};

inline vector<int> construirTablaFallos(const string& patron) {
    int N = patron.size();
    vector<int> tabla(N, 0);

    int longitud = 0;
    int i = 1; 

    while (i < N) {
        if (patron[i] == patron[longitud]) {
            longitud++;
            tabla[i] = longitud;
            i++;
        } else if (longitud != 0) {
            longitud = tabla[longitud - 1];
        } else {
            tabla[i] = 0;
            i++;
        }
    }

    return tabla;
}

inline int  kmpBuscarPrimera(const string& texto,
                      const string& patron,
                      vector<Evento>* log = nullptr,
                      const string& nombreTexto = "",
                      const string& nombrePatron = "") {

    if (patron.empty() || texto.empty()) return -1;

    vector<int> lps = construirTablaFallos(patron);
    int i = 0, j = 0;
    
    while (i < texto.size()) {
        if (log != nullptr) {
            log->push_back({"parte1", nombreTexto, nombrePatron, i, j, "comparando"});
        }

        if (texto[i] == patron[j]) {
            if (log != nullptr) {
                log->push_back({"parte1", nombreTexto, nombrePatron, i, j, "match_parcial"});
            }
            i++;
            j++;

            if (j == patron.size()) {
                if (log != nullptr) {
                    log->push_back({"parte1", nombreTexto, nombrePatron, i - j, j, "match_total"});
                }
                return i - j;
            }
        } else {
            if (log != nullptr) {
                log->push_back({"parte1", nombreTexto, nombrePatron, i, j, "mismatch"});
            }
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }

    if (log != nullptr) {
        log->push_back({"parte1", nombreTexto, nombrePatron, i, j, "no encontrado"});
    }

    return -1;
}

#endif