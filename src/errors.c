#include "main.h"
#include "errors.h"

ERRCODE errcode = DEFAULT;

// help msg
void usage(const char *arg0) {
  fprintf(stdout,
      "\n                                             "                         "Diff-ure v.%s - tiny cli patching companion\n"
      " _____   __  ______ ______                   "                           "Usage: %s | options | file ... file \n"
      "/\\  __-./\\ \\/\\  ___/\\  ___\\                 "                      " -p  --print       Print patch into stdout, don't generate file,\n"
      "\\ \\ \\/\\ \\ \\ \\ \\  __\\ \\  __\\                 "                 "                   negated by output specification\n"
      " \\ \\____-\\ \\_\\ \\_\\  \\ \\_\\                   "                  " -o  --output      Where to put patchfiles, pwd is default\n"
      "  \\/____/ \\/_/\\/_/   \\/_/                   "                        " -s  --skip        Skip editing resulting patchfile\n"
      "             __  __  ______  ______         "                            " -r  --reverse     Reverse order of diffed files\n"
      "            /\\ \\/\\ \\/\\  == \\/\\  ___\\        "                    " -e  --editor      Editor of your choice, falls back to env vars if\n"
      "            \\ \\ \\_\\ \\ \\  __<\\ \\  __\\        "                   "                   none supplied\n"
      "             \\ \\_____\\ \\_\\ \\_\\ \\_____\\      "                   " -d  --difftool    Diff tool of your preference, by default uses\n"
      "              \\/_____/\\/_/ /_/\\/_____/      "                         "                   *nix diff\n"
      "                                            "                            "                   To pass arguments to either option use as\n"
      "                                            "                            "                   \'tool -arg -arg\'\n"
      "                                            "                            " --help            Show this help message\n"
      "                                            "                            " --verbose         When you need all verbosity\n"
      "\n"
      "License GPLv3+: GNU GPL version 3 or later "
      "<http://gnu.org/licenses/gpl.html>\n"
      "This is free software: you are free to change and redistribute it.\n"
      "There is NO WARRANTY, to the extent permitted by law.\n"
  ,PACKAGE_VERSION, arg0);
}

void errproc (ERRCODE errcode) {//(errcode errcode) {
// case 0: No errors, return 0
// case 6: General error, just print usage
  if (2 <= debug)   fprintf(stderr, "[D] Errcode: %u\n", errcode);
  if (SHOW_HELP >= errcode) usage(ARG0);
  if (EXITSUCCESS < errcode) {
    fprintf(stderr, "[X] Error: ");
    switch (errcode) {
      case FILE_INVALID:
        fprintf(stderr, "File invalid");
        break;
      case NO_PATH:
        fprintf(stderr, "No path specified");
        break;
      case NO_EDITOR:
        fprintf(stderr, "No editor specified");
        break;
      case OUTPUT_EXISTS:
        fprintf(stderr, "Output file exists");
        break;
      case OUTPUT_DIR_CREAT:
        fprintf(stderr, "Can't create output directory");
        break;
      case WRONG_ARG:
        fprintf(stderr, "Wrong arg format");
        break;
      case NULL_ARG:
        fprintf(stderr, "Null editor args");
        break;
      case IO:
        fprintf(stderr, "IO error");
        break;
      case FILE_COPY:
        fprintf(stderr, "File copy error");
        break;
      case EXEC_FAIL:
        fprintf(stderr, "Failure executing program");
        break;
      default:
        fprintf(stderr, "SOMETHING REALLY WENT BAD");
    }
    fprintf(stderr, "!\n");
  }
  if (errno) {
    fprintf(stderr, "[X] Errno: %d\n", errno);
  }

  if (EXITSUCCESS >= errcode) exit(EXIT_SUCCESS);
  else exit(EXIT_FAILURE); // graceful exit
}

