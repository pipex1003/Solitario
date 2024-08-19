#ifndef GRAFICOS
#define GRAFICOS
#include "tapete.h"
#include <windows.h>

class Coor {
    public:
    Coor(int xi=0, int yi=0) : x(xi), y(yi) {}
    int &X() { return x; }
    int &Y() { return y; }
    private:
    int x, y;//variables donde se almacenan las coordenadas que se trabajaran, 
    //gracias a la sobrecarga de metodos se puede almacenar en dos enteros
};
/*Esta clase es la encargada de actualizar la ventana para que refleje el estado del tapete.*/
class Graficos {
    public:
    Graficos(HINSTANCE hInstance, HWND hwnd, Tapete &tapete);//constructor de la clase que Actualiza los valores recibidos como parámetros
    ~Graficos(); //destructor de la clase que Libera los mapas de bits, y los manipuladores de biblioteca y de pincel.
    void Iniciar();//metodo para Iniciar las coordenadas y regiones asociadas a cada zona. 
    void MostrarTapete();//metodo para Actualizar toda la ventana.
    void Puntuacion();//Muestra los contadores de puntuación de la partida actual.
    void MostrarCarta(const Naipe &carta, Coor c, int n=0)//Muestra el naipe indicado en las coordenadas indicadas
    {
        MostrarCarta(carta, c.X(), c.Y(), n);
    }
    void MostrarBitmap(HBITMAP hbm, int x, int y);//metodo que Muestra en pantalla el mapa de bits indicado, en las coordenadas indicadas.
    void MostrarCarta(const Naipe &carta, int x, int y, int n=0);/*Este método estará sobrecargado para que se puedan especificar
     las coordendas mediante un objeto 'Coor' o mediante dos enteros.*/
    void MostrarMovimiento(int Origen, int Destino);/*metodo que actualiza las zonas de pantalla correspondientes a la pila de origen y
     destino del movimiento de un naipe.*/
    int RatonEnZona(int x, int y);/*metodo que devuelve el identificador de la zona sobre la que se encuentran las coordenadas indicadas,
     o -1 si la pila asociada a esa zona está vacía o si las coordenadas no se corresponden con ninguna zona.*/
    int ComienzaArrastre(int x, int y);/*Llamaremos a este método cuando el usuario pulse el botón izquierdo del ratón
     para saber si se da comienzo a una operación de arrastre. Se comprueba si las coordenadas indicadas corresponden 
     a una pila, y se devuelve el valor de esa pila si es así, o -1 si no. También se actualiza el dato miembro 'desp',
     que nos servirá durante todo el proceso de arrastre del naipe.*/
    void IniciaCaptura(int zona);/*Las consecuencias gráficas del inicio de un arrastre son que se debe borrar 
    el naipe arrastrado, actualizando su área de pantalla, ajustar la región correspondiente a esa pila, capturar
    la zona de pantalla que ocupará la carta a continuación y, finalmente, mostrar la carta.*/
    void EnArrastre(int x, int y);/*Borra la carta en la posición anterior, usando el mapa de bits almacenado en movDC,
    captura el mapa de bits en la próxima posición del naipe arrastrado y vuelve a pintar el naipe en la nueva posición.*/
    int FinArrastre(int x, int y);/*Borra por última vez el naipe, usando el mapa de bits en movDC,
    y calcula la pila de destino del naipe arrastrado. Si no se ha soltado en una pila válida, retorna con -1.*/
    void AjustarRegion(int zona);//Ajusta la región correspondiente a la pila indicada como parámetro.
    Coor CoordenadasZona(int z);//Obtiene las coordenadas de la zona indicada en el parámetro.
    void CambiarDorso(int d);//Cambia la imagen usada para el dorso de las cartas.
    int Dorso()  //Devuelve el valor del identificador del mapa de bits usado para el dorso.
    { return dorso; }
    /*hInstance es un manipulador para la instancia del programa que estamos ejecutando.
     Cada vez que se ejecuta una aplicación, Windows crea una Instancia para ella*/
    HINSTANCE Lib()//Devuelve el manipulador de la biblioteca "cards.dll".
    //La palabra clave const especifica que el valor de una variable es constante e indica al compilador que evite que el programador lo modifique.
    const { return hLib; }//aca se retorna el manipulador para cards.dll
    int W() const { return w; }//retorna el ancho definido para el mapa de bits
    int H() const { return h; }//retorna la altura del mapa de bits
    HBITMAP Mascara() const { return hMascara; }//retorna en si donde se manipula el mapa de bits donde mostramos las cartas en el juego
    HCURSOR Mover() const { return hMover; }//retorna la funcion del movimiento con el raton
    HCURSOR Flecha() const { return hFlecha; }
    private://definicion de variables globales para los metodos
    HINSTANCE hInstance;
    Tapete &tapete;//direccion de memoria del tapete que se usara para actualizar la pantalla
    HBRUSH fondo;//identificador para darle color al tapete
    /* HBITMAP Crea un objeto basado en un identificador para un mapa de bits de interfaz 
    de dispositivo gráfico de Windows*/
    HBITMAP hMascara;//identificador de la mascara para redondear las esquinas de las cartas
    HBITMAP hDorso;//identificador del dorso de la carta, y lo usaremos para mostras cada carta del mazo
    int dorso;//almacena el identificador anterior
    /*las dos siguientes variables manipulan el mapa de bits motrando al fondo de una pila  una X o un O*/
    HBITMAP hX;
    HBITMAP hO;
    HBITMAP hNaipe[52];//array el cual tiene 52 manipuladores para poder controlar cada fase de las cartas
    HWND hWnd;//manipula la ventana donde se muestra el juego
    HDC hDC;//manipula la interfaz de la ventana
    HINSTANCE hLib;//identificador de una instancia El sistema operativo usa este valor para identificar el ejecutable (EXE) cuando se carga en memoria
    int w, h;//variables que almacenan el alto y el ancho del mapa de bits
    Coor zona[10];//guarda las coordenadas de cada pila en un array
    HRGN reg[10];//realiza la misma funcion que el anterior pero ademas sirve parra saber la pila seleccionada e ir actualizando la pantalla
    HBITMAP hArrastre;//identificador del arrastre de la carta,guardaremos lo que hay detrás del naipe para poder restaurar la pantalla en cada fase del movimiento
    HCURSOR hMover;//Manipulador del cursor que usaremos cuando se estén realizando operaciones de arrastre de naipes.
    HCURSOR hFlecha;//Manipulador del cursor que usaremos cuando no se estén realizando operaciones de arrastre.
    // Movimiento de carta:
    Coor raton;//guarda las coordenadas del cursor del usuario cuando realiza un arrastre
    Coor desp;//almacena los datos que se requieren para mover la carta sin importar que parte de esta haya sido pulsada
    HDC movDC;//manipula la memoria y maneja la parte oculta del naipe en el arratre
};
#endif
