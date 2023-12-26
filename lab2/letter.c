
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

int red, green, blue, yellow;
unsigned long foreground, background;


//*************************************************************************************************************************
//funkcja przydzielania kolorow

int AllocNamedColor(char *name, Display* display, Colormap colormap) {
    XColor col;
    XParseColor(display, colormap, name, &col);
    XAllocColor(display, colormap, &col);
    return col.pixel;
} 

//*************************************************************************************************************************
// inicjalizacja zmiennych globalnych okreslajacych kolory

int init_colors(Display* display, int screen_no, Colormap colormap) {
    background = WhitePixel(display, screen_no);  //niech tlo bedzie biale
    foreground = BlackPixel(display, screen_no);  //niech ekran bedzie czarny
    red=AllocNamedColor("red", display, colormap);
    green=AllocNamedColor("green", display, colormap);
    blue=AllocNamedColor("blue", display, colormap);
    yellow=AllocNamedColor("yellow", display, colormap);
}

int demoRectangles(Display* display, Window window, GC gc) {
    int i, num_rectangles = 30;
    int width, height, range = 10;
    float x, y, std = 1;
    float bar_width;
    XWindowAttributes attributes;

    XGetWindowAttributes(display, window, &attributes);
    width = attributes.width;
    height = attributes.height;
    bar_width = width / num_rectangles - 2;
    
    for(i = 0; i < num_rectangles; i++)
    {
        XSetForeground(display, gc, rand());
        x = (float)i * range / num_rectangles; 
        y = height * exp(- (x - range/2) * (x - range/2) / (2 * std * std));
        XFillRectangle(display, window, gc, i * (bar_width + 2), height - y, bar_width, y); 
    }
}


int drawLetterA(Display* display, Window window, GC gc) {
    int xStart = 0;
    int yStart = 0;

    int letterHeight = 200;
    int gapHeight = 50;

    int topOffset = 50;
    int topWidth = 50;
    int letterWidth = 300;
    
    // A letter points
    XPoint points[8];
    points[0].x = topOffset;
    points[0].y = 0;

    points[1].x = topOffset + topWidth;
    points[1].y = 0;

    points[2].x = letterWidth / 2;
    points[2].y = letterHeight;
    
    points[3].x = 100;
    points[3].y = letterHeight;

    points[4].x = 100;
    points[4].y = letterHeight - gapHeight;

    points[5].x = 50;
    points[5].y = letterHeight - gapHeight;

    points[6].x = 50;
    //points[6].y = letterHeight - 50 * 3;
    points[6].y = letterHeight;

    points[7].x = 0;
    points[7].y = letterHeight;

    // points[0].x = 100;
    // points[0].y = 0;

    // points[1].x = 150;
    // points[1].y = 0;

    // points[2].x = 250;
    // points[2].y = 200;
    
    // points[3].x = 200;
    // points[3].y = 200;

    // points[4].x = 150;
    // points[4].y = 150;

    // points[5].x = 100;
    // points[5].y = 150;

    // points[6].x = 50;
    // points[6].y = 200;

    // points[7].x = 0;
    // points[7].y = 200;

    // Middle triangle points
    XPoint trianglePoints[3];

    trianglePoints[0].x = 75;
    trianglePoints[0].y = 50;

    trianglePoints[1].x = 55;
    trianglePoints[1].y = 100;

    trianglePoints[2].x = 95;
    trianglePoints[2].y = 100;

    // draw A letter
    XSetForeground(display, gc, rand());
    XFillPolygon(display, window, gc, points, 8, Convex, CoordModeOrigin);

    // draw middle triangle
    XSetForeground(display, gc, 0xfffffff);
    XFillPolygon(display, window, gc, trianglePoints, 3, Convex, CoordModeOrigin);

    XSetForeground(display, gc, 0xffffff);
    XFillRectangle(display, window, gc, 50, 150, 50, 50); 

    XSetForeground(display, gc, rand());
    XFillArc(display, window, gc, 50, 120, 50, 50, 180 * 64, 180 * 64);

}



//*************************************************************************************************************************
// Glowna funkcja zawierajaca petle obslugujaca zdarzenia */

