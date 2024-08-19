#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "graficos.h"
#include "resource.h"
/* Declarar procedimiento de Windows  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DialogoOpciones(HWND, UINT, WPARAM, LPARAM);
void ProcesarScrollV(HWND hDlg, HWND Control, int Codigo, int Posicion);
/*  Convierte el nombre de la clase en una variable global  */
char szClassName[ ] = "Solitario";

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nFunsterStil)
{
    HWND hwnd;               /* Este es el handle de nuestra ventana. */
    MSG messages;            /* Aquí se guardan los mensajes a la aplicación. */
    WNDCLASSEX wincl;        /* Estructura de datos para la clase de ventana. */
    HACCEL hAcelerador;      /* Manipulador de aceleradores de los botones*/

    /* estructura de la ventana */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* Esta función es llamada por windows */
    wincl.style = CS_DBLCLKS;                 /* toma los doble click */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /*Usa el ícono predeterminado y el puntero del mouse de la computadora */
    wincl.hIcon = LoadIcon (hThisInstance, "Icono");
    wincl.hIconSm = LoadIcon (hThisInstance, "Icono");
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = "Menu";               
    wincl.cbClsExtra = 0;                      /* Sin bytes adicionales después de la clase de ventana */
    wincl.cbWndExtra = 0;                      /* la instancia de la ventana*/
    wincl.hbrBackground = CreateSolidBrush(RGB(11, 83, 69));//manipulador para cambiar el color del fondo

    /* Registre la clase de ventana, y si falla, salga del programa */
    if (!RegisterClassEx (&wincl))
        return 0;
    
    //creacion de la ventana
    Beep(1568,200);
    Beep(1568,200);
    Beep(1568,200);
    Beep(1245,200);
    Beep(1397,200);
    Beep(1397,200);
    Beep(1397,200);
    Beep(1175,200);
    hwnd = CreateWindowEx (
           0,                   /* Posibilidades ampliadas de variación */
           szClassName,         /* Nombre de la clase */
           "solitario(universidas distrital)",/* titulo de la ventana*/
           WS_OVERLAPPEDWINDOW, /* ventana predeterminada */
           CW_USEDEFAULT,       /* Windows decide la posición donde aparece la ventana*/
           CW_USEDEFAULT,       /* donde termina la ventana en la pantalla*/
           500,                 /* El ancho de la ventana */
           500,                 /* y altura en píxeles */
           HWND_DESKTOP,        /* La ventana es una ventana secundaria al escritorio. */
           NULL,               /*no menu de windows*/
           hThisInstance,       /* instancia del handler */
           NULL                 /* Sin datos de creación de ventanas */
           );

   
    ShowWindow (hwnd, nFunsterStil);/*Hacer que la ventana sea visible en la pantalla*/
    hAcelerador = LoadAccelerators(hThisInstance, "Aceleradores");
    char soundfile[] = "daga.wav";
    std::cout<<PlaySound((LPCSTR)soundfile,NULL,SND_FILENAME | SND_ASYNC);
    /* Ejecutar el bucle de mensajes hasta que GetMessage() retorno con un
    valor distinto de TRUE (-1 para error, o FALSE)  */
    while (TRUE == GetMessage (&messages, NULL, 0, 0))
    {
        if(!TranslateAccelerator(hwnd, hAcelerador, &messages)) {
           /* Traducir mensajes de teclas virtuales a mensajes de caracteres
              sólo si TranslateAccelerator regresa con nulo */
           TranslateMessage(&messages);
           /* Enviar el mensaje a WindowProcedure */
           DispatchMessage(&messages);
        }
    }

    /* El valor de retorno del programa es 0 - El valor que dio PostQuitMessage() */
    return messages.wParam;
}


