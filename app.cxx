#include "app.h"
#include "kolory.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <stdint.h>
#include <cmath>
#include <FL/Fl_Image.H>
#include <FL/Fl_BMP_Image.H>
#include <FL/Fl.H>

extern App *MyApp;

using namespace std;

App::App()
{
	/*
	 * Domy¶lna konfiguracja programu.
	 *
	 */

	for (unsigned int n = 0; n < sizeof(TABELA) / sizeof(*TABELA); n++)
		TABELA[n] = Macierz();
	opcje.siatka = true;
	opcje.osie = true;
	opcje.punkty = 300000;
	opcje.kroki = 3;
	opcje.tryb_koloru = 0;
	opcje.soczewki = true;
	opcje.tryb = RULETKA;
	opcje.zoom = 0.6;
	opcje.panx = 0;
	opcje.pany = 0;
	aktualna = -1;
	_ilosc_soczewek = 4;
	_nazwa[0] = '\0';
	_plik_wejsciowy[0] = '\0';
	aktualny_kolor = 0;
}

struct punkt_t {
	float x, y;
};

class Bitmapa {

	const int _w, _h;
	unsigned char *bmp;

	public:
		Bitmapa(int w, int h) : _w(w), _h(h) {
			bmp = new unsigned char[4 * w * h];
			memset(bmp, 0, 4 * w * h);
		}
		unsigned char * data() {
			return bmp;
		}

		int w() {
			return _w;
		}
		int h() {
			return _h;
		}

		// XXX
		void PutPixel(float x, float y, int R, int G, int B) {

			// XXX Zoom
			// Zamiana wspó³rzêdnych na ekranowe
			x += MyApp->opcje.panx;
			y += MyApp->opcje.pany;
			x *= (1.0 / MyApp->opcje.zoom) * _w / 2;
			x += _w / 2 - 3;
			y *= (1.0 / MyApp->opcje.zoom) * _h / 2;
			y = _h / 2 - y;

			if (x < 0 || x >= _w)
				return;
			else if (y < 0 || y >= _h)
				return;

			unsigned char (*row)[_w][4];
			row = (unsigned char(*)[_w][4])bmp;

			unsigned char *p = (unsigned char*)&row[int(y)][int(x)];

			*p++ = R;
			*p++ = G;
			*p++ = B;
			*p++ = 0xFF;

			return;
		}

		void data(const unsigned char *p) {
			if (bmp)
				delete bmp;
			bmp = (unsigned char*)p;
		}

		uint32_t GetPixel(punkt_t P) {
			if (P.x < 0 || lrint(P.x) >= _w || P.y < 0 || lrint(P.y) >= _h)
				return 0;
			else
				return *((uint32_t*)(bmp + (4 * _w * lrint(P.y)) + (4 * lrint(P.x))));
		}

		void PutPixel(punkt_t P, uint32_t W) {
			if (P.x >= 0 && lrintf(P.x) < _w && lrintf(P.y) >= 0 && P.y < _h)
				*(((uint32_t*)bmp) + lrintf(P.y) * _w + lrintf(P.x)) = W;
		}

		void Ramka(int R, int G, int B);
		void Fill(int R, int G, int B);

		void Wyczysc() {
			memset(bmp, 0, 4 * _w * _h);
		}
};

/*
 * Funkcja ju¿ nie jest u¿ywana
 */
void Bitmapa::Fill(int R, int G, int B)
{
	// XXX Zoom
	unsigned char (*row)[_w][4];
	row = (unsigned char(*)[_w][4])bmp;

	float x1 = -0.5;
	float y1 = -0.5;
	float x2 = 0.5;
	float y2 = 0.5;

	// Tutaj chyba jednak zoom nie powinno byæ
	x1 *= (1.0 / MyApp->opcje.zoom) * _w / 2;
	x1 += _w / 2 - 3;
	y1 *= (1.0 / MyApp->opcje.zoom) * _h / 2;
	y1 = _h / 2 - y1;

	x2 *= (1.0 / MyApp->opcje.zoom) * _w / 2;
	x2 += _w / 2 - 3;
	y2 *= (1.0 / MyApp->opcje.zoom) * _h / 2;
	y2 = _h / 2 - y2;

	unsigned char *p = (unsigned char*)(row + int(y2));
	unsigned char *koniec = p + 4 * _w * int(y1 - y2);
	unsigned char *r;

	while (p < koniec) {
		r = p + 4 * int(x1);
		for (int m = int(x1); m < x2; m++) {
			*r++ = R;
			*r++ = G;
			*r++ = B;
			*r++ = 0xFF;
		}
		p += 4 * _w;
	}

}

/*
 * Funkcja ju¿ nie u¿ywana
 */
