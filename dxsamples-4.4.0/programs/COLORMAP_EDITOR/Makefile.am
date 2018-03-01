## Process this file with automake to produce Makefile.in

sampleprogdir = $(prefix)/dx/samples/programs/COLORMAP_EDITOR

LINKLIST = \
         DataDrivenInteractors.cfg\
         DataDrivenInteractors.net\
         Imide_potential.cfg\
         Imide_potential.net\
         StandardColormaps.cfg\
         StandardColormaps.net\
         UsingColormaps.cfg\
         UsingColormaps.net\
         VolumeRendering.cfg\
         VolumeRendering.net

myinstall: 
	$(mkinstalldirs) $(sampleprogdir)
	@-sh -c '( echo "cd $(sampleprogdir)" ; \
		cd $(sampleprogdir) && for s in $(LINKLIST) ; do \
		$(LN_S) ../$$s $$s ; \
		echo "$(LN_S) ../$$s $$s" ; \
	done )'
