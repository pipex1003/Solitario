#include <cstdio>
#include "graficos.h"
#include "resource.h"

using namespace std;

Graficos::Graficos(HINSTANCE hInst, HWND hwnd, Tapete &t) :
    hWnd(hwnd), hInstance(hInst), tapete(t), dorso(64) {
    BITMAP bmp;
    int i;
    int n;

    hDC = GetDC(hWnd);
    movDC = CreateCompatibleDC(hDC);
    hLib = LoadLibrary("cards.dll");
    hMascara = LoadBitmap(hInstance, "Mascara");
   hDorso = LoadBitmap(hLib, MAKEINTRESOURCE(dorso));
    hX = LoadBitmap(hLib, MAKEINTRESOURCE(67)); // X
    hO = LoadBitmap(hLib, MAKEINTRESOURCE(68)); // O
    for(i = 0; i < 52; i++)
        hNaipe[i] = LoadBitmap(hLib, MAKEINTRESOURCE(i+1));

    fondo = CreateSolidBrush(RGB(0, 127, 0));
    GetObject(hMascara, sizeof(bmp), &bmp);
    w  = bmp.bmWidth;
    h = bmp.bmHeight;
    hArrastre = CreateCompatibleBitmap(hDC, w, h);
    SelectObject(movDC, hArrastre);
    hMover = LoadCursor(hInstance, "Mover");
    hFlecha = LoadCursor (NULL, IDC_ARROW);
    Iniciar();
}

Graficos::~Graficos() {
    for(int i = 0; i < 52; i++) DeleteObject(hNaipe[i]);
    DeleteObject(hO);
    DeleteObject(hX);
    DeleteObject(fondo);
    DeleteObject(hMascara);
    DeleteObject(hDorso);
    FreeLibrary(hLib);
    DeleteObject(hArrastre);
    DeleteObject(hMover);
    DeleteObject(hFlecha);
    DeleteDC(movDC);
    ReleaseDC(hWnd, hDC);
}

void Graficos::Iniciar() {
    int i;

    zona[MAZO] = Coor(10, 10);
    zona[MONTON] = Coor(20+w, 10);
    for(i = 0; i < 4; i++) {
        zona[TRABAJO1+i] = Coor(10+(i+2)*(w+10), 20+h);
        zona[SALIDA1+i]  = Coor(10+(i+2)*(w+10), 10);
    }
    // Actualizar regiones:
    for(i = MAZO; i <= TRABAJO4; i++) {
        reg[i] = CreateRectRgn(zona[i].X(), zona[i].Y(),
          zona[i].X()+w, zona[i].Y()+h);
    }
}
void Graficos::MostrarBitmap(HBITMAP hbm, int x, int y) {
    HDC memDC;
    POINT punto[3];

    memDC = CreateCompatibleDC(hDC);
    punto[0].x = x;
    punto[0].y = y;
    punto[1].x = punto[0].x+w;
    punto[1].y = punto[0].y;
    punto[2].x = punto[0].x;
    punto[2].y = punto[0].y+h;
    SelectObject(memDC, hbm);
    PlgBlt(hDC, punto, memDC, 0, 0, w, h, (HBITMAP)hMascara, 0, 0);
    DeleteDC(memDC);
}

void Graficos::MostrarCarta(const Naipe &carta, int x, int y, int n) {
    HBITMAP hBitmap;

    if(carta.Visible())
       hBitmap = hNaipe[carta.Orden()];
    else
     hBitmap = hDorso;
    MostrarBitmap(hBitmap, x, y+n*25);
}

void Graficos::MostrarTapete() {
    int n;

    // Mostrar cartas de MAZO a TRABAJO:
    for(int i = MAZO; i <= SALIDA4; i++) {
        // Si no está vacio, mostrar primera carta:
        if(!tapete.Pila(i).Vacia()) {
           tapete.Pila(i).Ultimo();
           MostrarCarta(tapete.Pila(i).Actual(), zona[i]);
        } else {
           if(i == MAZO) MostrarBitmap(hX, zona[i].X(), zona[i].Y());
           else MostrarBitmap(hO, zona[i].X(), zona[i].Y());
        }
    }
    // Mostrar cartas de SALIDA:
    for(int i = TRABAJO1; i <= TRABAJO4; i++) {
        // Si no está vacio, mostrar primera carta:
        if(!tapete.Pila(i).Vacia()) {
            n = 0;
            tapete.Pila(i).Primero();
            do {
                MostrarCarta(tapete.Pila(i).Actual(), zona[i], n++);
            } while(tapete.Pila(i).Siguiente());
        } else {
            MostrarBitmap(hO, zona[i].X(), zona[i].Y());
        }
    }
    Puntuacion();
}

void Graficos::Puntuacion() {
    HBRUSH ant;
    char cad[64];
    char msg[128];
    ant = (HBRUSH)SelectObject(hDC, fondo);
    Rectangle(hDC, 10, 120, 160, 200);
    SetBkMode(hDC, TRANSPARENT);
    LoadString(hInstance, ID_CARTASMAZO, msg, 128);
    sprintf(cad, msg, tapete.Pila(MAZO).Cuenta());
    TextOut(hDC, 15, 130, cad, strlen(cad));
    LoadString(hInstance, ID_CARTASMONTON, msg, 128);
    sprintf(cad, msg, tapete.Pila(MONTON).Cuenta());
    TextOut(hDC, 15, 150, cad, strlen(cad));
    LoadString(hInstance, ID_CARTASCOLOCADAS, msg, 128);
    sprintf(cad, msg,
       tapete.Pila(SALIDA1).Cuenta() + tapete.Pila(SALIDA2).Cuenta() +
       tapete.Pila(SALIDA3).Cuenta() + tapete.Pila(SALIDA4).Cuenta());
    TextOut(hDC, 15, 170, cad, strlen(cad));
    SelectObject(hDC, ant);
}

