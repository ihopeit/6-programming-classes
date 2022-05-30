#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv){
  int fd;
  char *filename = "/tmp/file";
  fd = open(filename, O_RDONLY);

  if(fd == -1){
    fprintf(stderr, "Cannot open /tmp/file.\n");
    exit(1);
  }

  char buf[20];
  size_t nbytes = sizeof(buf);
  size_t bytes_read = read(fd, buf, nbytes);
  close(fd);

  fwrite(buf, sizeof(char), bytes_read, stdout);
  printf("bytes read:%zd ", bytes_read);
  return 0;
}
