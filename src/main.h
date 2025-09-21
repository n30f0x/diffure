// #include <config.h>

// main includes
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
 
// libs
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <errno.h>
#include <libgen.h>
#include <assert.h>
#include <sys/wait.h>
#include <ctype.h>

#include "../config.h"

#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION "0.0.0"
#endif
#ifndef PACKAGE_NAME
#define PACKAGE_NAME "diffure"
#endif

#define UNUSED(...)

#define FINALEXT "patch"
#define ARG0 PACKAGE_NAME
//"diffure"

extern int debug;

// essential structs
struct files {
  int filefd;
  int tmpfd;
  int targetfd;
  char *filepath;
  char *tmppath;
  char *target;
};

struct params {
  bool dry;
  bool finaled;
  bool outpath;
  bool reverse;
  const char *editor;
  const char *difftool;
  const char *placeholder;
  const char *target;
};
