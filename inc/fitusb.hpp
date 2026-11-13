#pragma once
#include <string_view>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
namespace fitusb
{
    int handleFopenError(std::string_view file)
    {
        int err = errno;
        fprintf(stderr, "Error opening file '%s' in mode 'wb'\n", file.data());
        switch (err)
        {
        case ENOENT:
            fprintf(stderr, "  Reason: File does not exist\n");
            break;
        case EACCES:
            fprintf(stderr, "  Reason: Permission denied (check file permissions or if directory is writable)\n");
            break;
        case EISDIR:
            fprintf(stderr, "  Reason: Path is a directory, not a regular file\n");
            break;
        case ENFILE:
            fprintf(stderr, "  Reason: Too many open files in system\n");
            break;
        case EMFILE:
            fprintf(stderr, "  Reason: Process has too many files open\n");
            break;
        case ENAMETOOLONG:
            fprintf(stderr, "  Reason: Filename is too long\n");
            break;
        case ENOSPC:
            fprintf(stderr, "  Reason: No space left on device\n");
            break;
        case EROFS:
            fprintf(stderr, "  Reason: Read-only filesystem\n");
            break;
        case EINTR:
            fprintf(stderr, "  Reason: System call interrupted\n");
            break;
        case EIO:
            fprintf(stderr, "  Reason: I/O error\n");
            break;
        case ETXTBSY:
            fprintf(stderr, "  Reason: Text file busy (file is currently being executed)\n");
            break;
        default:
            fprintf(stderr, "  Reason: %s\n", strerror(err));
        }
        return err;
    }
    int flash(std::string_view device, std::string_view isoPath, const int bs)
    {
        int isofd = open(isoPath.data(), O_RDONLY);
        int devfd = open(device.data(), O_WRONLY);
        size_t bsInBytes = (unsigned long long)bs << 20; // for example 4 gets turned into 4MiB
        unsigned char *buf = (unsigned char *)malloc(bsInBytes);
        if (!buf)
        {
            fprintf(stderr, "bs: %dMiB is too big, couldn't allocate r/w buffer.\n", bs);
            close(devfd);
            close(isofd);
            return -1;
        }
        ssize_t completed = 0;
        ssize_t bytesRead = 0;
        while((bytesRead = read(isofd, buf, bsInBytes)) > 0) {
            ssize_t bytesWritten = 0;
            while(bytesWritten < bytesRead) {
                ssize_t wtStat = write(devfd, buf + bytesWritten, bytesRead - bytesWritten);
                if(wtStat < 0) {
                    int err = errno;
                    fprintf(stderr, "ISO Writing Error: %s (%d)\n", strerror(err), err);
                    free(buf);
                    close(devfd);
                    close(isofd);
                    return err;
                }
                bytesWritten += wtStat;
            }
            completed += bytesRead;
        }
        free(buf);
        close(devfd);
        close(isofd);
        return 0;
    }

}
