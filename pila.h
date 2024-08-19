#ifndef PILA
#define PILA
template<class TIPO> class pila;
template<class TIPO>
class nodo {
    public:
    nodo(TIPO v, nodo<TIPO> *sig = 0, nodo<TIPO> *ant = 0) :
       valor(v), siguiente(sig), anterior(ant) {}

    private:
    TIPO valor;
    nodo<TIPO> *siguiente;
    nodo<TIPO> *anterior;

    friend class pila<TIPO>;
};

template<class TIPO>
class pila {
    public:
    pila() : ultimo(0), primero(0), actual(0), cuenta(0) {}//constructor de la clase
    ~pila();//destructor de la clase

    void Push(TIPO v);//pone una carta en la pila
    TIPO Pop();//quita una carta de la pila
    void Vaciar();//vacia toda la pila al iniciar nueva partida
    bool Vacia();//retorna verdadero si la pila esta vacia
    bool Primero();//puntero que apunta al primero de la pila
    bool Ultimo();//puntero que apunta al ultimo nodo de la pila
    bool Siguiente();//avanza al siguiente elemento de la pila
    const TIPO &Actual() const;//apunta al nodo actual en la secuencia del recorrido
    const int Cuenta() const { return cuenta; }//retorna el numero de elementos de la pila
    private:
    //nodos que podemos referenciar como las cartas en la pila
    nodo<TIPO> *ultimo;
    nodo<TIPO> *primero;
    nodo<TIPO> *actual;
    int cuenta;//numero de elementos de la pila
};

template<class TIPO>
pila<TIPO>::~pila() {
    Vaciar();
}

template<class TIPO>
void pila<TIPO>::Vaciar() {
    while(ultimo) Pop();
    cuenta = 0;
}

template<class TIPO>
void pila<TIPO>::Push(TIPO v) {
    nodo<TIPO> *nuevo;

    /* Crear un nodo nuevo */
    nuevo = new nodo<TIPO>(v, 0, ultimo);
    /* Si la pila estaba vacía, el nuevo nodo es el pripero */
    if(!primero) primero = nuevo;
    /* Actualizar puntero siguiente de último, si existe */
    if(ultimo) ultimo->siguiente = nuevo;
    /* Ahora, el comienzo de nuestra pila es en nuevo nodo */
    ultimo = nuevo;
    cuenta++;
}

template<class TIPO>
TIPO pila<TIPO>::Pop() {
    nodo<TIPO> *Nodo; /* variable auxiliar para manipular nodo */
    TIPO v;      /* variable auxiliar para retorno */

    /* Nodo apunta al primer elemento de la pila */
    Nodo = ultimo;
    /* Asignamos a pila toda la pila menos el primer elemento */
    ultimo = Nodo->anterior;
    if(!ultimo) primero = actual = 0;
    else ultimo->siguiente = 0;
    /* Guardamos el valor de retorno */
    v = Nodo->valor;
    /* Borrar el nodo */
    delete Nodo;
    cuenta--;
    return v;
}

template<class TIPO>
bool pila<TIPO>::Vacia() {
    return primero == 0;
}

template<class TIPO>
bool pila<TIPO>::Primero() {
   actual = primero;
   return actual != 0;
}

template<class TIPO>
bool pila<TIPO>::Siguiente() {
   actual = actual->siguiente;
   return actual != 0;
}

template<class TIPO>
bool pila<TIPO>::Ultimo() {
   actual = ultimo;
   return actual != 0;
}

template<class TIPO>
const TIPO &pila<TIPO>::Actual() const {
   return actual->valor;
}
#endif
