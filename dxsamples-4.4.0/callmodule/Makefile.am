## Process this file with automake to produce Makefile.in

datadir = $(prefix)/dx/samples/callmodule

data_DATA = README Simple.c ThunderStreamlines.c renderimage.c xapp_supervise.c

EXTRA_DIST = $(data_DATA)

install-data-hook:
	cp localmk $(DESTDIR)$(datadir)/makefile

uninstall-local:
	rm $(datadir)/makefile

