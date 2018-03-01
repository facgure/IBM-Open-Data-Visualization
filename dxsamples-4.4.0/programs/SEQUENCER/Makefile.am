## Process this file with automake to produce Makefile.in

sampleprogdir = $(prefix)/dx/samples/programs/SEQUENCER

LINKLIST = \
         ContoursAndCaption.cfg\
         ContoursAndCaption.net\
         FlyThrough.cfg\
         FlyThrough.net\
         FlyThrough2.cfg\
         FlyThrough2.net\
         Imide_potential.cfg\
         Imide_potential.net\
         MRI_1.cfg\
         MRI_1.net\
         MovingCamera.cfg\
         MovingCamera.net\
         MovingSheet.cfg\
         MovingSheet.net\
         Sort.cfg\
         Sort.net\
         SpecialCharacters.cfg\
         SpecialCharacters.net\
         Streamline.cfg\
         Streamline.net

myinstall: 
	$(mkinstalldirs) $(sampleprogdir)
	@-sh -c '( echo "cd $(sampleprogdir)" ; \
		cd $(sampleprogdir) && for s in $(LINKLIST) ; do \
		$(LN_S) ../$$s $$s ; \
		echo "$(LN_S) ../$$s $$s" ; \
	done )'
