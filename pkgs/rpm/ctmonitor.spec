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

%define pkg_name %(echo $PKGNAME)
%define pkg_version %(echo $VERSION)
%define pkg_release %(echo $RELEASE)
%define dist %(echo $DIST)
%define descr %(cat $DESCRIPTION)
%define kernel_rel %(uname -r)
%define is_suse %(test -e /etc/SuSE-release && echo 1 || echo 0)
%define is_redhat %(test -e /etc/redhat-release && echo 1 || echo 0)

%if %is_suse
%define buildrequires gcc
%endif

%if %is_redhat
%define buildrequires gcc
%endif

Summary: CPU core temperature monitor
Name: %{pkg_name} 
Version: %{pkg_version}
RELEASE: %{pkg_release}.%{dist}
License: GPL
Group: System/Gyoza
Source: %{name}-%{version}.tar.gz
URL: http://www.gyoza.com/
Vendor: Gyoza International Inc
Packager: Gyoza Support <support@gyoza.com>
Requires: %{requires}
BuildRequires: %{buildrequires}
Buildroot: %{_tmppath}/%{name}-%{version}

%description
%{descr}

# 4 pre-defined variables in rpm system for building package
# $RPM_SOURCE_DIR : default /usr/src/packages/SOURCE if suse
#                   where the source pkg-ver-rel.arch.tar.gz
#		    will be found.
# $RPM_BUILD_DIR  : default /usr/src/packages/BUILD if suse
#		    where the source will be untar and make
# $RPM_SPEC_DIR	  : default /usr/src/packages/SPECS if suse
#		    where the pkg-ver-rel.arch.spec file will 
#		    be found by rpmbuild -ba xxx.spec
# $RPM_SRPMS_DIR  : default /usr/src/packages/SPECS if suse
#		    where the srpm will be packaged.

# prepare build up source, for ex, when you need to do patch, usually it's
# ./configure --prefix=$RPM_BUILD_ROOT before make if having autoconf.
# using the pre-defined macro %setup, which will use default
# directory $RPM_SOURCE_DIR/$NAME-$VERSION as the untar destination.
%prep
%setup -q

# build up source and make option, additional options for make
# in our case, only make since we don't have configure script
%build
if test -x configure; then 
	./configure --prefix=/usr; make; 
fi

# install the files form source into fake root directory, not real system.
# pre-define environment variables
# RPM_BUILD_ROOT : default /, fake root directory, the same as Buildroot:
#                  during building process, we usually set it up for not
#                  messing up the system.
# Because we have no autoconf and ./configure script, we can not determine the
# prefix dynamically then just using make install for this section. We can use 
# make prefix=$RPM_BUILD_ROOT install.
%install
if test -x configure; then 
	./configure --prefix=$RPM_BUILD_ROOT;
	make install
fi

# clean the fake root, RPM_BUILD_ROOT, clean.
%clean
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT

# files, install files from fake root to real system in the future.
%files
%defattr(-,root,root)
%{_bindir}/*
%{_sysconfdir}/*
%{_includedir}/*
%{_libdir}/*
%{_datadir}/*

# script before installation,
%pre
echo "performing preinstall"

# script after installation, write conf, write init script, restart and so on.
%post
echo "performing postinstall"

# script before uninstallation, check package usage, dependancy,etc.
%preun

#script after installation, clean up log, etc,
%postun

%changelog
* Thu Aug 20 2011 Gyoza <gyoza at gmx dot com>
  init spec modified
