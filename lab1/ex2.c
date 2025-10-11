#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define PETRI_FILE "./bin/dummy"
#define FUNC_SIZE sysconf(_SC_PAGE_SIZE)

int main() {

  /* Open an executable file here */
  /* ... */
  FILE* fd = fopen(PETRI_FILE, "rb");
  if (!fd) {
    perror(strerror(errno));
    return errno;
  }

  if (fseek(fd, 0x1106, SEEK_SET)) {
    perror(strerror(errno));
    fclose(fd);
    return errno;
  }

  /* Fill in the details here! */
	void *ptr = mmap(NULL, FUNC_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (ptr == MAP_FAILED) {
    perror(strerror(errno));
    fclose(fd);
    return errno;
  }

	/* Copy the bytes here */
  // Copy one function, 74 bytes long
  if (fread(ptr, 74, 1, fd) < 74 && feof(fd) && ferror(fd)) {
    perror(strerror(errno));
    fclose(fd);
    return errno;
  }

	/* This monster casts ptr to a function pointer with no args and calls it. Basically jumps to your code. */
	(*(void(*)()) ptr)();

  return 0;
}
