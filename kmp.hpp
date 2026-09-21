#ifndef KMP_HPP
#define KMP_HPP

#include <string>
#include <vector>

// ---------------------------------------------------------------------
// Evento: representa un paso del algoritmo, pensado para que despues
// lo consuma la interfaz visual (consola, ImGui, Python, lo que sea).
// No es necesario usarlo desde ya, pero ya queda el "gancho" listo:
// solo hay que hacer log->push_back({...}) en los puntos clave de tu KMP.
// ---------------------------------------------------------------------
struct Evento {
    std::string parte;          // "parte1"
    std::string archivoTexto;   // ej "transmission1.txt"
    std::string archivoPatron;  // ej "mcode1.txt"
    int posTexto;               // indice (0-based) en el texto que se esta comparando
    int posPatron;              // indice (0-based) en el patron que se esta comparando
    std::string tipo;           // "comparando" | "match_parcial" | "mismatch" | "match_total"
};

// ---------------------------------------------------------------------
// TODO (tuyo): construirTablaFallos
// Tabla de prefijos-sufijos (failure function) de KMP.
// tabla[i] = longitud del prefijo propio mas largo de patron[0..i]
//            que tambien es sufijo de patron[0..i].
// ---------------------------------------------------------------------
std::vector<int> construirTablaFallos(const std::string& patron);

// ---------------------------------------------------------------------
// TODO (tuyo): kmpBuscarPrimera
// Busca la PRIMERA ocurrencia de patron dentro de texto usando la tabla
// de fallos. Regresa la posicion 0-based donde inicia el match, o -1 si
// no se encontro.
//
// log es opcional (pasa nullptr si todavia no quieres llenar eventos);
// nombreTexto / nombrePatron son solo para etiquetar los eventos.
// ---------------------------------------------------------------------
int kmpBuscarPrimera(const std::string& texto,
                      const std::string& patron,
                      std::vector<Evento>* log = nullptr,
                      const std::string& nombreTexto = "",
                      const std::string& nombrePatron = "");

#endif
