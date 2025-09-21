#include "os.h"

#include "main.h"
#include "args.h"


void copyfiles(int file_from, int file_to) {

  if (3 <= debug)
    fprintf(stderr, "[D] Copying from %i to %i\n", file_from, file_to);
  if (2 > file_to && -1 == lseek(file_from, 0, SEEK_SET))
    errproc(IO);

#if __APPLE__

#ifdef MACCOPYFILE

  copyfile_state_t copyfilet = copyfile_state_alloc();
  if (3 <= debug)
    fprintf(stderr, "[D] Copyfile state allocated, sending file...\n");
  if (fcopyfile(file_from, file_to, copyfilet, COPYFILE_ALL | COPYFILE_CLONE | COPYFILE_EXCL) != 0) {
    perror(NULL);
    errproc(FILE_COPY);
  };
  copyfile_state_free(copyfilet);
  if (3 <= debug)
    fprintf(stderr, "[D] Copyfile state freed\n");

#endif
 
#elif __linux__

#ifdef COPYFILERANGE
	struct stat sbuf;
	fstat(file_from, &sbuf);
  off_t ret, size = sbuf.st_size;
  do {
    ret = copy_file_range(file_from, NULL, file_to, NULL, SIZE_MAX, 0);
    size-= ret;
  } while (size > 0 && ret > 0);
  if (ret == 0)
    return;
  if (1 <= debug)
    fprintf(stderr, "[X] Copy file range failed with errno: %d\n", errno);
#endif

#ifdef SENDFILE
  ssize_t flag = sendfile(file_from, file_to, 0, 0);
  if (flag == 0)
   return;
  if (1 <= debug)
    fprintf(stderr, "[X] Sendfile failed\n");
#endif

#endif

#ifdef CATIMPL

#ifdef CATFALLBACK 
  if (2 > file_to &&-1 == lseek(file_from, 0, SEEK_SET))
    errproc(IO);
#endif

  if (1 <= debug)
    fprintf(stderr, "[V] Copyfile cat fallback used\n");

	int64_t off, nr, nw;
	static char *buf = NULL;

	static size_t bsize = 1048576;

  buf = malloc(bsize);
	if (buf == NULL)
    fprintf(stderr, "malloc() failure of IO buffer\n");
  while ((nr = read(file_from, buf, bsize)) > 0)
   for (off = 0; nr; nr -= nw, off += nw)
     if ((nw = write(file_to, buf + off, (size_t) nr)) < 0)	break;
  if (nr < 0) {
    perror(NULL);
    errproc(FILE_COPY);
  }
  if (buf) free(buf);
#endif

}
