#include "ui.h"
#include <FL/Fl_File_Chooser.H>
#include <FL/fl_ask.H>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>

using namespace std;

#define DEFAULT_SAVE_DIR "data"

int Zapisz_Plik()
{
	Fl_File_Chooser fc(DEFAULT_SAVE_DIR,
			"Uklady soczewek (*.ifs)",
			Fl_File_Chooser::CREATE,
			"Wybierz plik do zapisu");
	fc.show();

	while (fc.visible())
		Fl::wait();

	if (fc.count() == 1) {
		FILE *PLIK;
		if ((PLIK = fopen(fc.value(), "r"))) {
			fclose(PLIK);
			if (0 == fl_ask("Nadpisaæ %s?", fc.value()))
				return -1;
		}

		if (NULL == (PLIK = fopen(fc.value(), "w"))) {
			fl_alert("B³±d: %s", strerror(errno));
			return -1;
		}

		fprintf(PLIK, "%d\n", strlen(MyApp->nazwa()));
		fprintf(PLIK, "%s\n", MyApp->nazwa());
		fprintf(PLIK, "\n");

		fprintf(PLIK, "%d %d %d %u\n\n",
				MyApp->opcje.siatka,
				MyApp->opcje.osie,
				MyApp->opcje.soczewki,
				MyApp->opcje.punkty);

		unsigned int ilosc = MyApp->ilosc_soczewek();

		fprintf(PLIK, "%d\n", ilosc);
		for (unsigned int n = 0; n < ilosc; n++)
			fprintf(PLIK, "%3.0f %g %g %g %g %g %g\n",
					MyApp->TABELA[n].prawd,
					MyApp->TABELA[n].a,
					MyApp->TABELA[n].b,
					MyApp->TABELA[n].c,
					MyApp->TABELA[n].d,
					MyApp->TABELA[n].e,
					MyApp->TABELA[n].f);

		fprintf(PLIK, "\n");

		fclose(PLIK);
		fl_message("Dane zosta³y poprawnie zapisane.");
	}

	return 0;
}

int Wczytaj_Plik()
{

	Fl_File_Chooser fc(DEFAULT_SAVE_DIR,
			"Uklady soczewek (*.ifs)",
			Fl_File_Chooser::SINGLE,
			"Wybierz plik do odczytu");
	fc.show();

	while (fc.visible())
		Fl::wait();

	if (fc.count() == 1) {
		FILE *PLIK;

		if (NULL == (PLIK = fopen(fc.value(), "r"))) {
			fl_alert("B³±d: %s", strerror(errno));
			return -1;
		}

		int len;
		int sia, osi, soc, pun;
		unsigned int ilosc;
		char *nazwa;

		try {
			if (fscanf(PLIK, "%d", &len) != 1)
				throw "Nie uda³o siê wczytaæ d³ugo¶ci nazwy uk³adu.";
			while (fgetc(PLIK) != '\n')
				;

			nazwa = new char[len + 1];
			memset(nazwa, 0, len + 1);
			if (fread(nazwa, 1, len, PLIK) != size_t(len))
				throw "Nie uda³o siê wczytaæ nazwy uk³adu.";
			while (fgetc(PLIK) != '\n')
				;

			if (fscanf(PLIK, "%d %d %d %u", &sia, &osi, &soc, &pun) != 4)
				throw "Nie uda³o siê wczytaæ opcji uk³adu.";
			if (fscanf(PLIK, "%d", &ilosc) != 1)
				throw "Nie uda³o siê wczytaæ ilo¶ci soczewek.";

			Macierz *TMP = new Macierz[ilosc];

			for (unsigned int n = 0; n < ilosc; n++) {
				if (fscanf(PLIK, "%f %f %f %f %f %f %f",
							&TMP[n].prawd,
							&TMP[n].a, &TMP[n].b, &TMP[n].c,
							&TMP[n].d, &TMP[n].e, &TMP[n].f) != 7)
					throw "Nie uda³o siê wczytaæ parametrów soczewek";
			}

			/*
			 * Wczyta³o siê poprawnie, wiêc ustawienie nowych parametrów
			 */
			MyApp->nazwa(nazwa);
			MyApp->opcje.siatka = sia;
			MyApp->opcje.osie = osi;
			MyApp->opcje.soczewki = soc;
			MyApp->opcje.punkty = pun;
			MyApp->ilosc_soczewek(ilosc);

			for (unsigned int n = 0; n < sizeof(MyApp->TABELA) / sizeof(*(MyApp->TABELA)); n++)
				if (n < ilosc)
					MyApp->TABELA[n] = TMP[n];
				else
					MyApp->TABELA[n] = Macierz();

		} catch (const char *str) {
			fl_alert("Niezrozumia³a zawarto¶æ pliku.\n%s", str);
			return -1;
		}

		fl_message("Dane wczytane poprawnie.");
	}

	return 0;
}

