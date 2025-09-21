#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Disable caching
define([AC_CACHE_LOAD], )dnl
define([AC_CACHE_SAVE], )dnl

AC_PREREQ([2.72])
AC_INIT([diffure], [0.0.5], [github.com/n30f0x/diffure])

# Checks for programs.
AC_PROG_CC
# AC_PROG_AR
# LT_INIT

AC_CONFIG_COMMANDS_PRE(
	[m4_ifndef([AH_HEADER],
		[
			AC_CONFIG_SRCDIR([src/])
			AC_CONFIG_HEADERS([config.h])
			# AC_PREFIX_CONFIG_H(src/config.h)
			AC_CANONICAL_HOST
		]
	)]
)


# Checks for libraries.

AC_CHECK_LIB(copyfile.h, copyfile)
AC_CHECK_LIB(sendfile.h, copy_file_range)

# Checks for header files.

# AC_CHECK_HEADERS([fcntl.h assert.h wait.h errno.h string.h])
AC_CHECK_HEADERS([getopt.h libgen.h ctype.h])

# Checks for typedefs, structures, and compiler characteristics.
# AC_CHECK_HEADER_STDBOOL
# AC_TYPE_INT64_T
# AC_TYPE_OFF_T
# AC_C_RESTRICT
# AC_TYPE_SIZE_T
# AC_TYPE_UINT32_T

# Checks for library functions.
# AC_FUNC_FORK
# AC_FUNC_MALLOC
AC_CHECK_FUNCS([dup2 getcwd mkdir realpath strdup strstr])

AC_CONFIG_FILES([
	Makefile
])
AC_OUTPUT
