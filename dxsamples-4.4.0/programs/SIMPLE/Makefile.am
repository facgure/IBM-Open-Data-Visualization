## Process this file with automake to produce Makefile.in

sampleprogdir = $(prefix)/dx/samples/programs/SIMPLE

LINKLIST = \
         Arrange.cfg\
         Arrange.net\
         AutoAxes.cfg\
         AutoAxes.net\
         AutoColor.cfg\
         AutoColor.net\
         AutoGlyph.cfg\
         AutoGlyph.net\
         AutoGrid.cfg\
         AutoGrid.net\
         Band.cfg\
         Band.net\
         Caption.cfg\
         Caption.net\
         Color.cfg\
         Color.net\
         ColorBar.cfg\
         ColorBar.net\
         Compute2.cfg\
         Compute2.net\
         Connect.cfg\
         Connect.net\
         Construct.cfg\
         Construct.net\
         Describe.cfg\
         Describe.net\
         FaceNormals.cfg\
         FaceNormals.net\
         GetSet.cfg\
         GetSet.net\
         Gradient.cfg\
         Gradient.net\
         Grid.cfg\
         Grid.net\
         Histogram.cfg\
         Histogram.net\
         Include.cfg\
         Include.net\
         Isolate.cfg\
         Isolate.net\
         Isosurface.cfg\
         Isosurface.net\
         Light.cfg\
         Light.net\
         Map.cfg\
         Map.net\
         MapToPlane.cfg\
         MapToPlane.net\
         MarkUnmark.cfg\
         MarkUnmark.net\
         PlotColumn.cfg\
         PlotColumn.net\
         Post.cfg\
         Post.net\
         QuantizeImage.cfg\
         QuantizeImage.net\
         Reduce.cfg\
         Reduce.net\
         Refine.cfg\
         Refine.net\
         Regrid.cfg\
         Regrid.net\
         Route.cfg\
         Route.net\
         Rubbersheet.cfg\
         Rubbersheet.net\
         ScaleScreen.cfg\
         ScaleScreen.net\
         ShowBoundary.cfg\
         ShowBoundary.net\
         Slab.cfg\
         Slab.net\
         Sort.cfg\
         Sort.net\
         Streamline.cfg\
         Streamline.net\
         Supervise.cfg\
         Supervise.net\
         Switch.cfg\
         Switch.net\
         Verify.cfg\
         Verify.net\
         VisualObject.cfg\
         VisualObject.net

myinstall: 
	$(mkinstalldirs) $(sampleprogdir)
	@-sh -c '( echo "cd $(sampleprogdir)" ; \
		cd $(sampleprogdir) && for s in $(LINKLIST) ; do \
		$(LN_S) ../$$s $$s ; \
		echo "$(LN_S) ../$$s $$s" ; \
	done )'
