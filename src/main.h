// main includes
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <unistd.h>
 
// libs
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <errno.h>
#include <libgen.h>
#include <assert.h>
#include <sys/wait.h>
#include <ctype.h>

#define UNUSED(...)

#define FINALEXT "patch"
#define ARG0 "diffure"

// essential structs
struct files {
  int filefd;
  int tmpfd;
  int targetfd;
  int fifo;
  char *filepath;
  char *tmppath;
  char *target;
};

struct params {
  bool dry;
  bool finaled;
  bool outpath;
  bool reverse;
  // int pathperms;
  // int fileperms;
  const char *editor;
  const char *difftool;
  const char *placeholder;
  const char *target;
};
