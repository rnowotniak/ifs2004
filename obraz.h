#ifndef __HAVE_OBRAZ_H
#define __HAVE_OBRAZ_H 1

#include <FL/Fl_Box.H>
#include "macierze.h"

class Macierz;

class Obraz : public Fl_Box {
	public:
		Obraz(int x, int y, int w, int h, const char *label = 0);
		bool rysuj_klas;

	protected:
		void draw();

	private:
		void draw_soczewka(const Macierz &);
		void Obraz::Rysuj_Tryb_Klasyczny(int kroki, const Macierz & M, bool t);
};


#endif
