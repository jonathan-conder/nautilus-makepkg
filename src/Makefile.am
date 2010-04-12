nautilus_extensiondir = $(NAUTILUS_EXTENSION_DIR)
nautilus_extension_LTLIBRARIES = lib@PACKAGE_TARNAME@.la

lib@PACKAGE_TARNAME@_la_SOURCES = nautilus-extension.c nautilus-makepkg.c
lib@PACKAGE_TARNAME@_la_CFLAGS = $(GLIB_CFLAGS) $(GCONF_CFLAGS) $(GNOME_CFLAGS) $(NAUTILUS_CFLAGS) -include $(CONFIG_HEADER)
lib@PACKAGE_TARNAME@_la_LIBADD = $(GLIB_LIBS) $(GCONF_LIBS) $(GNOME_LIBS) $(NAUTILUS_LIBS)
lib@PACKAGE_TARNAME@_la_LDFLAGS = -module -no-undefined -avoid-version

scriptdir = $(prefix)/lib/$(PACKAGE_TARNAME)
script_SCRIPTS = makepkg

schemadir = $(GCONF_SCHEMA_FILE_DIR)
schema_DATA = $(PACKAGE_TARNAME).schemas

DEFS = -DG_LOG_DOMAIN=\"Makepkg\" -DNAUTILUS_MAKEPKG_COMMAND=\"$(scriptdir)/makepkg\"

EXTRA_DIST = nautilus-makepkg.h $(script_SCRIPTS).in $(schema_DATA).in

if GCONF_SCHEMAS_INSTALL
install-data-local:
	GCONF_CONFIG_SOURCE=$(GCONF_SCHEMA_CONFIG_SOURCE) $(GCONFTOOL) --makefile-install-rule $(schema_DATA)

uninstall-local:
	GCONF_CONFIG_SOURCE=$(GCONF_SCHEMA_CONFIG_SOURCE) $(GCONFTOOL) --makefile-uninstall-rule $(schema_DATA)
endif

DISTCLEANFILES = $(script_SCRIPTS) $(schema_DATA)
MAINTAINERCLEANFILES = Makefile.in
