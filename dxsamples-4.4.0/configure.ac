#include "aclocal.m4"

dnl Process this file with autoconf to produce a configure script.

AC_INIT([dxsamples],[4.4.0])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([programs/AutoColor.net])
AM_INIT_AUTOMAKE

AC_SUBST(SHELL)

AC_SUBST(DXINST)
AC_SUBST(DXARCH)
AC_SUBST(DXCC)
AC_SUBST(DX_JAVA_CLASSPATH)
AC_SUBST(JAVAC)
AC_SUBST(JAR)
AC_SUBST(DX_JAR)
AC_SUBST(MAKE_JAVADX_SAMPLES)
AC_SUBST(SHARED_LINK)
AC_SUBST(DXEXEC_EXP)
AC_SUBST(DXEXEC_IMP)
AC_SUBST(C_LDARGS)
AC_SUBST(RTL_LIBS)

SHELL=/bin/sh

AC_ARG_WITH(javadx,
              [  --with-javadx           enable JAVADX (default is yes)],
              [with_javadx=$withval],
              [with_javadx='yes'])

AC_PROG_INSTALL
if test -z "$LN_S" ; then
    AC_PROG_LN_S
fi

DX_INSTALL_PATH
DX_GET_PREFIX
DX_GET_ARCH
DX_ARCH_SPECIFIC

MAKE_JAVADX_SAMPLES=""
if test "with_javadx" != 'no' ; then
  failed=0;
  passed=0;
  DX_PROG_JAVAC(passed=`expr $passed + 1`,failed=`expr $failed + 1`)
  DX_PROG_JAR(passed=`expr $passed + 1`,failed=`expr $failed + 1`)

  if test $failed -eq 0 ; then
     DX_JAVADX_SETUP(passed=`expr $passed + 1`,failed=`expr $failed + 1`)
     if test $failed -eq 0 ; then
       MAKE_JAVADX_SAMPLES=samples
     fi
  fi
fi

AC_OUTPUT( Makefile callmodule/Makefile callmodule/localmk \
	data/Makefile dxlink/Makefile dxlink/dxlink_make \
	java/samples/Makefile java/Makefile java/userdata/Makefile  \
	java/usermacros/Makefile  java/nets/Makefile  java/makes/Makefile  \
	macros/Makefile outboard/Makefile \
	program_guide/Makefile program_guide/localmk program_guide/guidemake \
	programs/2D_DATA/Makefile programs/3D_DATA/Makefile \
	programs/ANNOTATION/Makefile programs/DATA_DRIVEN_INTERACTORS/Makefile \
	programs/DEBUGGING/Makefile programs/IMPORTING/Makefile \
	programs/INTERFACE_CONTROL/Makefile programs/LOOPING/Makefile \
	programs/CATEGORICAL/Makefile programs/COMPUTE/Makefile \
	programs/IMAGE_PROCESSING/Makefile programs/SEQUENCER/Makefile 
	programs/COLORMAP_EDITOR/Makefile programs/PROBES/Makefile \
	programs/RENDERING/Makefile programs/DISTRIBUTED_PROCESSING/Makefile 
	programs/SIMPLE/Makefile programs/PLOTTING/Makefile \
	programs/SCATTERED/Makefile programs/SUPERVISE/Makefile \
	programs/MISC/Makefile programs/Makefile scripts/Makefile \
	supervise/simpledemo/Makefile \
	supervise/simpledemo/interactors/Makefile \
	supervise/simpledemo/interactors/module_make \
	supervise/mediumdemo/Makefile \
	supervise/mediumdemo/interactors/Makefile \
	supervise/mediumdemo/interactors/module_make \
	supervise/imagedemo/Makefile \
	supervise/interactors/Makefile supervise/interactors/module_make \
	supervise/complexdemo/interactors/Makefile \
	supervise/complexdemo/interactors/module_make \
	supervise/complexdemo/nets/Makefile supervise/complexdemo/data/Makefile \
	supervise/complexdemo/Makefile supervise/Makefile tutorial/Makefile \
	user/Makefile user/user_inboard_make \
	user/user_loadable_make user/user_outboard_make \
	util/dxf/Makefile util/plot3d/Makefile util/mpeg/Makefile \
	util/fluent/Makefile util/biorad-pic/Makefile util/Makefile \
	stereo/Makefile stereo/src/Makefile stereo/src/localmk )

