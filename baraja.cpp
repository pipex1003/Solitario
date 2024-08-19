#include <cstdlib>
#include "baraja.h"

using namespace std;

Naipe Naipe::Sucesor() const {
    int num;
    num = this->numero+1;
    if(num == 11) num = 0;
    return Naipe(this->palo, num);
}

Naipe Naipe::Predecesor() const {
    int num;

    num = this->numero-1;
    if(num == 0) num = 0;
    return Naipe(this->palo, num);
}

int Naipe::Orden() const {
    int n = numero;
    if(n > 6) n = n+3;
    return palo*13+n;
}

Baraja::Baraja() {
    int numero;
    int palo;

    for(palo = 0; palo < 4; palo++)
       for(numero = 0; numero < 10; numero++) {
           baraja[palo*10+numero] = Naipe(palo, numero);
       }
}

void Baraja::Mezclar() {
    Naipe aux;
    int i, j, x;

    for(j = 0; j < 10; j++)
        for(i = 0; i < 40; i++) {
            x = 40*rand()/RAND_MAX;
            aux = baraja[i];
            baraja[i] = baraja[x];
            baraja[x] = aux;
        }
}
