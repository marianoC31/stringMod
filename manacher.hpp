#ifndef MANACHER_HPP
#define MANACHER_HPP

#include <string>
#include <vector>
#include <iostream>
#include "kmp.hpp"

using namespace std;

void manacher(string texto, vector<Evento>* log = nullptr, string nombreTexto = "") {

    string t = "#";
    for (int i = 0; i < texto.length(); i++) {
        t += texto[i];
        t += "#";
    }

    vector<int> P(t.length(),0);
    int C = 0;
    int R = 0;

    for (int i = 0; i < t.length(); i++) {

        int mirror = 2*C-i;
        if (i < R){
            P[i] = min(P[mirror], R-i);
        }

        while((i-P[i] - 1 >= 0) && (i + P[i] + 1 < t.length()) && t[i-P[i]-1] == t[i+P[i]+1]) {
            P[i]++;   

            if (log != nullptr) {
                log ->push_back({"parte2", nombreTexto,"",i,P[i],"expandiendo"});
            }
        }

        if (i+P[i]>R) {
            C = i;
            R = i + P[i];

            if (log != nullptr) {
                log -> push_back({"parte2", nombreTexto, "", i, P[i], "nuevoCentro"});
            }
        }
    }

    int maxLen= 0;
    int centro = 0;

    for (int i = 0; i < P.size(); i++) {
        if (P[i] > maxLen) {
            maxLen=P[i];
            centro = i;
        }
    }

    if (log != nullptr) {
        log->push_back({"parte2", nombreTexto, "", centro, maxLen, "palindromo_max"});
    }

    int inicio = (centro - maxLen) / 2+1;
    int fin = (centro +maxLen) / 2;
    cout << inicio << " " << fin << endl;

};

#endif