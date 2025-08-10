#include "errors.h"

extern int debug;

struct params;
struct files;

void setopts(int argc, char **argv, struct params *, struct files *);
void setenvs(struct params *);
