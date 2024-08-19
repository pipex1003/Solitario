#ifndef TAPETE
#define TAPETE
#include "pila.h"
#include "baraja.h"

#define MAZO        0
#define MONTON      1
#define SALIDA1     2
#define SALIDA2     3
#define SALIDA3     4
#define SALIDA4     5
#define TRABAJO1    6
#define TRABAJO2    7
#define TRABAJO3    8
#define TRABAJO4    9

class Movimiento {
    public:
    Movimiento() : origen(-1), destino(-1) {}
    Movimiento(int o, int d) : origen(o), destino(d) {}
    const int Origen() const { return origen; }
    const int Destino() const { return destino; }

    private:
    int origen;
    int destino;
};

class Tapete {
    public:
    Tapete() : baraja(), origen(-1) {}
    void Limpiar();
    void Iniciar();
    bool MoverNaipe(int origen, int destino);
    Movimiento DesHacer();
    pila<Naipe> &Pila(int i) { return zona[i]; }
    bool CogeNaipe(int zona);
    void SueltaNaipe(int zona);
    bool EnArrastre() const { return origen != -1; }
    int VerificarDobleClick(int zona);
    int Origen() const { return origen; }
    const Naipe &Mano() const { return mano; }
    bool MovimientoLegal(const int zona);
    bool Terminado();
    bool SinSolucion();
    bool MovimientoPosible(const int o, const int d);

    private:
    Baraja baraja;
    pila<Naipe> zona[10];
    pila<Movimiento> movimiento;
    Naipe mano;
    int origen; // identificador de pila o -1 (ninguna)
};

#endif
