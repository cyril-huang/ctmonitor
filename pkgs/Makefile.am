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

DEB_DISTRO	=	/etc/debian_version
RPM_DISTRO	=	/etc/redhat-release /etc/SuSE-release

pkg	= $(shell \
	  	for distro in $(DEB_DISTRO); do \
			if test -e $$distro; then \
				echo deb; \
				exit 0; \
			fi; \
		done; \
	  	for distro in $(RPM_DISTRO); do \
			if test -e $$distro; then \
				echo rpm; \
				exit 0; \
			fi; \
		done; \
		echo No distro defined; \
	    )
all:
	make -C $(pkg)
clean:
	make -C $(pkg) clean
distclean: clean
	-rm -f Makefile Makefile.in
	for p in deb rpm; do make -C $$p distclean; done
