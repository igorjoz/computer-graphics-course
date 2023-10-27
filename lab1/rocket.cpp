/******************************************************************
 Grafika komputerowa, środowisko MS Windows - program  przykładowy
 *****************************************************************/

#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;


//deklaracja funkcji obslugi okna
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//funkcja Main - dla Windows
 int WINAPI WinMain(HINSTANCE hInstance,
               HINSTANCE hPrevInstance,
               LPSTR     lpCmdLine,
               int       nCmdShow)
{
	MSG meldunek;		  //innymi slowy "komunikat"
	WNDCLASS nasza_klasa; //klasa głównego okna aplikacji
	HWND okno;
	static char nazwa_klasy[] = "Podstawowa";
	
	GdiplusStartupInput gdiplusParametry;// parametry GDI+; domyślny konstruktor wypełnia strukturę odpowiednimi wartościami
	ULONG_PTR	gdiplusToken;			// tzw. token GDI+; wartość uzyskiwana przy inicjowaniu i przekazywana do funkcji GdiplusShutdown
   
	// Inicjujemy GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusParametry, NULL);

	//Definiujemy klase głównego okna aplikacji
	//Okreslamy tu wlasciwosci okna, szczegoly wygladu oraz
	//adres funkcji przetwarzajacej komunikaty
	nasza_klasa.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	nasza_klasa.lpfnWndProc   = WndProc; //adres funkcji realizującej przetwarzanie meldunków 
 	nasza_klasa.cbClsExtra    = 0 ;
	nasza_klasa.cbWndExtra    = 0 ;
	nasza_klasa.hInstance     = hInstance; //identyfikator procesu przekazany przez MS Windows podczas uruchamiania programu
	nasza_klasa.hIcon         = 0;
	nasza_klasa.hCursor       = LoadCursor(0, IDC_ARROW);
	nasza_klasa.hbrBackground = (HBRUSH) GetStockObject(GRAY_BRUSH);
	nasza_klasa.lpszMenuName  = "Menu" ;
	nasza_klasa.lpszClassName = nazwa_klasy;

    //teraz rejestrujemy klasę okna głównego
    RegisterClass (&nasza_klasa);
	
	/*tworzymy okno główne
	okno będzie miało zmienne rozmiary, listwę z tytułem, menu systemowym
	i przyciskami do zwijania do ikony i rozwijania na cały ekran, po utworzeniu
	będzie widoczne na ekranie */
 	okno = CreateWindow(nazwa_klasy, "Grafika komputerowa", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
						CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	
	
	/* wybór rozmiaru i usytuowania okna pozostawiamy systemowi MS Windows */
   	ShowWindow (okno, nCmdShow) ;
    
	//odswiezamy zawartosc okna
	UpdateWindow (okno) ;

	// GŁÓWNA PĘTLA PROGRAMU
	while (GetMessage(&meldunek, NULL, 0, 0))
     /* pobranie komunikatu z kolejki; funkcja GetMessage zwraca FALSE tylko dla
	 komunikatu WM_QUIT; dla wszystkich pozostałych komunikatów zwraca wartość TRUE */
	{
		TranslateMessage(&meldunek); // wstępna obróbka komunikatu
		DispatchMessage(&meldunek);  // przekazanie komunikatu właściwemu adresatowi (czyli funkcji obslugujacej odpowiednie okno)
	}

	GdiplusShutdown(gdiplusToken);
	
	return (int)meldunek.wParam;
}

/********************************************************************
FUNKCJA OKNA realizujaca przetwarzanie meldunków kierowanych do okna aplikacji*/
LRESULT CALLBACK WndProc (HWND okno, UINT kod_meldunku, WPARAM wParam, LPARAM lParam)
{
	HMENU mPlik, mInfo, mGlowne;
    	
/* PONIŻSZA INSTRUKCJA DEFINIUJE REAKCJE APLIKACJI NA POSZCZEGÓLNE MELDUNKI */
	switch (kod_meldunku) 
	{
	case WM_CREATE:  //meldunek wysyłany w momencie tworzenia okna
		mPlik = CreateMenu();
		AppendMenu(mPlik, MF_STRING, 100, "&Zapiszcz...");
		AppendMenu(mPlik, MF_SEPARATOR, 0, "");
		AppendMenu(mPlik, MF_STRING, 101, "&Koniec");
		mInfo = CreateMenu();
		AppendMenu(mInfo, MF_STRING, 200, "&Autor...");
		mGlowne = CreateMenu();
		AppendMenu(mGlowne, MF_POPUP, (UINT_PTR) mPlik, "&Plik");
		AppendMenu(mGlowne, MF_POPUP, (UINT_PTR) mInfo, "&Informacja");
		SetMenu(okno, mGlowne);
		DrawMenuBar(okno);

	case WM_COMMAND: //reakcje na wybór opcji z menu
		switch (wParam)
		{
		case 100: if(MessageBox(okno, "Zapiszczeć?", "Pisk", MB_YESNO) == IDYES)
					MessageBeep(0);
                  break;
		case 101: DestroyWindow(okno); //wysylamy meldunek WM_DESTROY
        		  break;
		case 200: MessageBox(okno, "Imię i nazwisko: Igor Józefowicz\nNumer indeksu: 193257", "Autor", MB_OK);
		}
		return 0;
	
	case WM_LBUTTONDOWN: //reakcja na lewy przycisk myszki
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		return 0;
	}

	case WM_PAINT:
	{
		PAINTSTRUCT paint;
		HDC kontekst;

		kontekst = BeginPaint(okno, &paint);
		
		// MIEJSCE NA KOD GDI

		//HPEN pioro = CreatePen(PS_SOLID, 10, RGB(255,0,0));
		//SelectObject(kontekst, pioro);

		//MoveToEx(kontekst, 100, 300, NULL);
		//LineTo(kontekst, 800, 300);
			
		//DeleteObject(pioro);

		// utworzenie obiektu umożliwiającego rysowanie przy użyciu GDI+
		// (od tego momentu nie można używać funkcji GDI
		Graphics graphics(kontekst);
			
		// MIEJSCE NA KOD GDI+
		// Brushes
		SolidBrush blueBrush((Color(255, 74, 134, 232)));
		SolidBrush silverBrush((Color(192, 192, 192, 232)));
		SolidBrush redBrush((Color(255, 255, 0, 0)));
		Pen blackPen(Color(255, 0, 0, 0), 3);

		// Offsets, Xs, Ys, Widths, Heights
		int leftOffset = 150;
		int topOffset = 150;

		// body settings
		int const bodyWidth = 100;
		int const bodyHeight = 200;
		int const bodyX = leftOffset;
		int const bodyY = topOffset;

		// body settings
		int const windowWidth = 60;
		int const windowHeight = 60;
		int const windowX = leftOffset + 20;
		int const windowY = topOffset + 10;
			
		// heat shield setting
		int const heatShieldWidth = 100;
		int const heatShieldHeight = 100;
		int const heatShieldX = 0 + leftOffset;
		int const heatShieldY = 0 + topOffset - (heatShieldHeight / 2);

		// booster settings
		int const boosterPointsCount = 4;
		int const boosterHeight = 50;
		Point boosterPoint1(leftOffset + 25, topOffset + bodyHeight);
		Point boosterPoint2(leftOffset + 75, topOffset + bodyHeight);
		Point boosterPoint3(leftOffset + bodyWidth, topOffset  + bodyHeight + boosterHeight);
		Point boosterPoint4(leftOffset, topOffset + bodyHeight + boosterHeight);
		Point boosterPoints[boosterPointsCount] = { boosterPoint1, boosterPoint2, boosterPoint3, boosterPoint4 };
		Point* boosterPolygonPoints = boosterPoints;

		// wings settings
		int const wingPointsCount = 4;
		int const wingHeight = 100;
		int const wingWidth = 100;

		// leftWing settings
		Point leftWingPoint1(leftOffset, topOffset + bodyHeight - wingHeight);
		Point leftWingPoint2(leftOffset, topOffset + bodyHeight);
		Point leftWingPoint3(leftOffset - wingWidth, topOffset + bodyHeight + 50);
		Point leftWingPoint4(leftOffset - wingWidth, topOffset + bodyHeight);
		Point leftWingPoints[wingPointsCount] = { leftWingPoint1, leftWingPoint2, leftWingPoint3, leftWingPoint4 };
		Point* leftWingPolygonPoints = leftWingPoints;

		// rightWing settings
		Point rightWingPoint1(leftOffset + bodyWidth, topOffset + bodyHeight - wingHeight);
		Point rightWingPoint2(leftOffset + bodyWidth, topOffset + bodyHeight);
		Point rightWingPoint3(leftOffset + bodyWidth + wingWidth, topOffset + bodyHeight + 50);
		Point rightWingPoint4(leftOffset + bodyWidth + wingWidth, topOffset + bodyHeight);
		Point rightWingPoints[wingPointsCount] = { rightWingPoint1, rightWingPoint2, rightWingPoint3, rightWingPoint4 };
		Point* rightWingPolygonPoints = rightWingPoints;

		// Draw heat shield of the rocket
		graphics.FillEllipse(&redBrush, heatShieldX, heatShieldY, heatShieldWidth, heatShieldHeight);

		// Draw body of the rocket
		graphics.FillRectangle(&blueBrush, bodyX, bodyY, bodyWidth, bodyHeight);

		// Draw window of the rocket
		graphics.FillEllipse(&silverBrush, windowX, windowY, windowWidth, windowHeight);

		// Draw booster of the rocket
		graphics.FillPolygon(&redBrush, boosterPolygonPoints, boosterPointsCount);

		// Draw left & right wing of the rocket
		graphics.FillPolygon(&blueBrush, leftWingPolygonPoints, wingPointsCount);
		graphics.FillPolygon(&blueBrush, rightWingPolygonPoints, wingPointsCount);

		// Delete brush
		DeleteObject(&blueBrush);
		DeleteObject(&silverBrush);
		DeleteObject(&redBrush);
		DeleteObject(&blackPen);

		// utworzenie czcionki i wypisanie tekstu na ekranie
		FontFamily  fontFamily(L"Times New Roman");
		Font        font(&fontFamily, 24, FontStyleRegular, UnitPixel);
		PointF      pointF(100.0f, 450.0f);
		SolidBrush  solidBrush(Color(255, 255, 255, 255));

		graphics.DrawString(L"Igor Józefowicz, 193257", -1, &font, pointF, &solidBrush);

		EndPaint(okno, &paint);

		return 0;
	}
	case WM_KEYDOWN:
	{
		switch (wParam) {
		case VK_LEFT:
		{
			break;
		}

		case VK_RIGHT:
		{
			Matrix matrix;
			matrix.Translate(200, 200);
			break;
		}

		case VK_UP:
			// Process the UP ARROW key. 
			break;

		case VK_DOWN:
			// Process the DOWN ARROW key. 
			break;
		}

		break;
	}

	case WM_DESTROY: //obowiązkowa obsługa meldunku o zamknięciu okna
		PostQuitMessage(0);
		return 0;

	default: //standardowa obsługa pozostałych meldunków
		return DefWindowProc(okno, kod_meldunku, wParam, lParam);
	}
}
