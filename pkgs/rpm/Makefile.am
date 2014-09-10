# 
# Copyright (C) Gyoza Work Group 2000
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
# 
# $Id$
#

SRCDIR		= $(shell cd ../../; pwd)
PKGNAME		= @PACKAGE@
VERSION		= @PACKAGE_VERSION@
CPUARCH		= $(shell uname -m)
WORKDIR		= $(PKGNAME)-$(VERSION)
TARBALL		= $(WORKDIR).tar.gz
DATE		= $(shell date "+%h%d.%H.%M")
CURRDIR		= $(shell pwd)
FAKEROOT	= /tmp/$(PKGNAME)
PKG		= $(PKGNAME)-$(VERSION)-$(RELEASE)
FAKE_PREFIX	= $(FAKEROOT)/usr
RELEASE = 1
TOP		= $(SRCDIR)/..
SPEC		= $(PKGNAME).spec

TARGET_CPU	:= $(shell \
			rpmbuild --showrc | \
				awk '{ if ($$2 ~ /_target_cpu/) print $$3;}'; \
		)

RPM_ROOT	:= $(shell \
			if test `whoami` = 'root'; then \
				rpmbuild --showrc | \
					awk '{\
						if ($$2 ~ /_topdir/) { \
							split($$3,r,/\//); \
							print "/usr/src/"r[2]; \
						} \
					}' ;\
			else \
				echo $(FAKEROOT); \
			fi; \
		)

DIST		:= $(shell \
			rpmbuild --showrc | \
				awk '{ if ($$2 ~ /_host_vendor/) print $$3;}' \
			)

RPM_SOURCE_DIR	:= $(RPM_ROOT)/SOURCES
RPM_BUILD_DIR	:= $(RPM_ROOT)/BUILD
RPM_SPEC_DIR	:= $(RPM_ROOT)/SPECS
RPM_RPMS_DIR	:= $(RPM_ROOT)/RPMS
RPM_SRPMS_DIR	:= $(RPM_ROOT)/SRPMS

.PHONY:	rpm clean

rpm: clean rpm_env
	cd $(SRCDIR) && make distclean
	cd $(TOP); \
	/bin/ln -sf $(SRCDIR) $(WORKDIR); \
	/bin/tar chzf $(TARBALL) $(WORKDIR); \
	/bin/cp $(TARBALL) $(RPM_SOURCE_DIR)
	/bin/cp $(SPEC) $(RPM_SPEC_DIR); \
	/bin/rm -f $(TARBALL); /bin/rm -f $(WORKDIR)
	export PKGNAME=$(PKGNAME); \
	export VERSION=$(VERSION); \
	export RELEASE=$(RELEASE); \
	export DIST=$(DIST); \
	export DESCRIPTION=$(DESCRIPTION); \
	rpmbuild -ba --rmsource --rmspec --clean \
		--define='_topdir $(RPM_ROOT)' $(RPM_SPEC_DIR)/$(SPEC);
	/bin/mv -f $(RPM_RPMS_DIR)/$(TARGET_CPU)/$(PKG)*.rpm .
	/bin/mv -f $(RPM_SRPMS_DIR)/$(TARGET_CPU)/$(PKG)*.src.rpm .

rpm_env:
	if test ! -d $(RPM_SPEC_ROOT); then \
		mkdir -p $(RPM_SPEC_ROOT) && \
		mkdir -p $(RPM_SOURCE_ROOT) && \
		mkdir -p $(RPM_BUILD_ROOT) && \
		mkdir -p $(RPM_RPMS_ROOT) && \
		mkdir -p $(RPM_SRPMS_ROOT); \
	fi

clean:
	/bin/rm -f $(PKG)*rpm
distclean: clean
	-rm -f Makefile Makefile.in
