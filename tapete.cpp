#include <windows.h>
#include <stdio.h>
#include "tapete.h"

void Tapete::Limpiar() {
    // Vaciar todas las pilas;
    for(int i = MAZO; i <= TRABAJO4; i++) zona[i].Vaciar();
    movimiento.Vaciar();
}

void Tapete::Iniciar() {
    int i;

    baraja.Mezclar();
    // Colocar cartas en el mazo:
    for(i = 0; i < 40; i++) zona[MAZO].Push(baraja.Carta(i));
    // Mover un Naipe a cada pila de trabajo:
    for(i = 0; i < 4; i++) MoverNaipe(MAZO, TRABAJO1+i);
    // Y una más a la pila del montón:
    MoverNaipe(MAZO, MONTON);
    // Vaciar pila de "deshacer", estos movimientos no son reversibles:
    movimiento.Vaciar();
}

bool Tapete::MoverNaipe(int origen, int destino) {
    Naipe naipe;

    // Si la pila de origen no está vacía:
    if(!zona[origen].Vacia()) {
        naipe = zona[origen].Pop();
        // Mover naipe:
        if(origen == MAZO) naipe.Volver();
        zona[destino].Push(naipe);

        // Guardar movimiento:
        movimiento.Push(Movimiento(origen, destino));

        return true;
    }
    return false;
}


Movimiento Tapete::DesHacer() {
    Movimiento mov;
    Naipe naipe;

    if(!movimiento.Vacia()) {
        mov = movimiento.Pop();
        naipe = zona[mov.Destino()].Pop();
        // Mover naipe:
        if(mov.Origen() == MAZO) naipe.Volver();
        zona[mov.Origen()].Push(naipe);
    }
    return mov;
}

bool Tapete::CogeNaipe(int zona) {
    origen = zona;
    mano = Pila(zona).Pop();
}

void Tapete::SueltaNaipe(int zona) {
    if(zona != -1) Pila(zona).Push(mano);
    else Pila(origen).Push(mano);
    if(origen != zona) movimiento.Push(Movimiento(origen, zona));
    origen = -1;
}

int Tapete::VerificarDobleClick(int z) {
    Naipe no, nd;

    // Si la pila no está vacía, obtener el último naipe (referencia):
    if(!Pila(z).Vacia()) {
       Pila(z).Ultimo();
       no = Pila(z).Actual();

       // Verificar si n es el sucesor de la carta en la cima de alguna salida:
       for(int d = SALIDA1; d <= SALIDA4; d++) {
           if(!Pila(d).Vacia()) {
               Pila(d).Ultimo();
               nd = Pila(d).Actual();
               if(nd.Sucesor() == no) return d;
           } else
               if(no.Numero() == 0) return d;
       }
    }
    return -1;
}

bool Tapete::MovimientoLegal(const int z) {
    Naipe n;
    bool vacia = true;

    // Si la pila no está vacía, obtener el último naipe (referencia):
    if(!Pila(z).Vacia()) {
       vacia = false;
       Pila(z).Ultimo();
       n = Pila(z).Actual();
    }

    // Regla 1: As sobre SALIDAn vacía
    if(vacia && mano.Numero() == 0 && z >= SALIDA1 && z <= SALIDA4)
        return true;

    // Regla 2: Naipe sobre predecesor del mismo palo en SALIDAn
    else if(!vacia && n.Sucesor() == mano && z >= SALIDA1 && z <= SALIDA4)
        return true;

    // Regla 3: Naipe sobre sucesor de distinto palo o pila vacía en TRABAJOn
    else if((vacia || (n.Predecesor().Numero() == mano.Numero()
                 && n.Predecesor().Palo() != mano.Palo())) &&
            z >= TRABAJO1 && z <= TRABAJO4) return true;
    return false;
}

bool Tapete::Terminado() {
    // Comprobar si todas las cartas en la cima de las pilas de salida son
    // reyes, o si todas las pilas están vacías, salvo las de salida.
    // O si las cuatro pilas de salida tienen diez cartas:
    if(Pila(SALIDA1).Cuenta() + Pila(SALIDA2).Cuenta() +
       Pila(SALIDA3).Cuenta() + Pila(SALIDA4).Cuenta() == 40)
       return true;
    return false;
}

bool Tapete::MovimientoPosible(const int o, const int d) {
    Naipe no, nd;
    bool vaciad = true;

    // Si la pila origen no está vacía, obtener el último naipe (referencia):
    if(!Pila(o).Vacia()) {
       Pila(o).Ultimo();
       no = Pila(o).Actual();
    } else return false;
    // Si la pila de origen está vacía, el movimiento no es posible:

    // Si la pila destino no está vacía, obtener el último naipe (referencia):
    if(!Pila(d).Vacia()) {
       vaciad = false;
       Pila(d).Ultimo();
       nd = Pila(d).Actual();
    }

    // Regla 1: As sobre SALIDAn vacía
    if(vaciad && no.Numero() == 0 && d >= SALIDA1 && d <= SALIDA4)
        return true;

    // Regla 2: Naipe sobre predecesor del mismo palo en SALIDAn
    else if(!vaciad && nd == no.Predecesor() && d >= SALIDA1 && d <= SALIDA4)
        return true;

    // Regla 3: Naipe sobre sucesor de distinto palo o pila vacía en TRABAJOn
    else if((vaciad || (nd.Predecesor().Numero() == no.Numero()
                 && nd.Predecesor().Palo() != no.Palo())) &&
            d >= TRABAJO1 && d <= TRABAJO4) return true;
    return false;
}

bool Tapete::SinSolucion() {
    int i, j;
    // Si quedan cartas en el mazo, hay esperanza:
    if(Pila(MAZO).Cuenta() > 0) return false;
    // Verificar movimientos:
    // 1) entre Montón y TRABAJOn
    for(i = TRABAJO1; i <= TRABAJO4; i++)
        if(MovimientoPosible(MONTON, i)) return false;

    // 2) entre Montón y SALIDAn
    for(i = SALIDA1; i <= SALIDA4; i++)
        if(MovimientoPosible(MONTON, i)) return false;

    // 3) entre TRABAJOn y TRABAJOn
    for(i = TRABAJO1; i <= TRABAJO4; i++)
        for(j = TRABAJO1; j <= TRABAJO4; j++)
            if(MovimientoPosible(i, j)) return false;

    // 4) entre TRABAJOn y SALIDAn
    for(i = TRABAJO1; i <= TRABAJO4; i++)
        for(j = SALIDA1; j <= SALIDA4; j++)
            if(MovimientoPosible(i, j)) return false;

    return true;
}
