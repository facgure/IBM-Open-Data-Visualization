## Process this file with automake to produce Makefile.in

sampleprogdir = $(prefix)/dx/samples/programs/3D_DATA

LINKLIST = \
         AnnotationGlyphs.cfg\
         AnnotationGlyphs.net\
         AutoColor.cfg\
         AutoColor.net\
         CappedIso.cfg\
         CappedIso.net\
         ComputeOnData.cfg\
         ComputeOnData.net\
         ContoursAndCaption.cfg\
         ContoursAndCaption.net\
         Distributed.cfg\
         Distributed.net\
         FlyThrough.cfg\
         FlyThrough.net\
         FlyThrough2.cfg\
         FlyThrough2.net\
         Imide_potential.cfg\
         Imide_potential.net\
         Isolate.cfg\
         Isolate.net\
         MRI_2.cfg\
         MRI_2.net\
         MappedIso.cfg\
         MappedIso.net\
         MovingCamera.cfg\
         MovingCamera.net\
         MovingSheet.cfg\
         MovingSheet.net\
         PickStreamline.cfg\
         PickStreamline.net\
         PlotLine.cfg\
         PlotLine.net\
         PlotLine2.cfg\
         PlotLine2.net\
         PlotTwoLines.cfg\
         PlotTwoLines.net\
         ProbeText.cfg\
         ProbeText.net\
         RubberTube.cfg\
         RubberTube.net\
         SharedCamera.cfg\
         SharedCamera.net\
         SimplifySurface.cfg\
         SimplifySurface.net\
         Streamline.cfg\
         Streamline.net\
         ThunderGlyphSheet.cfg\
         ThunderGlyphSheet.net\
         ThunderStreamlines.cfg\
         ThunderStreamlines.net\
         Thunder_cellcentered.cfg\
         Thunder_cellcentered.net\
         UsingClipPlane.cfg\
         UsingClipPlane.net\
         UsingGlyphs.cfg\
         UsingGlyphs.net\
         UsingIsosurface.cfg\
         UsingIsosurface.net\
         UsingMap.cfg\
         UsingMap.net\
         UsingStreakline.cfg\
         UsingStreakline.net\
         VolumeClip.cfg\
         VolumeClip.net\
         VolumeRendering.cfg\
         VolumeRendering.net\
         VolumeRenderingSimple.cfg\
         VolumeRenderingSimple.net\
         WindVorticity.cfg\
         WindVorticity.net

myinstall: 
	$(mkinstalldirs) $(sampleprogdir)
	@-sh -c '( echo "cd $(sampleprogdir)" ; \
		cd $(sampleprogdir) && for s in $(LINKLIST) ; do \
		$(LN_S) ../$$s $$s ; \
		echo "$(LN_S) ../$$s $$s" ; \
	done )'
