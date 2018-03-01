## Process this file with automake to produce Makefile.in


datadir = $(prefix)/dx/samples/supervise/complexdemo/interactors

data_DATA =  Makefile_alphax Makefile_hp700 Makefile_ibm6000 Makefile_sgi Makefile_solaris interactors.c module_make Makefile

EXTRA_DIST = $(data_DATA)

loadable: module_make
	$(MAKE) -f module_make loadable

inboard: module_make
	$(MAKE) -f module_make inboard

