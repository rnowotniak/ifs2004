#include "obraz.h"
#include "app.h"
#include "macierze.h"
#include "ui.h"
#include "kolory.h"
#include <FL/Fl_Box.H>
#include <FL/Fl_Image.H>
#include <FL/fl_draw.H>
#include <cstdio>

using namespace std;

Fl_Color color_SIATKA = fl_rgb_color(0x40, 0x40, 0x40);
Fl_Color color_SOCZEWKA = FL_WHITE;
Fl_Color color_SOCZEWKA_AKTUALNA = FL_YELLOW;
Fl_Color color_OSIE = FL_RED;
Fl_Color color_OBSZAR = fl_rgb_color(0, 0, 0x7F);
Fl_Color color_KLASYCZNY_FRAKTAL = fl_rgb_color(0xFF, 0xA0, 0xA0);

// XXX max soczewek
Fl_Color kolor_Soczewki[10] = {
	FL_BLUE,
	FL_CYAN,
	FL_DARK_BLUE,
	FL_DARK_CYAN,
	FL_DARK_GREEN,
	FL_DARK_MAGENTA,
	FL_DARK_RED,
	FL_DARK_YELLOW,
	FL_GREEN,
	FL_MAGENTA
};

Obraz::Obraz(int x, int y, int w, int h, const char *label ) : Fl_Box(x, y, w, h, label)
{
	return;
}

void Obraz::draw()
{
	fl_push_clip(0, 0, w(), h());

	fl_color(FL_BLACK);
	fl_rectf(0, 0, w(), h());

	fl_push_matrix();
	fl_mult_matrix(1, 0, 0, -1, w() / 2, h() / 2);
	fl_scale((1.0 / MyApp->opcje.zoom) * w() / 2, (1.0 / MyApp->opcje.zoom) * h() / 2);
	fl_translate(MyApp->opcje.panx, MyApp->opcje.pany);


	/*
	 * Siatka
	 */

	if (MyApp->opcje.siatka) {

		fl_color(color_SIATKA);

		for (float n = -0.6; n <= 0.6; n += 0.1) {
			fl_begin_line();
			fl_vertex(n, -0.6);
			fl_vertex(n, 0.6);
			fl_end_line();
		}

		for (float n = -0.6; n <= 0.6; n += 0.1) {
			fl_begin_line();
			fl_vertex(-0.6, n);
			fl_vertex(0.6, n);
			fl_end_line();
		}

	}


	/*
	 * Osie uk³adu
	 */


	if (MyApp->opcje.osie) {
		fl_color(color_OSIE);

		fl_begin_line();
		fl_vertex(-0.6, 0);
		fl_vertex(0.6, 0);
		fl_end_line();

		fl_begin_line();
		fl_vertex(0, 0.6);
		fl_vertex(0, -0.6);
		fl_end_line();

		fl_begin_line();
		fl_vertex(-0.025, 0.6 - 0.025);
		fl_vertex(0, 0.6);
		fl_vertex(0.025, 0.6 - 0.025);
		fl_end_line();

		fl_begin_line();
		fl_vertex(0.6 - 0.025, 0.025);
		fl_vertex(0.6, 0);
		fl_vertex(0.6 - 0.025, -0.025);
		fl_end_line();

		// Obszar kopiowanej powierzchni
		fl_color(color_OBSZAR);
		fl_line_style(FL_DASHDOTDOT);
		fl_begin_loop();
		fl_vertex(-0.5, 0.5);
		fl_vertex(0.5, 0.5);
		fl_vertex(0.5, -0.5);
		fl_vertex(-0.5, -0.5);
		fl_end_loop();
		fl_line_style(0);
	}

	/*
	 * Narysowanie fraktala w odpowiedni sposób
	 */


	if (MyApp->opcje.tryb == App::RULETKA || MyApp->opcje.tryb == App::TRYB_Z_OBRAZEM) {
		fl_pop_matrix();

		label(0);
		Fl_Box::draw_label();

		fl_push_matrix();
		fl_mult_matrix(1, 0, 0, -1, w() / 2, h() / 2);
		// XXX zoom
		fl_scale((1.0 / MyApp->opcje.zoom) * w() / 2, (1.0 / MyApp->opcje.zoom) * h() / 2);
		fl_translate(MyApp->opcje.panx, MyApp->opcje.pany);
	} else if (MyApp->opcje.tryb == App::KLASYCZNY) {
		if (MyApp->opcje.tryb_koloru == 0)
			fl_color(kolor_Soczewki[MyApp->aktualny_kolor]);
		if (rysuj_klas) {
			Rysuj_Tryb_Klasyczny(MyApp->opcje.kroki, Macierz(1, 0, 0, 0, 1, 0), true);
			rysuj_klas = false;
		} else
			Rysuj_Tryb_Klasyczny(2, Macierz(1, 0, 0, 0, 1, 0), true);
	}



	/*
	 * Soczewki
	 */
	if (MyApp->opcje.soczewki) {

		for (unsigned int n = 0; n < MyApp->ilosc_soczewek(); n++) {
			if (int(n) == MyApp->aktualna)
				fl_color(color_SOCZEWKA_AKTUALNA);
			else
				fl_color(color_SOCZEWKA);
			draw_soczewka(MyApp->TABELA[n]);
		}

	}
	/***/

	fl_pop_matrix();

	/***/


	fl_color(FL_GREEN);
	fl_font(PL_Font, 14);
	fl_draw(MyApp->nazwa(), 3, 17);

	fl_pop_clip();
}


