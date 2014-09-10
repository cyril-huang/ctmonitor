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
PKG		= $(PKGNAME)_$(VERSION)_$(CPUARCH).deb
FAKE_PREFIX	= $(FAKEROOT)/usr

all: $(PKG)
$(PKG):
	cd $(SRCDIR); \
	if [ -x configure ]; then \
		./configure --prefix=$(FAKE_PREFIX); make install; \
	else \
		make prefix=$(FAKE_PREFIX); make prefix=$(FAKE_PREFIX) install; \
	fi
	cd $(FAKEROOT); ln -sf $(CURRDIR)/DEBIAN .
	cd $(FAKEROOT)/..; dpkg -b $(FAKEROOT) $@; rm -rf $(FAKEROOT); \
	/bin/mv -f $@ $(CURRDIR);

clean:
	-rm -rf $(FAKEROOT)
	-rm -f $(PKGNAME)*deb

distclean: clean
	-rm -f Makefile Makefile.in
