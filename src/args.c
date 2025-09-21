// autoconf
// #include <config.h>
// per os defines/includes
#include "os.h"
// local includes
#include "main.h"
#include "errors.h"
#include "helpers.h"

// local defines
static int helptrigger;
static bool teststr(int (*funcptr)(int), char * testarr) {
  size_t ii = strlen(testarr), i = 0;
  for (; i<ii; i++)
    if(!funcptr(testarr[i])) return false;
      return true;
}

// took from https://cfengine.com/blog/2021/optional-arguments-with-getopt-long/
#define IS_OPTARG \
    ((optarg == NULL && optind < argc && argv[optind][0] != '-' && argv[optind][0] != '.') \
      && argv[optind][0] != '\n' && argv[optind][0] != '\r' && argv[optind][0] != '\0' \
      ? (bool) (optarg = argv[optind++] ) \
      : (optarg != NULL))

#define IS_DIGIT_OPTARG \
    ((optarg == NULL && optind < argc \
      && argv[optind][0] != '-' && argv[optind][0] != '.' \
      && argv[optind][0] != '\n' && argv[optind][0] != '\r' && argv[optind][0] != '\0' \
      && teststr(&isdigit, argv[optind]) ) \
      ? (bool) (optarg = argv[optind++]) \
      : (optarg != NULL))
     
// getopts short
const char *const short_opt = "psro:e:d:v::";
// getopts long
const struct option long_opt[] = {
  {"print",    0, NULL,         'p'},
  {"skip",     0, NULL,         's'},
  {"reverse",  0, NULL,         'r'},
  {"output",   1, NULL,         'o'},
  {"editor",   1, NULL,         'e'},
  {"difftool", 1, NULL,         'd'},
  {"verbose",  2, NULL,         'v'},
  {"help",     0, &helptrigger,  1 },
  { NULL,      0, NULL,          0 }
};

// internal helper
void setopts(int argc, char **argv, struct params *options, struct files *files, ...) {
  int next_opt = 0;
  while ((next_opt = getopt_long(argc, argv, short_opt, long_opt, NULL)) != -1) {
    switch (next_opt) {
      case 'r':
        options->reverse = true;
        break;
      case 'p':
        options->dry = true;
        if (options->target && !errcode) errcode = WRONG_ARG;
        break;
      case 's':
        options->finaled = false;
        break;
      case 'o':
        if (IS_OPTARG) {
          options->target = optarg;
          if (!basename(optarg) || optarg[strlen(optarg)-1] == '/')
            options->outpath = true;
        } else
          if (!errcode) errcode = FILE_INVALID;
        if (options->dry) options->dry = false;
        break;
      case 'e':
        options->editor = optarg;
        break;
      case 'd':
        options->difftool = optarg;
        break;
      case 'v':
        if (IS_DIGIT_OPTARG)
          debug = atoi(optarg);
        else
          debug = 1;
        break;
      case '?':
      case 1:
        break;
      case 0:
      default:
        if (argv[optind] == NULL && !errcode)
          errcode = SHOW_HELP;
        break;
    }
  }
  if (2 <= debug) {
    fprintf(stderr,
            "[D] Debug: opts ingested:\n"
            "------------------------\n"
            "    Debugging: %u \n"
            "    Difftool: %s\n"
            "    Editor: %s\n"
            "    Output to shell: %d\n"
            "    Output to: %s\n"
            "    Output is path: %u\n"
            "    Result editing: %d\n"
            "    Max path size: %d\n"
            "------------------------\n",
    debug, options->difftool, options->editor, options->dry, options->target, options->outpath, options->finaled, PATH_MAX);
  }

  if (helptrigger || argv[optind] == NULL) errcode = SHOW_HELP;
  if (errcode) errproc(errcode);
}

static char *envchoice (int envc, const char **envs) {
  char *choice;
  int i = 0;
  while (i < envc) {
    choice = getenv(envs[i]);
    if (!choice) {
      if (2 <= debug)
        fprintf(stderr, "[D] Environment unresolved: %s\n", envs[i]);
    } else {
      if (2 <= debug)
        fprintf(stderr, "[D] Environment resolved: %s as %s\n", envs[i], choice);
      return choice;
    }
    i++;
  }
  return 0;
}

void setenvs(struct params *options) {
  const char *editorvars[] = {"VISUAL", "EDITOR"};
  // get defaults from env vars, fallback to simple text mode
  if(!options->editor) options->editor = envchoice((sizeof(editorvars)/sizeof(char *)), editorvars);
  if(!options->editor) errproc(errcode = NO_EDITOR);
}
