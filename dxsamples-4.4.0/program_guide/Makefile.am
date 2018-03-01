## Process this file with automake to produce Makefile.in

datadir = $(prefix)/dx/samples/program_guide

data_DATA =  README add.c add.cfg add.mb add.mdf add.net add2.c add2.cfg \
	     add2.mb add2.mdf add2.net add2_invalid.cfg add2_invalid.net \
	     add2invalid.c add2invalid.mb add2invalid.mdf add_parallel.c \
	     add_parallel.cfg add_parallel.net addpar.mdf all.mdf averagecell.c \
	     averagecell.cfg averagecell.mb averagecell.mdf averagecell.net \
	     averagecell2.mdf averagecell_parallel.c averagecell_parallel.cfg \
	     averagecell_parallel.net averagecellpar.mdf hello.c hello.cfg \
	     hello.mdf hello.net hello_errorchecking.c hello_errorchecking.cfg \
	     hello_errorchecking.net hello_loadable.mdf hello_outboard.mdf \
	     helloerr.mdf makex.c makex.cfg makex.mb makex.mdf makex.net \
	     makex_efficient.cfg makex_efficient.net makexeff.c makexeff.mb \
	     makexeff.mdf showpick.c showpick.cfg showpick.mdf showpick.net \
	     simpleimport.c simpleimport.mdf simpleimportfilter.c

EXTRA_DIST = $(data_DATA) 

install-data-hook:
	cp localmk $(DESTDIR)$(datadir)/makefile

uninstall-local:
	rm $(datadir)/makefile
