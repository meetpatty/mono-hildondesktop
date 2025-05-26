AC_INIT(mono-hildondesktop, 0.0.1)

AM_INIT_AUTOMAKE

AC_CANONICAL_HOST

AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_RANLIB
AC_PROG_LIBTOOL

AC_C_CONST
AC_HEADER_STDC

AC_HEADER_STDBOOL

CFLAGS="$CFLAGS -Wall -ansi -Werror -std=c99 -rdynamic"

AC_SUBST(CFLAGS)

#++++++++++++++++
# Hildon Desktop
#++++++++++++++++

PKG_CHECK_MODULES(LIBHILDONDESKTOP, libhildondesktop)
AC_SUBST(LIBHILDONDESKTOP_LIBS)
AC_SUBST(LIBHILDONDESKTOP_CFLAGS)

PKG_CHECK_MODULES(HILDONDESKTOP, hildon-desktop)
AC_SUBST(HILDONDESKTOP_CFLAGS)

hildondesktopincludedir=`pkg-config libhildondesktop --variable=includedir`
AC_SUBST(hildondesktopincludedir)

hildonpluginloaderlibdir=`pkg-config hildon-desktop --variable=hildonpluginloaderlibdir`
AC_SUBST(hildonpluginloaderlibdir)

hildondesktoplibdir=`pkg-config hildon-desktop --variable=hildondesktoplibdir`
AC_SUBST(hildondesktoplibdir)

#++++++++
# Mono
#++++++++

if test "x$have_mono" != "xno"; then
        AC_PATH_PROG([MONO], [mono], [no])
        if test "x$MONO" = "x:"; then
                have_mono=no
        fi
fi

if test "x$have_mono" != "xno"; then
        MONO_LIBS=`$PKG_CONFIG --libs mono`
        MONO_CFLAGS=`$PKG_CONFIG --cflags mono`
        AC_MSG_NOTICE([Host is: $host])

        # Hack to compile targeting arm as we cannot link against the mono devkit's i386 libs 
        case "$host" in
            arm*)
                MONO_LIBS=`echo "$MONO_LIBS" | sed 's|-L\S*/devkits/mono\S*/lib|-L'"$ac_abs_confdir"'/libmono_armel|g'`
                ;;
        esac

        AC_SUBST(MONO_LIBS)
        AC_SUBST(MONO_CFLAGS)
fi

if test "x$have_mono" = "xno"; then
        AC_MSG_ERROR([Mono not found])
fi

AC_PATH_PROG(GACUTIL, gacutil, no)
if test "x$GACUTIL" = "xno" ; then
    AC_MSG_ERROR([No gacutil tool found])
fi

GACUTIL_FLAGS='/package libhildondesktop-sharp /root $(DESTDIR)$(prefix)/lib'
AC_SUBST(GACUTIL)
AC_SUBST(GACUTIL_FLAGS)

dnl Check for gtk-sharp
PKG_CHECK_MODULES(GTK_SHARP, gtk-sharp-2.0 >= 1.9.0)
AC_SUBST(GTK_SHARP_CFLAGS)
AC_SUBST(GTK_SHARP_LIBS)
gtksharp_prefix=`pkg-config --variable=prefix gtk-sharp-2.0`
AC_SUBST(gtksharp_prefix)

dnl Check for gapi
AC_PATH_PROG(GAPI_FIXUP, gapi2-fixup, no)
if test "x$GAPI_FIXUP" = "xno"; then
    AC_MSG_CHECKING(for gapi2-fixup.exe)
    GAPI_FIXUP=`which gapi2-fixup.exe 2> /dev/null`
    if test "x$GAPI_FIXUP" = "xno" ; then
        AC_MSG_ERROR(['gapi2_fixup'/'gapi2-fixup.exe' not found.])
    fi
    AC_MSG_RESULT($GAPI_FIXUP)
    GAPI_FIXUP="$MONO $GAPI_FIXUP"
fi
AC_SUBST(GAPI_FIXUP)

AC_PATH_PROG(GAPI_CODEGEN, gapi2-codegen, no)
if test "x$GAPI_CODEGEN" = "xno"; then
    AC_MSG_CHECKING(for gapi2_codegen.exe)
    GAPI_CODEGEN=`which gapi2_codegen.exe 2> /dev/null`
    if test "x$GAPI_CODEGEN" = "xno" ; then
        AC_MSG_ERROR(['gapi2-codegen'/'gapi2_codegen.exe' not found.])
    fi
    AC_MSG_RESULT([$GAPI_CODEGEN])
    GAPI_CODEGEN="$MONO $GAPI_CODEGEN"
fi
AC_SUBST(GAPI_CODEGEN)

dnl API version
API_VERSION="1.0.0.0"
AC_SUBST(API_VERSION)

AC_CONFIG_FILES([
Makefile
libhildondesktop/libhildondesktop-sharp.pc
libhildondesktop/Makefile
libhildondesktop/AssemblyInfo.cs
loader/Makefile
])

AC_OUTPUT