void Bitmapa::Ramka(int R, int G, int B)
{
	// XXX Zoom
	unsigned char (*row)[_w][4];
	row = (unsigned char(*)[_w][4])bmp;

	float x1 = -0.5;
	float y1 = -0.5;
	float x2 = 0.5;
	float y2 = 0.5;

	// Chyba nie powinno byæ zoom
	x1 *= (1.0 / MyApp->opcje.zoom) * _w / 2;
	x1 += _w / 2 - 3;
	y1 *= (1.0 / MyApp->opcje.zoom) * _h / 2;
	y1 = _h / 2 - y1;

	x2 *= (1.0 / MyApp->opcje.zoom) * _w / 2;
	x2 += _w / 2 - 3;
	y2 *= (1.0 / MyApp->opcje.zoom) * _h / 2;
	y2 = _h / 2 - y2;

	// Tylko little-endian
	uint32_t W = R + (G << 8) + (B << 16) + (0xFF << 24);

	uint32_t *p1;
	uint32_t *p2;
	p1 = (uint32_t*)&row[int(y1)][int(x1)];
	p2 = (uint32_t*)&row[int(y1)][int(x2)];
	while (p1 < p2)
		*p1++ = W;
	p1 = (uint32_t*)&row[int(y2)][int(x1)];
	p2 = (uint32_t*)&row[int(y2)][int(x2)];
	while (p1 < p2)
		*p1++ = W;

	p1 = (uint32_t*)&row[int(y2)][int(x1)];
	p2 = (uint32_t*)&row[int(y1)][int(x1)];
	for (;p1 < p2; p1 += _w)
		*p1 = W;

	p1 = (uint32_t*)&row[int(y2)][int(x2)];
	p2 = (uint32_t*)&row[int(y1)][int(x2)];
	for (;p1 < p2; p1 += _w)
		*p1 = W;
}


/*
 * Zwraca punkt P w przekszta³ceniu S
 */
static punkt_t
Odwzorowanie(punkt_t P, const Macierz & S)
{
	punkt_t ret;

	ret.x = S.a * P.x + S.b * P.y + S.c;
	ret.y = S.d * P.x + S.e * P.y + S.f;

	return ret;
}

/*
 * Zwraca punkt, który w przekszta³ceniu da³by punkt P
 */
static punkt_t
Odwrotne(punkt_t P, const Macierz & S)
{
	punkt_t ret;

	if (S.d * S.b == S.e * S.a) {
		/*
		 * Nale¿y zwróciæ punkt, który na bitmapie na pewno
		 * nie bêdzie zapalony.
		 */
		ret.x = 100;
		ret.y = 100;
	} else {
		ret.x = S.b * (P.y - S.f) + S.e * (S.c - P.x);
		ret.x /= (S.d * S.b - S.e * S.a);

		ret.y = S.d * (P.x - S.c) + S.a * (S.f - P.y);
		ret.y /= (S.b * S.d - S.a * S.e);
	}

	return ret;
}


unsigned char* App::Tryb_Ruletka(int w, int h)
{
	Bitmapa *BMP1 = new Bitmapa(w, h);
	unsigned char kolor[sizeof(kolor_Soczewki) / sizeof(*kolor_Soczewki)][3];

	for (unsigned int n = 0; n < sizeof(kolor) / sizeof(*kolor); n++)
		Fl::get_color(kolor_Soczewki[n], kolor[n][0], kolor[n][1], kolor[n][2]);

	punkt_t P1 = {0, 0}, P2 = {0, 0};

	float min = 0, maks = 0, los = 0;
	for (unsigned int n = 0; n < _ilosc_soczewek; n++)
		maks += TABELA[n].prawd;

	unsigned char R, G, B;
	if (opcje.tryb_koloru == 0) {
		R = kolor[aktualny_kolor][0];
		G = kolor[aktualny_kolor][1];
		B = kolor[aktualny_kolor][2];
	}
	for (unsigned int n = 0; n < opcje.punkty; n++) {

		/*
		 * Wybór soczewki z odpowiednim prawdopodobieñstwem.
		 */

		min = 0;
		los = maks * rand() / (RAND_MAX + 1.0);
		for (unsigned int m = 0; m < _ilosc_soczewek; m++) {
			if (los < min + TABELA[m].prawd) {
				P2 = Odwzorowanie(P1, TABELA[m]);
				if (opcje.tryb_koloru == 1) {
					R = kolor[m][0];
					G = kolor[m][1];
					B = kolor[m][2];
				}
				break;
			}
			min += TABELA[m].prawd;
		}

		P1 = P2;

		if (opcje.tryb_koloru == 1)
			BMP1->PutPixel(P1.x, P1.y, R, G, B);
		else if (opcje.tryb_koloru == 2) {
			los = rand() % _ilosc_soczewek;
			BMP1->PutPixel(P1.x, P1.y,
					kolor[int(los)][0], kolor[int(los)][1], kolor[int(los)][2]);
		}
		else
			BMP1->PutPixel(P1.x, P1.y, R, G, B);
	}

	return BMP1->data();
}

