#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#ifdef DEBUG
#undef NDEBUG  // do not compile assert()
#define debug(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#else
#define NDEBUG  // compile assert()
#define debug(fmt, ...)
#endif