int main(int argc, char *argv[]) {
    char            icon_name[] = "Grafika";
    char            title[]     = "Grafika komputerowa";
    Display*        display;    //gdzie bedziemy wysylac dane (do jakiego X servera)
    Window          window;     //nasze okno, gdzie bedziemy dokonywac roznych operacji
    GC              gc;         //tu znajduja sie informacje o parametrach graficznych
    XEvent          event;      //gdzie bedziemy zapisywac pojawiajace sie zdarzenia
    KeySym          key;        //informacja o stanie klawiatury 
    Colormap        colormap;
    int             screen_no;
    XSizeHints      info;       //informacje typu rozmiar i polozenie ok

    char            info1[]     = "LEWY przycisk  - PROSTOKATY";
    char            info2[]     = "PRAWY przycisk - ELIPSY";
    
    char            buffer[8];  //gdzie bedziemy zapamietywac znaki z klawiatury
    int             hm_keys;    //licznik klawiszy
    int             to_end;

    display    = XOpenDisplay("");                //otworz polaczenie z X serverem pobierz dane od zmiennej srodowiskowej DISPLAY ("")
    screen_no  = DefaultScreen(display);          //pobierz domyslny ekran dla tego wyswietlacza (0)
    colormap = XDefaultColormap(display, screen_no);
    init_colors(display, screen_no, colormap);

    //okresl rozmiar i polozenie okna
    info.x = 100;
    info.y = 150;
    info.width = 1000;
    info.height = 700;
    info.flags = PPosition | PSize;

    //majac wyswietlacz, stworz okno - domyslny uchwyt okna
    window = XCreateSimpleWindow(display, DefaultRootWindow(display),info.x, info.y, info.width, info.height, 7/* grubosc ramki */, foreground, background);
    XSetStandardProperties(display, window, title, icon_name, None, argv, argc, &info);
    //utworz kontekst graficzny do zarzadzania parametrami graficznymi (0,0) domyslne wartosci
    gc = XCreateGC(display, window, 0, 0);
    XSetBackground(display, gc, background);
    XSetForeground(display, gc, foreground);

    // // A letter points
    // XPoint points[4];
    // points[0].x = 100;
    // points[0].y = 100;
    // points[1].x = 150;
    // points[1].y = 150;
    // points[2].x = 200;
    // points[2].y = 200;
    // points[3].x = 250;
    // points[3].y = 250;

    // // draw A letter
    // XFillPolygon(display, window, gc, points, 4, Convex, CoordModeOrigin);

    //okresl zdarzenia jakie nas interesuja, np. nacisniecie klawisza
    XSelectInput(display, window, (KeyPressMask | ExposureMask | ButtonPressMask| ButtonReleaseMask | Button1MotionMask));
    XMapRaised(display, window);  //wyswietl nasze okno na samym wierzchu wszystkich okien
        
    to_end = FALSE;

    /* petla najpierw sprawdza, czy warunek jest spelniony
        i jesli tak, to nastepuje przetwarzanie petli
        a jesli nie, to wyjscie z petli, bez jej przetwarzania */
    while (to_end == FALSE) {
        XNextEvent(display, &event);  // czekaj na zdarzenia okreslone wczesniej przez funkcje XSelectInput

        switch(event.type) {
        case Expose:
            if (event.xexpose.count == 0) {
                XSetForeground(display, gc, foreground);
                XClearWindow(display, window);
                
                //demoRectangles(display, window, gc);

                drawLetterA(display, window, gc);
            }
            break;

        case MappingNotify:
            // zmiana ukladu klawiatury - w celu zabezpieczenia sie przed taka zmiana trzeba to wykonac
            XRefreshKeyboardMapping(&event.xmapping);
            break;

        // sprawdzenie czy wcisnieto lewy przycisk
        case ButtonPress:
            if (event.xbutton.button == Button1) {
                        
        
            }
            break;

        case KeyPress:
            hm_keys = XLookupString(&event.xkey, buffer, 8, &key, 0);
            if (hm_keys == 1) {
            if (buffer[0] == 'q') to_end = TRUE;        // koniec programu
            
            }

        default:
            break;
        }
    }

    XFreeGC(display, gc);
    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}
