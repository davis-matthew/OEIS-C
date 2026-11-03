/* div_ceil: ceil(a/b).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_DIV_CEIL_H
#define OEISC_UTILS_DIV_CEIL_H
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

/* ceil division for any signs */
static inline long long div_ceil(long long a, long long b)
{
    long long q = a / b;
    long long r = a % b;
    if (r && ((r > 0) == (b > 0))) q++;
    return q;
}

#endif
