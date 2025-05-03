// per os defines
#include "os.h"
// local includes
#include "main.h"
#include "args.h"

void *strictrealloc(void *ptr, size_t sz){
  void *newptr = calloc(1, sz);
  if (newptr == NULL)
    abort();
  if (4 <= debug) fprintf(stderr, "[D] Newptr: %p allocated bytes: %lu\n", newptr, sz);
  memcpy(newptr, ptr, sz);
  free(ptr); 
  return newptr;
}

int sanstr(int mode, char *restrict str) {
  int verdict = 0;  // selected mode will result in either distructive
  while (*str++)    // or nondestructive behavior
    switch (*str){
      case '\n':
      case '\t':
      case '\r':
      case '\f':
      case '\v':
      case '\b':
        verdict++;
      case '\\':
        if (mode) *str = ' ';
        break;
    }
  return verdict;
}

/*
  It's impossible to know beforehand how much args we have due to lack of metadata.
  This is C after all! So we have to supply cnt for variadics and iterate thru.
  Functions were rewritten to supply as much args as possible.
*/

void pathgen(size_t cnt, char *restrict *fileout, ... ) {
  assert(fileout);
  // ------------v null byte padding
  size_t strsz = 1, bufsz = PATH_MAX;
  char *buf = calloc(cnt, bufsz);
  va_list args;
  va_start(args, fileout);
  int i;
  for (i = 0; i < cnt; i++) {
    const char *tmpbuf = va_arg(args, char *);
    if (sanstr(false, (char *)tmpbuf)) errproc(25);
    strsz += strlen(tmpbuf);
    strlcat(buf, tmpbuf, bufsz);
  }
  va_end(args);
  *fileout = strictrealloc(*fileout, strsz);
  if (3 <= debug) fprintf(stderr, "[D] Template name: %s\n", buf);
  snprintf(*fileout, strsz, "%s", buf);
  free(buf);
}

void execthing(size_t cnt, int fd, ...) {
  int i = 0, arglen = 0, statloc = 0;
  va_list args;
  // allow user to specify as many use flags on any argument as possible in input
  const char **argList = calloc(cnt, MAX_INPUT);
  if (3 <= debug) fprintf(stderr, "[D] Args to exec:");
  va_start(args, fd);
  while (i < cnt) {
    char* argarr = va_arg(args, char *);
    if (sanstr(false, argarr)) break;
    if (3 <= debug) fprintf(stderr, " %s", argarr);
    while (strstr(argarr, " ")) {
      argList[arglen] = strsep(&argarr, " "); 
      arglen++;
    }
    argList[arglen] = argarr;
    i++; arglen++;
  }  
  argList[arglen+1] = NULL;
  va_end(args);
  if (3 <= debug) fprintf(stderr, "\n[D] Total args: %d\n", arglen);

  if (!fork()) {
    if (fd) dup2(fd, 1); // redirect stdout to a file
    execvp(argList[0], (char *const *)argList);
  };

  free(argList);
  wait(&statloc);
  if (statloc > 0 && statloc != 256) {
    errno = statloc; errproc(28);
  }
}
