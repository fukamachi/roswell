AM_CPPFLAGS = -DLISP_PATH=\"$(roslispdir)\"
roslispdir = $(datadir)/common-lisp/source/$(PACKAGE)
roslisp_DATA = lisp/init.lisp lisp/install.lisp
roslispinstalldir = $(roslispdir)/install
roslispinstall_DATA = $(wildcard lisp/install/*.lisp)
roslispsubcmddir = $(roslispdir)/subcmd
roslispsubcmd_DATA = $(wildcard lisp/*.ros)

noinst_PROGRAMS = gen
gen_SOURCES = generate.c util_list.c util.c util_string.c util_dir.c

gend.h: gen$(EXEEXT) FORCE
	(which $(CC)&&(./gen$(EXEEXT) $(CC) > $@.tmp;cmp -s $@.tmp $@||cp $@.tmp $@))||echo $(CC) not found
	rm -f $@.tmp
cmd-version.c: gend.h
bin_PROGRAMS = ros
ros_SOURCES = lsp.c opt.c download.c archive.c html.c internal.c \
	util_list.c util.c util_string.c util_dir.c \
	cmd-version.c \
	cmd-run.c cmd-run-sbcl.c cmd-run-ccl.c \
	cmd-install.c cmd-install-sbcl-bin.c cmd-install-quicklisp.c \
	cmd-help.c \
	cmd-config.c \
	cmd-setup.c
noinst_HEADERS = util.h opt.h cmd-install.h gend.h
CLEANFILES = gend.h

%.1: %.html ros$(EXEEXT)
	./ros$(EXEEXT) setup;./ros$(EXEEXT) ../scripts/man.ros $< $@

man1_MANS = ../documents/ros.1
CLEANFILES = $(man1_MANS)

FORCE:
.PHONY: FORCE