/* Esta función es llamada por la función de Windows DispatchMessage() */
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static Tapete tapete;
    static Graficos *graficos;
    static HINSTANCE hInstance;
    PAINTSTRUCT ps;
    int zona, destino;
    Movimiento mov;
    POINT punto;
    char titulo[64], msg[128];

    switch (message)                  /* manejo de los mensajes */
    {
        case WM_CREATE:
            hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
            srand(time(0));
            graficos = new Graficos(hInstance, hwnd, tapete);
            MoveWindow(hwnd, 30, 30, 500, 500, FALSE);//definir tamaño de pantalla al iniciar
            tapete.Iniciar();
            return false;
        case WM_LBUTTONDBLCLK:
            // La función ComienzaArrastre sirve para saber sobre qué
            // pila se encuentra el cursor:
            zona = graficos->RatonEnZona(LOWORD(lParam), HIWORD(lParam));
            // Si no es la zona del MAZO, verificar si se puede extraer la carta:
            if(zona != -1 && zona != MAZO) {
                destino = tapete.VerificarDobleClick(zona);
                if(destino != -1) {
                    tapete.MoverNaipe(zona, destino);
                    graficos->MostrarMovimiento(zona, destino);
                    if(tapete.Terminado()) {
                        LoadString(hInstance, ID_RESUELTO, msg, 128);
                        MessageBox(hwnd, msg, titulo, MB_OK);
                    } else if(tapete.SinSolucion()) {
                        LoadString(hInstance, ID_SINSOLUCION, msg, 128);
                        MessageBox(hwnd, msg, titulo, MB_OK);
                    }
                }
            }
            break;
        case WM_LBUTTONDOWN:
            zona = graficos->ComienzaArrastre(LOWORD(lParam), HIWORD(lParam));
            if(zona == MAZO) {
                tapete.MoverNaipe(MAZO, MONTON);
                graficos->MostrarMovimiento(MAZO, MONTON);
                if(tapete.SinSolucion()) {
                    LoadString(hInstance, ID_SINSOLUCION, msg, 128);
                    MessageBox(hwnd, msg, titulo, MB_OK);
                }
            } else if(zona > MAZO && tapete.CogeNaipe(zona)) {
                SetCursor(graficos->Mover());
                SetCapture(hwnd);
                graficos->IniciaCaptura(zona);
            }
            break;
        case WM_MOUSEMOVE:
            if(tapete.EnArrastre()) {
                graficos->EnArrastre(LOWORD(lParam), HIWORD(lParam));
            }
            break;
        case WM_LBUTTONUP:
            if(tapete.EnArrastre()) {
                zona = graficos->FinArrastre(LOWORD(lParam), HIWORD(lParam));
                ReleaseCapture();
                // Si el movimiento NO es legal, devolver naipe a origen:
                if(zona == -1 || !tapete.MovimientoLegal(zona)) {
                    zona = tapete.Origen();
                }
                tapete.SueltaNaipe(zona);
                graficos->MostrarMovimiento(-1, zona);
                SetCursor(graficos->Flecha());
                if(tapete.Terminado()) {
                    LoadString(hInstance, ID_RESUELTO, msg, 128);
                    MessageBox(hwnd, msg, titulo, MB_OK);
                } else if(tapete.SinSolucion()) {
                    LoadString(hInstance, ID_SINSOLUCION, msg, 128);
                    MessageBox(hwnd, msg, titulo, MB_OK);
                }
            }
            break;
        case WM_PAINT:
            BeginPaint(hwnd, &ps);
            graficos->MostrarTapete();
            EndPaint(hwnd, &ps);
            break;
        case WM_COMMAND:
            switch(LOWORD(wParam)) {
                case CMD_NUEVO:
                    tapete.Limpiar();
                    tapete.Iniciar();
                    graficos->Iniciar();
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case CMD_AYUDA:
                    DialogBox(hInstance, "DlgAyuda", hwnd, DialogProc);
                    break;
               case CMD_DESHACER:
                   mov = tapete.DesHacer();
                    if(mov.Origen() != -1 )
                       graficos->MostrarMovimiento(mov.Destino(), mov.Origen());
                    break;
                case CMD_SOBRE:
                    DialogBox(hInstance, "DlgSobre", hwnd, DialogProc);
                    break;
                    break;
                case CMD_SALIR:
                    PostQuitMessage (0);
                    break;
            }
            break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* enviar un WM_QUIT a la cola de mensajes */
            break;
        default:                      /* para mensajes que no manejamos en el programa */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

BOOL CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg)                  // manipulador del mensaje 
    {
        case WM_COMMAND:
           EndDialog(hDlg, FALSE);
           return TRUE;
    }
    return FALSE;
}

