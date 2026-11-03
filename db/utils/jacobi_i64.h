/* jacobi_i64: the Jacobi symbol (a/n).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_JACOBI_I64_H
#define OEISC_UTILS_JACOBI_I64_H
#include <stdbool.h>
#include <stdint.h>

/* Jacobi symbol (x|y), y odd positive */
static int jacobi_i64(long long x, long long y)
{
    int t = 1;
    x %= y;
    if (x < 0) x += y;
    while (x != 0) {
        while ((x & 1) == 0) {
            long long r = y & 7;
            x >>= 1;
            if (r == 3 || r == 5) t = -t;
        }
        long long tmp = x; x = y; y = tmp;
        if ((x & 3) == 3 && (y & 3) == 3) t = -t;
        x %= y;
    }
    return y == 1 ? t : 0;
}

#endif
