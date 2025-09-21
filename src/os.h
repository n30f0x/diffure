// autoconf
#include "../config.h"
// per os libs and defines
#ifdef __APPLE__

  #include <sys/syslimits.h>
  #include <sys/_types/_pid_t.h>
  // copyfiles
  #include <copyfile.h>

#elif __linux__

  // copyfiles
  #include <sys/sendfile.h>

#endif

#if defined( __APPLE__) || defined(__linux__)

  #include <stdlib.h>
  #include <stdint.h>
  #include <sys/stat.h>

#endif

extern int debug;

struct params;

void copyfiles (int file_from, int file_to);
