// per os defines/includes
#include "os.h"
// local includes
#include "main.h"
#include "helpers.h"

// reinit global vars
int debug = 0;

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
const char *const short_opt = "pso:e:d:v::";
// getopts long
const struct option long_opt[] = {
  {"print",    0, NULL,         'p'},
  {"skip",     0, NULL,         's'},
  {"output",   1, NULL,         'o'},
  {"editor",   1, NULL,         'e'},
  {"difftool", 1, NULL,         'd'},
  {"temp",     1, NULL,         't'},
  {"verbose",  2, NULL,         'v'},
  {"help",     0, &helptrigger,  1 },
  { NULL,      0, NULL,          0 }
};
// getopts opt
const struct option opt_opt[] = {
    // todo (potentially unused)
    // {},
    {NULL, 0, NULL, 0}
};
// help msg
void usage(const char *arg0, ...) {
  fprintf(stdout,
      "\n                                             "                         "Diff-ure - tiny cli patching companion\n"
      " _____   __  ______ ______                   "                           "Usage: %s | options | file ... file \n"
      "/\\  __-./\\ \\/\\  ___/\\  ___\\                 "                      " -p  --print       Print patch into stdout, don't generate file,\n"
      "\\ \\ \\/\\ \\ \\ \\ \\  __\\ \\  __\\                 "                 "                   negated by output specification\n"
      " \\ \\____-\\ \\_\\ \\_\\  \\ \\_\\                   "                  " -o  --output      Where to put patchfiles, pwd is default\n"
      "  \\/____/ \\/_/\\/_/   \\/_/                   "                        " -s  --skip        Skip editing resulting patchfile\n"
      "             __  __  ______  ______         "                            " -e  --editor      Editor of your choice, falls back to env vars if\n"
      "            /\\ \\/\\ \\/\\  == \\/\\  ___\\        "                    "                   none supplied\n"
      "            \\ \\ \\_\\ \\ \\  __<\\ \\  __\\        "                   " -d  --difftool    Diff tool of your preference, by default uses\n"
      "             \\ \\_____\\ \\_\\ \\_\\ \\_____\\      "                   "                   *nix diff\n"
      "              \\/_____/\\/_/ /_/\\/_____/      "                         "                   To pass arguments to either option use as \'tool -arg -arg\'\n"
      "                                            "                            " --help            Show this help message\n"
      "                                            "                            " --verbose         When you need all verbosity\n"
      "\n"
      "License GPLv3+: GNU GPL version 3 or later "
      "<http://gnu.org/licenses/gpl.html>\n"
      "This is free software: you are free to change and redistribute it.\n"
      "There is NO WARRANTY, to the extent permitted by law.\n"
  ,arg0);
}

void errproc(int errcode) {
  char *errmsg = calloc(1, 2048); // yes, i know what i'm doing
  switch (errcode) {
    case 0: // No errors, return 0;
      break;
    case 6: //"General error, just print usage";
      break;
    case 12:
      snprintf(errmsg, 2048, "File invalid");
      break;
    case 13:
      snprintf(errmsg, 2048, "No path specified");
      break;
    case 14:
      snprintf(errmsg, 2048, "No editor specified");
      break;
    case 15:
      snprintf(errmsg, 2048, "Output file exists");
      break;
    case 16:
      snprintf(errmsg, 2048, "Can't create output directory");
      break;
    case 25:
      snprintf(errmsg, 2048, "Wrong arg format");
      break;
    case 26:
      snprintf(errmsg, 2048, "Null editor args");
      break;
    case 20:
      snprintf(errmsg, 2048, "IO error, errno: %i", errno);
      break;
    case 27:
      snprintf(errmsg, 2048, "File copy error, %i", errno);
      break;
    case 28:
      snprintf(errmsg, 2048, "Failure executing program, errno: %i", errno);
      break;
  };
  if (2 <= debug)   fprintf(stderr, "[D] Errcode: %u\n", errcode);
  if (12 <= errcode) fprintf(stderr, "[X] Error: %s!\n", errmsg);
  free (errmsg);

  if (6 >= errcode) usage(ARG0);
  if (10 >= errcode) exit(EXIT_SUCCESS);
  exit(EXIT_FAILURE); // graceful exit
}

// internal helper
// todo: set hard stuff later
void setopts(int argc, char **argv, struct params *options, struct files *files, ...) {
  int next_opt = 0, errcode = 0;
  while ((next_opt = getopt_long(argc, argv, short_opt, long_opt, NULL)) != -1) {
    // const char *tmpoptarg = optarg;
    switch (next_opt) {
      case 'p':
        options->dry = true;
        if (options->target && !errcode) errcode = 25;
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
          if (!errcode) errcode = 13;
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
          errcode = 6;
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
  if (helptrigger) errproc(6);
  else if (errcode) errproc(errcode);
  else if (argv[optind] == NULL) errproc(6);
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
  if(!options->editor) errproc(14);
}
