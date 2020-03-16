#if _WIN32
#include <io.h>
#else
#include <unistd.h>
#define _isatty isatty
#define _fileno fileno
#endif
#include "isatty.h"

namespace eoskeygen {

bool isatty(int fd) {
    return ::_isatty(fd);
}

bool isatty(FILE* fd) {
	// fileno() segfaults if fd is null.
    return fd ? isatty(_fileno(fd)) : false;
}

} // namespace eoskeygen
