#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

static void alarmMsg();

int main(int argc, char *argv[])
{
    int iFileSrc;
    int iFileDst;
    int x = 0;
    char buffer[131072];
    ssize_t xSizeSrc = 0;
    ssize_t xSizeDst = 0;

    alarm(1);
    signal(SIGALRM, alarmMsg);

    if (argc != 3)
    {
        printf("Copyit: Invalid number of arguments: %s SOURCE_FILE DESTINATION_FILE\n", argv[0]);
        return 1;
    }

    iFileSrc = open(argv[1], O_RDONLY);

    if (iFileSrc < 0)
    {
        printf("Copyit: Error file %s is not found: %s\n", argv[1], strerror(errno));
        return 1;
    }

    iFileDst = open(argv[2], O_CREAT | O_WRONLY, S_IRWXU);

    if (iFileDst < 0)
    {
        printf("Copyit: Error create file %s: %s\n", argv[2], strerror(errno));
        return 1;
    }

    while (x = read(iFileSrc, buffer, sizeof buffer), x > 0)
    {
        char *cText = buffer;
        ssize_t xSizeDst;
        xSizeSrc = x;

        while (x > 0)
        {
            xSizeDst = write(iFileDst, cText, x);

            if (xSizeDst >= 0)
            {
                x -= xSizeDst;
                cText += xSizeDst;
            }
            else
            {
                printf("Copyit: Error on write file %s: %s\n", argv[2], strerror(errno));
                return 1;
            }
        }

        if (x < 0)
        {
            printf("Copyit: Error on read file %s: %s\n", argv[1], strerror(errno));
            return 1;
        }
    }

    close(iFileSrc);
    close(iFileDst);

    printf("Copyit: Successfully copied\n");
    printf("Copyit: %ld bytes were copied from file %s to file %s", xSizeSrc, argv[1], argv[2]);

    return 0;
}

static void alarmMsg()
{
    printf("Copyit: Copy in progress ...\n");
    alarm(1);
}