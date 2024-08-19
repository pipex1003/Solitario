#ifndef BARAJA
#define BARAJA
//definimos un numero para cada figura de las cartas
#define TREBOLES  0
#define DIAMANTES 1
#define CORAZONES 2
#define PICAS     3
class Naipe {
    public:
    //MEOTODOS PARA INCICIAR LA CARTA BOCABAJO EN LA BARAJA
    Naipe() : palo(TREBOLES), numero(0), visible(false) {}//inicia la carta en trebol
    Naipe(int p, int n, int v=false) : palo(p), numero(n), visible(v) {}
    Naipe Sucesor() const;//define el orden de salida del as cartas, donde K tiene un sucesor nulo
    Naipe Predecesor() const;//define la carta anterior en la pila donde el del as tiene un valor nulo
    int Palo() const { return palo; }//un getter que retorna el valor del palo
    int Numero() const { return numero; }//un getter que devuelve el numero de la carta
    int Orden() const;//contructor que genera Número calculado correspondiente al gráfico dentro de la DLL de cards.dll.
    bool Visible() const { return visible; }//retorna un valor segun si la carta esta boca arriba o no
    void Volver() { visible = !visible; }//si la carta esta boca abajo la gira o viceversa
    void Levantar() { visible = true; }//le da un valor true al metodo visible colocando bocarriba la carta
    void Ocultar() { visible = false; }//por el contrario este le da un valor false para colocar la carta bocaabajo
    bool operator==(Naipe n) { return palo==n.palo && numero==n.numero; }//se sobrecarga == para hacer la comparacion de naipes
    //atrubutos de cada carta
    private:
    int palo;//el tipo de carta ya sea de diamantes, corazones o picas
    int numero;//guarda numeros del 1 al 10 donde del 8 al 10 se almacenan las figuras
    bool visible;//indica si la carta esta boca arriba
};
class Baraja {
    public:
    Baraja();//se insertan las 40 cartas de la baraja con numeros de 1 a 7 y las figuras
    void Mezclar();//hace una mezcla de cartas cambiandolas de posicion dentro del array por medio de un algoritmo de mezcla
    const Naipe &Carta(int i) const { return baraja[i]; }//obtiene la direccion de la carta en la baraja

    private:
    Naipe baraja[40];//array donde se almacenan todas las cartas
};

#endif
