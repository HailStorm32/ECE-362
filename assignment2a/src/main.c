#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <cstdint>



int main(int argv, char* argc[])
{
/*    const uint8_t STDIN_FILENO = 0;
    const uint8_t STDOUT_FILENO = 1;
    const uint8_t STDERR_FILENO = 2;*/

    int logFD = -1;

    //Open (and create if needed) the log file
    logFD = open("output.log", O_APPEND );

    //Send error if opening file failed
    if(logFD <= -1)
    {
       write(STDERR_FILENO, strerror(errno), strlen(strerror(errno)));
    }

    return 0;
}
