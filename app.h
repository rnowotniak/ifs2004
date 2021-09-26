#ifndef __HAVE_APP_H
#define __HAVE_APP_H 1

#include "macierze.h"
#include <cstring>

#ifndef PI
#define PI 3.1415926535897932384626433832795029L
#endif
#define NAZWA_MAXLEN 30

#ifndef PATH_MAXLEN
#define PATH_MAXLEN 1024
#endif

class App {

	private:
		unsigned int _ilosc_soczewek;
		char _nazwa[NAZWA_MAXLEN];
		char _plik_wejsciowy[PATH_MAXLEN];

	public:
		App();

		unsigned int ilosc_soczewek() {
			return _ilosc_soczewek;
		}

		void ilosc_soczewek(int ile) {
			_ilosc_soczewek = ile;
		}

		enum tryb_t {
			KLASYCZNY,
			TRYB_Z_OBRAZEM,
			RULETKA
		};

		struct {
			bool siatka;
			bool osie;
			bool soczewki;
			tryb_t tryb;
			unsigned long punkty;
			unsigned int kroki;
			unsigned int tryb_koloru;
			float zoom;
			float panx;
			float pany;
		} opcje;

		Macierz TABELA[10];
		int aktualna;
		int aktualny_kolor;

		const char* nazwa() const {
			return _nazwa;
		}
		char* nazwa(const char *s) {
			if (!s) {
				_nazwa[0] = '\0';
			} else {
				strncpy(_nazwa, s, sizeof(_nazwa) - 1);
				_nazwa[sizeof(_nazwa) - 1] = '\0';
			}
			return _nazwa;
		}

		const char* plik_wejsciowy() const {
			return _plik_wejsciowy;
		}
		char* plik_wejsciowy(const char *s) {
			if (!s) {
				_plik_wejsciowy[0] = '\0';
			} else {
				strncpy(_plik_wejsciowy, s, sizeof(_plik_wejsciowy) - 1);
				_plik_wejsciowy[sizeof(_plik_wejsciowy) - 1] = '\0';
			}
			return _plik_wejsciowy;
		}

		unsigned char* fraktal(int w, int h);
		unsigned char* App::Tryb_Ruletka(int w, int h);
		unsigned char* App::Tryb_Z_Obrazem(int w, int h);

};


#endif