void Graficos::AjustarRegion(int z) {
    SetRectRgn(reg[z], zona[z].X(), zona[z].Y(),
        zona[z].X()+w, zona[z].Y()+h);
    if(z >= TRABAJO1 && z <= TRABAJO4 && tapete.Pila(z).Cuenta())
        OffsetRgn(reg[z], 0, 25*(tapete.Pila(z).Cuenta()-1));
}

void Graficos::MostrarMovimiento(int Origen, int Destino) {
    FillRgn(hDC, reg[Origen], fondo);

    if(Origen != -1)
    if(Origen >= TRABAJO1 && Origen <= TRABAJO4) {
        if(!tapete.Pila(Origen).Vacia()) {
            tapete.Pila(Origen).Ultimo();
            MostrarCarta(tapete.Pila(Origen).Actual(), zona[Origen], tapete.Pila(Origen).Cuenta()-1);
            // Actualizar Región:
            AjustarRegion(Origen);
        } else { // Pila vacía
            MostrarBitmap(hO, zona[Origen].X(), zona[Origen].Y());
        }
    } else {
        if(!tapete.Pila(Origen).Vacia()) {
            tapete.Pila(Origen).Ultimo();
            MostrarCarta(tapete.Pila(Origen).Actual(), zona[Origen]);
        } else {
            if(Origen == MAZO)
                MostrarBitmap(hX, zona[Origen].X(), zona[Origen].Y());
            else
                MostrarBitmap(hO, zona[Origen].X(), zona[Origen].Y());
        }
    }

    if(Destino != -1)
    if(Destino >= TRABAJO1 && Destino <= TRABAJO4) {
        tapete.Pila(Destino).Ultimo();
        MostrarCarta(tapete.Pila(Destino).Actual(), zona[Destino], tapete.Pila(Destino).Cuenta()-1);
        // Actualizar Región:
        AjustarRegion(Destino);
    } else {
        tapete.Pila(Destino).Ultimo();
        MostrarCarta(tapete.Pila(Destino).Actual(), zona[Destino]);
    }
    Puntuacion();
}

int Graficos::RatonEnZona(int x, int y) {
    int retval = -1;
    // Sólo se pueden mover naipes a partir del montón o
    // de una de las pilas de trabajo, si tienen algo:
    if(PtInRegion(reg[MAZO], x, y)) retval = MAZO;
    else if(PtInRegion(reg[MONTON], x, y)) retval = MONTON;
    else
    for(int i = TRABAJO1; retval && i <= TRABAJO4; i++)
        if(PtInRegion(reg[i], x, y)) {
            retval = i;
        }
    if(tapete.Pila(retval).Vacia()) retval = -1;
    return retval;
}

int Graficos::ComienzaArrastre(int x, int y) {
    int z;

    z = RatonEnZona(x, y);
    if(z != -1) {
        raton = Coor(x, y);
        desp = Coor(x - CoordenadasZona(z).X(), y - CoordenadasZona(z).Y());
    }
    return z;
}

void Graficos::IniciaCaptura(int z) {
    FillRgn(hDC, reg[z], fondo);
    if(z >= TRABAJO1 && z <= TRABAJO4) {
        if(!tapete.Pila(z).Vacia()) {
            tapete.Pila(z).Ultimo();
            MostrarCarta(tapete.Pila(z).Actual(), zona[z], tapete.Pila(z).Cuenta()-1);
            // Actualizar Región:
            AjustarRegion(z);
        } else { // Pila vacía
            MostrarBitmap(hO, zona[z].X(), zona[z].Y());
        }
    } else {
        if(!tapete.Pila(z).Vacia()) {
            tapete.Pila(z).Ultimo();
            MostrarCarta(tapete.Pila(z).Actual(), zona[z]);
        } else {
            if(z == MAZO)
                MostrarBitmap(hX, zona[z].X(), zona[z].Y());
            else
            MostrarBitmap(hO, zona[z].X(), zona[z].Y());
        }
    }

    BitBlt(movDC, 0, 0, w, h, hDC, raton.X()-desp.X(), raton.Y()-desp.Y(), SRCCOPY);
    MostrarCarta(tapete.Mano(), raton.X()-desp.X(), raton.Y()-desp.Y());
}

void Graficos::EnArrastre(int x, int y) {
    BitBlt(hDC, raton.X()-desp.X(), raton.Y()-desp.Y(), w, h, movDC, 0, 0, SRCCOPY);
    raton = Coor(x, y);
    BitBlt(movDC, 0, 0, w, h, hDC, raton.X()-desp.X(), raton.Y()-desp.Y(), SRCCOPY);
    MostrarCarta(tapete.Mano(), raton.X()-desp.X(), raton.Y()-desp.Y());
}

int Graficos::FinArrastre(int x, int y) {
    RECT re = {x+w/2-5-desp.X(), y-desp.Y(),
       x+w/2+5-desp.X(), y+h-desp.Y()};

    BitBlt(hDC, raton.X()-desp.X(), raton.Y()-desp.Y(), w, h, movDC, 0, 0, SRCCOPY);
    for(int i = SALIDA1; i <= TRABAJO4; i++)
        if(RectInRegion(reg[i], &re)) return i;
    return -1;
}

Coor Graficos::CoordenadasZona(int z) {
    Coor res;

    res = zona[z];
    if(z >= TRABAJO1 && z <= TRABAJO4)
        res.Y() += 25*(tapete.Pila(z).Cuenta()-1);
    return res;
}
