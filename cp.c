#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include "tlpi_hdr.h"

#define BUFF_SIZE 1024

int main(int argc, char *argv[])
{
    int ip_file, op_file, open_flags;
    mode_t file_perm;
    char buf[BUFF_SIZE];
    ssize_t num_read;
    int hold_size = 0;

    if(argc != 3 || (strcmp(argv[1], "--help") == 0))
        printf("./cp <src> <dst>");

    // Open source file
    open_flags = O_CREAT | O_WRONLY | O_TRUNC;
    ip_file = open(argv[1],O_RDONLY);
    if(ip_file < 0)
    {
        printf("fail to open source file : %s", argv[1]);
        exit(0);
    }

    // Open destination file

    file_perm  = S_IRUSR | S_IWUSR | S_IRGRP |
		         S_IROTH | S_IWOTH;

    op_file = open(argv[2],open_flags,file_perm);
    if(op_file < 0)
    {
        printf("fail to open destination file : %s", argv[2]);
        exit(0);
    }

    while((num_read = read(ip_file,buf,BUFF_SIZE)) > 0)
    {
        for(int i = 0; i < num_read; i++)
        {
            if(buf[i] == '\0')
            {
                hold_size++;
            }

            else
            {
                lseek(op_file, hold_size, SEEK_CUR);
                write(op_file, &buf[i], 1);
                hold_size = 0;
            }
        }
    }
    close(op_file);
    close(ip_file);
}