void Obraz::draw_soczewka(const Macierz & M)
{
//	M.pokaz();
	fl_push_matrix();
	fl_mult_matrix(M.a, M.d, M.b, M.e, M.c, M.f);

#if 0
	fl_begin_loop();
	fl_vertex(-1, 1);
	fl_vertex(1, 1);
	fl_vertex(1, -1);
	fl_vertex(-1, -1);
	fl_end_loop();

	fl_begin_line();
	fl_vertex(-0.9, 0.9);
	fl_vertex(-0.7, 0.7);
	fl_end_line();

	fl_begin_line();
	fl_vertex(-0.9, 0.7);
	fl_vertex(-0.7, 0.9);
	fl_end_line();
#else
	fl_begin_loop();
	fl_vertex(-0.5, 0.5);
	fl_vertex(0.5, 0.5);
	fl_vertex(0.5, -0.5);
	fl_vertex(-0.5, -0.5);
	fl_end_loop();

	fl_begin_line();
	fl_vertex(-0.45, 0.45);
	fl_vertex(-0.35, 0.35);
	fl_end_line();

	fl_begin_line();
	fl_vertex(-0.45, 0.35);
	fl_vertex(-0.35, 0.45);
	fl_end_line();
#endif

	fl_pop_matrix();
}

void Obraz::Rysuj_Tryb_Klasyczny(int kroki, const Macierz & M, bool t = false)
{
	if (kroki < 1)
		return;
	else if (kroki == 1) {
		Macierz M2;
		for (unsigned int n = 0; n < MyApp->ilosc_soczewek(); n++) {
			M2 = MyApp->TABELA[n] * M;
			fl_push_matrix();
			fl_mult_matrix(M2.a, M2.d, M2.b, M2.e, M2.c, M2.f);

			if (MyApp->opcje.tryb_koloru == 2)
				fl_color(kolor_Soczewki[n]);

			fl_begin_polygon();
			fl_vertex(0.5, 0.5);
			fl_vertex(-0.5, 0.5);
			fl_vertex(-0.5, -0.5);
			fl_vertex(0.5, -0.5);
			fl_end_polygon();

			fl_pop_matrix();
		}
	} else
		for (unsigned int n = 0; n < MyApp->ilosc_soczewek(); n++) {
			if (t && MyApp->opcje.tryb_koloru == 1)
				fl_color(kolor_Soczewki[n]);
			Rysuj_Tryb_Klasyczny(kroki - 1, MyApp->TABELA[n] * M);
		}
}

