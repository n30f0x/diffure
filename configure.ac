#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Disable caching
define([AC_CACHE_LOAD], )dnl
define([AC_CACHE_SAVE], )dnl

AC_PREREQ([2.72])
AC_INIT([diffure], [0.0.6], [github.com/n30f0x/diffure])

# Checks for programs.
AC_PROG_CC

AC_CONFIG_COMMANDS_PRE(
	[m4_ifndef([AH_HEADER],
		[
			AC_CONFIG_SRCDIR([src/main.c])
			AC_CONFIG_HEADERS([config.h])
			AC_CANONICAL_HOST
		]
	)]
)

AC_CHECK_DECLS([__APPLE__], [
	target_os=macos

  AC_DEFINE([PLACEHOLDER], "XXXXXXXXXXXXXXXX", "Placeholder")
  AC_DEFINE([PLACEHOLDERLEN], [0], "Placeholder length")

	AC_CHECK_HEADERS([sys/_types/_pid_t.h sys/syslimits.h copyfile.h])
	AC_CHECK_LIB(copyfile.h, fcopyfile,
		AC_DEFINE(MACCOPYFILE, 1, "Use Copyfile implementation")
	)
])

AC_CHECK_DECLS([__linux__], [
  target_os=linux

  AC_DEFINE([_GNU_SOURCE], 1, "Use gnu source style")
  AC_DEFINE([O_EXLOCK], [F_EXLCK], "TODO: fix")
  AC_DEFINE([PATH_MAX], [1024], "Ensure path size is correct")
  AC_DEFINE([MAX_INPUT], [1024], "Use sufficient")
  AC_DEFINE([BUFSIZE], [4096*1000], "Use buffer of sufficient size")

  AC_DEFINE([PLACEHOLDER], "XXXXXX", "Placeholder")
  AC_DEFINE([PLACEHOLDERLEN], [6], "Placeholder length")
  
	AC_DEFINE([CATIMPL], 1, "Use fallback")
  AC_SEARCH_LIBS([copy_file_range], [stdlib.h sys/sendfile.h], [
			AC_DEFINE([COPYFILERANGE], 1, "Use copyfilerange implementation")
		  AC_DEFINE([CATFALLBACK], 1, "Use fallback")
		], [
  	AC_MSG_WARN("Copy_file_range not found!")
		AC_SEARCH_LIBS([sendfile], [stdlib.h sys/sendfile.h], [
			AC_DEFINE([SENDFILE], 1, "Use copyfilerange implementation")
			AC_DEFINE([CATFALLBACK], 1, "Use fallback")
			], [
			AC_MSG_WARN([UNABLE TO FIND FILE TRANSFER FUNC! USING DUMMY METHOD!])
		])
	])
])

if test "$target_os" = linux || test "$target_os" = macos;
 then
  AC_CHECK_HEADERS_ONCE([fcntl.h assert.h wait.h errno.h])
  AC_CHECK_HEADERS_ONCE([getopt.h libgen.h ctype.h])
  AC_CHECK_FUNCS([dup2 getcwd mkdir realpath strdup strstr])

	AC_DEFINE([HOME], "HOME=", "Home dir")
	AC_DEFINE([PATH], "PATH=", "Path")

	AC_ARG_VAR([PREFIX], [Check env prefix])
  if test "${PREFIX+set}" = set; then
		AC_DEFINE_UNQUOTED([TMPDIR], ["$PREFIX/tmp"], "Tmp dir")
		AC_MSG_NOTICE("Non-standard prefix found!")
		AC_MSG_NOTICE([Current prefix/tmp is ${TMPDIR}])
  else
		AC_DEFINE([TMPDIR], "/tmp", "Tmp dir")
  fi

	AC_SEARCH_LIBS([O_FSYNC], [fcntl.h unistd.h], [
		AC_MSG_NOTICE("O_FSYNC found!")
		AC_DEFINE([USE_FSYNC], 1, "Use fs background")
	], [
		AC_MSG_NOTICE("O_FSYNC not found!")
		AC_DEFINE([USE_SYNC], 1, "Use os background")
	]) 
fi

if test "x$target_os" = "x"; then
	AC_MSG_WARN(["No known os found. Please rewrite suitable files. Fallbacks used."])

  AC_DEFINE([_GNU_SOURCE], 1, "Use gnu source style")
  AC_DEFINE([PLACEHOLDER], "XXXXXX", "Placeholder")
  AC_DEFINE([PLACEHOLDERLEN], [6], "Placeholder length")
  AC_DEFINE([PATH_MAX], [1024], "Ensure path size is correct")
  AC_DEFINE([MAX_INPUT], [1024], "Use sufficient")
  AC_DEFINE([BUFSIZE], [4096*1000], "Use buffer of sufficient size")
  AC_DEFINE([TMPDIR], "/tmp", "Tmp dir")
	AC_DEFINE([HOME], "HOME=", "Home dir")
	AC_DEFINE([PATH], "PATH=", "Path")

	AC_DEFINE([CATIMPL], 1, "Use fallback")
	AC_MSG_WARN([UNABLE TO FIND FILE TRANSFER FUNC! USING DUMMY METHOD!])
fi

AC_CONFIG_FILES([
	Makefile
])
AC_OUTPUT