/*
 * Nanosi na bitmapê B1 bitmapê B2 przekszta³con± wed³ug M
 * do wy¶wietlenia w aktualnym widoku.
 */
static void
Skopiuj(Bitmapa *B1, Bitmapa *B2, Macierz M)
{
	// XXX Zoom
	uint32_t W;
	punkt_t P1, P2;

	int w = B1->w();
	int h = B1->h();

//	M.c += MyApp->opcje.panx;
//	M.f += MyApp->opcje.pany;

	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++) {
			// Zamiana ze wspó³rzêdnych ekranowych
			P2.x = x - w / 2;
			P2.y = -y + h / 2;
			P2.x /= w / 2 * (1.0 / MyApp->opcje.zoom);
			P2.y /= h / 2 * (1.0 / MyApp->opcje.zoom);

//			P2.x += MyApp->opcje.panx;
//			P2.y -= MyApp->opcje.pany;

			P1 = Odwrotne(P2, M);

//			P2.x *= (1.0 / MyApp->opcje.zoom) * w / 2;
//			P2.x += w / 2 - 3;
//			P2.y *= (1.0 / MyApp->opcje.zoom) * h / 2;
//			P2.y = h / 2 - P2.y;
			P2.x = x - 3;
			P2.y = y;

			// XXX
			P1.x *= (1.0 / MyApp->opcje.zoom) * w / 2;
			P1.x += w / 2 - 3;
			P1.y *= (1.0 / MyApp->opcje.zoom) * h / 2;
			P1.y = h / 2 - P1.y;

			if ((W = B2->GetPixel(P1)))
				B1->PutPixel(P2, W);
		}

}

static Bitmapa*
Wczytaj_BMP(const char *plik)
{
	Fl_BMP_Image *T = new Fl_BMP_Image(plik);

	int w = T->w();
	int h = T->h();
	Bitmapa *ret = new Bitmapa(w, h);

	unsigned char *buf = new unsigned char[4 * w * h];
	unsigned char *p1 = buf;
	unsigned char *p2 = (unsigned char*)(T->array);

	for (int n = 0; n < w * h; n++) {
		*p1++ = *p2++;
		*p1++ = *p2++;
		*p1++ = *p2++;
		*p1++ = 0xFF;
	}

	ret->data(buf);

	delete T;

	return ret;
}

/*
 * W tym trybie korzysta siê z szybkich funkcji rysowania po Widgecie,
 * dostarczanych przez FLTK.
 */
/*
unsigned char*
App::Tryb_Klasyczny(int w, int h)
{
}
*/

/*
 * XXX XXX XXX
 *
 * Tutaj brakuje dobrego algorytmu, jak to realizowaæ...
 * bior±c pod uwagê, ¿e nie wiadomo w zasadzie, jak du¿y fraktal powstanie,
 * bo tego nigdy nie wiadomo.
 * Wiêc nie wiadomo, jaki obszar kopiowaæ.
 *
 */
unsigned char*
App::Tryb_Z_Obrazem(int w, int h)
{
	if (! *plik_wejsciowy())
		return 0;

	Bitmapa *TMP;

	Bitmapa *OBRAZ = Wczytaj_BMP(plik_wejsciowy());

	Bitmapa *BMP2 = new Bitmapa(w, h);
	Bitmapa *BMP1 = new Bitmapa(w, h);

	// XXX Zoom
	Macierz K;
	K.a = .5 / (opcje.zoom) * (1.0 * w / OBRAZ->w());
	K.e = .5 / (opcje.zoom) * (1.0 * h / OBRAZ->h());

	// XXX max zoom
//	K.a = (1.0 / opcje.zoom) * w / OBRAZ->w();
//	K.e = (1.0 / opcje.zoom) * h / OBRAZ->h();
	K.c = (K.a -.03)* (w - OBRAZ->w()) * opcje.zoom / w;
	K.f = -K.e * (h - OBRAZ->h()) * opcje.zoom / h;

	Skopiuj(BMP1, OBRAZ, K);
	delete OBRAZ;

//	BMP1->Fill(0xFF, 0xA0, 0xA0);

	for (unsigned int m = 0; m < opcje.kroki; m++) {
		for (unsigned int n = 0; n < _ilosc_soczewek; n++)
			Skopiuj(BMP2, BMP1, TABELA[n]);
		TMP = BMP2;
		BMP2 = BMP1;
		BMP1 = TMP;
		BMP2->Wyczysc();
	}

	return BMP1->data();

	delete BMP2;
	BMP2 = new Bitmapa(w, h);

	K = Macierz();
	K.a = opcje.zoom;
	K.e = opcje.zoom;
	Skopiuj(BMP2, BMP1, K);

	return BMP2->data();
}


