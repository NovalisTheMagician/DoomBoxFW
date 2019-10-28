#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <unistd.h>

#include "hal/uart.h"
#include "hal/dbg.h"
#include "hal/scb.h"

extern unsigned long _ssdram;
extern unsigned long _esdram;

char *__env[1] = { 0 };
char **environ = __env;

char *__brkval = (char *)(&_ssdram);

// process and threading

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

int _wait(int *status)
{
    errno = ECHILD;
    return -1;
}

void _exit(int status)
{
    SCB_SystemReset();
    while(1);
}

// heap memory stuff

void *_sbrk(int incr)
{
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
}

// io stuff

#include <ff.h>
#include "io.h"
#include "term/term.h"

#define MAX_FILES 255
typedef struct _file_map
{
    int used;
    FIL *file;
} file_map_t;

static file_map_t files[MAX_FILES] = { 0 };

int _write(int file, char *buf, int nbytes)
{
    //write to term
    if(STDOUT_FILENO == file || STDERR_FILENO == file)
    {
       for(int i = 0; i < nbytes; ++i)
        {
            char c = buf[i];
            TERM_Write(c);
        }
        return nbytes;
    }

    //write to uart
    if(IO_UART_OUT == file)
    {
        for(int i = 0; i < nbytes; ++i)
        {
            char c = buf[i];
            UART_Send(UART1, c);
        }
        return nbytes;
    }

    //write to the arm debug port
    if(IO_DBG_OUT == file)
    {
        for(int i = 0; i < nbytes; ++i)
        {
            char c = buf[i];
            ITM_STIM->STIM[0] = c;
        }
        return nbytes;
    }

    if(file > 255)
    {
        errno = EBADF;
        return -1;
    }

    file_map_t file_map = files[file];
    if(!file_map.used)
    {
        errno = EBADF;
        return -1;
    }

    FIL *fil = file_map.file;
    uint32_t bytesWritten;
    FRESULT res = f_write(fil, buf, nbytes, &bytesWritten);
    if(res != FR_OK)
    {
        errno = EIO;
        return -1;
    }

    return bytesWritten;
}

int _read(int file, char *buf, int len)
{
    if(STDIN_FILENO == file)
    {
        return 0;
    }

    if(IO_UART_IN == file)
    {
        return 0;
    }

    if(file > 255)
    {
        errno = EBADF;
        return -1;
    }

    file_map_t file_map = files[file];
    if(!file_map.used)
    {
        errno = EBADF;
        return -1;
    }

    FIL *fil = file_map.file;
    uint32_t bytesRead;
    FRESULT res = f_read(fil, buf, len, &bytesRead);
    if(res != FR_OK)
    {
        errno = EIO;
        return -1;
    }

    return bytesRead;
}

static int findFreeFile()
{
    for(int i = 6; i < MAX_FILES; ++i)
    {
        file_map_t fileMap = files[i];
        if(!fileMap.used)
        {
            return i;
        }
    }
    return -1;
}

// todo check for more file open flags
int _open(const char *name, int flags, int mode)
{
    uint8_t mod = FA_READ;
    if(flags & O_WRONLY) mod = FA_WRITE | FA_CREATE_ALWAYS;
    if(flags & O_RDWR) mod = FA_READ | FA_WRITE | FA_CREATE_ALWAYS;

    if(flags & O_CREAT) mod |= FA_OPEN_ALWAYS;
    if(flags & O_APPEND) mod |= FA_OPEN_APPEND;
    if(flags & O_TRUNC) mod |= FA_CREATE_ALWAYS;
    if(flags & (O_CREAT | O_EXCL)) mod |= FA_CREATE_NEW;

    FIL *fil;
    FRESULT res = f_open(fil, name, mod);
    if(res != FR_OK)
    {
        errno = ENOENT;
        return -1;
    }

    int fileNo = findFreeFile();
    if(fileNo == -1)
    {
        errno = ENFILE;
        return -1;
    }

    file_map_t fileMap = files[fileNo];
    fileMap.used = true;
    fileMap.file = fil;

    return fileNo;
}

int _close(int file)
{
    if(file >= MAX_FILES)
    {
        errno = EBADF;
        return -1;
    }

    file_map_t fileMap = files[file];
    if(!fileMap.used)
    {
        errno = EBADF;
        return -1;
    }

    FRESULT res = f_close(fileMap.file);
    if(res != FR_OK)
    {
        errno = EIO;
        return -1;
    }

    return 0;
}

int _lseek(int file, int offset, int whence)
{
    if(file >= MAX_FILES)
    {
        errno = EBADF;
        return -1;
    }

    file_map_t fileMap = files[file];
    if(!fileMap.used)
    {
        errno = EBADF;
        return -1;
    }

    int offt = 0;
    if(whence == SEEK_SET)
        offt = offset;
    else if(whence == SEEK_CUR)
        offt = f_tell(fileMap.file) + offset;
    else if(whence == SEEK_END)
        offt = f_size(fileMap.file) + offset;

    FRESULT res = f_lseek(fileMap.file, offt);
    if(res != FR_OK)
    {
        errno = EIO;
        return -1;
    }

    return 0;
}

int _isatty(int file)
{
    if(file == STDOUT_FILENO || file == STDERR_FILENO || file == STDIN_FILENO ||
        file == IO_UART_OUT || file == IO_UART_IN || file == IO_DBG_OUT)
        return 1;
    return 0;
}

int _fstat(int file, struct stat *st)
{
    if(file == STDOUT_FILENO || file == STDERR_FILENO || file == STDIN_FILENO ||
        file == IO_UART_OUT || file == IO_UART_IN || file == IO_DBG_OUT)
    {
        st->st_mode = S_IFCHR;
        return 0;
    }

    if(file >= MAX_FILES)
    {
        errno = EBADF;
        return -1;
    }

    file_map_t fileMap = files[file];
    if(!fileMap.used)
    {
        errno = EBADF;
        return -1;
    }

    st->st_mode = S_IFREG;
    st->st_size = f_size(fileMap.file);
    return 0;
}

int _stat(char *file, struct stat *st)
{
    FIL *fp;
    FRESULT res = f_open(fp, file, FA_READ);
    if(res != FR_OK)
    {
        errno = ENOENT;
        return -1;
    }

    st->st_mode = S_IFREG;
    st->st_size = f_size(fp);
    return 0;
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

// time stuff

int _gettimeofday(struct timeval *tv, void *tzvp)
{
    uint64_t t = 1000000;//__your_system_time_function_here__();  // get uptime in nanoseconds
    tv->tv_sec = t / 1000000000;  // convert to seconds
    tv->tv_usec = ( t % 1000000000 ) / 1000;  // get remaining microseconds
    return 0;  // return non-zero for error
}