BOOL CALLBACK DialogoOpciones(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static Graficos *graficos;
    static int valor;
    SCROLLINFO siv = {
       sizeof(SCROLLINFO),
       SIF_POS | SIF_RANGE | SIF_PAGE,
       53, 69,
       5,
       0,
       0};
    PAINTSTRUCT ps;
    HDC hDC;
    HDC memDC;
    POINT punto[3];
    HBITMAP hDorso;

    switch (uMsg)                  /* manipulador del mensaje */
    {
        case WM_INITDIALOG:
            graficos = (Graficos*)lParam;
           siv.nPos = valor = graficos->Dorso();
            SetScrollInfo(GetDlgItem(hDlg, IDC_OPC_SCROLL), SB_CTL, &siv, TRUE);
            break;
        case WM_VSCROLL:
            ProcesarScrollV(hDlg, (HWND)lParam,
               (int)LOWORD(wParam), (int)HIWORD(wParam));
            valor = SendDlgItemMessage(hDlg, IDC_OPC_SCROLL, SBM_GETPOS, 0, 0);
            InvalidateRect(hDlg, NULL, TRUE);
            return FALSE;
        case WM_PAINT:
            hDC = BeginPaint(hDlg, &ps);
            hDorso = LoadBitmap(graficos->Lib(), MAKEINTRESOURCE(valor));
            memDC = CreateCompatibleDC(hDC);
            punto[0].x = 50;
            punto[0].y = 20;
            punto[1].x = punto[0].x+graficos->W();
            punto[1].y = punto[0].y;
            punto[2].x = punto[0].x;
            punto[2].y = punto[0].y+graficos->H();
            SelectObject(memDC, hDorso);
            PlgBlt(hDC, punto, memDC, 0, 0, graficos->W(), graficos->H(),
                graficos->Mascara(), 0, 0);
            DeleteObject(hDorso);
            DeleteDC(memDC);
            EndPaint(hDlg, &ps);
            break;
        case WM_COMMAND:
            switch(LOWORD(wParam)) {
                case IDCANCEL:
                    EndDialog(hDlg, FALSE);
                    break;
            }
            return TRUE;
    }
    return FALSE;
}
void ProcesarScrollV(HWND hDlg, HWND Control, int Codigo, int Posicion) {
   SCROLLINFO si = {
       sizeof(SCROLLINFO),
       SIF_ALL, 0, 0, 0, 0, 0};
   GetScrollInfo(Control, SB_CTL, &si);
   switch(Codigo) {
      case SB_BOTTOM:
         si.nPos = si.nMin;
         break;
      case SB_TOP:
         si.nPos = si.nMax;
         break;
      case SB_LINEDOWN:
         si.nPos++;
         break;
      case SB_LINEUP:
         si.nPos--;
         break;
      case SB_PAGEDOWN:
         si.nPos += si.nPage;
         break;
      case SB_PAGEUP:
         si.nPos -= si.nPage;
         break;
      case SB_THUMBPOSITION:
      case SB_THUMBTRACK:
         si.nPos = Posicion;
      case SB_ENDSCROLL:
         break;
   }
   if(si.nPos < si.nMin) si.nPos = si.nMin;
   if(si.nPos > si.nMax-si.nPage+1) si.nPos = si.nMax-si.nPage+1;
   SetScrollInfo(Control, SB_CTL, &si, true);
}
