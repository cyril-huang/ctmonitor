#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([ctmonitor], [0.0.1], [gyoza@gmx.com])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/ctm.h])
AC_CONFIG_HEADERS([config.h])

AC_ARG_WITH([gtk3],
      [AC_HELP_STRING([--with-gtk3], [compiled with GTK3])],
      [gtk_version="gtk+-3.0"],
      [gtk_version="gtk+-2.0"]
)

AC_ARG_ENABLE([debug],
      [AC_HELP_STRING([--enable-debug], [compiled with DEBUG])]
)

AM_CONDITIONAL([with_gtk3], [test "x$with_gtk3" = "xyes"])
AM_CONDITIONAL([DEBUG], [test "x$enable_debug" = "xyes"])

# Checks for programs.
AC_PROG_CC
IT_PROG_INTLTOOL
GETTEXT_PACKAGE=ctmonitor
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE",
		   [The gettext catalog name])
AM_GLIB_GNU_GETTEXT
#ALL_LINGUAS="zh_TW"
ALL_LINGUAS="en_US"

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_PID_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_CHECK_FUNCS([strstr])

# Checks for package
PKG_CHECK_MODULES([GTK], ["$gtk_version"])

AC_CONFIG_FILES([Makefile
		 po/Makefile.in
		 pkgs/Makefile
		 pkgs/deb/Makefile
		 pkgs/rpm/Makefile
                 src/Makefile]
)
AC_OUTPUT
