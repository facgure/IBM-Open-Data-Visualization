javanetsdir = $(prefix)/dx/java/server/nets
javanets_DATA = $(NET_FILES)
EXTRA_DIST = $(NET_FILES)
NET_FILES = AutoInsurance.cfg AutoInsurance.net \
	CCNet2.cfg CCNet2.net \
	GroceryList.cfg GroceryList.net \
	HomeOwn.cfg HomeOwn.net \
	MRI_2.cfg MRI_2.net \
	MutualFund.cfg MutualFund.net \
	Navigation.cfg Navigation.net \
	ScatterData.cfg ScatterData.net \
	Streamline.cfg Streamline.net \
	ThunderGlyphSheet.cfg ThunderGlyphSheet.net \
	Topo.cfg Topo.net \
	ddi.cfg ddi.net \
	gordon.cfg gordon.net \
	hedge.cfg hedge.net \
	ibmqw.cfg ibmqw.net \
	npvcall.cfg npvcall.net \
	optval.cfg optval.net \
	wacc.cfg wacc.net

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(javanetsdir)
	chmod 777 $(DESTDIR)$(javanetsdir)
