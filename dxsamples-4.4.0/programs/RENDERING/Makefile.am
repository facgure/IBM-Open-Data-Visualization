## Process this file with automake to produce Makefile.in

sampleprogdir = $(prefix)/dx/samples/programs/RENDERING

LINKLIST = \
         FatLines.cfg\
         FatLines.net\
         Isolate.cfg\
         Isolate.net\
         SharedCamera.cfg\
         SharedCamera.net\
         TextureMapOpenGL.cfg\
         TextureMapOpenGL.net\
         UsingClipPlane.cfg\
         UsingClipPlane.net\
         UsingLights.cfg\
         UsingLights.net\
         LightingColorSelector.cfg\
         LightingColorSelector.net\
         UsingShade.cfg\
         UsingShade.net\
         VolumeRendering.cfg\
         VolumeRendering.net\
         VolumeRenderingSimple.cfg\
         VolumeRenderingSimple.net

myinstall: 
	$(mkinstalldirs) $(sampleprogdir)
	@-sh -c '( echo "cd $(sampleprogdir)" ; \
		cd $(sampleprogdir) && for s in $(LINKLIST) ; do \
		$(LN_S) ../$$s $$s ; \
		echo "$(LN_S) ../$$s $$s" ; \
	done )'
