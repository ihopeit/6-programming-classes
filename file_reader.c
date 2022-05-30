#include <stdio.h>  // printf() 函数的定义
#include <fcntl.h>  // O_RDONLY 的定义
#include <stdlib.h> // exit() 函数的定义
#include <unistd.h> // read(), close() 函数在 unistd.h 定义的(unix standard)
#include <string.h> // memset() 函数的定义

int main(int argc, char** argv){
  int fd;
  char *filename = "/tmp/file";
  fd = open(filename, O_RDONLY);

  if(fd == -1){
    fprintf(stderr, "Cannot open /tmp/file.\n");
    exit(1);
  }

  char buf[20];
  size_t nbytes;
  size_t bytes_read, total_bytes_read = 0;
  nbytes = sizeof(buf);
  while((bytes_read = read(fd, buf, nbytes)) >0){  
      total_bytes_read += bytes_read;
      fwrite(buf, sizeof(char), bytes_read, stdout);
      memset(buf, 0, sizeof buf);
  }
  printf("bytes read:%zd ", total_bytes_read);
  close(fd);

  return 0;
}
