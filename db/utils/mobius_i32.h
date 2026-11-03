/* mobius_i32: the Moebius function mu(n).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_MOBIUS_I32_H
#define OEISC_UTILS_MOBIUS_I32_H
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int mobius_i32(int m)
{
    int r = 1, p;
    for (p = 2; p * p <= m; p++)
        if (m % p == 0) { m /= p; if (m % p == 0) return 0; r = -r; }
    if (m > 1) r = -r;
    return r;
}

#endif
