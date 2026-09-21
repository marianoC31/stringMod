#ifndef KMP_HPP
#define KMP_HPP

#include <string>
#include <vector>

using namespace std;

struct Evento {
    string parte;          
    string archivoTexto;   
    string archivoPatron;  
    int posTexto;               
    int posPatron;              
    string tipo;           
};


vector<int> construirTablaFallos(const std::string& patron);

int kmpBuscarPrimera(const std::string& texto,
                      const std::string& patron,
                      std::vector<Evento>* log = nullptr,
                      const std::string& nombreTexto = "",
                      const std::string& nombrePatron = "");

#endif
