AUTOMAKE_OPIONS=foreign no-dependencies subdir-objects
ACLOCAL_AMFLAGS =
SUBDIRS = src

#INCLUDES = @INCLUDES@

install-data-local:
	test -z "$(DESTDIR)$(bindir)" || /bin/mkdir -p "$(DESTDIR)$(bindir)"
	$(INSTALL) -m 755 src/normie "$(DESTDIR)$(bindir)/normie"
	@echo ""
	@echo "------------------------------------------------------------------------------"
	@echo "Normie has been installed!"
	@echo "------------------------------------------------------------------------------"
	@echo ""



