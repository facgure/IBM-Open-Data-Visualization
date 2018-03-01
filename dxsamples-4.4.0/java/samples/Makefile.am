javaclassdir = $(DXINST)/java/htmlpages
javaclass_DATA = samples.jar
EXTRA_DIST = $(JAVA_FILES) $(WRL_USERS)

CLEANFILES = samples.jar

CLASSPATH = @DX_JAVA_CLASSPATH@:@DX_JAR@:./
export CLASSPATH

# WRL_CLASSPATH may have been set, but WRLApplication may not have been built
#  so do additional test for WRLApplication in dx.jar archive prior to building
#  wrl-dependent samples


$(JAVA_CLASSES): %.class: %.java
	$(JAVAC) $<

samples.jar: $(JAVA_CLASSES)
# dave, your version 1.5 loop on JAVA_FILES is better 
#  than maintaining a list of JAVA_CLASSES
# but maybe someone can come up with a rule to build them (like above)
#  rather than a loop. --pete
# whoever does, please delete these comments!
	if test -n "`$(JAR) -tf $(DX_JAR) 2>&1 | grep WRLApplication`" ; then \
	   $(JAVAC) $(WRL_USERS); \
	fi ;
	$(JAR) cf samples.jar *.class


WRL_USERS = ThunderGlyphSheet.java Navigation.java

JAVA_FILES = AutoInsurance.java CCNet2.java GroceryList.java \
	HomeOwn.java MRI_2.java MutualFund.java  ScatterData.java \
	Streamline.java Topo.java ddi.java gordon.java hedge.java \
	ibmqw.java npvcall.java optval.java wacc.java


JAVA_CLASSES = AutoInsurance.class CCNet2.class GroceryList.class \
	HomeOwn.class MRI_2.class MutualFund.class  ScatterData.class \
	Streamline.class Topo.class ddi.class gordon.class hedge.class \
	ibmqw.class npvcall.class optval.class wacc.class

clean-local:
	rm -f *.class
