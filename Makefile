#
# g/ new Fl_/s/(\(\d\+\)/\="(" . (submatch(1) - 48) . " + x"/
# g/ new Fl_/s/, \(\d\+\)/\=", " . (submatch(1) - 47) . " + y"/
#

#CPP := ccmalloc g++
CPP := g++
PROG := ifs
SHELL = /bin/bash

LIBS += $(shell fltk-config --use-images --ldflags)
#LIBS += -lefence
#CFLAGS += $(shell fltk-config --use-images --cxxflags) -Wall -g
CFLAGS += -Os -O3 -mcpu=pentium2 -march=pentium2 -ffast-math \
	-s -fomit-frame-pointer


all: $(PROG)
	./$<

$(PROG): plik.o obraz.o app.o przekUI.o macierzUI.o ui.o macierze.h
	$(CPP) $(CFLAGS) $(LIBS) $(filter %.o,$^) -o $@

#%.o: %.h %.cxx
%.o: %.h %.cxx ui.h
	$(CPP) $(CFLAGS) -c $(filter %.cxx,$^) -o $@

ui.cxx ui.h: ui.fl macierzUI.h przekUI.h plik.h kolory.h obraz.h macierze.h
	fluid -c $<

obraz.cxx app.cxx: kolory.h

macierzUI.cxx: macierzUI.h
macierzUI.h: macierzUI.fl
	fluid -c $<
	echo -e '/^extern unsigned macierzUI::ostatni = 0;/d\nwq' | vim -e macierzUI.h
	echo -e '/^macierzUI::macierzUI(int/+1\n.,+3d\nwq' | ed macierzUI.cxx
	echo -e '$$\n?o->end();$$?\n-1,.d\nwq' | ed macierzUI.cxx
	echo -e 'g/ new Fl_/s/(\(\d\+\)/\="(" . (submatch(1) - 48) . " + x"/\nwq' | vim -e macierzUI.cxx
	echo -e 'g/ new Fl_/s/, \(\d\+\)/\=", " . (submatch(1) - 47) . " + y"/\nwq' | vim -e macierzUI.cxx

przekUI.cxx: przekUI.h
przekUI.h: przekUI.fl
	fluid -c $<
	echo -e '/^extern unsigned przekUI::ostatni = 0;/d\nwq' | vim -e przekUI.h
	echo -e '/^przekUI::przekUI(int/+1\n.,+3d\nwq' | ed przekUI.cxx
	echo -e '$$\n?o->end();$$?\n-1,.d\nwq' | ed przekUI.cxx
	echo -e 'g/ new Fl_/s/(\(\d\+\)/\="(" . (submatch(1) - 48) . " + x"/\nwq' | vim -e przekUI.cxx
	echo -e 'g/ new Fl_/s/, \(\d\+\)/\=", " . (submatch(1) - 47) . " + y"/\nwq' | vim -e przekUI.cxx


clean:
	-rm *.o $(PROG) core ui.cxx ui.h

.PHONY: clean all


