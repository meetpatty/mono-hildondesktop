hildonpluginloaderlib_LTLIBRARIES = libmonopluginloader.la

libmonopluginloader_la_LIBADD = \
	$(LIBHILDONDESKTOP_LIBS)  \
	$(MONO_LIBS)            \
	$(PYGTK_LIBS)             \
	$(PYGOBJECT_LIBS)

libmonopluginloader_la_CFLAGS =  \
	$(HILDONDESKTOP_CFLAGS)    \
   	$(LIBHILDONDESKTOP_CFLAGS) \
	$(MONO_CFLAGS)           \
	-DHD_DESKTOP_MODULE_PATH=\"$(hildondesktoplibdir)\"

libmonopluginloader_la_SOURCES = \
	hd-plugin-loader-mono.c  \
	hd-plugin-loader-mono.h

CLEANFILES = *~
