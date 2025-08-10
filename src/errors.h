#pragma once

typedef enum {
  //default,          // case 0
  DEFAULT,
  SHOW_HELP,        // case 1-6
  EXITSUCCESS,
  FILE_INVALID,
  NO_PATH,
  NO_EDITOR,
  OUTPUT_EXISTS,
  OUTPUT_DIR_CREAT,
  IO,
  WRONG_ARG,
  NULL_ARG,
  FILE_COPY,
  EXEC_FAIL
} ERRCODE;

extern int debug;
extern ERRCODE errcode;

void usage(const char *arg0);
void errproc(ERRCODE);
