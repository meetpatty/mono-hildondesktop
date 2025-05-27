
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libhildondesktop-sharp.pc

MCS=mcs
RAW_API=libhildondesktop-api.raw
API=libhildondesktop-api.xml
METADATA=libhildondesktop.metadata

ASSEMBLY_NAME= libhildondesktop-sharp
ASSEMBLY= $(ASSEMBLY_NAME).dll

PARSER=gapi2-parser
CODEGEN=gapi2-codegen
FIXUP=gapi2-fixup

BINDING_LIBHILDONDESKTOP_IN = \
	$(hildondesktopincludedir)/libhildondesktop/hildon-desktop-popup-menu.h			\
	$(hildondesktopincludedir)/libhildondesktop/hildon-thumb-menu-item.h			\
	$(hildondesktopincludedir)/libhildondesktop/hildon-desktop-panel-window-composite.h	\
	$(hildondesktopincludedir)/libhildondesktop/hildon-desktop-popup-window.h		\
	$(hildondesktopincludedir)/libhildondesktop/hildon-desktop-toggle-button.h		\
	$(hildondesktopincludedir)/libhildondesktop/hildon-desktop-window.h			\
	$(hildondesktopincludedir)/libhildondesktop/hildon-desktop-panel.h 			\
	$(hildondesktopincludedir)/libhildondesktop/hildon-desktop-notification-manager.h 	\
	$(hildondesktopincludedir)/libhildondesktop/hildon-desktop-container.h 			\
	$(hildondesktopincludedir)/libhildondesktop/hildon-desktop-panel-window.h		\
	$(hildondesktopincludedir)/libhildondesktop/hildon-desktop-panel-expandable.h 		\
	$(hildondesktopincludedir)/libhildondesktop/hildon-desktop-panel-window-dialog.h	\
	$(hildondesktopincludedir)/libhildondesktop/hildon-desktop-panel-item.h 		\
	$(hildondesktopincludedir)/libhildondesktop/hildon-desktop-multiscreen.h		\
	$(hildondesktopincludedir)/libhildondesktop/hildon-home-window.h			\
	$(hildondesktopincludedir)/libhildondesktop/hildon-home-area.h				\
	$(hildondesktopincludedir)/libhildondesktop/hildon-home-titlebar.h			\
	$(hildondesktopincludedir)/libhildondesktop/hildon-desktop-home-item.h			\
	$(hildondesktopincludedir)/libhildondesktop/hildon-desktop-item.h 			\
	$(hildondesktopincludedir)/libhildondesktop/home-item.h					\
	$(hildondesktopincludedir)/libhildondesktop/statusbar-item.h				\
	$(hildondesktopincludedir)/libhildondesktop/tasknavigator-item.h 

sources =

build_sources = AssemblyInfo.cs AppDomainHelper.cs $(sources)

customs = HomeItem.custom

CLEANFILES = $(ASSEMBLY) generated-stamp generated/*.cs $(API)

DISTCLEANFILES= AssemblyInfo.cs libhildondesktop-sharp.pc

noinst_DATA = $(ASSEMBLY)

all: generated-stamp $(ASSEMBLY)

get-sources:
	@rm -rf libhildondesktop
	@mkdir -p libhildondesktop
	@$(foreach hdr, $(BINDING_LIBHILDONDESKTOP_IN), cp $(hdr) libhildondesktop/; )

regen-mono-raw-api: get-sources
	PERLLIB=$(GTKSHARP_PREFIX)/share/perl5 \
	PATH=$(GTKSHARP_PREFIX)/bin:$$PATH \
	$(PARSER) libhildondesktop-sharp-sources.xml

$(API): $(METADATA)
	cp $(srcdir)/$(RAW_API) $(API)
	chmod u+w $(API)
	$(FIXUP) --api=$(srcdir)/$(API) --metadata=$(srcdir)/$(METADATA)

generated-stamp: $(API)
	 $(CODEGEN) --generate $(srcdir)/$(API) $(GTK_SHARP_CFLAGS) --outdir=generated --customdir=$(srcdir) --assembly-name=libhildondesktop-sharp && touch generated-stamp

$(ASSEMBLY): $(build_sources) generated-stamp
	$(MCS) -nowarn:169 -unsafe -target:library $(GTK_SHARP_LIBS) \
	$(build_sources) generated/*.cs -out:$(ASSEMBLY)

install-data-local:
	echo "$(GACUTIL) /i $(ASSEMBLY) /f $(GACUTIL_FLAGS)";  \
        $(GACUTIL) /i $(ASSEMBLY) /f $(GACUTIL_FLAGS) || exit 1;

uninstall-local:
	echo "$(GACUTIL) /u $(ASSEMBLY_NAME) $(GACUTIL_FLAGS)"; \
        $(GACUTIL) /u $(ASSEMBLY_NAME) $(GACUTIL_FLAGS) || exit 1;

EXTRA_DIST = $(RAW_API) $(sources) $(customs) $(METADATA) AssemblyInfo.cs.in libhildondesktop-sharp.pc.in
