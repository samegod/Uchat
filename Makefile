.PHONY: all install uninstall reinstall

all: install

install:
	@$(MAKE) -C client/
	@$(MAKE) -C server/

uninstall:
	@$(MAKE) -C server/ uninstall
	@$(MAKE) -C client/ uninstall

reinstall: uninstall install
