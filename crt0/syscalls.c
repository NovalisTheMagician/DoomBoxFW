#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "fat/ff.h"

char *__env[1] = { 0 };
char **environ = __env;

//char *__brkval = (char *)(&_ssdram);

int _execve(char *name, char **argv, char **env)
{
    errno = ENOMEM;
    return -1;
}

int _fork()
{
    errno = EAGAIN;
    return -1;
}

int _getpid()
{
    return 1;
}

int _kill(int pid, int sig)
{
    errno = EINVAL;
    return -1;
}

void *_sbrk(int incr)
{
    /*
    char *prev = __brkval;
    if(incr != 0)
    {
        if(prev + incr >= (char *)&_esdram)
        {
            errno = ENOMEM;
            return (void *)-1;
        }
        __brkval = prev + incr;
    }
    return prev;
    */
   return 0;
}

void _exit(int status)
{
    //SCB_SystemReset();
    while(1);
}

int _write(int file, char *buf, int nbytes)
{
    for(int i = 0; i < nbytes; ++i)
    {
        char c = buf[i];
        //ITM_STIM->STIM[0] = c;
        //UART_Send(UART1, c);
    }

    return nbytes;
}

int _read(int file, char *buf, int len)
{
    return 0;
}

int _open(const char *name, int flags, int mode)
{
    errno = ENOSYS;
    return -1;
}

int _lseek(int file, int offset, int whence)
{
    return 0;
}

int _isatty(int file)
{
    return 1;
}

int _fstat(int file, struct stat *st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

int _stat(char *file, struct stat *st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

int _close(int file)
{
    errno = EBADF;
    return -1;
}

int _link(char *old, char *new)
{
    errno = EMLINK;
    return -1;
}

int _unlink(char *name)
{
    errno = ENOENT;
    return -1;
}

int _wait(int *status)
{
    errno = ECHILD;
    return -1;
}

