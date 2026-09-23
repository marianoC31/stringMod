#ifndef MANACHER_HPP
#define MANACHER_HPP

#include <string>
#include <vector>
#include "kmp.hpp"

using namespace std;

struct ResultadoManacher {
    string subcadena;
    int longitud = 0;
    int inicio = -1;   // 0-based
    int fin = -1;      
};

inline ResultadoManacher manacher(const string& texto,
                                  vector<Evento>* log = nullptr,
                                  const string& nombreTexto = "")
{
    string t = "#";
    for (char c : texto) { t += c; t += "#"; }

    vector<int> P(t.length(), 0);
    int C = 0, R = 0;

    for (int i = 0; i < (int)t.length(); i++) {
        int mirror = 2*C - i;
        if (i < R) P[i] = min(P[mirror], R - i);

        while ((i - P[i] - 1 >= 0) && (i + P[i] + 1 < (int)t.length()) &&
               t[i-P[i]-1] == t[i+P[i]+1]) {
            P[i]++;
            if (log) log->push_back({"parte2", nombreTexto, "", i, P[i], "expandiendo"});
        }

        if (i + P[i] > R) {
            C = i;
            R = i + P[i];
            if (log) log->push_back({"parte2", nombreTexto, "", i, P[i], "nuevoCentro"});
        }
    }

    int maxLen = 0, centro = 0;
    for (int i = 0; i < (int)P.size(); i++)
        if (P[i] > maxLen) { maxLen = P[i]; centro = i; }

    if (log) log->push_back({"parte2", nombreTexto, "", centro, maxLen, "palindromo_max"});

    ResultadoManacher res;
    res.longitud  = maxLen;
    res.inicio    = (centro - maxLen) / 2;
    res.fin       = res.inicio + maxLen - 1;
    res.subcadena = texto.substr(res.inicio, maxLen);
    return res;
}

#endif