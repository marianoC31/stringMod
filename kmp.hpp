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


vector<int> construirTablaFallos(const std::string& patron){
    int N = patron.size();

    vector<int> tabla(N, 0);

    int longitud = 0, i = 0;

    while (i<N){
        if(patron[i]==patron[longitud]){
            longitud+=1;
            tabla[i]=longitud;
            i++;
        } else if(longitud!=0){
            longitud = tabla[longitud-1];
        } else{
            tabla[i]=0;
            i++;
        }
    }

    return tabla;
}

int kmpBuscarPrimera(const std::string& texto,
                      const std::string& patron,
                      std::vector<Evento>* log = nullptr,
                      const std::string& nombreTexto = "",
                      const std::string& nombrePatron = ""){

        vector<int> lps = construirTablaFallos(patron);
        int i = 0,j = 0;
        
        while(i<texto.size()){
            if(log!=nullptr){
                log->push_back({"parte1", nombreTexto, nombrePatron, i, j, "comparando"});
            }
            if(texto[i]==patron[j]){
                if(log!=nullptr){
                    log->push_back({"parte1", nombreTexto, nombrePatron, i, j, "match_parcial"});
                }
                i++;
                j++;
            }
            if(j==patron.size()){
                if(log!=nullptr){
                    log->push_back({"parte1", nombreTexto, nombrePatron, i, j, "match_total"});
                }
                return i-j;
            }
            if(i < texto.size() && texto[i]!=patron[j]){
                if(log!=nullptr){
                    log->push_back({"parte1", nombreTexto, nombrePatron, i, j, "mismatch"});
                }
                if(j!=0){
                    j = lps[j-1];
                }else{
                    i++;
                }
            }
        }
        if(log!=nullptr){
            log->push_back({"parte1", nombreTexto, nombrePatron, i, j, "no encontrado"});
        }

        return -1;

                      }

#endif
