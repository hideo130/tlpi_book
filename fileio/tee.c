
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

#ifndef BUF_SIZE /* Allow "cc -D" to override definition */
#define BUF_SIZE 1024
#endif

int main(int argc, char *argv[])
{
    int inputFd, outputFd, openFlags;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE];
    printf("%d", argc);
    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s old-file new-file\n", argv[0]);
    int inputIdx = 1;

    openFlags = O_CREAT | O_WRONLY;
    if (strcmp(argv[1], "--a") == 0)
    {
        inputIdx = 2;
        openFlags = openFlags | O_APPEND;
    }
    else
    {        
        openFlags = openFlags | O_TRUNC;
    }

    // inputFd = open(argv[inputIdx], O_RDONLY);
    // if (inputFd == -1)
    //     errExit("opening file %s", argv[1]);

    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                S_IROTH | S_IWOTH; /* rw-rw-rw- */
    outputFd = open(argv[inputIdx + 1], openFlags, filePerms);
    if (outputFd == -1)
        errExit("opening file %s", argv[2]);

    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
        if (write(outputFd, buf, numRead) != numRead)
            fatal("write() returned error or partial write occurred");
    if (numRead == -1)
        errExit("read");

    if (close(inputFd) == -1)
        errExit("close input");
    if (close(outputFd) == -1)
        errExit("close output");

    exit(EXIT_SUCCESS);
}