#if !defined(UNIX)

typedef char BOOLEAN;
typedef short INTEGER;
typedef long LONGINT;
#define LONGORPTR PTR

typedef unsigned short ushort;

#else

#define LONGORPTR LONGINT
#include "rsys/common.h"
#include "rsys/libcproto.h"

#endif

typedef unsigned long  ulong;

#if !defined(UNIX)
#define PRIVATE static
#define PUBLIC
#endif /* UNIX */
