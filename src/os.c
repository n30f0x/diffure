#include "os.h"

#include "main.h"
#include "args.h"

#if __APPLE__

void copyfiles(int file_from, int file_to) {
  if (3 <= debug) {fprintf(stderr, "[D] Copyfile from %i to %i\n", file_from, file_to);}
  if (2 > file_to)
    if (-1 == lseek(file_from, 0, SEEK_SET)) errproc(20);
  copyfile_state_t copyfilet = copyfile_state_alloc();
  if (3 <= debug) {fprintf(stderr, "[D] Copyfile state allocated, sending file...\n");}
  if (fcopyfile(file_from, file_to, copyfilet, COPYFILE_ALL | COPYFILE_CLONE | COPYFILE_EXCL) != 0) {
    perror(NULL);
    errproc(27);
  };
  copyfile_state_free(copyfilet);
  if (3 <= debug) {fprintf(stderr, "[D] Copyfile state freed\n");}
}
 
#elif __linux__

void copyfiles(int file_from, int file_to) {
	struct stat sbuf;
	fstat(file_from, &sbuf);
	off_t size = sbuf.st_size;

  if (3 <= debug) {fprintf(stderr, "[D] Sendfile from %i to %i\n", file_from, file_to);}
  if (2 > file_to)
    if (-1 == lseek(file_from, 0, SEEK_SET)) errproc(20);
  off_t ret;
  do {
    ret = copy_file_range(file_from, NULL, file_to, NULL, SIZE_MAX, 0);
    size-= ret;
  } while (size > 0 && ret > 0);
  if (ret == 0) return;
  if (1 <= debug)
    fprintf(stderr, "[X] Copyfile file range failed with errno: %d\n", errno);
  /* int flag = sendfile(file_from, file_to, 0, 0);
  if (flag == 0)
   return;
  if (1 <= debug)
    fprintf(stderr, "[X] Copyfile sendfile failed\n"
                    "[V] Copyfile cat fallback used\n"); */
  if (2 > file_to)
    if (-1 == lseek(file_from, 0, SEEK_SET)) errproc(20);
	int64_t off, nr, nw;
	static size_t bsize;
	static char *buf = NULL;

  if (fstat(file_to, &sbuf))
    fprintf(stderr, "fstat");
  bsize = 4096;
  buf = malloc(bsize);
	if (buf == NULL)
    fprintf(stderr, "malloc() failure of IO buffer\n");
  while ((nr = read(file_from, buf, bsize)) > 0)
   for (off = 0; nr; nr -= nw, off += nw)
     if ((nw = write(file_to, buf + off, (size_t) nr)) < 0)	break;
  if (nr < 0) {
    perror(NULL);
    errproc(27);
  }
  if (buf) free(buf);
}

#endif
