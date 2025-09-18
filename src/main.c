//
//  diffure
//  Tiny cli patching companion
//  Created by N30F0X, 2025
//
//  GPLv3 License
//
//  main.c
//

// autoconf
// #include <config.h>
// per os defines/includes/functions
#include "os.h"
// local includes
#include "main.h"
#include "args.h"
#include "errors.h"
#include "helpers.h"


int debug = 0;

int main(int argc, char *argv[]) {
// run options live here
  struct params options = {
    .dry         = false,
    .finaled     = true,
    .outpath     = false,
    .difftool    = "diff",
    .placeholder = PLACEHOLDER,
    .prefix      = "/"
  };
  struct files files;

  setopts(argc, argv, &options, &files);
  setenvs(&options);
 
// main cycle
  int i = optind;
  do {
    if (1 <= debug) {
      fprintf(stderr, "[V] Diffing amount of files: %d out of: %d\n", i-optind+1, argc-optind);
      fprintf(stderr, "[V] File: %s\n", argv[i]);
    }

    files.filefd = open(argv[i], O_RDONLY | O_CLOEXEC);
    if (files.filefd == -1) errproc(FILE_INVALID);
    files.filepath = realpath(argv[i], NULL);

    /*---------------------------------- STAGE FINISHED ----------------------------------*/

    // create dup fd
    files.target = calloc(1, PATH_MAX);
    if (options.target) {
      if (options.outpath)
        pathgen(5, &files.target, options.target, "/", basename(argv[i]), ".", FINALEXT);
      else
        pathgen(1, &files.target, options.target);
      char *tmp = strdup(files.target); // on linux dirname func modifies the adressed value for some reason
      mkdir(dirname(tmp), S_IRWXU | S_IRGRP | S_IWGRP | S_IROTH | S_IXOTH);
      free(tmp);
      if (errno == EEXIST) errno = 0;
      files.targetfd = open(files.target, O_RDWR | O_CREAT | O_EXLOCK | O_APPEND | O_FSYNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    } else {
      if (options.dry) {
        pathgen(6, &files.target, options.prefix, TMPDIR, "/", basename(argv[i]), ".", (char *)options.placeholder);
        files.targetfd = mkostemps(files.target, 0, 0);
      } else {
        pathgen(5, &files.target, getcwd(files.target, PATH_MAX), "/", basename(argv[i]), ".", FINALEXT);
        files.targetfd = open(files.target, O_RDWR | O_CREAT | O_EXLOCK | O_EXCL | O_FSYNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
      }
    }
    
    if (3 <= debug) fprintf(stderr, "[D] Target path: %s\n", files.target);
    if (files.targetfd == -1) errproc(OUTPUT_EXISTS); 

    /*---------------------------------- STAGE FINISHED ----------------------------------*/

    // make a tmp file using template
    files.tmppath = calloc(1, PATH_MAX); // make a name template
    pathgen(6, &files.tmppath, options.prefix, TMPDIR, "/", basename(files.filepath), ".", (char *)options.placeholder);
    files.tmpfd = mkostemps(files.tmppath, 0, 0);

    /*---------------------------------- STAGE FINISHED ----------------------------------*/

    // copy file contents to tmp
    if (2 <= debug) fprintf(stderr, "[D] Copy file from: %s to: %s\n", files.filepath, files.tmppath);
    copyfiles(files.filefd, files.tmpfd);

    /*---------------------------------- STAGE FINISHED ----------------------------------*/

    // after that we'll transfer ownership over files to executables
    close(files.filefd); close(files.tmpfd);

    /*---------------------------------- STAGE FINISHED ----------------------------------*/

    // make a patch
    execthing(2, 0, options.editor, files.tmppath);   

    // diff a thing
    if (options.reverse)
      execthing(3, (options.dry && !options.finaled) ? 0: files.targetfd, options.difftool, files.tmppath, files.filepath);
    else 
      execthing(3, (options.dry && !options.finaled) ? 0: files.targetfd, options.difftool, files.filepath, files.tmppath);
    if (options.finaled) // edit result
      execthing(2, 0, options.editor, files.target);
    if (options.dry) { // out to shell
      if (3 <= debug) fprintf(stderr, "[D] Dry output should be here\n");
      copyfiles(files.targetfd, STDOUT_FILENO);
      remove(files.target);
      if (3 <= debug) fprintf(stderr, "[D] Target path deleted: %s\n", files.tmppath);
    }

    /*---------------------------------- STAGE FINISHED ----------------------------------*/

    // cleanup tmp
    remove(files.tmppath);
    close(files.targetfd);
    if (3 <= debug) fprintf(stderr, "[D] Tmp path deleted: %s\n", files.tmppath);
    // unload names
    free(files.filepath); free(files.tmppath); free(files.target);

    /*---------------------------------- ITERATION DONE ----------------------------------*/

    i++;
  } while (
    i < argc
  );
  exit(EXIT_SUCCESS);
}
