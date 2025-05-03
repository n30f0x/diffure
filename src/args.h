#include <stdint.h>

extern int debug;

struct params;
struct files;

void usage(const char *arg0);
void errproc(uint32_t errcode);
void setopts(int argc, char **argv, struct params *, struct files *);
void setenvs(struct params *);
