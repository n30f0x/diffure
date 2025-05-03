// per os libs and defines
#ifdef __APPLE__

  #include <sys/syslimits.h>
  #include <sys/_types/_pid_t.h>

  #define PLACEHOLDER "XXXXXXXXXXXXXXXX"
  #define PLACEHOLDERLEN 0

  // copyfiles
  #include <copyfile.h>

#elif __WIN32__
  #include <windows.h>

#elif __linux__

  #define _GNU_SOURCE
  #define PLACEHOLDER "XXXXXX"
  #define PLACEHOLDERLEN 6
  #define O_EXLOCK F_EXLCK
  #define PATH_MAX 1024
  #define MAX_INPUT 1024
  #define BUFSIZE 4096*1000

  // copyfiles
  #include <sys/sendfile.h>

#endif

#if defined( __APPLE__) || defined(__linux__)

  #include <stdlib.h>
  #include <stdint.h>

  #include <sys/stat.h>

 // || defined (__BSD__)
 // todo: bsd support
  #define TMPDIR "/tmp"
  #define HOME "HOME="
  #define PATH "PATH="
#else
  #define TMPDIR 00
  #define HOME 00
  #define PATH 00
#endif

extern int debug;

struct params;

void copyfiles (int file_from, int file_to);
