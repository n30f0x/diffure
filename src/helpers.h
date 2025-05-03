#include <stdint.h>

extern int debug;

struct params;

void *strictrealloc(void *ptr, int sz);
int sanstr(int mode, char **str);

void execthing(int cnt, int fd, ... );
char *pathgen(int cnt, char **fileout, ... );
