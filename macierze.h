#ifndef __HAVE_MACIERZE_H
#define __HAVE_MACIERZE_H 1

#include <cstdio>

class Macierz {
	public:
		float a, b, c, d, e, f;
		float prawd;
		Macierz(float _a, float _b, float _c, float _d, float _e, float _f)
			: a(_a), b(_b), c(_c), d(_d), e(_e), f(_f) { }
		Macierz() : a(0), b(0), c(0), d(0), e(0), f(0) { }

		void pokaz() const {
			printf("a: %f\n", a);
			printf("b: %f\n", b);
			printf("c: %f\n", c);
			printf("d: %f\n", d);
			printf("e: %f\n", e);
			printf("f: %f\n", f);
			printf("-----\n");
		}
	
		Macierz operator*(const Macierz & M) const {
			Macierz ret;

			ret.a = (a * M.a) + (M.b * d);
			ret.b = (M.a * b) + (e * M.b);
			ret.c = (c * M.a) + (f * M.b) + M.c;
			ret.d = (a * M.d) + (d * M.e);
			ret.e = (b * M.d) + (e * M.e);
			ret.f = (c * M.d) + (f * M.e) + M.f;

			return ret;
		}
};

#endif
