#ifndef LCS_HPP
#define LCS_HPP

#include <string>
#include <vector>
#include <iostream>

using namespace std;

// Estructura para registrar eventos en la visualización de la Parte 3 (LCS)
struct EventoLCS {
    string parte = "parte3";
    string archivoTexto;
    string archivoPatron;
    int posTexto = 0;
    int posPatron = 0;
    string tipo;
    int longitud = 1;
};

// Estructura para almacenar el resultado del algoritmo LCS
struct ResultadoLCS {
    int inicioTexto1 = 0; // Posición inicial en s1 (base 1)
    int finTexto1 = 0;    // Posición final en s1 (base 1)
    int inicioTexto2 = 0; // Posición inicial en s2 (base 1)
    int finTexto2 = 0;    // Posición final en s2 (base 1)
    int longitud = 0;     // Longitud de la subcadena común más larga
    string subcadena = ""; // Contenido de la subcadena
};

/**
 * Encuentra la subcadena común más larga entre dos cadenas utilizando Programación Dinámica.
 * Complejidad de Tiempo: O(N * M)
 * Complejidad de Espacio: O(N * M)
 *
 * @param s1 Primera cadena (ej. transmission1)
 * @param s2 Segunda cadena (ej. transmission2)
 * @param log Vector opcional para registrar eventos para la interfaz gráfica
 * @param nombreTexto1 Nombre o identificador del primer archivo
 * @param nombreTexto2 Nombre o identificador del segundo archivo
 * @return ResultadoLCS con las posiciones (base 1), longitud y subcadena encontrada.
 */
inline ResultadoLCS longestCommonSubstring(
    const string& s1,
    const string& s2,
    vector<EventoLCS>* log = nullptr,
    const string& nombreTexto1 = "transmission1.txt",
    const string& nombreTexto2 = "transmission2.txt") {

    ResultadoLCS res;
    int n = s1.size();
    int m = s2.size();

    if (n == 0 || m == 0) return res;

    // Matriz de Programación Dinámica
    // dp[i][j] representa la longitud del sufijo común más largo de s1[0..i-1] y s2[0..j-1]
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    int maxLen = 0;
    int fin1 = -1;
    int fin2 = -1;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
                int curLen = dp[i][j];
                int start1 = (i - 1) - curLen + 1;
                int start2 = (j - 1) - curLen + 1;

                if (curLen > maxLen) {
                    maxLen = curLen;
                    fin1 = i - 1;
                    fin2 = j - 1;

                    if (log != nullptr) {
                        log->push_back({
                            "parte3",
                            nombreTexto1,
                            nombreTexto2,
                            start1,
                            start2,
                            "nuevo_maximo",
                            curLen
                        });
                    }
                } else if (curLen > 1 && log != nullptr) {
                    log->push_back({
                        "parte3",
                        nombreTexto1,
                        nombreTexto2,
                        start1,
                        start2,
                        "match_parcial",
                        curLen
                    });
                }
            } else {
                dp[i][j] = 0;
            }
        }
    }

    if (maxLen > 0) {
        int start1 = fin1 - maxLen + 1;
        int start2 = fin2 - maxLen + 1;

        res.longitud = maxLen;
        res.inicioTexto1 = start1 + 1; // Convertir a base 1
        res.finTexto1 = fin1 + 1;       // Convertir a base 1
        res.inicioTexto2 = start2 + 1; // Convertir a base 1
        res.finTexto2 = fin2 + 1;       // Convertir a base 1
        res.subcadena = s1.substr(start1, maxLen);

        if (log != nullptr) {
            log->push_back({
                "parte3",
                nombreTexto1,
                nombreTexto2,
                start1,
                start2,
                "match_total",
                maxLen
            });
        }
    }

    return res;
}

#endif // LCS_HPP
